using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.Runtime.Serialization;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using Microsoft.Win32;

namespace winagent
{
    public partial class AgtFormMain : Form
    {
        [DllImport("user32.dll")]
        static extern IntPtr GetDC(IntPtr hWnd);

        public bool CloseAgent = false;

        private AgtFormConnectSupport myFormConnectSupport = null;
        private AgtFormLogin myFormLogin = null;
        private AgtFormOpenSupport myFormOpenSupport = null;
        private AgtFormAcceptDesktop myFormAcceptDesktop = null;
        private System.Threading.Thread myDrawThread = null;

        public AgtFormMain()
        {
            int iWidth, iHeight;

            InitializeComponent();

            // Crea le form di lavoro
            winagent.Globals.frmDraw = new AgtFormDraw();
            winagent.Globals.frmDraw.Disposed += new EventHandler(frmDraw_Disposed);

            // Definisci i contorni del desktop
            iWidth = SystemInformation.VirtualScreen.Right - SystemInformation.VirtualScreen.Left;
            iHeight = SystemInformation.VirtualScreen.Bottom - SystemInformation.VirtualScreen.Top;

            // Inizializza gli handler degli eventi
            winagent.Globals.connSelf.ConnectionOpened += new EventHandler(HandleConnectionOpened);
            winagent.Globals.connSelf.ConnectionClosed += new EventHandler(HandleConnectionClosed);
            winagent.Globals.connSelf.SupportOpened += new EventHandler(HandleSupportOpened);
            winagent.Globals.connSelf.SupportSuspended += new EventHandler(HandleSupportSuspended);
            winagent.Globals.connSelf.SupportResumed += new EventHandler(HandleSupportResumed);
            winagent.Globals.connSelf.SupportClosed += new EventHandler(HandleSupportClosed);
            winagent.Globals.connSelf.SupportAccepted += new EventHandler(HandleSupportAccepted);
            winagent.Globals.connSelf.SupportConnected += new EventHandler(HandleSupportConnected);
            winagent.Globals.connSelf.SupportSolicited += new EventHandler(HandleSupportSolicited);
            winagent.Globals.connSelf.ChatTextReceived += new EventHandler(HandleChatTextReceived);
            // winagent.Globals.connSelf.DrawFrameReceived += new EventHandler(HandleDrawFrameReceived);
            winagent.Globals.connSelf.CallOpened += new EventHandler(HandleCallOpened);
            winagent.Globals.connSelf.CallClosed += new EventHandler(HandleCallClosed);
            winagent.Globals.connSelf.CallAborted += new EventHandler(HandleCallAborted);
            winagent.Globals.connSelf.DesktopRequested += new EventHandler(HandleDesktopRequested);
            winagent.Globals.connSelf.DesktopClosed += new EventHandler(HandleDesktopClosed);
            // winagent.Globals.connSelf.VideoFrameReceived += new EventHandler(HandleVideoFrameReceived);
            winagent.Globals.connSelf.WhiteboardOpened += new EventHandler(HandleWhiteboardOpened);
            winagent.Globals.connSelf.WhiteboardCleaned += new EventHandler(HandleWhiteboardCleaned);
            winagent.Globals.connSelf.WhiteboardClosed += new EventHandler(HandleWhiteboardClosed);
            winagent.Globals.connSelf.DrawPointerMoved += new EventHandler(HandleDrawPointerMoved);
            winagent.Globals.connSelf.DrawPointerReset += new EventHandler(HandleDrawPointerReset);

            // Inizializza lo stato della sessione
            JADA.GUI.AddControlText(txtStatus, "In attesa dell'apertura di sessione...");
            // JADA.Common.SetProxySettings();

            // Apri la form di login
            myFormLogin = new AgtFormLogin();
            myFormLogin.ShowDialog();
            if (myFormLogin.CloseAgent == true)
            {
                this.CloseAgent = true;
                return;
            }

            string myTimestamp = DateTime.UtcNow.ToString("dd/MM/yyyy HH:mm:ss");
            JADA.GUI.AddControlText(txtStatus, String.Format("{0}: mi sono collegato al server", myTimestamp));

            int iSuspendedId = winagent.Globals.suppSelf.SuspendedRequestId;
            if (iSuspendedId > 0)
            {
                DialogResult iResult = MessageBox.Show(String.Format("Nell'ultima sessione stavi lavorando con la richiesta n. {0}:\nvuoi riprenderla in gestione per continuare a lavorarla?", iSuspendedId), winagent.Globals.PROCESS_NAME, MessageBoxButtons.YesNo);
                if (iResult == System.Windows.Forms.DialogResult.Yes)
                    winagent.Globals.suppSelf.Resume(iSuspendedId);
                else
                    winagent.Globals.suppSelf.SuspendedRequestId = -1;
            }
        }

