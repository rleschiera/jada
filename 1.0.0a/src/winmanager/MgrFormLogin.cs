using System;
using System.Threading;
using System.Windows.Forms;

namespace winmanager
{
    public partial class MgrFormLogin : Form
    {
        private JADA.SimpleAES myAES;
        public bool CloseManager;

        public MgrFormLogin()
        {
            InitializeComponent();
            this.CloseManager = true;

            myAES = new JADA.SimpleAES();
        }

        private void frmLogin_Load(object sender, EventArgs e)
        {
            txtNome.Text = winmanager.Globals.connSelf.Nome;
            txtUsername.Text = winmanager.Globals.connSelf.Username;
            if (winmanager.Globals.connSelf.Password.Length > 0)
            {
                txtPassword.Text = myAES.Decrypt(winmanager.Globals.connSelf.Password);
            }
            txtTelefono.Text = winmanager.Globals.connSelf.Telefono;
            txtCellulare.Text = winmanager.Globals.connSelf.Cellulare;
            txtMail.Text = winmanager.Globals.connSelf.Mail;
            chkUsaAudio.Checked = winmanager.Globals.connSelf.UsaAudio;
            chkUseProxy.Checked = JADA.Common.UseProxy;
        }

        private void btnConnetti_Click(object sender, EventArgs e)
        {
            int iRet;

            if ((txtUsername.Text.Length == 0) || (txtPassword.Text.Length == 0) ||
                (txtTelefono.Text.Length == 0) || (txtCellulare.Text.Length == 0) ||
                (txtMail.Text.Length == 0))
            {
                MessageBox.Show("I campi vanno compilati tutti: completa prima la maschera.");
                return;
            }

            if (txtUsername.Text.Contains(" "))
            {
                MessageBox.Show("Il campo user name non può contenere spazi.");
                return;
            }

            winmanager.Globals.connSelf.Nome = txtNome.Text;
            winmanager.Globals.connSelf.Username = txtUsername.Text;
            winmanager.Globals.connSelf.Password = myAES.Encrypt(txtPassword.Text);
            winmanager.Globals.connSelf.Telefono = txtTelefono.Text;
            winmanager.Globals.connSelf.Cellulare = txtCellulare.Text;
            winmanager.Globals.connSelf.Mail = txtMail.Text;
            winmanager.Globals.connSelf.UsaAudio = chkUsaAudio.Checked;
            JADA.Common.UseProxy = chkUseProxy.Checked;

            iRet = winmanager.Globals.connSelf.Open();
            switch (iRet) {
            case JADA.Common.CREDENTIAL_OK:
            case 0:
                this.CloseManager = false;
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
    }
}
