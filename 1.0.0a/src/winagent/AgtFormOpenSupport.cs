using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace winagent
{
    public partial class AgtFormOpenSupport : Form
    {
        public AgtFormOpenSupport()
        {
            InitializeComponent();
        }

        private void btnCrea_Click(object sender, EventArgs e)
        {
            int iRet;

            winagent.Globals.suppSelf.Coda = cmbCoda.Text;
            winagent.Globals.suppSelf.Description = txtDescription.Text;

            iRet = winagent.Globals.suppSelf.Open();
            if (iRet == 0)
            {
                this.Close();
            }
            else
            {
                MessageBox.Show("Creazione della richiesta non riuscita.");
            }
        }

        private void FormOpenRequest_Load(object sender, EventArgs e)
        {
            JADA.GUI.LoadCombo(winagent.Globals.connSelf, JADA.Common.COMBOTYPE_CODA, "", cmbCoda, false);
            JADA.GUI.SelectComboValue(cmbCoda, winagent.Globals.suppSelf.Coda);
            txtDescription.Text = winagent.Globals.suppSelf.Description;
        }

        private void btnAnnulla_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void cmbArea_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
