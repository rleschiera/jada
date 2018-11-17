using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;

namespace winmanager
{
    public partial class MgrFormMain : Form
    {
        public bool CloseManager = false;

        private MgrFormLoadSupport myFormConnectRequest = null;
        private MgrFormLogin myFormLogin = null;
        private MgrFormAcceptSupport myFormOpenRequest = null;
        private MgrFormParameters myFormParameters = null;
        private bool myAgentUsaAudio = true;

        public MgrFormMain()
        {
            InitializeComponent();

            // Inizializza gli handler degli eventi
            winmanager.Globals.connSelf.ConnectionOpened += new EventHandler(HandleConnectionOpened);
            winmanager.Globals.connSelf.ConnectionClosed += new EventHandler(HandleConnectionClosed);
            winmanager.Globals.connSelf.SupportOpened += new EventHandler(HandleSupportOpened);
            winmanager.Globals.connSelf.SupportAccepted += new EventHandler(HandleSupportAccepted);
            winmanager.Globals.connSelf.SupportLoaded += new EventHandler(HandleSupportLoaded);
            winmanager.Globals.connSelf.SupportConnected += new EventHandler(HandleSupportConnected);
            winmanager.Globals.connSelf.SupportSolicited += new EventHandler(HandleSupportSolicited);
            winmanager.Globals.connSelf.SupportSuspended += new EventHandler(HandleSupportSuspended);
            winmanager.Globals.connSelf.SupportResumed += new EventHandler(HandleSupportResumed);
            winmanager.Globals.connSelf.SupportClosed += new EventHandler(HandleSupportClosed);
            winmanager.Globals.connSelf.CountersList += new EventHandler(HandleListCountersReceived);
            winmanager.Globals.connSelf.ChatTextReceived += new EventHandler(HandleChatTextReceived);
            winmanager.Globals.connSelf.CallOpened += new EventHandler(HandleCallOpened);
            winmanager.Globals.connSelf.CallClosed += new EventHandler(HandleCallClosed);
            winmanager.Globals.connSelf.CallAborted += new EventHandler(HandleCallAborted);
            winmanager.Globals.connSelf.DesktopOpened += new EventHandler(HandleDesktopOpened);
            winmanager.Globals.connSelf.PingReply += new EventHandler(HandlePingReply);

            // Inizializza lo stato della sessione
            JADA.GUI.AddControlText(txtStatus, "In attesa dell'apertura di sessione...");

            // Effettua il login al sistema
            myFormLogin = new MgrFormLogin();
            myFormLogin.ShowDialog();
            if (myFormLogin.CloseManager == true)
            {
                this.CloseManager = true;
                return;
            }

            string myTimestamp = DateTime.UtcNow.ToString("dd/MM/yyyy HH:mm:ss");
            JADA.GUI.AddControlText(txtStatus, string.Format("{0}: mi sono collegato al server", myTimestamp));

            int iSuspendedId = winmanager.Globals.suppSelf.SuspendedRequestId;
            if (iSuspendedId > 0)
            {
                DialogResult iResult = MessageBox.Show(String.Format("Nell'ultima sessione stavi lavorando con la richiesta n. {0}:\nvuoi riprenderla in gestione per continuare a lavorarla?", iSuspendedId), winmanager.Globals.PROCESS_NAME, MessageBoxButtons.YesNo);
                if (iResult == System.Windows.Forms.DialogResult.Yes)
                    winmanager.Globals.suppSelf.Resume(iSuspendedId);
                else
                    winmanager.Globals.suppSelf.SuspendedRequestId = -1;
            }
        }

        ~MgrFormMain()
        {
        }

        //********************************************************************* 
        //
        //     S E R V I Z I     B A S E
        // 
        //********************************************************************* 
        public void PrintChat(string szName, string myChatText)
        {
            string myHeader = string.Format("{0}", szName);
            string myBody = string.Format("{0}", myChatText) + Environment.NewLine;

            if (txtChatList.InvokeRequired)
            {
                txtChatList.Invoke(new MethodInvoker(delegate
                {
                    int currLength = txtChatList.Text.Length;

                    if (txtChatList.Text.Length > 0) txtChatList.Text += Environment.NewLine;
                    txtChatList.Font = new Font(txtChatList.Font, FontStyle.Regular);
                    txtChatList.AppendText(myHeader + ": " + myBody);
                    txtChatList.Select(currLength, myHeader.Length);
                    txtChatList.SelectionFont = new Font(txtChatList.Font, FontStyle.Bold);
                    txtChatList.SelectionStart = txtChatList.Text.Length;
                    txtChatList.ScrollToCaret();
                    txtChatList.Refresh();
                }));
            }
            else {
                int currLength = txtChatList.Text.Length;

                if (txtChatList.Text.Length > 0) txtChatList.Text += Environment.NewLine;
                txtChatList.Font = new Font(txtChatList.Font, FontStyle.Regular);
                txtChatList.AppendText(myHeader + ": " + myBody);
                txtChatList.Select(currLength, myHeader.Length);
                txtChatList.SelectionFont = new Font(txtChatList.Font, FontStyle.Bold);
                txtChatList.SelectionStart = txtChatList.Text.Length;
                txtChatList.ScrollToCaret();
                txtChatList.Refresh();
            }
        }

