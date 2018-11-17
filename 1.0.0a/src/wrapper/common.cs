using System;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Security.Cryptography;
using System.Net;
using System.Windows.Forms;
using System.Drawing;

namespace JADA
{
    public class Common
    {
        // Costanti di utilizzo
        public const int WHOAMI_UNKNOWN = 1111;
        public const int WHOAMI_AGENT = 2222;
        public const int WHOAMI_MANAGER = 3333;
        public const int WHOAMI_SERVER_FTP = 4444;
        public const int WHOAMI_SERVER_MEDIA = 5555;
        public const int WHOAMI_SERVER_SUPPORT = 6666;
        public const int WHOAMI_SERVER_VNC = 7777;
        public const int WHOAMI_SERVER_ALL = 8888;
        
        public const int CHATTEXT_SIZE = 1024;
        public const int TRUE = 1;
        public const int FALSE = -1;
        public const int PROXY_AUTHENTICATE = -9876;
        public const int CALL_TIMEOUT = 20;
        
        public const int CREDENTIAL_OK = 121;
        public const int CREDENTIAL_UNKNOWN_USER = 232;
        public const int CREDENTIAL_WRONG_PASSWORD = 343;

        public const int POINTER_HAND = 641;
        public const int POINTER_MOUSE_HAND = 642;
        public const int POINTER_ARROW = 643;
        public const int POINTER_CURVED_ARROW = 644;
        public const int POINTER_ROUND_ARROW = 645;

        public const int SESSTYPE_FTP = 3131;
        public const int SESSTYPE_CALL = 4242;
        public const int SESSTYPE_SUPPORT = 5353;
        public const int SESSTYPE_VNC = 6464;

        public const int COMBOTYPE_REGIONE = 4101;
        public const int COMBOTYPE_CLIENTE = 4202;
        public const int COMBOTYPE_CODA = 4303;
        public const int COMBOTYPE_MANAGER_CODA = 4404;

        // Funzioni native nella DLL: common
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_Start(int iWhoAmI, string szHomeDir);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_Stop();
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern string JADA_TimeToString(long timSelf, StringBuilder szMyStringTime, int iLength);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_LogMessage(string szMessage);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_LogError(string szMessage);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_IntToRequestId(int irequestId, ref int piReqId);

