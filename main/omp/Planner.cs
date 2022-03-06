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

        private Lucid.Orbit.Mechanics orbitalMechainics = null;

        private float aspectRatio = 1.0f;
        private Lucid.Orbit.CameraFrame cameraFrame = null;

        private ListViewItem trackedFrameItem = null;
        private Lucid.Orbit.Frame trackedFrame = null;

        public Planner()
        {
            InitializeComponent();
            changeState(Starting.Instance);
        }

        private void setMainMenuDefaults()
        {
            mainMenuFileNewMission.Enabled = true;
            mainMenuFileOpenMission.Enabled = true;

            mainMenuFileSave.Enabled = false;
            mainMenuFileSaveAs.Enabled = false;

            mainMenuFileExit.Enabled = true;

            mainMenuViewSettings.Enabled = true;
        }

        private void onFormLoad(object sender, EventArgs e)
        {
            if (Properties.Settings.Default.Maximized)
            {
                WindowState = FormWindowState.Maximized;
            }
            else if (Properties.Settings.Default.Minimized)
            {
                WindowState = FormWindowState.Minimized;
            }

            Location = Properties.Settings.Default.Location;
            Size = Properties.Settings.Default.Size;
            mainSplitter.SplitterDistance = Properties.Settings.Default.SplitterDistance;
        }

        private void onFormClosing(object sender, FormClosingEventArgs e)
        {
            if (WindowState == FormWindowState.Maximized)
            {
                Properties.Settings.Default.Location = RestoreBounds.Location;
                Properties.Settings.Default.Size = RestoreBounds.Size;
                Properties.Settings.Default.Maximized = true;
                Properties.Settings.Default.Minimized = false;
            }
            else if (WindowState == FormWindowState.Normal)
            {
                Properties.Settings.Default.Location = Location;
                Properties.Settings.Default.Size = Size;
                Properties.Settings.Default.Maximized = false;
                Properties.Settings.Default.Minimized = false;
            }
            else
            {
                Properties.Settings.Default.Location = RestoreBounds.Location;
                Properties.Settings.Default.Size = RestoreBounds.Size;
                Properties.Settings.Default.Maximized = false;
                Properties.Settings.Default.Minimized = true;
            }
            Properties.Settings.Default.SplitterDistance = mainSplitter.SplitterDistance;

            Properties.Settings.Default.Save();

            changeState(Stopping.Instance);
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

        private void onMouseMove(object sender, MouseEventArgs e)
        {
            state.onMouseMove(this, e.Location);
        }

        private void onMouseClick(object sender, MouseEventArgs e)
        {
            state.onMouseClick(this, e.Location);
        }

        private void onMainViewResize(object sender, EventArgs e)
        {
            state.onMainViewResize(this);
        }

        private void onFileNewMission(object sender, EventArgs e)
        {
        }

        private void onFileOpenMission(object sender, EventArgs e)
        {
        }

        private void onFileSave(object sender, EventArgs e)
        {
        }

        private void onFileSaveAs(object sender, EventArgs e)
        {
        }

        private void onFileExit(object sender, EventArgs e)
        {
            changeState(Stopping.Instance);
            Application.Exit();
        }

        private void onViewSettings(object sender, EventArgs e)
        {
        }

        private void onFrameListClicked(object sender, MouseEventArgs e)
        {
            state.onFrameListClicked(this, e);
        }

        private void onFrameListChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            state.onFrameListChanged(this, e);
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
