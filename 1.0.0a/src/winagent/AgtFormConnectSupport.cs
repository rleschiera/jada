using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace winagent
{
    public partial class AgtFormConnectSupport : Form
    {
        public AgtFormConnectSupport()
        {
            InitializeComponent();
        }

        private void btnAnnulla_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            int iRet, iRequestId;

            if (Int32.TryParse(txtRequestId.Text, out iRequestId)) {
                iRet = winagent.Globals.suppSelf.Connect(iRequestId);
                if (iRet == 0)
                {
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Aggancio alla richiesta non riuscito.");
                }
            }
            else
            {
                MessageBox.Show("Il numero di richiesta non e' stato scritto correttamente: riprova, per favore.");
            }
        }
    }
}
