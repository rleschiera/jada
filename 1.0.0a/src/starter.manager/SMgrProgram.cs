using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace winmanager
{
    static class SMgrProgram
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

            Application.Run(new FormStarter("manager", winmanager.Properties.Settings.Default.JADA_URL.Trim(), winmanager.Properties.Settings.Default.CUSTOMER.Trim()));
        }
    }
}
