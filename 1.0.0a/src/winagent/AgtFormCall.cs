using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.Runtime.Serialization;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;

namespace winagent
{
    public partial class AgtFormCall : Form
    {
        [DllImport("user32.dll")]
        static extern IntPtr GetDC(IntPtr hWnd);

        private bool myCallStatus = false;
        private bool myMuteStatus = false;

        public AgtFormCall()
        {
            InitializeComponent();
        }


        //********************************************************************* 
        //
        //     G E S T I O N E     E V E N T I
        // 
        //********************************************************************* 
        public void CallOpened()
        {
            myCallStatus = true;

            myMuteStatus = false;
            if (btnMute.InvokeRequired)
            {
                btnMute.Invoke(new MethodInvoker(delegate
                {
                    btnCloseCall.Enabled = true;
                    btnMute.Enabled = true;
                    btnMute.Image = winagent.Properties.Resources.microphone_on;
                }));
            }
            else
            {
                btnCloseCall.Enabled = true;
                btnMute.Enabled = true;
                btnMute.Image = winagent.Properties.Resources.microphone_on;
            }

            if (btnCallReset.InvokeRequired)
            {
                btnCallReset.Invoke(new MethodInvoker(delegate
                {
                    btnCallReset.Enabled = true;
                }));
            }
            else
            {
                btnCallReset.Enabled = true;
            }

            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(delegate
                {
                    this.Show();
                }));
            }
            else
            {
                this.Show();
            }
        }

        public void CallClosed()
        {
            myCallStatus = false;

            myMuteStatus = false;
            if (btnMute.InvokeRequired)
            {
                btnMute.Invoke(new MethodInvoker(delegate
                {
                    btnCloseCall.Enabled = false;
                    btnCallReset.Enabled = false;
                    btnMute.Enabled = false;
                    btnCloseCall.Image = winagent.Properties.Resources.call_off;
                    btnMute.Image = winagent.Properties.Resources.microphone_off;
                    picVideo.InitialImage = null;
                    picVideo.Image = null;
                    picVideo.Invalidate();
                }));
            }
            else
            {
                btnCloseCall.Enabled = false;
                btnCallReset.Enabled = false;
                btnMute.Enabled = false;
                btnCloseCall.Image = winagent.Properties.Resources.call_off;
                btnMute.Image = winagent.Properties.Resources.microphone_off;
                picVideo.InitialImage = null;
                picVideo.Image = null;
                picVideo.Invalidate();
            }

            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(delegate
                {
                    this.Hide();
                }));
            }
            else
            {
                this.Hide();
            }
        }

        public void VideoFrameReceived(JADA.Frame frmSelf)
        {
            MemoryStream myStream = frmSelf.Stream;

            if (myCallStatus == true) {
                if (picVideo.InvokeRequired)
                {
                    picVideo.Invoke(new MethodInvoker(delegate
                    {
                        picVideo.Image = Image.FromStream(myStream);
                    }));
                }
                else
                {
                    picVideo.Image = Image.FromStream(myStream);
                }
            }
        }

        //********************************************************************* 
        //
        //     E V E N T I     G U I
        // 
        //********************************************************************* 
        private void btnMute_Click(object sender, EventArgs e)
        {
            int iRet;

            if (myMuteStatus == false) {
                myMuteStatus = true;
                iRet = winagent.Globals.suppSelf.MuteAudio(1);   
                btnMute.Image = winagent.Properties.Resources.microphone_off;
            }
            else {
                myMuteStatus = false;
                iRet = winagent.Globals.suppSelf.MuteAudio(0);
                btnMute.Image = winagent.Properties.Resources.microphone_on;
            }
        }

        private void btnCloseCall_Click(object sender, EventArgs e)
        {
            winagent.Globals.suppSelf.CloseCall();
            this.Hide();
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            winagent.Globals.suppSelf.SetAudioVolume(trackBar1.Value);
        }

        private void FormCall_FormClosing(object sender, FormClosingEventArgs e)
        {
            winagent.Globals.suppSelf.CloseCall();
        }

        private void btnCallReset_Click(object sender, EventArgs e)
        {
            winagent.Globals.suppSelf.ResetCall(); 
        }
    }
}
