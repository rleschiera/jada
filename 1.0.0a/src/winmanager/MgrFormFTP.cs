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
    public partial class MgrFormFTP : Form
    {
        public MgrFormFTP()
        {
            InitializeComponent();

            winmanager.Globals.suppSelf.DirectoryListReceived += new EventHandler(HandleDirectoryListReceived);
            winmanager.Globals.connSelf.FileReceived += new EventHandler(HandleFileReceived);
            winmanager.Globals.connSelf.FileSent += new EventHandler(HandleFileSent);

            txtAgentCurrDir.Text = "C:\\";
            txtManagerCurrDir.Text = "C:\\";
        }

        public void FTPOpened()
        {
            this.Show(); 
        }

        public void FTPClosed()
        {
            this.Hide();
        }

        public void HandleDirectoryListReceived(Object s, EventArgs e)
        {
            JADA.EventArgs args = (JADA.EventArgs)e;

            if (System.IO.File.Exists(args.FileName))
            {
                string[] textData = System.IO.File.ReadAllLines(args.FileName);

                if (lstAgent.InvokeRequired) {
                    lstAgent.Invoke(new MethodInvoker(delegate
                    {
                        for (int i = 0; i < textData.Length; i++)
                            lstAgent.Items.Add(textData[i]);
                        lstAgent.Refresh();
                    }));
                }
                else
                {
                    for (int i = 0; i < textData.Length; i++)
                        lstAgent.Items.Add(textData[i]);
                    lstAgent.Refresh();
                }

                System.IO.File.Delete(args.FileName);
            }
        }

        public void HandleFileReceived(Object s, EventArgs e)
        {
            int iProgress, iIndex;
            JADA.EventArgs args = (JADA.EventArgs)e;

            iProgress = (args.CurrFrame * 100) / args.Frames;
            if (txtReceivingFile.InvokeRequired) {
                txtReceivingFile.Invoke(new MethodInvoker(delegate
                {
                    if (iProgress < 100)
                    {
                        txtReceivingFile.Text = args.FileName;
                        progGetFile.Value = iProgress;
                    }
                    else {
                        txtReceivingFile.Text = "";
                        progGetFile.Value = 0;
                        iIndex = lstAgent.FindString(Path.GetFileName(args.FileName));
                        if (iIndex != -1)
                            lstAgent.SetSelected(iIndex,false);

                        if (btnCancel.Enabled == true) {
                            if (lstAgent.SelectedItems.Count > 0) {
                                string szLocalFile = txtManagerCurrDir.Text + lstAgent.SelectedItems[0].ToString();
                                string szRemoteFile = txtAgentCurrDir.Text + lstAgent.SelectedItems[0].ToString();
                                int iRet = winmanager.Globals.suppSelf.GetFile(szRemoteFile, szLocalFile);
                            }
                        }
                        else {
                            LoadLocalDir(txtManagerCurrDir.Text);
                        }
                    }
                }));
            }
            else {
                if (iProgress < 100)
                {
                    txtReceivingFile.Text = args.FileName;
                    progGetFile.Value = iProgress;
                }
                else
                {
                    txtReceivingFile.Text = "";
                    progGetFile.Value = 0;
                    iIndex = lstAgent.FindString(Path.GetFileName(args.FileName));
                    if (iIndex != -1)
                        lstAgent.SetSelected(iIndex, false);

                    if (btnCancel.Enabled == true)
                    {
                        if (lstAgent.SelectedItems.Count > 0)
                        {
                            string szLocalFile = txtManagerCurrDir.Text + lstAgent.SelectedItems[0].ToString();
                            string szRemoteFile = txtAgentCurrDir.Text + lstAgent.SelectedItems[0].ToString();
                            int iRet = winmanager.Globals.suppSelf.GetFile(szRemoteFile, szLocalFile);
                        }
                    }
                    else
                    {
                        LoadLocalDir(txtManagerCurrDir.Text);
                    }
                }
            }
        }

        public void HandleFileSent(Object s, EventArgs e)
        {
            int iProgress, iIndex;
            JADA.EventArgs args = (JADA.EventArgs)e;

            iProgress = (args.CurrFrame * 100) / args.Frames;
            if (txtSendingFile.InvokeRequired) {
                txtSendingFile.Invoke(new MethodInvoker(delegate
                {
                    if (iProgress < 100) {
                        txtSendingFile.Text = args.FileName;
                        progPutFile.Value = iProgress;
                    }
                    else {
                        txtSendingFile.Text = "";
                        progPutFile.Value = 0;
                        iIndex = lstManager.FindString(Path.GetFileName(args.FileName));
                        if (iIndex != -1)
                            lstManager.SetSelected(iIndex, false);

                        if (btnCancel.Enabled == true)
                        {
                            if (lstManager.SelectedItems.Count > 0)
                            {
                                string szLocalFile = txtManagerCurrDir.Text + lstManager.SelectedItems[0].ToString();
                                string szRemoteFile = txtAgentCurrDir.Text + lstManager.SelectedItems[0].ToString();
                                int iRet = winmanager.Globals.suppSelf.PutFile(szLocalFile, szRemoteFile);
                            }
                        }
                        else
                        {
                            LoadRemoteDir(txtAgentCurrDir.Text);
                        }
                    }
                }));
            }
            else {
                if (iProgress < 100)
                {
                    txtSendingFile.Text = args.FileName;
                    progPutFile.Value = iProgress;
                }
                else
                {
                    txtSendingFile.Text = "";
                    progPutFile.Value = 0;
                    iIndex = lstManager.FindString(Path.GetFileName(args.FileName));
                    if (iIndex != -1)
                        lstManager.SetSelected(iIndex, false);

                    if (btnCancel.Enabled == true) {
                        if (lstManager.SelectedItems.Count > 0)
                        {
                            string szLocalFile = txtManagerCurrDir.Text + lstManager.SelectedItems[0].ToString();
                            string szRemoteFile = txtAgentCurrDir.Text + lstManager.SelectedItems[0].ToString();
                            int iRet = winmanager.Globals.suppSelf.PutFile(szLocalFile, szRemoteFile);
                        }
                    }
                    else
                    {
                        LoadRemoteDir(txtAgentCurrDir.Text);
                    }
                }
            }
        }

        private void FormFTP_Load(object sender, EventArgs e)
        {
            LoadLocalDir(txtManagerCurrDir.Text);
            LoadRemoteDir(txtAgentCurrDir.Text);
        }

        private bool UpdateDir(string szCurrDir, string szCurrSelection, ref string szNextDir)
        {
            szNextDir = szCurrDir;

            if (szCurrSelection.Equals("[..]"))
            {
                if (szCurrDir.Equals("C:\\") == false)
                {
                    if (szCurrDir.EndsWith("\\") == true)
                        szCurrDir = szCurrDir.Substring(0, szCurrDir.Length - 1);

                    int iIndex = szCurrDir.LastIndexOf("\\");
                    if (iIndex > 0)
                        szNextDir = szCurrDir.Substring(0, iIndex);
                    if (szNextDir.EndsWith("\\") == false)
                        szNextDir += "\\";
                }
            }
            else if (szCurrSelection.StartsWith("["))
            {
                string myNextDir = szCurrSelection.Substring(1, szCurrSelection.Length - 2);
                szNextDir += myNextDir + "\\";
            }

            if (szNextDir != szCurrDir)
                return (true);
            return(false);
        }

        private void LoadLocalDir(string szCurrDir)
        {
            txtManagerCurrDir.Text = szCurrDir;
            txtManagerCurrDir.ScrollToCaret();
            lstManager.Items.Clear();

            string[] filePaths = Directory.GetFiles(szCurrDir);
            string[] dirPaths = Directory.GetDirectories(szCurrDir);

            lstManager.Items.Add("[..]");
            foreach (string dirPath in dirPaths)
            {
                string dirName = "[" + Path.GetFileName(dirPath) + "]";
                lstManager.Items.Add(dirName);
            }
            foreach (string filePath in filePaths)
            {
                string fileName = Path.GetFileName(filePath);
                lstManager.Items.Add(fileName);
            }
        }

        private void LoadRemoteDir(string szCurrDir)
        {
            txtAgentCurrDir.Text = szCurrDir;
            txtAgentCurrDir.ScrollToCaret();
            lstAgent.Items.Clear();
            lstAgent.Items.Add("[..]");

            winmanager.Globals.suppSelf.ListDirectory(szCurrDir);
        }

        private void lstManager_DoubleClick(object sender, EventArgs e)
        {
            bool refreshDir;
            string currDir, currSelection, nextDir = "";

            if (lstManager.SelectedItem == null)
                return;

            currSelection = lstManager.SelectedItem.ToString();
            currDir = txtManagerCurrDir.Text;

            refreshDir = UpdateDir(currDir, currSelection, ref nextDir);
            if (refreshDir == true) {
                LoadLocalDir(nextDir);
            }
        }

        private void lstAgent_DoubleClick(object sender, EventArgs e)
        {
            bool refreshDir;
            string currDir, currSelection, nextDir = "";

            if (lstAgent.SelectedItem == null)
                return;

            currSelection = lstAgent.SelectedItem.ToString();
            currDir = txtAgentCurrDir.Text;

            refreshDir = UpdateDir(currDir, currSelection, ref nextDir);
            if (refreshDir == true)
            {
                LoadRemoteDir(nextDir);
            }

        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            if (lstManager.SelectedItems.Count == 0)
                return;

            txtSendingFile.Text = "";
            progPutFile.Value = 0;

            if (lstManager.SelectedItems.Count > 0) {
                string szLocalFile = txtManagerCurrDir.Text + lstManager.SelectedItems[0].ToString();
                string szRemoteFile = txtAgentCurrDir.Text + lstManager.SelectedItems[0].ToString();
                int iRet = winmanager.Globals.suppSelf.PutFile(szLocalFile, szRemoteFile);
            }

            btnCancel.Tag = "Put";
            btnCancel.Enabled = true;
        }

        private void btnReceive_Click(object sender, EventArgs e)
        {
            if (lstAgent.SelectedItems.Count == 0)
                return;

            txtReceivingFile.Text = "";
            progGetFile.Value = 0;

            if (lstAgent.SelectedItems.Count > 0) {
                string szLocalFile = txtManagerCurrDir.Text + lstAgent.SelectedItems[0].ToString();
                string szRemoteFile = txtAgentCurrDir.Text + lstAgent.SelectedItems[0].ToString();
                int iRet = winmanager.Globals.suppSelf.GetFile(szRemoteFile, szLocalFile);
            }

            btnCancel.Tag = "Get";
            btnCancel.Enabled = true;
        }

        private void btnChiudi_Click(object sender, EventArgs e)
        {
            FTPClosed();
        }

        private void btnManagerRefresh_Click(object sender, EventArgs e)
        {
            LoadLocalDir(txtManagerCurrDir.Text);
        }

        private void btnAgentRefresh_Click(object sender, EventArgs e)
        {
            LoadRemoteDir(txtAgentCurrDir.Text);
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            if ("Put" == btnCancel.Tag.ToString())
            {
                winmanager.Globals.suppSelf.CancelPutFile();
                btnCancel.Tag = "";
                btnCancel.Enabled = false;
            }
            if ("Get" == btnCancel.Tag.ToString())
            {
                winmanager.Globals.suppSelf.CancelGetFile();
                btnCancel.Tag = "";
                btnCancel.Enabled = false;
            }
        }
    }
}
