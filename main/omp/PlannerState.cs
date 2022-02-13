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
    /// </summary>
    partial class Planner
    {

        /// <summary>
        /// </summary>
        private abstract class State
        {
            public virtual void onEnter(Planner planner) { }

            public virtual void onLeave(Planner planner) { }

            public virtual void onMainViewResize(Planner planner) { }

            public virtual void onPaint(Planner planner) { }

            public virtual void onTrackFrame(Planner planner, ListViewItem item) { }

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

            // test {
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
            // } test

            public override void onEnter(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;

                unsafe { lucid.Pipeline.initialize(clientSize.Width, clientSize.Height, 1, panel.Handle.ToPointer()); }

                // test {
                // initial user action should be to specify an ephemeris
                // or to have a default that is user specified.
                planner.orbitalMechainics = new lucid.OrbitalMechanics("content/bsc5.starcatalog", "content/j2000.ephemeris", 2451544.0);
                testPopulateListview(planner, planner.orbitalMechainics.RootFrame());
                // } test

                planner.renderContext = new lucid.Context("content/render.context");

                planner.cameraFrame = planner.orbitalMechainics.CreateFrame(1, "camera", "dynamic frame for camera");
                planner.orbitalMechainics.Attach(planner.orbitalMechainics.RootFrame(), planner.cameraFrame);
                planner.cameraFrame.RelativePosition = new lucid.Vector3(10, 10, 3);

                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

                planner.camera = new lucid.Camera2D();
                planner.camera.InitPerspective(0.25f * 3.1415926f, planner.aspectRatio, 1, 1000);
                planner.camera.Look(new lucid.Vector3(20, 20, 15), new lucid.Vector3(0, 0, 0), new lucid.Vector3(0, 0, 1));

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

            public override void onMainViewResize(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;
                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

                lucid.Pipeline.resize(clientSize.Width, clientSize.Height);

                planner.camera.InitPerspective(0.25f * 3.1415926f, planner.aspectRatio, 1, 1000);
                planner.renderMainView();
            }

            public override void onPaint(Planner planner)
            {
                planner.renderMainView();
            }

            public override void onTrackFrame(Planner planner, ListViewItem item)
            {
                if (planner.trackedFrameItem == item)
                {
                    int stateIndex = (item.StateImageIndex + 1) % 2;
                    item.StateImageIndex = stateIndex;

                    if (0 == stateIndex)
                        planner.trackedFrameItem = null;
                }
                else
                {
                    if (null != planner.trackedFrameItem)
                        planner.trackedFrameItem.StateImageIndex = 0;
                    planner.trackedFrameItem = item;
                    planner.trackedFrameItem.StateImageIndex = 1;
                }

                planner.orbitalMechainics.Detach(planner.cameraFrame);
                if (null != planner.trackedFrameItem)
                {
                    uint id = uint.Parse(planner.trackedFrameItem.SubItems[1].Text);
                    planner.trackedFrame = planner.orbitalMechainics.Frame(id);
                    planner.orbitalMechainics.Attach(planner.trackedFrame, planner.cameraFrame);
                }
                else
                {
                    planner.trackedFrame = null;
                    planner.orbitalMechainics.Attach(planner.orbitalMechainics.RootFrame(), planner.cameraFrame);
                }

                planner.cameraFrame.RelativePosition = new lucid.Vector3(10, 10, 2);
                planner.cameraFrame.RelativePosition = new lucid.Vector3(10, 10, 2);
            }

            public override void updateSimulation(Planner planner)
            {
                planner.orbitalMechainics.Update();
            }

            public override void renderMainView(Planner planner)
            {
                /// test {
                lucid.Vector3 position = planner.orbitalMechainics.InterpolatePosition(planner.cameraFrame);
                lucid.Vector3    focus = (null != planner.trackedFrame)
                    ? planner.orbitalMechainics.InterpolatePosition(planner.trackedFrame)
                    : new lucid.Vector3(0, 0, 0);
                planner.camera.Look(position, focus, new lucid.Vector3(0, 0, 1));
                planner.renderContext.Set(planner.camera);
                /// } test

                lucid.Pipeline.beginScene();
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
                planner.orbitalMechainics.Shutdown();

                Application.Exit();
                planner.changeState(Stopped.Instance);
            }
        }

    }   // planner
}       // omp
