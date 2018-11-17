using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Net;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;

namespace JADA.Starter
{
    public class Common
    {
        [DllImport("KERNEL32.DLL", EntryPoint = "GetPrivateProfileStringW", SetLastError = true, CharSet = CharSet.Unicode, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        private static extern int GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnString, int nSize, string lpFilename);

        // Variabili globali
        private static bool myTestMode = true;
        private static string myModuleName = "";
        private static string myBaseURL = "";
        private static string myCustomer = "";
        private static string myBaseDir = "";
        private static string myIniFile = "";
        private static string myLogFile = "";

        public static void LogMessage(string szMessage)
        {
            if (myTestMode == true) {
                JADA.Starter.Common.CreateDir(myBaseDir + "log");
                File.AppendAllText(myLogFile, myModuleName + ": " + szMessage + Environment.NewLine);
            }
        }

        public static void CreateDir(string szDirName)
        {
            bool exists = System.IO.Directory.Exists(szDirName);

            if (!exists) {
                System.IO.Directory.CreateDirectory(szDirName);
                LogMessage("Creata la directory " + szDirName);
            }
        }

        public static int SetProxySettings(string szFileName)
        {
            StringBuilder szTmp = new StringBuilder(256);

            GetPrivateProfileString("base", "URL", myBaseURL, szTmp, szTmp.Capacity, myIniFile);
            string szURLBase = szTmp.ToString();
            string szURL = szURLBase + szFileName;
            UriBuilder myUri = new UriBuilder(szURL);
            IWebProxy myProxy = WebRequest.GetSystemWebProxy();
            Uri myProxyURI = myProxy.GetProxy(myUri.Uri);
            string myHost = myProxyURI.Host;
            int myPort = myProxyURI.Port;
            string szProxy = myProxyURI.ToString();
            bool isBypassed = myProxy.IsBypassed(myUri.Uri);

            return (0);
        }

        public static int DownloadFile(System.Windows.Forms.TextBox txtStatus, string szSrcFileName, string szDestFileName)
        {
            int iRetCode = -1;
            string szURLFile, szDestFile;
            // StringBuilder szTmp = new StringBuilder(256);

            /* Carica la configurazione dal file .INI, se presente */
            // GetPrivateProfileString("base", "URL", myBaseURL, szTmp, szTmp.Capacity, myIniFile);
            szURLFile = myBaseURL + szSrcFileName;
            szDestFile = myBaseDir + szDestFileName;

            try
            {
                txtStatus.Text += string.Format("Sto scaricando {0}...", szURLFile);
                txtStatus.Refresh();
                System.Net.WebClient myWebClient = new System.Net.WebClient();
                myWebClient.DownloadFile(szURLFile, szDestFile);
                LogMessage("Scaricato " + szSrcFileName + " in " + szDestFile);
                txtStatus.Text += string.Format(" fatto.\r\n");
                try
                {
                    txtStatus.SelectionStart = txtStatus.Text.Length;
                    txtStatus.ScrollToCaret();
                }
                finally
                {
                }
                iRetCode = 0;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Impossibile effettuare il download del file " + szURLFile + ":\n" + ex.Message + ". Controllare le impostazioni del proxy.");
            }

            return (iRetCode);
        }

        public static bool CheckMD5File(System.Windows.Forms.TextBox txtStatus, string szFileName, string szMD5string)
        {
            string szLocalMD5 = "", szRemoteMD5 = szMD5string.ToUpper();
            int j;
            bool iCheck = false;

            if (File.Exists(myBaseDir + szFileName))
            {
                using (var reader = new System.IO.FileStream(myBaseDir + szFileName, System.IO.FileMode.Open, System.IO.FileAccess.Read))
                {
                    byte[] szFileHash;

                    using (var md5 = new System.Security.Cryptography.MD5CryptoServiceProvider())
                    {
                        md5.ComputeHash(reader);
                        szFileHash = md5.Hash;
                    }

                    for (j = 0; j < szFileHash.Length; j++)
                    {
                        szLocalMD5 = szLocalMD5 + String.Format("{0:X2}", szFileHash[j]);
                    }
                    // txtStatus.Text += string.Format("File {0}: remote {1}, local {2}\r\n", szFileName, szRemoteMD5, szLocalMD5);

                    if (szLocalMD5 == szRemoteMD5)
                    {
                        iCheck = true;
                    }
                }

                if (myTestMode == true)
                {
                    if (szRemoteMD5 == "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
                    {
                        iCheck = true;
                    }
                }
            }

            return (iCheck);
        }

        public static void CreateShortcutToDesktop(System.Windows.Forms.TextBox txtStatus, string szLinkName, string szModuleName, string szExeName)
        {
            string deskDir = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);

            txtStatus.Text += string.Format("Sto creando sul desktop il link al file {0}...", szExeName);

            using (StreamWriter writer = new StreamWriter(deskDir + "\\" + szLinkName + " " + szModuleName + ".url"))
            {
                // [InternetShortcut]
                // URL=file:///C:/jada.test/bin/winagent.exe
                // IconIndex=0
                // IconFile=C:\jada.test\bin\wa.exe
                // IDList=
                // HotKey=0
                writer.WriteLine("[InternetShortcut]");
                writer.WriteLine("URL=file:///" + myBaseDir + szExeName);
                writer.WriteLine("IconIndex=0");
                string icon = myBaseDir + szExeName;
                icon.Replace('\\', '/');
                writer.WriteLine("IconFile=" + icon);
                writer.WriteLine("IDList=");
                writer.WriteLine("HotKey=0");
                writer.Flush();
                writer.Close();
                writer.Dispose();
            }

            txtStatus.Text += string.Format(" fatto.\r\n");
        }

        public static int Update(System.Windows.Forms.TextBox txtStatus, string myModuleName)
        {
            int iRet;
            StreamReader file = null;
            String line, action, md5, srcname, destname;
            StringBuilder szTmp = new StringBuilder(256);
            bool iCheck;

            /* Crea l'albero di directory del prodotto */
            CreateDir(myBaseDir);
            CreateDir(myBaseDir + "bin");
            CreateDir(myBaseDir + "config");
            CreateDir(myBaseDir + "log");

            /* Scarica il file di manifesto del prodotto */
            iRet = DownloadFile(txtStatus, "/" + myModuleName + ".manifest.txt", myModuleName + ".manifest.txt");
            if (iRet == 0)
            {
                try
                {
                    /* Analizza i singoli file facenti parte del prodotto */
                    file = new StreamReader(myBaseDir + myModuleName + ".manifest.txt");
                    while ((line = file.ReadLine()) != null)
                    {
                        var tokens = line.Split(' ');

                        action = tokens[0].Trim();
                        md5 = tokens[1].Trim();
                        srcname = tokens[2].Trim();
                        destname = tokens[3].Trim();

                        srcname = srcname.Replace("$Customer", myCustomer);

                        if (action == "notfound")
                        {
                            if (File.Exists(myBaseDir + destname) == false)
                            {
                                LogMessage("Il file " + destname + " non e' stato trovato: lo scarico.");
                                // txtStatus.Text += string.Format("Il file " + name + "non e' stato trovato.\r\n");
                                iRet = DownloadFile(txtStatus, srcname, destname);
                            }
                        }
                        else if (action == "updated")
                        {
                            iCheck = CheckMD5File(txtStatus, destname, md5);
                            if (iCheck == false)
                            {
                                LogMessage("Il file " + destname + " non e' aggiornato: lo scarico.");
                                // txtStatus.Text += string.Format("Il file " + name + "non e' aggiornato.\r\n");
                                iRet = DownloadFile(txtStatus, srcname, destname);
                            }
                        }
                        else if (action == "delayed")
                        {
                            iCheck = CheckMD5File(txtStatus, destname, md5);
                            if (iCheck == false)
                            {
                                LogMessage("Il file " + destname + " non e' aggiornato: lo scarico.");
                                // txtStatus.Text += string.Format("Il file " + name + "non e' aggiornato.\r\n");
                                iRet = DownloadFile(txtStatus, srcname, destname + ".delayed");
                            }
                        }
                        else if (action == "always")
                        {
                            LogMessage("Il file " + destname + " deve essere sempre aggiornato: lo scarico.");
                            // txtStatus.Text += string.Format("Il file " + name + "deve essere sempre aggiornato.\r\n");
                            iRet = DownloadFile(txtStatus, srcname, destname);
                        }
                    }
                }
                finally
                {
                    if (file != null)
                        file.Close();
                }

                File.Delete(myBaseDir + myModuleName + ".manifest.txt");

                CreateShortcutToDesktop(txtStatus, "JADA", myModuleName, "bin/win" + myModuleName + ".exe");

                txtStatus.Text += string.Format("Il prodotto è aggiornato.\r\n");
                txtStatus.Refresh();

                //iRet = starter.Common.SetProxySettings(starter.Common.DIR_BASE + myModuleName + ".manifest.txt");
            }

            return (0);
        }

        public static int Start(System.Windows.Forms.TextBox txtStatus, string myProcessName)
        {
            txtStatus.Text += string.Format("Sto per lanciare il comando " + myBaseDir + "bin/" + myProcessName);
            if (File.Exists(myBaseDir + "bin/" + myProcessName))
            {
                Process.Start(myBaseDir + "bin/" + myProcessName);
            }
            else
            {
                MessageBox.Show("Impossibile lanciare il processo: contattare l'amministratore.");
            }

            return (0);
        }

        public static int Init(string szModuleName, string szBaseURL, string szCustomer)
        {
            myModuleName = szModuleName;

            if (myTestMode == true)
            {
                myBaseDir = "C:/jada.test/";
                myBaseURL = "http://jadatest/jada/";
                myCustomer = "test";
            }
            else
            {
                myBaseDir = "C:/jada/";
                myBaseURL = szBaseURL;
                if (szBaseURL.EndsWith("/"))
                {
                    myBaseURL = myBaseURL + "/";
                }
                myCustomer = szCustomer;
                if (myBaseURL.StartsWith("http://AAA.BBB.CCC.DDD"))
                {
                    MessageBox.Show("Modulo JADA non inizializzato (URL): controllare le impostazioni.");
                    return (-1); 
                }
                if (myCustomer.StartsWith("EFGHI"))
                {
                    MessageBox.Show("Modulo JADA non inizializzato (Customer): controllare le impostazioni.");
                    return (-1);
                }
            }

            myIniFile = myBaseDir + "config/starter.ini";
            myLogFile = myBaseDir + "log/starter.log";

            return (0);
        }
    }
}
