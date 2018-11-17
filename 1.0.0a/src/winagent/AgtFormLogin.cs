using System;
using System.Threading;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Runtime.InteropServices;
using System.Text;

namespace winagent
{
    public partial class AgtFormLogin : Form
    {
        // Funzioni native nella DLL
        //[DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        //static extern int JADA_IniGetString(string szSection, string szKey, StringBuilder szOut, int iOutLength);
        //[DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        //static extern int JADA_IniSetString(string szSection, string szKey, string szOut);

        public bool CloseAgent;

        public AgtFormLogin()
        {
            InitializeComponent();
            this.CloseAgent = true;
        }

        private void frmLogin_Load(object sender, EventArgs e)
        {
            StringBuilder szRegionArea = new StringBuilder(256);

            JADA.GUI.LoadCombo(winagent.Globals.connSelf, JADA.Common.COMBOTYPE_REGIONE, "", cmbRegione, false);
            JADA.GUI.SelectComboValue(cmbRegione, winagent.Globals.connSelf.Regione);
            txtNome.Text = winagent.Globals.connSelf.Nome;
            txtTelefono.Text = winagent.Globals.connSelf.Telefono;
            txtCellulare.Text = winagent.Globals.connSelf.Cellulare;
            txtMail.Text = winagent.Globals.connSelf.Mail;
            JADA.GUI.LoadCombo(winagent.Globals.connSelf, JADA.Common.COMBOTYPE_CLIENTE, cmbRegione.Text, cmbCliente, false);
            JADA.GUI.SelectComboValue(cmbCliente, winagent.Globals.connSelf.Cliente);
            chkUsaAudio.Checked = winagent.Globals.connSelf.UsaAudio;
        }

        private void btnConnetti_Click(object sender, EventArgs e)
        {
            int iRet;

            if ((txtNome.Text.Length == 0) || (txtTelefono.Text.Length == 0) || (txtCellulare.Text.Length == 0) || 
                (txtMail.Text.Length == 0) || (cmbCliente.Text.Length == 0))
            {
                MessageBox.Show("I campi vanno compilati tutti: completa prima la maschera.");
                return;
            }
            
            winagent.Globals.connSelf.Nome = txtNome.Text;
            winagent.Globals.connSelf.Telefono = txtTelefono.Text;
            winagent.Globals.connSelf.Cellulare = txtCellulare.Text;
            winagent.Globals.connSelf.Mail = txtMail.Text;
            winagent.Globals.connSelf.Regione = cmbRegione.Text;
            winagent.Globals.connSelf.Cliente = cmbCliente.Text;
            winagent.Globals.connSelf.UsaAudio = chkUsaAudio.Checked;
            winagent.Globals.connSelf.SetSystemInfo();

            iRet = winagent.Globals.connSelf.Open();
            switch (iRet)
            {
                case JADA.Common.CREDENTIAL_OK:
                case 0:
                    this.CloseAgent = false;
                    this.Close();
                    break;
                case JADA.Common.CREDENTIAL_UNKNOWN_USER:
                    MessageBox.Show("Utente non abilitato alla connessione.");
                    break;
                case JADA.Common.CREDENTIAL_WRONG_PASSWORD:
                    MessageBox.Show("Password non valida.");
                    break;
                default:
                    string myMessage = String.Format("Non riesco a collegarmi con il server:\n  - il server potrebbe essere temporaneamente indisponibile;\n  - la rete internet/intranet potrebbe non essere disponibile;\n  - devo usare un proxy per la connessione.");
                    MessageBox.Show(myMessage);
                    break;
            }
        }

        private void btnAnnulla_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            string myLink;

            myLink = winagent.Globals.connSelf.Agreement;
            if (myLink.Length > 0)
            {
                System.Diagnostics.Process.Start(myLink);
            }
        }

        private void chkAccept_CheckedChanged(object sender, EventArgs e)
        {
            btnConnetti.Enabled = chkAccept.Checked;
        }

        private void cmbRegione_SelectedValueChanged(object sender, EventArgs e)
        {
            JADA.GUI.LoadCombo(winagent.Globals.connSelf, JADA.Common.COMBOTYPE_CLIENTE, cmbRegione.Text, cmbCliente, false);
        }

        private void chkUseProxy_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
