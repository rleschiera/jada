using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections.ObjectModel;

namespace make_manifest
{
    class Program
    {
        struct ManifestFile
        {
            private readonly string myMode;
            private readonly string myFile;
            private readonly string myDestDir;

            public ManifestFile (string mode, string file, string destDir)
            {
                this.myMode = mode;
                this.myFile = file;
                this.myDestDir = destDir;
            }

            public string Mode { get { return myMode; } }
            public string File { get { return myFile; } }
            public string DestDir { get { return myDestDir; } }
        }

        static readonly IList<ManifestFile> AgentList = new ReadOnlyCollection<ManifestFile>
        (new[] {
             new ManifestFile ("notfound", "clienti/$Customer/winagent.exe", "bin/winagent.exe"),
             new ManifestFile ("delayed",  "clienti/$Customer/winagent.exe", "bin/winagent.exe"),
             new ManifestFile ("notfound", "clienti/$Customer/agent.ini.txt", "config/agent.ini"),
             new ManifestFile ("updated",  "clienti/$Customer/server.ini.txt", "config/server.ini"),
             new ManifestFile ("updated",  "download/jada.dll", "bin/jada.dll"),
             new ManifestFile ("updated",  "download/jadawrapper.dll", "bin/jadawrapper.dll"),
             new ManifestFile ("updated",  "download/msvcp100d.dll", "bin/msvcp100d.dll"),
             new ManifestFile ("updated",  "download/msvcr100d.dll", "bin/msvcr100d.dll"),
             new ManifestFile ("updated",  "download/pthreadVC.dll", "bin/pthreadVC.dll"),
             new ManifestFile ("updated",  "download/UltraVNC.ini.txt", "bin/UltraVNC.ini"),
             new ManifestFile ("updated",  "download/jadavncserver.exe", "bin/jadavncserver.exe"),
             new ManifestFile ("updated",  "download/wa.exe", "bin/wa.exe"),
             new ManifestFile ("updated",  "download/zlib.dll", "bin/zlib.dll")
        });

        static readonly IList<ManifestFile> ManagerList = new ReadOnlyCollection<ManifestFile>
        (new[] {
             new ManifestFile ("notfound", "clienti/$Customer/winmanager.exe", "bin/winmanager.exe"),
             new ManifestFile ("delayed",  "clienti/$Customer/winmanager.exe", "bin/winmanager.exe"),
             new ManifestFile ("notfound", "clienti/$Customer/manager.ini.txt", "config/manager.ini"),
             new ManifestFile ("updated",  "clienti/$Customer/server.ini.txt", "config/server.ini"),
             new ManifestFile ("updated",  "download/jada.dll", "bin/jada.dll"),
             new ManifestFile ("updated",  "download/jadawrapper.dll", "bin/jadawrapper.dll"),
             new ManifestFile ("updated",  "download/msvcp100d.dll", "bin/msvcp100d.dll"),
             new ManifestFile ("updated",  "download/msvcr100d.dll", "bin/msvcr100d.dll"),
             new ManifestFile ("updated",  "download/pthreadVC.dll", "bin/pthreadVC.dll"),
             new ManifestFile ("updated",  "download/UltraVNC.ini.txt", "bin/UltraVNC.ini"),
             new ManifestFile ("updated",  "download/jadavncviewer.exe", "bin/jadavncviewer.exe"),
             new ManifestFile ("updated",  "download/WebCamCapture.dll", "bin/WebCamCapture.dll"),
             new ManifestFile ("updated",  "download/wm.exe", "bin/wm.exe"),
             new ManifestFile ("updated",  "download/zlib.dll", "bin/zlib.dll")
        });

        static void Main(string[] args)
        {
            int i;
            string szCustomer, szFullFileName, szMD5string;
            StreamWriter myManifest;

            if (args.Length == 0) {
                WaitKey("Il nome del cliente non è definito.\n");
                return;
            }
            szCustomer = args[0];
            if (szCustomer.Length == 0)
            {
                WaitKey("Il nome del cliente non è definito.\n");
                return;
            }

            File.Delete("agent.manifest.txt");
            myManifest = new StreamWriter("agent.manifest.txt");
            for (i = 0; i < AgentList.Count; i++)
            {
                szFullFileName = "C:/jada/site/" + AgentList[i].File;
                szFullFileName = szFullFileName.Replace("$Customer", szCustomer);

                szMD5string = EvalMD5File(szFullFileName);
                if (szMD5string.Length == 0)
                    szMD5string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                myManifest.WriteLine(AgentList[i].Mode + " " + szMD5string + " " + AgentList[i].File + " " + AgentList[i].DestDir);
            }
            myManifest.Close();

            File.Delete("manager.manifest.txt");
            myManifest = new StreamWriter("manager.manifest.txt");
            for (i = 0; i < ManagerList.Count; i++)
            {
                szFullFileName = "C:/jada/site/" + ManagerList[i].File;
                szFullFileName = szFullFileName.Replace("$Customer", szCustomer);

                szMD5string = EvalMD5File(szFullFileName);
                if (szMD5string.Length == 0)
                    szMD5string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                myManifest.WriteLine(ManagerList[i].Mode + " " + szMD5string + " " + ManagerList[i].File + " " + ManagerList[i].DestDir);
            }
            myManifest.Close();

            WaitKey("Il manifest e' stato creato.\n");
        }

        public static string EvalMD5File(string szFileName)
        {
            string szFileMD5 = "";
            int i;

            if (File.Exists(szFileName))
            {
                using (var reader = new System.IO.FileStream(szFileName, System.IO.FileMode.Open, System.IO.FileAccess.Read))
                {
                    byte[] szFileHash;

                    using (var md5 = new System.Security.Cryptography.MD5CryptoServiceProvider())
                    {
                        md5.ComputeHash(reader);
                        szFileHash = md5.Hash;
                    }

                    for (i = 0; i < szFileHash.Length; i++)
                    {
                        szFileMD5 = szFileMD5 + String.Format("{0:X2}", szFileHash[i]);
                    }
                    return(szFileMD5.ToUpper());  
                }
            }

            return ("");
        }

        public static void WaitKey(string szPrompt)
        {
            Console.Write(szPrompt);
            //Console.Write("Premi Invio per continuare...\n");
            //do
            //{
            //    while (! Console.KeyAvailable) {
            //    }       
            //} while (Console.ReadKey(true).Key != ConsoleKey.Enter);
        }
    }
}
