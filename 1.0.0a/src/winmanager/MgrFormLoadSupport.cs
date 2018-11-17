using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace winmanager
{
    public partial class MgrFormLoadSupport : Form
    {
        public MgrFormLoadSupport()
        {
            InitializeComponent();
        }

        private void btnAnnulla_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnGestisci_Click(object sender, EventArgs e)
        {
            int iRet, iRequestId;

            if (grdRichieste.SelectedRows.Count > 0)
            {
                try
                {
                    iRequestId = Convert.ToInt32(grdRichieste.SelectedRows[0].Cells[0].Value);

                    iRet = winmanager.Globals.suppSelf.Load(iRequestId);
                    if (iRet == 0)
                        this.Close();
                    else
                        MessageBox.Show("Presa in carico della richiesta non riuscita.");
                }
                catch
                {
                }
            }
            else
            {
                MessageBox.Show("Selezionare almeno una richiesta.");
            }
        }

        private void ListPendingRequests()
        {
            JADA.Stream myStream;
            string myLine;

            myStream = winmanager.Globals.connSelf.ListPendingRequests(cmbCoda.Text);
            if (myStream != null)
            {
                grdRichieste.Rows.Clear();

                myLine = myStream.GetFirstLine();
                while (myLine != "")
                {
                    if (myLine.Length > 0)
                        grdRichieste.Rows.Add(myLine.Split(';'));
                    myLine = myStream.GetNextLine();
                }

                if (grdRichieste.Rows.Count > 0)
                    grdRichieste.Rows[0].Selected = true;
            }
            else
            {
                MessageBox.Show("Lettura dell'elenco di richieste non riuscita.");
            }
        }

        private void FormLoadRequest_Load(object sender, EventArgs e)
        {
            JADA.GUI.LoadCombo(winmanager.Globals.connSelf, JADA.Common.COMBOTYPE_MANAGER_CODA, winmanager.Globals.connSelf.Username, cmbCoda, false);
            cmbCoda.Text = winmanager.Globals.connSelf.Coda;

            ListPendingRequests();
        }

        private void cmbArea_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListPendingRequests();
        }

        private void grdRichieste_DoubleClick(object sender, EventArgs e)
        {
            btnGestisci_Click(sender, e); 
        }
    }
}