        ~AgtFormMain()
        {
            winagent.Globals.frmDraw.Dispose();
        }

        void frmDraw_Disposed(object sender, EventArgs e)
        {
            winagent.Globals.frmDraw = new AgtFormDraw();
            winagent.Globals.frmDraw.Disposed += new EventHandler(frmDraw_Disposed);
        }

        //********************************************************************* 
        //
        //     S E R V I Z I     B A S E
        // 
        //********************************************************************* 
        private void SetSupportStatus(int iStatus)
        {
            switch (iStatus)
            {
                case JADA.Common.TRUE:
                    JADA.GUI.SetControlBackColor(picStatus, Color.Green);
                    JADA.GUI.SetControlEnabled(btnSolicitSupport, true);
                    JADA.GUI.SetControlEnabled(btnCloseSupport, true);
                    break;

                case JADA.Common.FALSE:
                    JADA.GUI.SetControlBackColor(picStatus, Color.Red);
                    JADA.GUI.SetControlEnabled(btnSolicitSupport, false);
                    JADA.GUI.SetControlEnabled(btnCloseSupport, false);
                    break;
            }
        }

        private void PrintChat(string szName, string myChatText)
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

        public void HandleConnectionClosed(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: sessione {1} caduta",
                                      args.Time, args.SessionId);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            SetSupportStatus(JADA.Common.FALSE);

            MessageBox.Show("La connessione con il server e' caduta, per favore chiudi l'agent e riaprilo per ripristinarla.", winagent.Globals.PROCESS_NAME);
        }

