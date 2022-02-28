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

            Lucid.Core.Profiler profiler = new Lucid.Core.Profiler();

            Planner planner = new Planner();
            planner.Show();

            while (!planner.IsDisposed)
            {
                planner.updateSimulation();
                planner.renderMainView();
                Application.DoEvents();
            }

            profiler.dump("profile.log");
        }
    }
}
