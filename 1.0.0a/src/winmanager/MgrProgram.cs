using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using JADA;
using System.IO;

namespace winmanager
{

    public static class Globals
    {
        public const string PROCESS_NAME = "JADA Manager";

        public static JADA.Common jadaSelf = null;
        public static JADA.Connection connSelf = null;
        public static JADA.Support suppSelf = null;
        public static JADA.VNC vncSelf = null;
        public static MgrFormMain frmMain = null;
    }

    static class MgrProgram
    {
        /// <summary>
        /// Punto di ingresso principale dell'applicazione.
        /// </summary>

        [STAThread]
        static void Main()
        {
            int iRet;

            // Inizializza le classi della libreria JADA
            winmanager.Globals.jadaSelf = new JADA.Common();
            iRet = winmanager.Globals.jadaSelf.Start(JADA.Common.WHOAMI_MANAGER);

            // Crea le classi fondamentali della libreria JADA
            winmanager.Globals.connSelf = new JADA.Connection();
            if (winmanager.Globals.connSelf == null)
            {
                MessageBox.Show("Errore nella creazione dell'oggetto Session", winmanager.Globals.PROCESS_NAME);
                Environment.Exit(0);
            }
            iRet = winmanager.Globals.connSelf.Connect(JADA.Common.SESSTYPE_SUPPORT);
            if (iRet != 0)
            {
                MessageBox.Show("Errore nella connessione al server JADA", winmanager.Globals.PROCESS_NAME);
                Environment.Exit(0);
            }
            winmanager.Globals.suppSelf = new JADA.Support(winmanager.Globals.connSelf);
            if (winmanager.Globals.suppSelf == null)
            {
                MessageBox.Show("Errore nella creazione dell'oggetto Support", winmanager.Globals.PROCESS_NAME);
                Environment.Exit(0);
            }
            winmanager.Globals.vncSelf = new JADA.VNC(winmanager.Globals.connSelf);
            if (winmanager.Globals.vncSelf == null)
            {
                MessageBox.Show("Errore nella creazione dell'oggetto VNC", winmanager.Globals.PROCESS_NAME);
                Environment.Exit(0);
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            try
            {
                winmanager.Globals.frmMain = new MgrFormMain();
                if (winmanager.Globals.frmMain.CloseManager == false)
                {
                    Application.Run(winmanager.Globals.frmMain);
                }
            }
            catch (Exception e)
            {
                JADA.Common.LogMessage(String.Format("FormMain {0}\n{1}", e.Message, e.StackTrace));
                MessageBox.Show("Errore nell'applicazione Manager: i dettagli sono stati salvati nel file di log", winmanager.Globals.PROCESS_NAME);
            }
            Environment.Exit(0);
        }
    }
}
