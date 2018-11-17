using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace winmanager
{
    public partial class MgrFormParameters : Form
    {
        public MgrFormParameters()
        {
            InitializeComponent();
        }

        private void ListParameters()
        {
            JADA.Stream myStream;
            string myLine;

            myStream = winmanager.Globals.suppSelf.ListParameters();
            if (myStream != null)
            {
                grdParametri.Rows.Clear();

                myLine = myStream.GetFirstLine(); 
                while (myLine != "")
                {
                    if (myLine.Length > 0)
                        grdParametri.Rows.Add(myLine.Split(';'));
                    myLine = myStream.GetNextLine();
                }

                if (grdParametri.Rows.Count > 0)
                    grdParametri.Rows[0].Selected = true;
            }
            else
            {
                MessageBox.Show("Lettura dell'elenco di parametri non riuscita.");
            }
        }

        private void FormAcceptRequest_Load(object sender, EventArgs e)
        {
            ListParameters();
        }

        private void btnAnnulla_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
