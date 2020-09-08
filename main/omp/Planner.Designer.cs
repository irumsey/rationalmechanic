namespace omp
{
    partial class Planner
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
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.mainMenuFileSave = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.mainMenuFileExit = new System.Windows.Forms.ToolStripMenuItem();
            this.mainSplitter = new System.Windows.Forms.SplitContainer();
            this.mainMenuEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuView = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuViewSettings = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
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
            this.mainMenuFile,
            this.mainMenuEdit,
            this.mainMenuView});
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
            this.toolStripSeparator1,
            this.mainMenuFileSave,
            this.mainMenuFileSaveAs,
            this.toolStripSeparator2,
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
            this.mainMenuFileNewMission.Click += new System.EventHandler(this.onFileNewMission);
            // 
            // mainMenuFileOpenMission
            // 
            this.mainMenuFileOpenMission.Name = "mainMenuFileOpenMission";
            this.mainMenuFileOpenMission.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileOpenMission.Text = "Open Mission...";
            this.mainMenuFileOpenMission.Click += new System.EventHandler(this.onFileOpenMission);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(153, 6);
            // 
            // mainMenuFileSave
            // 
            this.mainMenuFileSave.Name = "mainMenuFileSave";
            this.mainMenuFileSave.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileSave.Text = "Save";
            this.mainMenuFileSave.Click += new System.EventHandler(this.onFileSave);
            // 
            // mainMenuFileSaveAs
            // 
            this.mainMenuFileSaveAs.Name = "mainMenuFileSaveAs";
            this.mainMenuFileSaveAs.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileSaveAs.Text = "Save As...";
            this.mainMenuFileSaveAs.Click += new System.EventHandler(this.onFileSaveAs);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(153, 6);
            // 
            // mainMenuFileExit
            // 
            this.mainMenuFileExit.Name = "mainMenuFileExit";
            this.mainMenuFileExit.Size = new System.Drawing.Size(156, 22);
            this.mainMenuFileExit.Text = "Exit";
            this.mainMenuFileExit.Click += new System.EventHandler(this.onFileExit);
            // 
            // mainSplitter
            // 
            this.mainSplitter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainSplitter.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
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
            // mainMenuEdit
            // 
            this.mainMenuEdit.Name = "mainMenuEdit";
            this.mainMenuEdit.Size = new System.Drawing.Size(39, 20);
            this.mainMenuEdit.Text = "Edit";
            // 
            // mainMenuView
            // 
            this.mainMenuView.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripSeparator3,
            this.mainMenuViewSettings});
            this.mainMenuView.Name = "mainMenuView";
            this.mainMenuView.Size = new System.Drawing.Size(44, 20);
            this.mainMenuView.Text = "View";
            // 
            // mainMenuViewSettings
            // 
            this.mainMenuViewSettings.Name = "mainMenuViewSettings";
            this.mainMenuViewSettings.Size = new System.Drawing.Size(152, 22);
            this.mainMenuViewSettings.Text = "Settings...";
            this.mainMenuViewSettings.Click += new System.EventHandler(this.onViewSettings);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(149, 6);
            // 
            // Planner
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1197, 604);
            this.Controls.Add(this.mainSplitter);
            this.Controls.Add(this.mainStatusStrip);
            this.Controls.Add(this.mainMenu);
            this.MainMenuStrip = this.mainMenu;
            this.Name = "Planner";
            this.Text = "OMP (Orbital Mission Planner)";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.onFormClosing);
            this.Load += new System.EventHandler(this.onFormLoad);
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
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem mainMenuEdit;
        private System.Windows.Forms.ToolStripMenuItem mainMenuView;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem mainMenuViewSettings;
    }
}

