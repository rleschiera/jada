using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using JADA;
using System.IO;

namespace winagent
{

    public static class Globals
    {
        public const string PROCESS_NAME = "JADA Agent";

        public static JADA.Common jadaSelf = null;
        public static JADA.Connection connSelf = null;
        public static JADA.Support suppSelf = null;
        public static JADA.VNC vncSelf = null;
        public static AgtFormMain frmMain = null;
        public static AgtFormDraw frmDraw = null;
    }

    static class AgtProgram
    {
        /// <summary>
        /// Punto di ingresso principale dell'applicazione.
        /// </summary>

        [STAThread]
        static void Main()
        {
            int iRet;

            // Inizializza le classi della libreria JADA
            winagent.Globals.jadaSelf = new JADA.Common();
            iRet = winagent.Globals.jadaSelf.Start(JADA.Common.WHOAMI_AGENT);

            // Crea le classi fondamentali della libreria JADA
            winagent.Globals.connSelf = new JADA.Connection();
            if (winagent.Globals.connSelf == null)
            {
                MessageBox.Show("Errore nella creazione dell'oggetto Connection", winagent.Globals.PROCESS_NAME);
                Environment.Exit(0);
            }
            iRet = winagent.Globals.connSelf.Connect(JADA.Common.SESSTYPE_SUPPORT);
            if (iRet != 0)
            {
                MessageBox.Show("Errore nella connessione al server JADA", winagent.Globals.PROCESS_NAME);
                Environment.Exit(0);
            }
            winagent.Globals.suppSelf = new JADA.Support(winagent.Globals.connSelf);
            if (winagent.Globals.suppSelf == null)
            {
                MessageBox.Show("Errore nella creazione dell'oggetto Support", winagent.Globals.PROCESS_NAME);
                Environment.Exit(0);
            }
            winagent.Globals.vncSelf = new JADA.VNC(winagent.Globals.connSelf);
            if (winagent.Globals.vncSelf == null)
            {
                MessageBox.Show("Errore nella creazione dell'oggetto VNC", winagent.Globals.PROCESS_NAME);
                Environment.Exit(0);
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            try
            {
                winagent.Globals.frmMain = new AgtFormMain();
                if (winagent.Globals.frmMain.CloseAgent == false)
                {
                    Application.Run(winagent.Globals.frmMain);
                }
            }
            catch (Exception e)
            {
                JADA.Common.LogMessage(String.Format("FormMain {0}\n{1}", e.Message, e.StackTrace));
                MessageBox.Show("Errore nell'applicazione Agent: i dettagli sono stati salvati nel file di log", winagent.Globals.PROCESS_NAME);
            }
            Environment.Exit(0);
        }
    }
}
