using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace omp
{

    /// <summary>
    /// </summary>
    public partial class Planner : Form
    {
        private State state = Stopped.Instance;

        private float aspectRatio = 1.0f;
        private lucid.Vector3 viewPosition = new lucid.Vector3(60.0f, 60.0f, 60.0f);
        private lucid.Vector3 viewDirection = new lucid.Vector3(-0.577f, -0.577f, -0.577f);
        private lucid.Context renderContext = null;

        private lucid.OrbitalMechanics orbitalMechainics = null;

        public Planner()
        {
            InitializeComponent();
            changeState(Starting.Instance);
        }

        private void changeState(State next)
        {
            state.onLeave(this);
            state = next;
            state.onEnter(this);
        }

        private void onPaint(object sender, PaintEventArgs e)
        {
            state.onPaint(this);
        }

        private void onMainViewResize(object sender, EventArgs e)
        {
            state.onMainViewResize(this);
        }

        private void onFileNewMission(object sender, EventArgs e)
        {
        }

        private void onFileExit(object sender, EventArgs e)
        {
            changeState(Stopping.Instance);
        }

        public void updateSimulation()
        {
            state.updateSimulation(this);
        }

        public void renderMainView()
        {
            state.renderMainView(this);
        }
    }
}
