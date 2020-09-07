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

            public override void onEnter(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;

                unsafe { lucid.Pipeline.initialize(clientSize.Width, clientSize.Height, 2, panel.Handle.ToPointer()); }

                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;
                planner.renderContext = new lucid.Context("content/test.context");

                // test {
                // initial user action should be to specify an ephemeris to use
                // or to have a default that is user specified.
                planner.orbitalMechainics = new lucid.OrbitalMechanics("content/j2000.ephemeris");
                planner.orbitalMechainics.initialize(2451544.0);
                // } test

                planner.mainMenuFileNewMission.Enabled = true;
                planner.mainMenuFileOpenMission.Enabled = true;
                planner.mainMenuFileSave.Enabled = false;
                planner.mainMenuFileSaveAs.Enabled = false;
                planner.mainMenuFileExit.Enabled = true;

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
                planner.renderMainView();
            }

            public override void updateSimulation(Planner planner)
            {
                // test {
                float fov = 0.25f * 3.1415926f;

                lucid.Vector3 focus = new lucid.Vector3(0, 0, 0);
                planner.viewDirection = lucid.Math.normalize(focus - planner.viewPosition);

                lucid.Vector3 e1 = planner.viewDirection;
                lucid.Vector3 e2 = new lucid.Vector3(0, 0, 1);
                lucid.Vector3 e0 = lucid.Math.cross(planner.viewDirection, e2);
                e2 = lucid.Math.cross(e0, e1);

                lucid.Matrix4x4 viewMatrix = lucid.Math.look(planner.viewPosition, planner.viewPosition + planner.viewDirection, e2);
                lucid.Matrix4x4 projMatrix = lucid.Math.perspective(fov, planner.aspectRatio, 1.0f, 1000.0f);

                lucid.Matrix4x4 invViewMatrix = lucid.Math.inverse(viewMatrix);

                planner.renderContext.Set("viewRight", e0);
                planner.renderContext.Set("viewForward", e1);
                planner.renderContext.Set("viewUp", e2);
                planner.renderContext.Set("viewPosition", planner.viewPosition);
                planner.renderContext.Set("viewMatrix", viewMatrix);
                planner.renderContext.Set("projMatrix", projMatrix);
                planner.renderContext.Set("viewProjMatrix", projMatrix * viewMatrix);
                // } test

                planner.orbitalMechainics.update();
            }

            public override void renderMainView(Planner planner)
            {
                lucid.Pipeline.beginScene();
                    lucid.Pipeline.clear(new lucid.Color(0, 0, 0, 0), 1.0f);
                    planner.orbitalMechainics.render(planner.renderContext);
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
