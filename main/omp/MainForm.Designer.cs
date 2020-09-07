namespace omp
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.mainStatusStrip = new System.Windows.Forms.StatusStrip();
            this.mainMenu = new System.Windows.Forms.MenuStrip();
            this.mainMenuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileNewMission = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileOpenMission = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileSave = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileExit = new System.Windows.Forms.ToolStripMenuItem();
            this.mainSplitter = new System.Windows.Forms.SplitContainer();
            this.mainMenu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mainSplitter)).BeginInit();
            this.mainSplitter.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainStatusStrip
            // 
            this.mainStatusStrip.Location = new System.Drawing.Point(0, 582);
            this.mainStatusStrip.Name = "mainStatusStrip";
            this.mainStatusStrip.Size = new System.Drawing.Size(1197, 22);
            this.mainStatusStrip.TabIndex = 0;
            this.mainStatusStrip.Text = "Status";
            // 
            // mainMenu
            // 
            this.mainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainMenuFile});
            this.mainMenu.Location = new System.Drawing.Point(0, 0);
            this.mainMenu.Name = "mainMenu";
            this.mainMenu.Size = new System.Drawing.Size(1197, 24);
            this.mainMenu.TabIndex = 1;
            this.mainMenu.Text = "menuStrip1";
            // 
            // mainMenuFile
            // 
            this.mainMenuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainMenuFileNewMission,
            this.mainMenuFileOpenMission,
            this.mainMenuFileSave,
            this.mainMenuFileSaveAs,
            this.mainMenuFileExit});
            this.mainMenuFile.Name = "mainMenuFile";
            this.mainMenuFile.Size = new System.Drawing.Size(37, 20);
            this.mainMenuFile.Text = "File";
            // 
            // mainMenuFileNewMission
            // 
            this.mainMenuFileNewMission.Name = "mainMenuFileNewMission";
            this.mainMenuFileNewMission.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileNewMission.Text = "New Mission...";
            // 
            // mainMenuFileOpenMission
            // 
            this.mainMenuFileOpenMission.Name = "mainMenuFileOpenMission";
            this.mainMenuFileOpenMission.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileOpenMission.Text = "Open Mission...";
            // 
            // mainMenuFileSave
            // 
            this.mainMenuFileSave.Name = "mainMenuFileSave";
            this.mainMenuFileSave.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileSave.Text = "Save";
            // 
            // mainMenuFileSaveAs
            // 
            this.mainMenuFileSaveAs.Name = "mainMenuFileSaveAs";
            this.mainMenuFileSaveAs.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileSaveAs.Text = "Save As...";
            // 
            // mainMenuFileExit
            // 
            this.mainMenuFileExit.Name = "mainMenuFileExit";
            this.mainMenuFileExit.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileExit.Text = "Exit";
            // 
            // mainSplitter
            // 
            this.mainSplitter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainSplitter.Location = new System.Drawing.Point(0, 24);
            this.mainSplitter.Name = "mainSplitter";
            // 
            // mainSplitter.Panel1
            // 
            this.mainSplitter.Panel1.Resize += new System.EventHandler(this.onMainViewResize);
            this.mainSplitter.Size = new System.Drawing.Size(1197, 558);
            this.mainSplitter.SplitterDistance = 897;
            this.mainSplitter.TabIndex = 2;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1197, 604);
            this.Controls.Add(this.mainSplitter);
            this.Controls.Add(this.mainStatusStrip);
            this.Controls.Add(this.mainMenu);
            this.MainMenuStrip = this.mainMenu;
            this.Name = "MainForm";
            this.Text = "OMP (Orbital Mission Planner)";
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.onPaint);
            this.mainMenu.ResumeLayout(false);
            this.mainMenu.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mainSplitter)).EndInit();
            this.mainSplitter.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip mainStatusStrip;
        private System.Windows.Forms.MenuStrip mainMenu;
        private System.Windows.Forms.ToolStripMenuItem mainMenuFile;
        private System.Windows.Forms.ToolStripMenuItem mainMenuFileNewMission;
        private System.Windows.Forms.ToolStripMenuItem mainMenuFileOpenMission;
        private System.Windows.Forms.ToolStripMenuItem mainMenuFileSave;
        private System.Windows.Forms.ToolStripMenuItem mainMenuFileSaveAs;
        private System.Windows.Forms.ToolStripMenuItem mainMenuFileExit;
        private System.Windows.Forms.SplitContainer mainSplitter;
    }
}

