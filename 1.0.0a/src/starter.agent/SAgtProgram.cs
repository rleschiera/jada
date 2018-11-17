using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace winagent
{
    static class SAgtProgram
    {
        /// <summary>
        /// Punto di ingresso principale dell'applicazione.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string[] args = Environment.GetCommandLineArgs();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            if (args.Length > 1)
            {
                JADA.Starter.Common.SetTestMode(true);   
            }            
            
            Application.Run(new FormStarter("agent", winagent.Properties.Settings.Default.JADA_URL.Trim(), winagent.Properties.Settings.Default.CUSTOMER.Trim()));
        }
    }
}
