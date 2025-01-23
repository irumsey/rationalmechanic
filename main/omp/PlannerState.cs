﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

using Logger = Lucid.Core.Logger;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;
using Lucid.Orbit;
using System.Security.Permissions;

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

            public virtual void onMainViewMouseClick(Planner planner, Point point) { }

            public virtual void onMainViewMouseWheel(Planner planner, MouseEventArgs e) { }
            public virtual void onMainViewMouseDown(Planner planner, MouseEventArgs e) { }

            public virtual void onMainViewMouseRelease(Planner planner, MouseEventArgs e) { }
            public virtual void onMainViewMouseMove(Planner planner, Point point) { }

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

            private void populateListview(Planner planner, Lucid.Orbit.Frame frame)
            {
                ListViewItem item = new ListViewItem();
                item.Tag = frame;
                item.SubItems.Add(frame.ID.ToString());
                item.SubItems.Add(frame.Name);
                item.SubItems.Add(frame.Description);
                item.StateImageIndex = 0;
                planner.orbitalFrameList.Items.Add(item);

                for (Lucid.Orbit.Frame child = frame.FirstChild; child != null; child = child.NextSibling)
                {
                    populateListview(planner, child);
                }
            }

            public override void onEnter(Planner planner)
            {
                Logger.Log("INFO", "Planner.Starting.onEnter(...)");

                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;

                unsafe { Lucid.GAL.Pipeline.initialize(clientSize.Width, clientSize.Height, 1, panel.Handle.ToPointer()); }

                // test {
                // initial user action should be to specify an ephemeris
                // or to have a default that can be user specified.
                // creating orbital mechanics without a JDN will default to current time.
                Lucid.Orbit.Ephemeris.Initialize("content/j2000.ephemeris");
                Lucid.Orbit.StarCatalog.Initialize("content/bsc5.starcatalog");
                planner.orbitalMechainics = new Lucid.Orbit.Mechanics();

                populateListview(planner, planner.orbitalMechainics.Root);
                // } test

                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

                planner.cameraFrame = new Lucid.Orbit.CameraFrame(-1, "camera", "");
                planner.cameraFrame.RelativePosition = new Lucid.Math.Vector3(0, 0, 6.37101e010f);

                Frame focusFrame = planner.orbitalMechainics[399];
                planner.cameraFrame.Focus = focusFrame;
                planner.orbitalMechainics.Attach(focusFrame, planner.cameraFrame);

                float distance = (float)((focusFrame as OrbitalBody).PhysicalProps.Radius);
                planner.cameraFrame.RelativePosition = new Lucid.Math.Vector3(distance, distance, 10.0f * distance);

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
                Logger.Log("INFO", "Planner.Editing.onEnter(...)");
            }

            public override void onMainViewResize(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;

                Lucid.GAL.Pipeline.resize(clientSize.Width, clientSize.Height);

                planner.renderMainView();
            }

            public override void onPaint(Planner planner)
            {
                planner.renderMainView();
            }

            public override void onMainViewMouseClick(Planner planner, Point point)
            {
#if false
                Lucid.Orbit.Selection selection = planner.orbitalMechainics.Hit(point.X, point.Y);

                if (Lucid.Orbit.SelectionType.TYPE_NONE == selection.Type)
                {
                    // TBD: implement
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_STAR == selection.Type)
                {
                    // TBD: implement
                    return;
                }
                 
                if (Lucid.Orbit.SelectionType.TYPE_ORBIT == selection.Type)
                {
                    // TBD: implement
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_FRAME == selection.Type)
                {
                    Lucid.Orbit.Frame frame = planner.orbitalMechainics[selection.Token];

                    // TBD: use a better way to get the item instead of this linear search
                    foreach (ListViewItem item in planner.orbitalFrameList.Items)
                    {
                        Lucid.Orbit.Frame itemFrame = (Lucid.Orbit.Frame)(item.Tag);
                        if (itemFrame.ID == frame.ID)
                        {
                            item.Selected = true;
                            break;
                        }
                    }

                    Logger.Log("INFO", frame.Name + " selected using main view");
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_CAMERA == selection.Type)
                {
                    // TBD: implement
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_CALLOUT == selection.Type)
                {
                    // TBD: implement
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_OTHER == selection.Type)
                {
                    // TBD: implement
                    return;
                }
#endif
            }

            public override void onMainViewMouseWheel(Planner planner, MouseEventArgs e)
            {
                // test {
                float scale = (e.Delta < 0) ? 1e7f : -1e7f;
                Lucid.Math.Vector3 delta = scale * Lucid.Math.Util.normalize(planner.cameraFrame.RelativePosition);

                planner.cameraFrame.RelativePosition = planner.cameraFrame.RelativePosition + delta;
                // } test
            }

            public override void onMainViewMouseDown(Planner planner, MouseEventArgs e)
            {
                // test {
                planner.cameraRotating = true;
                planner.cameraDownOrigin = e.Location;
                // } test
            }

            public override void onMainViewMouseRelease(Planner planner, MouseEventArgs e)
            {
                // test {
                planner.cameraRotating = false;
                // } test
            }

            public override void onMainViewMouseMove(Planner planner, Point point)
            {
                // test {
                if (planner.cameraRotating)
                {
                    Point origin = planner.cameraDownOrigin;
                    float theta = 0.01f * (origin.X - point.X);
                    float phi = 0.01f * (point.Y - origin.Y);
                    planner.cameraDownOrigin = point;

                    Lucid.Math.Vector3 viewUp = planner.cameraFrame.zaxis;
                    Lucid.Math.Vector3 viewRight = planner.cameraFrame.xaxis;

                    Lucid.Math.Quaternion qz = Lucid.Math.Util.rotateUsingAxis(viewUp, theta);
                    Lucid.Math.Matrix3x3 Rz = Lucid.Math.Util.matrixFromQuaternion(qz);

                    Lucid.Math.Quaternion qx = Lucid.Math.Util.rotateUsingAxis(viewRight, phi);
                    Lucid.Math.Matrix3x3 Rx = Lucid.Math.Util.matrixFromQuaternion(qx);
                    
                    planner.cameraFrame.RelativePosition = Rx * Rz * planner.cameraFrame.RelativePosition;

                    return;
                }
                // } test

                Lucid.Orbit.Selection selection = planner.orbitalMechainics.Hit(point.X, point.Y);

                if (Lucid.Orbit.SelectionType.TYPE_NONE == selection.Type)
                {
                    planner.statusLabel.Text = "<no selection>";
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_STAR == selection.Type)
                {
                    StarCatalogEntry entry = StarCatalog.At((UInt32)(selection.ID));
                    planner.statusLabel.Text = "BSC   : " + entry.XNO.ToString() + " Type: " + entry.Type.ToString() + " Mag: " + entry.Mag.ToString();
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_ORBIT == selection.Type)
                {
                    EphemerisEntry entry = Ephemeris.LookupEntry(selection.ID);
                    planner.statusLabel.Text = "Orbit : " + entry.Name;
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_FRAME == selection.Type)
                {
                    String origin =
                        (Lucid.Orbit.SelectionOrigin.ORIGIN_ARTIFICIAL == selection.Origin) ?
                        " (artificial)" :
                        " (natural)";

                    EphemerisEntry entry = Ephemeris.LookupEntry(selection.ID);
                    planner.statusLabel.Text = "Frame : " + entry.Name + origin;
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_CAMERA == selection.Type)
                {
                    planner.statusLabel.Text = "Camera: ";
                    return;
                }

                if (Lucid.Orbit.SelectionType.TYPE_ICON == selection.Type)
                {
                    String origin =
                        (Lucid.Orbit.SelectionOrigin.ORIGIN_ARTIFICIAL == selection.Origin) ?
                        " (artificial)" :
                        " (natural)";

                    EphemerisEntry entry = Ephemeris.LookupEntry(selection.ID);
                    planner.statusLabel.Text = "Icon  : " + entry.Name + origin;
                    return;
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

                if (item == planner.cameraParentItem)
                    return;

                if (null != planner.cameraParentItem)
                    planner.cameraParentItem.StateImageIndex = 0;

                planner.cameraParentItem = item;
                planner.cameraParentItem.StateImageIndex = 1;

                Lucid.Orbit.Frame parentFrame = (Lucid.Orbit.Frame)(planner.cameraParentItem.Tag);

                planner.orbitalMechainics.Detach(planner.cameraFrame);
                planner.orbitalMechainics.Attach(parentFrame, planner.cameraFrame);

                float distance = (float)((parentFrame as OrbitalBody).PhysicalProps.Radius);
                planner.cameraFrame.RelativePosition = new Lucid.Math.Vector3(distance, distance, 10.0f * distance);

                Logger.Log("INFO", "camera attached to frame: " + parentFrame.Name);
            }

            public override void onFrameListChanged(Planner planner, ListViewItemSelectionChangedEventArgs e)
            {
                if (!e.IsSelected)
                    return;

                Lucid.Orbit.Frame frame = (Lucid.Orbit.Frame)(e.Item.Tag);
                planner.cameraFrame.Focus = frame;

                Logger.Log("INFO", "camera's focus switched to: " + frame.Name);
            } 

            public override void updateSimulation(Planner planner)
            {
                planner.orbitalMechainics.Update();
            }

            public override void renderMainView(Planner planner)
            {
                Lucid.GAL.Pipeline.beginScene();
                    planner.orbitalMechainics.Render(planner.cameraFrame, planner.useFXAA);
                Lucid.GAL.Pipeline.endScene();
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
                Logger.Log("INFO", "Planner.Stopping.onEnter(...)");

                planner.orbitalMechainics.Shutdown();
                
                Lucid.Orbit.Ephemeris.Shutdown();
                Lucid.Orbit.StarCatalog.Shutdown();

                planner.changeState(Stopped.Instance);
            }
        }

    }   // planner
}       // omp
