using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace omp
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            lucid.Profiler profiler = new lucid.Profiler();

            MainForm mainForm = new MainForm();
            mainForm.Show();

            while (!mainForm.IsDisposed)
            {
                mainForm.advanceSimulation();
                mainForm.render();
                Application.DoEvents();
            }

            /// TBD: destroy profiler ???
        }
    }
}
