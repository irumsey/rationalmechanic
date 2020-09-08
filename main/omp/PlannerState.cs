using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace omp
{

    /// <summary>
    /// 
    /// </summary>
    partial class Planner
    {

        /// <summary>
        /// </summary>
        private abstract class State
        {
            public virtual void onEnter(Planner planner) { }

            public virtual void onLeave(Planner planner) { }

            public virtual void onPaint(Planner planner) { }

            public virtual void onMainViewResize(Planner planner) { }

            public virtual void updateSimulation(Planner planner) { }

            public virtual void renderMainView(Planner planner) { }
        }

        /// <summary>
        /// </summary>
        private class Stopped : State
        {
            public static readonly State Instance = new Stopped();

            static Stopped() { }

            private Stopped() { }
        }

        /// <summary>
        /// </summary>
        private class Starting : State
        {
            public static readonly State Instance = new Starting();

            static Starting() { }

            private Starting() { }

            private void testPopulateListview(Planner planner, lucid.OrbitalFrame frame)
            {
                ListViewItem item = new ListViewItem();
                item.SubItems.Add(frame.ID.ToString());
                item.SubItems.Add(frame.Name);
                item.SubItems.Add(frame.Description);
                item.StateImageIndex = 0;
                planner.orbitalFrameList.Items.Add(item);

                for (lucid.OrbitalFrame childFrame = frame.FirstChild; childFrame != null; childFrame = childFrame.NextSibling)
                {
                    testPopulateListview(planner, childFrame);
                }
            }

            public override void onEnter(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;

                unsafe { lucid.Pipeline.initialize(clientSize.Width, clientSize.Height, 2, panel.Handle.ToPointer()); }

                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;
                planner.camera = new lucid.Camera2D();
                planner.renderContext = new lucid.Context("content/test.context");

                planner.camera.InitPerspective(0.25f * 3.1415926f, planner.aspectRatio, 1, 1000);
                planner.camera.Look(new lucid.Vector3(60, 60, 60), new lucid.Vector3(0, 0, 0), new lucid.Vector3(0, 0, 1));

                // test {
                // initial user action should be to specify an ephemeris
                // or to have a default that is user specified.
                planner.orbitalMechainics = new lucid.OrbitalMechanics("content/j2000.ephemeris");
                planner.orbitalMechainics.Initialize(2451544.0);
                // } test

                // test {
                lucid.OrbitalFrame rootFrame = planner.orbitalMechainics.RootFrame();
                testPopulateListview(planner, rootFrame);
                // } test

                planner.setMainMenuDefaults();
                planner.changeState(Editing.Instance);
            }
        }

        /// <summary>
        /// </summary>
        private class Editing : State
        {
            public static readonly State Instance = new Editing();

            static Editing() { }

            private Editing() { }

            public override void onEnter(Planner planner) { }

            public override void onLeave(Planner planner) { }

            public override void onPaint(Planner planner)
            {
                planner.renderMainView();
            }

            public override void onMainViewResize(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;
                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

                lucid.Pipeline.resize(clientSize.Width, clientSize.Height);

                planner.camera.InitPerspective(0.25f * 3.1415926f, planner.aspectRatio, 1, 1000);
                planner.renderMainView();
            }

            public override void updateSimulation(Planner planner)
            {
                /// test {
                lucid.Vector3 viewPosition = (null != planner.trackedFrame)
                    ? planner.trackedFrame.Position + new lucid.Vector3(20, 20, 20)
                    : planner.camera.position;
                lucid.Vector3 focus = (null != planner.trackedFrame)
                    ? planner.trackedFrame.Position
                    : new lucid.Vector3(0, 0, 0);
                /// } test
                
                planner.camera.Look(viewPosition, focus, new lucid.Vector3(0, 0, 1));
                planner.renderContext.Set(planner.camera);

                planner.orbitalMechainics.Update();
            }

            public override void renderMainView(Planner planner)
            {
                lucid.Pipeline.beginScene();
                    lucid.Pipeline.clear(new lucid.Color(0, 0, 0, 0), 1.0f);
                    planner.orbitalMechainics.Render(planner.renderContext);
                lucid.Pipeline.endScene();
            }
        }

        /// <summary>
        /// </summary>
        private class Stopping : State
        {
            public static readonly State Instance = new Stopping();

            static Stopping() { }

            private Stopping() { }

            public override void onEnter(Planner planner)
            {
                // TBD: clean up work here...

                Application.Exit();
                planner.changeState(Stopped.Instance);
            }
        }

    }   // planner
}       // omp
