using System;
using System.Threading;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Runtime.InteropServices;
using System.Text;
using System.ComponentModel;

namespace winagent
{
    public partial class AgtFormAcceptDesktop : Form
    {
        private BackgroundWorker bgWorker = null;

        public AgtFormAcceptDesktop()
        {
            InitializeComponent();

            bgWorker = new BackgroundWorker();

            bgWorker.DoWork += new DoWorkEventHandler(bgWorker_DoWork);
            bgWorker.ProgressChanged += new ProgressChangedEventHandler(bgWorker_ProgressChanged);
            bgWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bgWorker_RunWorkerCompleted);
            bgWorker.WorkerReportsProgress = true;
            bgWorker.WorkerSupportsCancellation = true;

            // Kickoff the worker thread to begin it's DoWork function.
            bgWorker.RunWorkerAsync();
        }

        private void btnAccetta_Click(object sender, EventArgs e)
        {
            bgWorker.CancelAsync(); 
            winagent.Globals.vncSelf.AcceptDesktop(true);
            this.Close();
        }

        private void btnRifiuta_Click(object sender, EventArgs e)
        {
            bgWorker.CancelAsync();
            winagent.Globals.vncSelf.AcceptDesktop(false);
            this.Close();
        }

        private void bgWorker_DoWork(object sender, System.ComponentModel.DoWorkEventArgs e)
        {
            int i;

            for (i = 100; i >= 0; i--)
            {
                bgWorker.ReportProgress(i / 10);
                System.Threading.Thread.Sleep(100);
                if (bgWorker.CancellationPending)
                {
                    e.Cancel = true;
                    return;
                } 
            }
        }

        private void bgWorker_ProgressChanged(object sender, System.ComponentModel.ProgressChangedEventArgs e)
        {
            btnAccetta.Text = "Si (" + e.ProgressPercentage.ToString() + ")";
            btnAccetta.Refresh();
        }

        private void bgWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled)
            {
            }
            else if (e.Error != null)
            {
            }
            else
            {  
                winagent.Globals.vncSelf.AcceptDesktop(true);
                this.Close();
            }
        }
    }
}
