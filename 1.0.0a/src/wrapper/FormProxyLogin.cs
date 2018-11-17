using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace JADA
{
    public partial class FormProxyLogin : Form
    {
        public bool RetryConnection;
        private JADA.SimpleAES myAES;

        public FormProxyLogin()
        {
            InitializeComponent();

            RetryConnection = false;
            myAES = new JADA.SimpleAES();
        }

        private void FormProxyLogin_Load(object sender, System.EventArgs e)
        {
            int iRet;
            StringBuilder szUsername = new StringBuilder(256), szPassword = new StringBuilder(256);

            iRet = JADA.Common.GetProxyCredentials(szUsername, szPassword);
            if (iRet == 0) {
                txtUsername.Text = szUsername.ToString();
                if (szPassword.Length > 0)
                {
                    txtPassword.Text = myAES.Decrypt(szPassword.ToString());
                }
            }
        }

        private void btnOK_Click(object sender, System.EventArgs e)
        {
            if ((txtUsername.Text.Length == 0) || (txtPassword.Text.Length == 0))
            {
                MessageBox.Show("I campi vanno compilati tutti: completa prima la maschera.");
                return;
            }

            JADA.Common.SetProxyCredentials(txtUsername.Text, myAES.Encrypt(txtPassword.Text), txtPassword.Text);
            RetryConnection = true;
            this.Close();
        }

        private void btnAnnulla_Click(object sender, System.EventArgs e)
        {
            this.Close();
        }
    }
}
