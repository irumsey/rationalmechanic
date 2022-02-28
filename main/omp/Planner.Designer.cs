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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Planner));
            this.mainStatusStrip = new System.Windows.Forms.StatusStrip();
            this.statusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.mainMenu = new System.Windows.Forms.MenuStrip();
            this.mainMenuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileNewMission = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileOpenMission = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.mainMenuFileSave = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuFileSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.mainMenuFileExit = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.mainMenuView = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.mainMenuViewSettings = new System.Windows.Forms.ToolStripMenuItem();
            this.mainSplitter = new System.Windows.Forms.SplitContainer();
            this.orbitalFrameList = new System.Windows.Forms.ListView();
            this.orbitalFrameListTrack = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.orbitalFrameListID = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.orbitFrameListName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.orbitalFrameListDescription = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.orbitalFrameListImages = new System.Windows.Forms.ImageList(this.components);
            this.mainStatusStrip.SuspendLayout();
            this.mainMenu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mainSplitter)).BeginInit();
            this.mainSplitter.Panel2.SuspendLayout();
            this.mainSplitter.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainStatusStrip
            // 
            this.mainStatusStrip.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.mainStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusLabel});
            this.mainStatusStrip.Location = new System.Drawing.Point(0, 512);
            this.mainStatusStrip.Name = "mainStatusStrip";
            this.mainStatusStrip.Padding = new System.Windows.Forms.Padding(1, 0, 19, 0);
            this.mainStatusStrip.Size = new System.Drawing.Size(1239, 26);
            this.mainStatusStrip.TabIndex = 0;
            this.mainStatusStrip.Text = "Status";
            // 
            // statusLabel
            // 
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(47, 20);
            this.statusLabel.Text = "status";
            // 
            // mainMenu
            // 
            this.mainMenu.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.mainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainMenuFile,
            this.mainMenuEdit,
            this.mainMenuView});
            this.mainMenu.Location = new System.Drawing.Point(0, 0);
            this.mainMenu.Name = "mainMenu";
            this.mainMenu.Size = new System.Drawing.Size(1239, 28);
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
            this.mainMenuFile.Size = new System.Drawing.Size(46, 24);
            this.mainMenuFile.Text = "File";
            // 
            // mainMenuFileNewMission
            // 
            this.mainMenuFileNewMission.Name = "mainMenuFileNewMission";
            this.mainMenuFileNewMission.Size = new System.Drawing.Size(191, 26);
            this.mainMenuFileNewMission.Text = "New Mission...";
            this.mainMenuFileNewMission.Click += new System.EventHandler(this.onFileNewMission);
            // 
            // mainMenuFileOpenMission
            // 
            this.mainMenuFileOpenMission.Name = "mainMenuFileOpenMission";
            this.mainMenuFileOpenMission.Size = new System.Drawing.Size(191, 26);
            this.mainMenuFileOpenMission.Text = "Open Mission...";
            this.mainMenuFileOpenMission.Click += new System.EventHandler(this.onFileOpenMission);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(188, 6);
            // 
            // mainMenuFileSave
            // 
            this.mainMenuFileSave.Name = "mainMenuFileSave";
            this.mainMenuFileSave.Size = new System.Drawing.Size(191, 26);
            this.mainMenuFileSave.Text = "Save";
            this.mainMenuFileSave.Click += new System.EventHandler(this.onFileSave);
            // 
            // mainMenuFileSaveAs
            // 
            this.mainMenuFileSaveAs.Name = "mainMenuFileSaveAs";
            this.mainMenuFileSaveAs.Size = new System.Drawing.Size(191, 26);
            this.mainMenuFileSaveAs.Text = "Save As...";
            this.mainMenuFileSaveAs.Click += new System.EventHandler(this.onFileSaveAs);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(188, 6);
            // 
            // mainMenuFileExit
            // 
            this.mainMenuFileExit.Name = "mainMenuFileExit";
            this.mainMenuFileExit.Size = new System.Drawing.Size(191, 26);
            this.mainMenuFileExit.Text = "Exit";
            this.mainMenuFileExit.Click += new System.EventHandler(this.onFileExit);
            // 
            // mainMenuEdit
            // 
            this.mainMenuEdit.Name = "mainMenuEdit";
            this.mainMenuEdit.Size = new System.Drawing.Size(49, 24);
            this.mainMenuEdit.Text = "Edit";
            // 
            // mainMenuView
            // 
            this.mainMenuView.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripSeparator3,
            this.mainMenuViewSettings});
            this.mainMenuView.Name = "mainMenuView";
            this.mainMenuView.Size = new System.Drawing.Size(55, 24);
            this.mainMenuView.Text = "View";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(151, 6);
            // 
            // mainMenuViewSettings
            // 
            this.mainMenuViewSettings.Name = "mainMenuViewSettings";
            this.mainMenuViewSettings.Size = new System.Drawing.Size(154, 26);
            this.mainMenuViewSettings.Text = "Settings...";
            this.mainMenuViewSettings.Click += new System.EventHandler(this.onViewSettings);
            // 
            // mainSplitter
            // 
            this.mainSplitter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainSplitter.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.mainSplitter.Location = new System.Drawing.Point(0, 28);
            this.mainSplitter.Margin = new System.Windows.Forms.Padding(4);
            this.mainSplitter.Name = "mainSplitter";
            // 
            // mainSplitter.Panel1
            // 
            this.mainSplitter.Panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.onPaint);
            this.mainSplitter.Panel1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.onMouseClick);
            this.mainSplitter.Panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.onMouseMove);
            this.mainSplitter.Panel1.Resize += new System.EventHandler(this.onMainViewResize);
            // 
            // mainSplitter.Panel2
            // 
            this.mainSplitter.Panel2.Controls.Add(this.orbitalFrameList);
            this.mainSplitter.Size = new System.Drawing.Size(1239, 484);
            this.mainSplitter.SplitterDistance = 936;
            this.mainSplitter.SplitterWidth = 5;
            this.mainSplitter.TabIndex = 2;
            // 
            // orbitalFrameList
            // 
            this.orbitalFrameList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.orbitalFrameListTrack,
            this.orbitalFrameListID,
            this.orbitFrameListName,
            this.orbitalFrameListDescription});
            this.orbitalFrameList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.orbitalFrameList.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.orbitalFrameList.FullRowSelect = true;
            this.orbitalFrameList.HideSelection = false;
            this.orbitalFrameList.Location = new System.Drawing.Point(0, 0);
            this.orbitalFrameList.Margin = new System.Windows.Forms.Padding(4);
            this.orbitalFrameList.MultiSelect = false;
            this.orbitalFrameList.Name = "orbitalFrameList";
            this.orbitalFrameList.Size = new System.Drawing.Size(298, 484);
            this.orbitalFrameList.StateImageList = this.orbitalFrameListImages;
            this.orbitalFrameList.TabIndex = 0;
            this.orbitalFrameList.UseCompatibleStateImageBehavior = false;
            this.orbitalFrameList.View = System.Windows.Forms.View.Details;
            this.orbitalFrameList.MouseClick += new System.Windows.Forms.MouseEventHandler(this.onFrameListClicked);
            // 
            // orbitalFrameListTrack
            // 
            this.orbitalFrameListTrack.Text = "";
            this.orbitalFrameListTrack.Width = 28;
            // 
            // orbitalFrameListID
            // 
            this.orbitalFrameListID.Text = "ID";
            this.orbitalFrameListID.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.orbitalFrameListID.Width = 51;
            // 
            // orbitFrameListName
            // 
            this.orbitFrameListName.Text = "Name";
            this.orbitFrameListName.Width = 79;
            // 
            // orbitalFrameListDescription
            // 
            this.orbitalFrameListDescription.Text = "Description";
            this.orbitalFrameListDescription.Width = 152;
            // 
            // orbitalFrameListImages
            // 
            this.orbitalFrameListImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("orbitalFrameListImages.ImageStream")));
            this.orbitalFrameListImages.TransparentColor = System.Drawing.Color.Transparent;
            this.orbitalFrameListImages.Images.SetKeyName(0, "notLinked.png");
            this.orbitalFrameListImages.Images.SetKeyName(1, "linked.png");
            // 
            // Planner
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1239, 538);
            this.Controls.Add(this.mainSplitter);
            this.Controls.Add(this.mainStatusStrip);
            this.Controls.Add(this.mainMenu);
            this.MainMenuStrip = this.mainMenu;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Planner";
            this.Text = "OMP (Orbital Mission Planner)";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.onFormClosing);
            this.Load += new System.EventHandler(this.onFormLoad);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.onPaint);
            this.mainStatusStrip.ResumeLayout(false);
            this.mainStatusStrip.PerformLayout();
            this.mainMenu.ResumeLayout(false);
            this.mainMenu.PerformLayout();
            this.mainSplitter.Panel2.ResumeLayout(false);
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
        private System.Windows.Forms.ListView orbitalFrameList;
        private System.Windows.Forms.ColumnHeader orbitalFrameListTrack;
        private System.Windows.Forms.ColumnHeader orbitalFrameListID;
        private System.Windows.Forms.ColumnHeader orbitFrameListName;
        private System.Windows.Forms.ColumnHeader orbitalFrameListDescription;
        private System.Windows.Forms.ImageList orbitalFrameListImages;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel;
    }
}

