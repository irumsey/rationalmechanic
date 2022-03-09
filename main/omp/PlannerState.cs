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

            public virtual void onMouseMove(Planner planner, Point point) { }

            public virtual void onMouseClick(Planner planner, Point point) { }

            public virtual void onFrameListClicked(Planner planner, MouseEventArgs e) { }
            
            public virtual void onFrameListChanged(Planner planner, ListViewItemSelectionChangedEventArgs e) { }
            
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
            private void testPopulateListview(Planner planner, Lucid.Orbit.Frame frame)
            {
                ListViewItem item = new ListViewItem();
                item.Tag = frame.ID;
                item.SubItems.Add(frame.ID.ToString());
                item.SubItems.Add(frame.Name);
                item.SubItems.Add(frame.Description);
                item.StateImageIndex = 0;
                planner.orbitalFrameList.Items.Add(item);

                for (Lucid.Orbit.Frame child = frame.FirstChild; child != null; child = child.NextSibling)
                {
                    testPopulateListview(planner, child);
                }
            }
            // } test

            public override void onEnter(Planner planner)
            {
                Lucid.Core.Logger.Log("INFO", "Planner.Starting.onEnter(...)");

                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;

                unsafe { Lucid.GAL.Pipeline.initialize(clientSize.Width, clientSize.Height, 1, panel.Handle.ToPointer()); }

                // test {
                // initial user action should be to specify an ephemeris
                // or to have a default that is user specified.
                Lucid.Orbit.Ephemeris.Initialize("content/j2000.ephemeris");
                Lucid.Orbit.StarCatalog.Initialize("content/bsc5.starcatalog");

                planner.orbitalMechainics = new Lucid.Orbit.Mechanics(2451544.0);
                testPopulateListview(planner, planner.orbitalMechainics.Root);
                // } test

                planner.cameraFrame = new Lucid.Orbit.CameraFrame(1001, "camera", "");
                planner.orbitalMechainics.Attach(planner.orbitalMechainics.Root, planner.cameraFrame);
                planner.cameraFrame.RelativePosition = new Lucid.Math.Vector3(10, 10, 3);

                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

                planner.cameraFrame.InitPerspective(0.25f * 3.1415926f, planner.aspectRatio, 1, 1000);
                planner.cameraFrame.Focus = planner.orbitalMechainics.Root;

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

            public override void onEnter(Planner planner)
            {
                Lucid.Core.Logger.Log("INFO", "Planner.Editing.onEnter(...)");
            }

            public override void onMainViewResize(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;
                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

                Lucid.GAL.Pipeline.resize(clientSize.Width, clientSize.Height);

                planner.cameraFrame.InitPerspective(0.25f * 3.1415926f, planner.aspectRatio, 1, 1000);

                planner.renderMainView();
            }

            public override void onPaint(Planner planner)
            {
                planner.renderMainView();
            }

            public override void onMouseMove(Planner planner, Point point)
            {
                Lucid.Orbit.Selection selection = planner.orbitalMechainics.Hit(point.X, point.Y);

                if (Lucid.Orbit.SelectionType.TYPE_NONE == selection.Type)
                {
                    planner.statusLabel.Text = "no selection";
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_STAR == selection.Type)
                {
                    Lucid.Orbit.StarCatalogEntry entry = Lucid.Orbit.StarCatalog.At(selection.Tag);
                    planner.statusLabel.Text = "BSC5: " + entry.XNO.ToString() + " Type: " + entry.Type + " Mag: " + entry.Mag.ToString();
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_FRAME == selection.Type)
                {
                    Lucid.Orbit.EphemerisEntry entry = Lucid.Orbit.Ephemeris.LookupEntry(selection.Tag);
                    if (null == entry)
                        return;
                    planner.statusLabel.Text = "Frame: " + entry.Name;
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_ORBIT == selection.Type)
                {
                    Lucid.Orbit.EphemerisEntry entry = Lucid.Orbit.Ephemeris.LookupEntry(selection.Tag);
                    if (null == entry)
                        return;
                    planner.statusLabel.Text = "Orbit: " + entry.Name;
                    return;
                }
            }

            public override void onMouseClick(Planner planner, Point point)
            {
                Lucid.Orbit.Selection selection = planner.orbitalMechainics.Hit(point.X, point.Y);
                if (0 == selection.Tag)
                    return;

                foreach(ListViewItem item in planner.orbitalFrameList.Items)
                {
                    if ((ulong)(item.Tag) == selection.Tag)
                        item.Selected = true;
                }
            }

            public override void onFrameListClicked(Planner planner, MouseEventArgs e)
            {
                ListViewHitTestInfo hitTest = planner.orbitalFrameList.HitTest(e.Location);
                if (null == hitTest)
                    return;

                ListViewItem item = hitTest.Item;
                ListViewItem.ListViewSubItem subItem = hitTest.SubItem;
                if ((null == item) || (null == subItem))
                    return;

                int column = item.SubItems.IndexOf(subItem);
                if (0 != column)
                    return;

                if (item == planner.trackedFrameItem)
                    return;

                if (planner.trackedFrameItem != null)
                    planner.trackedFrameItem.StateImageIndex = 0;

                planner.trackedFrameItem = item;
                planner.trackedFrameItem.StateImageIndex = 1;

                trackFrame(planner, planner.orbitalMechainics[(ulong)(planner.trackedFrameItem.Tag)]);
            }

            public override void onFrameListChanged(Planner planner, ListViewItemSelectionChangedEventArgs e)
            {
                planner.cameraFrame.Focus = planner.orbitalMechainics[(ulong)(e.Item.Tag)];
            } 

            public override void updateSimulation(Planner planner)
            {
                planner.orbitalMechainics.Update();
            }

            public override void renderMainView(Planner planner)
            {
                Lucid.GAL.Pipeline.beginScene();
                    planner.orbitalMechainics.Render(planner.cameraFrame);
                Lucid.GAL.Pipeline.endScene();
            }

            private void trackFrame(Planner planner, Lucid.Orbit.Frame frame)
            {
                if (planner.trackedFrame == frame)
                    return;

                planner.trackedFrame = frame;

                planner.orbitalMechainics.Detach(planner.cameraFrame);
                planner.orbitalMechainics.Attach(planner.trackedFrame, planner.cameraFrame);

                planner.cameraFrame.RelativePosition = new Lucid.Math.Vector3(10, 10, 3);
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
                Lucid.Core.Logger.Log("INFO", "Planner.Stopping.onEnter(...)");

                planner.orbitalMechainics.Shutdown();
                
                Lucid.Orbit.Ephemeris.Shutdown();
                Lucid.Orbit.StarCatalog.Shutdown();

                planner.changeState(Stopped.Instance);
            }
        }

    }   // planner
}       // omp