        public void HandleSupportOpened(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} aperta",
                                      args.Time, args.RequestId);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            SetSupportStatus(JADA.Common.TRUE);

            MessageBox.Show(String.Format("Una nuova richiesta di supporto (id {0}) e' stata aperta.", args.RequestId), winagent.Globals.PROCESS_NAME);
        }

        public void HandleSupportResumed(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} ripresa in gestione ",
                                      args.Time, args.RequestId);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            SetSupportStatus(JADA.Common.TRUE);

            if (args.Result != 0)
                MessageBox.Show(String.Format("La richiesta di supporto {0} e' stata ripresa in gestione,\nsono connesso al partner.", args.RequestId), winagent.Globals.PROCESS_NAME);
            else
                MessageBox.Show(String.Format("La richiesta di supporto {0} e' stata ripresa in gestione,\nsono in attesa di connessione del partner.", args.RequestId), winagent.Globals.PROCESS_NAME);
        }

        public void HandleSupportAccepted(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} presa in gestione da {2}",
                                      args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            SetSupportStatus(JADA.Common.TRUE);

            MessageBox.Show(String.Format("La tua richiesta di supporto {0} e' stata presa in gestione,\na breve sarai contattato da un nostro operatore.", args.RequestId), winagent.Globals.PROCESS_NAME);
        }

        public void HandleSupportConnected(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} nuovamente aperta, sei connesso con {2}",
                                      args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);
            SetSupportStatus(JADA.Common.TRUE);

            MessageBox.Show("La tua precedente richiesta di supporto e' stata ripresa.", winagent.Globals.PROCESS_NAME);
        }

        public void HandleSupportSolicited(Object s, EventArgs e)
        {
            string szMessage;

            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} sollecitata",
                                      args.Time, args.RequestId);
            JADA.GUI.AddControlText(txtStatus, szMessage);

            MessageBox.Show("La tua precedente richiesta di supporto e' stata sollecitata.", winagent.Globals.PROCESS_NAME);
        }

        public void HandleChatTextReceived(Object s, EventArgs e)
        {
            JADA.EventArgs args = (JADA.EventArgs)e;

            PrintChat(winagent.Globals.suppSelf.YourName, args.ChatText);
        }

        public void HandleSupportSuspended(Object s, EventArgs e)
        {
            string szMessage;
            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} sospesa, avendo perso la connessione con l'utente {2}",
                            args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);

            SetSupportStatus(JADA.Common.FALSE);

            winagent.Globals.frmDraw.DrawClosed();

            MessageBox.Show(String.Format("Ho perso la connessione con {0}: attendi che si riconnetta.", args.Utente), winagent.Globals.PROCESS_NAME);
        }

        public void HandleSupportClosed(Object s, EventArgs e)
        {
            string szMessage;
            JADA.EventArgs args = (JADA.EventArgs)e;

            szMessage = string.Format("{0}: richiesta {1} chiusa dall'utente {2}",
                            args.Time, args.RequestId, args.Utente);
            JADA.GUI.AddControlText(txtStatus, szMessage);

            SetSupportStatus(JADA.Common.FALSE);

            winagent.Globals.frmDraw.DrawClosed();

            MessageBox.Show(String.Format("La richiesta di supporto {0} e' stata chiusa.", args.RequestId), winagent.Globals.PROCESS_NAME);
        }

        public void HandleDesktopRequested(Object s, EventArgs e)
        {
            // Apri la form di accettazione connessione desktop remoto
            myFormAcceptDesktop = new AgtFormAcceptDesktop();
            myFormAcceptDesktop.TopMost = true; 
            myFormAcceptDesktop.ShowDialog();
        }

        public void HandleDesktopClosed(Object s, EventArgs e)
        {
            // Chiudi la lavagna, se presente
            HandleWhiteboardClosed(s, e);
        }

        public void HandleCallOpened(Object s, EventArgs e)
        {
            // Lanciare il modulo di call da qui o da DLL??
        }

        public void HandleCallClosed(Object s, EventArgs e)
        {
            // Arrestare il modulo di call da qui o da DLL??
        }

        public void HandleCallAborted(Object s, EventArgs e)
        {
            // Arrestare il modulo di call da qui o da DLL??
        }

        /**********************************************************************
         * 
         *      W H I T E B O A R D
         * 
         *********************************************************************/
        void DrawThread()
        {
            winagent.Globals.frmMain.Invoke((MethodInvoker)delegate()
            {
                try
                {
                    winagent.Globals.frmDraw.DrawOpened();
                }
                catch (Exception e)
                {
                    JADA.Common.LogMessage(String.Format("DrawThread {0}\n{1}", e.Message, e.StackTrace));
                    MessageBox.Show("Non e' stato possibile aprire la lavagna: i dettagli sono stati salvati nel file di log", winagent.Globals.PROCESS_NAME);
                }
            });
        }

        public void HandleWhiteboardOpened(Object s, EventArgs e)
        {
            this.myDrawThread = new Thread(new ThreadStart(DrawThread));
            this.myDrawThread.IsBackground = true; this.myDrawThread.Start(); 
        }
        
        public void HandleWhiteboardCleaned(Object s, EventArgs e)
        {
            winagent.Globals.frmMain.Invoke((MethodInvoker)delegate()
            {
                winagent.Globals.frmDraw.Clean();
            });
        }

        public void HandleWhiteboardClosed(Object s, EventArgs e)
        {
            if (this.myDrawThread != null)
            {
                this.myDrawThread.Abort(); this.myDrawThread = null;
            }
            if (winagent.Globals.frmDraw.InvokeRequired)
            {
                winagent.Globals.frmDraw.Invoke(new MethodInvoker(delegate
                {
                    winagent.Globals.frmDraw.DrawClosed();
                }));
            }
        }

        public void HandleDrawPointerMoved(Object s, EventArgs e)
        {
            JADA.EventArgs args = (JADA.EventArgs)e;

            winagent.Globals.frmDraw.SetPointer(args.ItemType, args.PointX, args.PointY);
        }

        public void HandleDrawPointerReset(Object s, EventArgs e)
        {
            JADA.EventArgs args = (JADA.EventArgs)e;

            winagent.Globals.frmDraw.ResetPointer();
        }

        public void HandleDrawFrameReceived(Object s, EventArgs e)
        {
            JADA.DrawEventArgs args = (JADA.DrawEventArgs)e;
            JADA.Draw drwSelf = args.Frame;

            winagent.Globals.frmDraw.Draw(drwSelf);
        }

        //********************************************************************* 
        //
        //     E V E N T I     G U I
        // 
        //********************************************************************* 
        private void btnOpenSupport_Click(object sender, EventArgs e)
        {
            // Apri la form di login
            myFormOpenSupport = new AgtFormOpenSupport();
            myFormOpenSupport.ShowDialog();
        }

        private void btnSendChat_Click(object sender, EventArgs e)
        {
            int iRet;
            string myChatText = txtChat.Text;

            if (myChatText.Length > 0)
            {
                iRet = winagent.Globals.suppSelf.SendChat(myChatText);
                if (iRet == 0) {
                    PrintChat(winagent.Globals.connSelf.Nome, myChatText);
                    txtChat.Text = "";
                }
                else {
                    MessageBox.Show("Invio del messaggio chat non riuscito.", winagent.Globals.PROCESS_NAME);
                }
            }
            else
            {
                MessageBox.Show("Nessun messaggio chat da inviare.", winagent.Globals.PROCESS_NAME);
            }
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            bool            iConnected;
            DialogResult    iResult;

            iConnected = winagent.Globals.suppSelf.Connected;
            if (iConnected == true)
            {
                iResult = MessageBox.Show("La richiesta e' ancora aperta: sei sicuro di voler uscire?", winagent.Globals.PROCESS_NAME, MessageBoxButtons.YesNo);
                if (iResult != System.Windows.Forms.DialogResult.Yes)
                {
                    return;
                }
            }

            this.Close(); 
        }

        private void btnConnectSupport_Click(object sender, EventArgs e)
        {
            // Apri la form di connessione ad una richiesta gia' aperta
            myFormConnectSupport = new AgtFormConnectSupport();
            myFormConnectSupport.ShowDialog();

        }

        private void txtChat_TextChanged(object sender, EventArgs e)
        {
            if (txtChat.TextLength > 0)
                btnSendChat.Enabled = true;
            else
                btnSendChat.Enabled = false;
        }

        private void FormMain_Load(object sender, EventArgs e)
        {

        }

        private void btnSolicitSupport_Click(object sender, EventArgs e)
        {
            int iRet;

            iRet = winagent.Globals.suppSelf.Solicit();
        }

        private void btnCloseSupport_Click(object sender, EventArgs e)
        {
            int iRet;

            iRet = winagent.Globals.suppSelf.Close();
            if (iRet == 0)
            {
                SetSupportStatus(JADA.Common.FALSE);
            }
        }

        private void btnTest_Click(object sender, EventArgs e)
        {
            //AgtFormDraw myFormDraw = new AgtFormDraw();
            //myFormDraw.Show();
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

                    iRet = winagent.Globals.suppSelf.SendChat(myChatText);
                    if (iRet == 0)
                    {
                        PrintChat(winagent.Globals.connSelf.Nome, myChatText);
                        txtChat.Text = "";
                    }
                    else
                    {
                        MessageBox.Show("Invio del messaggio chat non riuscito.", winagent.Globals.PROCESS_NAME);
                    }
                }
                return (true);
            }

            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void txtChatList_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
