using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using lucid;

namespace omp
{
    public partial class MainForm : Form
    {
        // getting resize event before constructor???
        private bool canRender = false;

        private float           aspectRatio = 1.0f;
        private lucid.Vector3  viewPosition = new lucid.Vector3(60.0f, 60.0f, 60.0f);
        private lucid.Vector3 viewDirection = new lucid.Vector3(-0.577f, -0.577f, -0.577f);
        private lucid.Context renderContext = null;

        private lucid.OrbitalMechanics orbitalMechainics = null;

        public MainForm()
        {
            InitializeComponent();

            SplitterPanel panel = mainSplitter.Panel1;
            Size clientSize = panel.ClientSize;
            unsafe { lucid.Pipeline.initialize(clientSize.Width, clientSize.Height, 2, panel.Handle.ToPointer()); }

            aspectRatio = (float)clientSize.Width / (float)clientSize.Height;
            canRender = true;

            renderContext = new lucid.Context("content/test.context");
            orbitalMechainics = new lucid.OrbitalMechanics("content/j2000.ephemeris");

            // test {
            orbitalMechainics.initialize(2451544.0);
            // } test
        }

        private void onPaint(object sender, PaintEventArgs e)
        {
            if (canRender)
                render();
        }

        private void onMainViewResize(object sender, EventArgs e)
        {
            SplitterPanel panel = mainSplitter.Panel1;
            Size clientSize = panel.ClientSize;
            aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

            if (canRender)
            {
                lucid.Pipeline.resize(clientSize.Width, clientSize.Height);
                render();
            }
        }

        public void advanceSimulation()
        {
            // test {
            float fov = 0.25f * 3.1415926f;

            lucid.Vector3 focus = new lucid.Vector3(0, 0, 0);
            viewDirection = lucid.Math.normalize(focus - viewPosition);

            lucid.Vector3 e1 = viewDirection;
            lucid.Vector3 e2 = new lucid.Vector3(0, 0, 1);
            lucid.Vector3 e0 = lucid.Math.cross(viewDirection, e2);
            e2 = lucid.Math.cross(e0, e1);

            lucid.Matrix4x4 viewMatrix = lucid.Math.look(viewPosition, viewPosition + 5.0f * viewDirection, e2);
            lucid.Matrix4x4 projMatrix = lucid.Math.perspective(fov, aspectRatio, 1.0f, 1000.0f);

            lucid.Matrix4x4 invViewMatrix = lucid.Math.inverse(viewMatrix);

            renderContext.Set("viewRight", e0);
            renderContext.Set("viewForward", e1);
            renderContext.Set("viewUp", e2);
            renderContext.Set("viewPosition", viewPosition);
            renderContext.Set("viewMatrix", viewMatrix);
            renderContext.Set("projMatrix", projMatrix);
            renderContext.Set("viewProjMatrix", projMatrix * viewMatrix);
            // } test

            orbitalMechainics.update();
        }

        public void render()
        {
            lucid.Pipeline.beginScene();
                lucid.Pipeline.clear(new lucid.Color(0, 0, 0, 0), 1.0f);
                orbitalMechainics.render(renderContext);
            lucid.Pipeline.endScene();
        }
    }
}
