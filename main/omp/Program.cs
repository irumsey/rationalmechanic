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

            Log theLog = new Log();
            theLog.Start();

            Lucid.Core.Profiler profiler = new Lucid.Core.Profiler();

            try
            {
                Planner planner = new Planner();
                planner.Show();

                while (!planner.IsDisposed)
                {
                    planner.updateSimulation();
                    planner.renderMainView();
                    Application.DoEvents();
                }
            }
            catch
            {
                /// if an exception leaks out, got to manually shutdown Lucid.Orbit singletons
                Lucid.Orbit.Ephemeris.Shutdown();
                Lucid.Orbit.StarCatalog.Shutdown();
            }
            
            profiler.dump("profile.log");
            theLog.Stop();
        }
    }
}
