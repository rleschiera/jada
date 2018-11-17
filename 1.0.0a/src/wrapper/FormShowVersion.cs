using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace JADA
{
    public partial class FormShowVersion : Form
    {
        public FormShowVersion(Version verAgent, Version verManager, Version verWrapper, Version verVideoMaker, Version verAgentStarter, Version verManagerStarter)
        {
            InitializeComponent();

            lblAgent.Text = verAgent.ToString();
            lblManager.Text = verManager.ToString();
            lblDLL.Text = verWrapper.ToString();
            lblVideoMaker.Text = verVideoMaker.ToString();
            if (verManagerStarter != null)
            {
                lblManagerStarter.Text = verManagerStarter.ToString();
            }
            if (verAgentStarter != null)
            {
                lblAgentStarter.Text = verAgentStarter.ToString();
            }
        }

        private void btnChiudi_Click(object sender, System.EventArgs e)
        {
            this.Close();
        }
    }
}
