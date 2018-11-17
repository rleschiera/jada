using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Web;
using System.IO;
using System.Diagnostics;
using System.Net;

namespace winagent
{
    public partial class FormStarter : Form
    {
        private string myModuleName = "";
        private string myBaseURL = "";
        private string myCustomer = "";
        private string myProcessName = "";
        private bool FirstActivate = true;

        public FormStarter(string szModuleName, string szBaseURL, string szCustomer)
        {
            InitializeComponent();

            if (JADA.Starter.Common.GetTestMode() == true)
                this.Text = this.Text + " (test mode)";

            myModuleName = szModuleName;
            myBaseURL = szBaseURL;
            myCustomer = szCustomer;

            if (myModuleName.Equals("agent") == true)
                myProcessName = "wa.exe";
            else if (myModuleName.Equals("manager") == true)
                myProcessName = "wm.exe";
        }

        private void FormStarter_Activated(object sender, EventArgs e)
        {
            if (FirstActivate != true)
                return;

            FirstActivate = false;
            this.Refresh();

            JADA.Starter.Common.Init("winagent", myBaseURL, myCustomer);

            JADA.Starter.Common.Update(this.txtStatus, myModuleName);

            JADA.Starter.Common.Start(this.txtStatus, myProcessName);

            this.Dispose(); 
        }

        private void FormStarter_Load(object sender, EventArgs e)
        {
        }
    }
}