        private void SetRequestStatus(int iStatus)
        {
            switch (iStatus)
            {
                case JADA.Common.TRUE:
                    JADA.GUI.SetControlBackColor(picStatus, Color.Green);
                    JADA.GUI.SetControlEnabled(btnShowConfig, true);
                    JADA.GUI.SetControlEnabled(btnOpenDesktop, true);
                    JADA.GUI.SetControlEnabled(btnOpenCall, true);
                    JADA.GUI.SetControlEnabled(btnCloseSupport, true);
                    winmanager.Globals.suppSelf.StartPing();
                    break;

                case JADA.Common.FALSE:
                    JADA.GUI.SetControlText(txtDescription, "");
                    JADA.GUI.SetControlBackColor(picStatus, Color.Red);

                    winmanager.Globals.vncSelf.CloseDesktop();
                    JADA.GUI.SetControlEnabled(btnShowConfig, false);
                    JADA.GUI.SetControlEnabled(btnOpenDesktop, false);
                    JADA.GUI.SetControlEnabled(btnOpenCall, false);
                    JADA.GUI.SetControlEnabled(btnCloseSupport, false);

                    winmanager.Globals.suppSelf.StopPing();
                    JADA.GUI.SetControlText(lblDelay, "");
                    JADA.GUI.SetControlForeColor(lblDelay, Color.Black);
                    JADA.GUI.SetControlToolTip(toolTip, lblDelay, "");
                    break;
            }
        }

        private void PrintRequestInfo(string szUtente, string szUserInfo, string szArea, string szDescrizione)
        {
            string myString = String.Format("{0} {1}\nArea {2}: {3}", szUtente, szUserInfo, szArea, szDescrizione);

            JADA.GUI.SetControlText(txtDescription, "");
            JADA.GUI.AddControlText(txtDescription, String.Format("{0} {1}", szUtente, szUserInfo));
            JADA.GUI.AddControlText(txtDescription, String.Format("Area {0}: {1}", szArea, szDescrizione));
        }

        private string GetWelcomeMessage()
        {
            string szChatText = "";
            int iHour = DateTime.Now.Hour;

            if (iHour <= 13)
                szChatText = "Buongiorno, come posso aiutarla?";
            else if (iHour <= 18)
                szChatText = "Buon pomeriggio, come posso aiutarla?";
            else
                szChatText = "Buona sera, come posso aiutarla?";

            return (szChatText);
        }

        //********************************************************************* 
        //
        //     G E S T I O N E     E V E N T I
        // 
        //********************************************************************* 
        public void HandleConnectionOpened(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs  args = (JADA.EventArgs) e;

            szMessage = string.Format("{0}: sessione {1} aperta",
                                      args.Time, args.SessionId);
            JADA.GUI.AddControlText(txtStatus, szMessage);
        }

        public void HandleSupportResumed(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} ripresa in gestione",
                                      args.Time, args.RequestId);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            SetRequestStatus(JADA.Common.TRUE);
            PrintRequestInfo(args.Utente, args.ChatText, args.Area, args.Description);

