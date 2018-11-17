using System;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Security.Cryptography;
using System.Net;
using System.Windows.Forms;
using System.Drawing;
using System.Reflection;

namespace JADA
{
    public class GUI
    {
        public static void LoadCombo(JADA.Connection connSelf, int iComboType, string szFilter, ComboBox comboBox, bool HasEmptyValue)
        {
            int iValues = 0;
            JADA.Stream myStream;
            string myLine;

            myStream = connSelf.GetCombo(iComboType, szFilter);
            if (myStream != null)
            {
                comboBox.Items.Clear();
                if (HasEmptyValue == true)
                {
                    comboBox.Items.Add(""); iValues++;
                }

                myLine = myStream.GetFirstLine(); 
                while (myLine != "")
                {
                    if (myLine.Length > 0)
                        comboBox.Items.Add(myLine); iValues++;
                    myLine = myStream.GetNextLine();
                }

                comboBox.SelectedIndex = -1;
                //if (iValues > 0) 
                //    comboBox.SelectedIndex = 1;
            }
        }

        public static void SelectComboValue(ComboBox comboBox, string szValue)
        {
            int i;

            for (i = 0; i < comboBox.Items.Count; i++)
            {
                if (comboBox.Items[i].Equals(szValue))
                {
                    comboBox.SelectedIndex = i;
                    return;
                }
            }
        }

        public static void SetControlText(System.Windows.Forms.Control ctlSelf, string Value)
        {
            if (ctlSelf.InvokeRequired)
            {
                ctlSelf.Invoke(new MethodInvoker(delegate
                {
                    ctlSelf.Text = Value;
                }));
            }
            else
            {
                ctlSelf.Text = Value;
            }
        }

        public static void AddControlText(System.Windows.Forms.TextBox ctlSelf, string Value)
        {
            if (ctlSelf.InvokeRequired)
            {
                ctlSelf.Invoke(new MethodInvoker(delegate
                {
                    if (ctlSelf.Text.Length > 0) 
                        ctlSelf.Text += Environment.NewLine;
                    ctlSelf.Text += Value;
                    ctlSelf.SelectionStart = ctlSelf.Text.Length;
                    ctlSelf.ScrollToCaret();
                }));
            }
            else
            {
                if (ctlSelf.Text.Length > 0)
                    ctlSelf.Text += Environment.NewLine;
                ctlSelf.Text += Value;
                ctlSelf.SelectionStart = ctlSelf.Text.Length;
                ctlSelf.ScrollToCaret();
            }
        }

        public static void SetControlToolTip(System.Windows.Forms.ToolTip toolTip, System.Windows.Forms.Control ctlSelf, string Value)
        {
            if (ctlSelf.InvokeRequired)
            {
                ctlSelf.Invoke(new MethodInvoker(delegate
                {
                    toolTip.SetToolTip(ctlSelf, Value);
                }));
            }
            else
            {
                toolTip.SetToolTip(ctlSelf, Value);
            }
        }

        public static void SetControlEnabled(System.Windows.Forms.Control ctlSelf, bool Value)
        {
            if (ctlSelf.InvokeRequired)
            {
                ctlSelf.Invoke(new MethodInvoker(delegate
                {
                    ctlSelf.Enabled = Value;
                }));
            }
            else
            {
                ctlSelf.Enabled = Value;
            }
        }

        public static void SetControlBackColor(System.Windows.Forms.Control ctlSelf, Color Value)
        {
            if (ctlSelf.InvokeRequired)
            {
                ctlSelf.Invoke(new MethodInvoker(delegate
                {
                    ctlSelf.BackColor = Value;
                }));
            }
            else
            {
                ctlSelf.BackColor = Value;
            }
        }

        public static void SetControlForeColor(System.Windows.Forms.Control ctlSelf, Color Value)
        {
            if (ctlSelf.InvokeRequired)
            {
                ctlSelf.Invoke(new MethodInvoker(delegate
                {
                    ctlSelf.ForeColor = Value;
                }));
            }
            else
            {
                ctlSelf.ForeColor = Value;
            }
        }

        public static void ShowAssemblyInfo()
        {
            Version verAgent, verManager, verWrapper, verVideoMaker, verAgentStarter = null, verManagerStarter = null;

            verAgent = Assembly.LoadFrom(string.Format("wa.exe", "assemblyshortname")).GetName().Version;
            verManager = Assembly.LoadFrom(string.Format("wm.exe", "assemblyshortname")).GetName().Version;
            verWrapper = Assembly.LoadFrom(string.Format("jadawrapper.dll", "assemblyshortname")).GetName().Version;
            verVideoMaker = Assembly.LoadFrom(string.Format("videomaker.exe", "assemblyshortname")).GetName().Version;
            if (File.Exists("C:/jada/bin/winagent.exe"))
            {
                verAgentStarter = Assembly.LoadFrom(string.Format("winagent.exe", "assemblyshortname")).GetName().Version;
            }
            if (File.Exists("C:/jada/bin/winmanager.exe"))
            {
                verManagerStarter = Assembly.LoadFrom(string.Format("winmanager.exe", "assemblyshortname")).GetName().Version;
            }

            FormShowVersion frmShowVersion = new FormShowVersion(verAgent, verManager, verWrapper, verVideoMaker, verAgentStarter, verManagerStarter);
            frmShowVersion.ShowDialog();
        }
    }
}