        // Funzioni native nella DLL: TCP
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_TcpSetUseProxy(int iUseProxy);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_TcpGetUseProxy(ref int iUseProxy);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_TcpSetProxyAddress(string szProxyName, int iPort);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_TcpSetProxyCredentials(string szUser, string szPassword, string szOpenPassword);
        [DllImport("jada.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        static extern int JADA_TcpGetProxyCredentials(StringBuilder szUser, int iUserLength, StringBuilder szPassword, int iPasswordLength);

        public int Start(int iWhoAmI)
        {
            int iRet;

            iRet = JADA_Start(iWhoAmI, "C:\\jada");

            if (File.Exists("C:/jada/bin/winagent.exe.delayed") == true) {
                File.Copy("C:/jada/bin/winagent.exe.delayed", "C:/jada/bin/winagent.exe", true);
                File.Delete("C:/jada/bin/winagent.exe.delayed");
            }
            if (File.Exists("C:/jada/bin/winmanager.exe.delayed") == true)
            {
                File.Copy("C:/jada/bin/winmanager.exe.delayed", "C:/jada/bin/winmanager.exe", true);
                File.Delete("C:/jada/bin/winmanager.exe.delayed");
            }

            return (iRet);
        }

        public int Stop(int iWhoAmI)
        {
            int iRet;

            iRet = JADA_Stop();

            return (iRet);
        }

        public static int GetProxyCredentials(StringBuilder szUser, StringBuilder szPassword)
        {
            int iRet;

            iRet = JADA_TcpGetProxyCredentials(szUser, szUser.Capacity, szPassword, szPassword.Capacity);

            return (iRet);
        }

        public static int SetProxyCredentials(string szUser, string szPassword, string szOpenPassword)
        {
            int iRet;

            iRet = JADA_TcpSetProxyCredentials(szUser, szPassword, szOpenPassword);

            return (iRet);
        }

        public static bool UseProxy
        {
            get
            {
                int iRet, iUseProxy = 0;

                iRet = JADA_TcpGetUseProxy(ref iUseProxy);
                if (iRet == 0) {
                    return(Convert.ToBoolean(iUseProxy));
                }
                return (false);
            }
            set
            {
                int iRet;

                iRet = JADA_TcpSetUseProxy(Convert.ToInt32(value));
            }
        }

        public static int LogMessage(string szMessage)
        {
            int iRet;
            string myMessage = String.Format("{0}\n", szMessage);

            iRet = JADA_LogMessage(myMessage);

            return (iRet);
        }

        public static int LogError(string szMessage)
        {
            int iRet;
            string myMessage = String.Format("{0}\n", szMessage);

            iRet = JADA_LogError(myMessage);

            return (iRet);
        }

        public static string TimeToString(long timTime)
        {
            StringBuilder szStringTime = new StringBuilder(64);

            JADA_TimeToString(timTime, szStringTime, szStringTime.Capacity);

            return (szStringTime.ToString());
        }

        public static string WhoamiToString(int iWhoAmI)
        {
            switch (iWhoAmI) {
            case WHOAMI_UNKNOWN:
                return ("Sconosciuto");
            case WHOAMI_AGENT:
                return ("Agent");
            case WHOAMI_MANAGER:
                return ("Manager");
            case WHOAMI_SERVER_ALL:
                return ("Server");
            default:
                return ("???");
            }
        }

        //public static int SetProxySettings()
        //{
        //    int     iRet;

        //    string szURL = "http://www.google.it/";
        //    UriBuilder myUri = new UriBuilder(szURL);
        //    IWebProxy myProxy = WebRequest.GetSystemWebProxy();
        //    Uri myProxyURI = myProxy.GetProxy(myUri.Uri);
        //    string myHost = myProxyURI.Host;
        //    int myPort = myProxyURI.Port;
        //    string szProxy = myProxyURI.ToString();
        //    bool isBypassed = myProxy.IsBypassed(myUri.Uri);
        //    System.Net.ICredentials myCredentials = myProxy.Credentials;

        //    if (szProxy != szURL)
        //    {
        //        iRet = JADA_TcpSetProxyAddress(myHost, myPort);
        //    }
        //    else
        //    {
        //        iRet = JADA_TcpSetProxyAddress("", 0);
        //    }

        //    return (0);
        //}

        public static string __FILE__
        {
            get
            {
                StackFrame CallStack = new StackFrame(1, true);
                string temp = CallStack.GetFileName();
                String file = String.Copy(String.IsNullOrEmpty(temp) ? "" : temp);
                return String.IsNullOrEmpty(file) ? "" : file;
            }
        }

        public static int __LINE__
        {
            get
            {
                StackFrame CallStack = new StackFrame(1, true);
                int line = new int();
                line += CallStack.GetFileLineNumber();
                return line;
            }
        }
    }

    public class SimpleAES
    {
        private static byte[] key = { 123, 217, 19, 11, 24, 26, 85, 45, 114, 184, 27, 162, 37, 112, 222, 209, 241, 24, 175, 144, 173, 53, 196, 29, 24, 26, 17, 218, 131, 236, 53, 209 };
        private static byte[] vector = { 146, 64, 191, 111, 23, 3, 113, 119, 231, 121, 221, 112, 79, 32, 114, 156 };
        private ICryptoTransform encryptor, decryptor;
        private UTF8Encoding encoder;

        public SimpleAES()
        {
            RijndaelManaged rm = new RijndaelManaged();
            encryptor = rm.CreateEncryptor(key, vector);
            decryptor = rm.CreateDecryptor(key, vector);
            encoder = new UTF8Encoding();
        }

        public string Encrypt(string unencrypted)
        {
            string szRet = "";

            try
            {
                szRet = Convert.ToBase64String(Encrypt(encoder.GetBytes(unencrypted)));
            }
            catch
            {
            }

            return szRet;
        }

        public string Decrypt(string encrypted)
        {
            string szRet = "";

            try
            {
                szRet = encoder.GetString(Decrypt(Convert.FromBase64String(encrypted)));
            }
            catch
            {
            }

            return szRet;
        }

        public byte[] Encrypt(byte[] buffer)
        {
            return Transform(buffer, encryptor);
        }

        public byte[] Decrypt(byte[] buffer)
        {
            return Transform(buffer, decryptor);
        }

        protected byte[] Transform(byte[] buffer, ICryptoTransform transform)
        {
            MemoryStream stream = new MemoryStream();
            using (CryptoStream cs = new CryptoStream(stream, transform, CryptoStreamMode.Write))
            {
                cs.Write(buffer, 0, buffer.Length);
            }
            return stream.ToArray();
        }
    }
}