            if (args.Result != 0)
                MessageBox.Show(String.Format("La richiesta di supporto {0} e' stata ripresa in gestione,\nsono connesso al partner.", args.RequestId), winmanager.Globals.PROCESS_NAME);
            else
                MessageBox.Show(String.Format("La richiesta di supporto {0} e' stata ripresa in gestione,\nsono in attesa di connessione del partner.", args.RequestId), winmanager.Globals.PROCESS_NAME);
        }

        public void HandleSupportSuspended(Object s, EventArgs e)
        {
            string szMessage;
            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} sospesa, avendo perso la connessione con l'utente {2}",
                            args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            SetRequestStatus(JADA.Common.FALSE);

            MessageBox.Show(String.Format("Ho perso la connessione con {0}: attendi che si riconnetta.", args.Utente), winmanager.Globals.PROCESS_NAME);
        }

        public void HandleConnectionClosed(Object s, EventArgs e)
        {
            string szMessage;
            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: sessione {1} caduta",
                                      args.Time, args.SessionId);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            SetRequestStatus(JADA.Common.FALSE);

            MessageBox.Show("La connessione con il server e' caduta, per favore chiudi il manager e riaprilo per ripristinarla.", winmanager.Globals.PROCESS_NAME);
        }

        public void HandleSupportOpened(Object s, EventArgs e)
        {
            int iRet;
            string szMessage, szChatText;
            JADA.EventArgs args = (JADA.EventArgs)e;

            myAgentUsaAudio = args.UsaAudio;

            szMessage = string.Format("{0}: richiesta {1} aperta",
                                      args.Time, args.RequestId);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            PrintRequestInfo(args.Utente, args.ChatText, args.Area, args.Description);

            szChatText = GetWelcomeMessage();
            iRet = winmanager.Globals.suppSelf.SendChat(szChatText);
            if (iRet == 0)
                PrintChat(winmanager.Globals.connSelf.Nome, szChatText);

            SetRequestStatus(JADA.Common.TRUE);

            MessageBox.Show(String.Format("Una nuova richiesta di supporto (id {0}) e' stata aperta.", args.RequestId), winmanager.Globals.PROCESS_NAME);
        }

        public void HandleSupportAccepted(Object s, EventArgs e)
        {
            int iRet;
            string szMessage, szChatText;
            JADA.EventArgs args = (JADA.EventArgs)e;

            myAgentUsaAudio = args.UsaAudio;

            szMessage = string.Format("{0}: richiesta {1} di {2} presa in gestione",
                                      args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);

            PrintRequestInfo(args.Utente, args.ChatText, args.Area, args.Description);

            SetRequestStatus(JADA.Common.TRUE);

            szChatText = GetWelcomeMessage();
            iRet = winmanager.Globals.suppSelf.SendChat(szChatText);
            if (iRet == 0)
                PrintChat(winmanager.Globals.connSelf.Nome, szChatText);

            MessageBox.Show(String.Format("La richiesta di supporto {0} e' stata presa in gestione.", args.RequestId), winmanager.Globals.PROCESS_NAME);
        }

        public void HandleSupportLoaded(Object s, EventArgs e)
        {
            string szMessage;
            JADA.EventArgs args = (JADA.EventArgs)e;

            myAgentUsaAudio = args.UsaAudio;

            szMessage = string.Format("{0}: richiesta pending {1} presa in carico",
                                      args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);

            MessageBox.Show(String.Format("La richiesta di supporto precedente {0} e' stata ripresa.", args.RequestId), winmanager.Globals.PROCESS_NAME);
        }

        public void HandleSupportConnected(Object s, EventArgs e)
        {
            int iRet;
            string szMessage, szChatText;
            JADA.EventArgs args = (JADA.EventArgs)e;

            myAgentUsaAudio = args.UsaAudio;

            szMessage = string.Format("{0}: richiesta pending {1} in gestione, sei connesso con {2}",
                                      args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);

            SetRequestStatus(JADA.Common.TRUE);

            szChatText = GetWelcomeMessage();
            iRet = winmanager.Globals.suppSelf.SendChat(szChatText);
            if (iRet == 0)
                PrintChat(winmanager.Globals.connSelf.Nome, szChatText);

            MessageBox.Show(String.Format("L'utente è collegato, la richiesta di supporto {0} e' stata presa in gestione.", args.RequestId), winmanager.Globals.PROCESS_NAME);
        }

        public void HandleSupportSolicited(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} sollecitata",
                                      args.Time, args.RequestId);
            JADA.GUI.AddControlText(txtStatus, szMessage);

            MessageBox.Show("La tua precedente richiesta di supporto e' stata sollecitata.", winmanager.Globals.PROCESS_NAME);
        }

        public void HandleSupportClosed(Object s, EventArgs e)
        {
            string szMessage;
            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} chiusa dall'utente {2}",
                                      args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);

            winmanager.Globals.vncSelf.CloseDesktop(); 
            SetRequestStatus(JADA.Common.FALSE);

            MessageBox.Show(String.Format("La richiesta di supporto {0} e' stata chiusa.", args.RequestId), winmanager.Globals.PROCESS_NAME);
        }

        public void HandleDesktopOpened(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: desktop remoto {1} aperto",
                                      args.Time, args.RequestId);
            JADA.GUI.AddControlText(txtStatus, szMessage);
        }

        public void HandlePingReply(Object s, EventArgs e)
        {
            JADA.EventArgs args = (JADA.EventArgs)e;

            if (lblDelay.InvokeRequired)
            {
                lblDelay.Invoke(new MethodInvoker(delegate
                {
                    lblDelay.Text = args.PointX.ToString();
                    if (args.PointX > 400)
                    {
                        lblDelay.ForeColor = System.Drawing.Color.Red;
                        toolTip.SetToolTip(lblDelay, "Connessione molto lenta: evita la chiamata audio/video");
                    }
                    else if (args.PointX > 200)
                    {
                        lblDelay.ForeColor = System.Drawing.Color.Yellow;
                        toolTip.SetToolTip(lblDelay, "Connessione lenta: sconsiglio la chiamata audio/video");
                    }
                    else
                    {
                        lblDelay.ForeColor = System.Drawing.Color.Green;
                        toolTip.SetToolTip(lblDelay, "Connessione accettabile: la chiamata audio/video e' possibile");
                    }
                }));
            }
            else
            {
                lblDelay.Text = args.PointX.ToString();
                if (args.PointX > 400)
                {
                    lblDelay.ForeColor = System.Drawing.Color.Red;
                    toolTip.SetToolTip(lblDelay, "Connessione molto lenta: evita la chiamata audio/video");
                }
                else if (args.PointX > 200)
                {
                    lblDelay.ForeColor = System.Drawing.Color.Orange;
                    toolTip.SetToolTip(lblDelay, "Connessione lenta: sconsiglio la chiamata audio/video");
                }
                else
                {
                    lblDelay.ForeColor = System.Drawing.Color.Green;
                    toolTip.SetToolTip(lblDelay, "Connessione accettabile: la chiamata audio/video e' possibile");
                }
            }
        }

        public void HandleChatTextReceived(Object s, EventArgs e)
        {
            JADA.EventArgs args = (JADA.EventArgs)e;

            PrintChat(winmanager.Globals.suppSelf.YourName, args.ChatText);
        }

        public void HandleCallOpened(Object s, EventArgs e)
        {
            // Aprire il modulo call d a qui o dalla DLL???
        }

        public void HandleCallClosed(Object s, EventArgs e)
        {
            // Chiudere il modulo call d a qui o dalla DLL???
        }

        public void HandleCallAborted(Object s, EventArgs e)
        {
            // Chiudere il modulo call d a qui o dalla DLL???
        }

        public void HandleListCountersReceived(Object s, EventArgs e)
        {
            JADA.EventArgs args = (JADA.EventArgs)e;
            string myText;

            myText = String.Format("Aggiornamento di contatori: totali {0}/{1}, pending {2}/{3}\n", 
                args.AreaRequests.ToString(), args.TotalRequests.ToString(), args.AreaRequests.ToString(), args.TotalRequests.ToString());
            JADA.Common.LogMessage(myText);

            if (btnSupports.InvokeRequired)
            {
                btnSupports.Invoke(new MethodInvoker(delegate
                {
                    myText = btnSupports.Text.Substring(0, btnSupports.Text.IndexOf("("));
                    btnSupports.Text = myText + "(" + args.AreaRequests.ToString() + "/" + args.TotalRequests.ToString() + ")";
                    if (args.TotalRequests == 0)
                        btnSupports.BackColor = Color.Green;
                    else if ((args.AreaRequests == 0) && (args.TotalRequests > 0))
                        btnSupports.BackColor = Color.Yellow;
                    else if (args.AreaRequests > 0)
                        btnSupports.BackColor = Color.Red;
                }));
            }
            else
            {
                myText = btnSupports.Text.Substring(0, btnSupports.Text.IndexOf("("));
                btnSupports.Text = myText + "(" + args.AreaRequests.ToString() + "/" + args.TotalRequests.ToString() + ")";
                if (args.TotalRequests == 0)
                    btnSupports.BackColor = Color.Green;
                else if ((args.AreaRequests == 0) && (args.TotalRequests > 0))
                    btnSupports.BackColor = Color.Yellow;
                else if (args.AreaRequests > 0)
                    btnSupports.BackColor = Color.Red;
            }

            if (btnPendingSupports.InvokeRequired)
            {
                btnPendingSupports.Invoke(new MethodInvoker(delegate
                {
                    myText = btnPendingSupports.Text.Substring(0, btnPendingSupports.Text.IndexOf("("));
                    btnPendingSupports.Text = myText + "(" + args.AreaPendingRequests.ToString() + "/" + args.TotalPendingRequests.ToString() + ")";
                    if (args.TotalPendingRequests == 0)
                        btnPendingSupports.BackColor = Color.Green;
                    else if ((args.AreaPendingRequests == 0) && (args.TotalPendingRequests > 0))
                        btnPendingSupports.BackColor = Color.Yellow;
                    else if (args.AreaPendingRequests > 0)
                        btnPendingSupports.BackColor = Color.Red;
                }));
            }
            else
            {
                myText = btnPendingSupports.Text.Substring(0, btnPendingSupports.Text.IndexOf("("));
                btnPendingSupports.Text = myText + "(" + args.AreaPendingRequests.ToString() + "/" + args.TotalPendingRequests.ToString() + ")";
                if (args.TotalPendingRequests == 0)
                    btnPendingSupports.BackColor = Color.Green;
                else if ((args.AreaPendingRequests == 0) && (args.TotalPendingRequests > 0))
                    btnPendingSupports.BackColor = Color.Yellow;
                else if (args.AreaPendingRequests > 0)
                    btnPendingSupports.BackColor = Color.Red;
            }
        }

        //********************************************************************* 
        //
        //     E V E N T I     G U I
        // 
        //********************************************************************* 
        private void btnOpenRequest_Click(object sender, EventArgs e)
        {
            myFormOpenRequest = new MgrFormAcceptSupport();
            myFormOpenRequest.ShowDialog();
        }

        private void btnSendChat_Click(object sender, EventArgs e)
        {
            int iRet;
            string myChatText = txtChat.Text;

            if (myChatText.Length > 0)
            {
                iRet = winmanager.Globals.suppSelf.SendChat(myChatText);
                if (iRet == 0) {
                    PrintChat(winmanager.Globals.connSelf.Nome, myChatText);
                    txtChat.Text = "";
                }
                else {
                    MessageBox.Show("Invio del messaggio chat non riuscito.", winmanager.Globals.PROCESS_NAME);
                }
            }
            else
            {
                MessageBox.Show("Nessun messaggio chat da inviare.", winmanager.Globals.PROCESS_NAME);
            }
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            bool            iConnected;
            DialogResult    iResult;

            iConnected = winmanager.Globals.suppSelf.Connected;
            if (iConnected == true)
            {
                iResult = MessageBox.Show("La richiesta e' ancora aperta: sei sicuro di voler uscire?", winmanager.Globals.PROCESS_NAME, MessageBoxButtons.YesNo);
                if (iResult != System.Windows.Forms.DialogResult.Yes)
                {
                    return;
                }
            }

            this.Close(); 
        }

        private void btnConnectRequest_Click(object sender, EventArgs e)
        {
            // Apri la form di connessione ad una richiesta gia' aperta
            myFormConnectRequest = new MgrFormLoadSupport();
            myFormConnectRequest.ShowDialog();

        }

        private void txtChat_TextChanged(object sender, EventArgs e)
        {
            if (txtChat.TextLength > 0)
                btnSendChat.Enabled = true;
            else
                btnSendChat.Enabled = false;
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            // Mostra la versione dei vari assembly
            if (keyData == (Keys.Control | Keys.Alt | Keys.V)) 
            {
                JADA.GUI.ShowAssemblyInfo();
            }
            
            // Invia il messaggio presente nella chat
            if (keyData == (Keys.Control | Keys.Enter)) 
            {
                string myChatText = txtChat.Text;

                if (myChatText.Length > 0)
                {
                    int iRet;

                    iRet = winmanager.Globals.suppSelf.SendChat(myChatText);
                    if (iRet == 0)
                    {
                        PrintChat(winmanager.Globals.connSelf.Nome, myChatText);
                        txtChat.Text = "";
                    }
                    else
                    {
                        MessageBox.Show("Invio del messaggio chat non riuscito.", winmanager.Globals.PROCESS_NAME);
                    }
                }
                return (true);
            }

            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void btnCloseSupport_Click(object sender, EventArgs e)
        {
            int iRet;

            iRet = winmanager.Globals.suppSelf.Close();
            if (iRet == 0)
            {
                SetRequestStatus(JADA.Common.FALSE);
            }
        }

        private void btnOpenDesktop_Click(object sender, EventArgs e)
        {
            int iRet;

            iRet = winmanager.Globals.vncSelf.OpenDesktop();
        }

        private void btnShowConfig_Click(object sender, EventArgs e)
        {
            myFormParameters = new MgrFormParameters();
            myFormParameters.ShowDialog();
        }

        private void btnOpenCall_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Funzionalità non ancora supportata.", winmanager.Globals.PROCESS_NAME);
        }
    }
}
