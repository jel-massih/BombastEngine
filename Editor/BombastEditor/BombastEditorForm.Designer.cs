namespace BombastEditor
{
    partial class BombastEditorForm
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
            Shutdown();
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.EditorViewportPanel = new System.Windows.Forms.Panel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.ContentTabControl = new System.Windows.Forms.TabControl();
            this.AssetsTabPage = new System.Windows.Forms.TabPage();
            this.AssetsTreeView = new System.Windows.Forms.TreeView();
            this.ActorsTabPage = new System.Windows.Forms.TabPage();
            this.ActorsTreeView = new System.Windows.Forms.TreeView();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.ContentTabControl.SuspendLayout();
            this.AssetsTabPage.SuspendLayout();
            this.ActorsTabPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1489, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openProjectToolStripMenuItem,
            this.closeProjectToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openProjectToolStripMenuItem
            // 
            this.openProjectToolStripMenuItem.Name = "openProjectToolStripMenuItem";
            this.openProjectToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openProjectToolStripMenuItem.Text = "Open Project";
            this.openProjectToolStripMenuItem.Click += new System.EventHandler(this.openProjectToolStripMenuItem_Click);
            // 
            // closeProjectToolStripMenuItem
            // 
            this.closeProjectToolStripMenuItem.Name = "closeProjectToolStripMenuItem";
            this.closeProjectToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.closeProjectToolStripMenuItem.Text = "Close Project";
            this.closeProjectToolStripMenuItem.Click += new System.EventHandler(this.closeProjectToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.aboutToolStripMenuItem.Text = "About Bombast Editor";
            // 
            // EditorViewportPanel
            // 
            this.EditorViewportPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.EditorViewportPanel.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.EditorViewportPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EditorViewportPanel.Location = new System.Drawing.Point(0, 24);
            this.EditorViewportPanel.Name = "EditorViewportPanel";
            this.EditorViewportPanel.Size = new System.Drawing.Size(1489, 855);
            this.EditorViewportPanel.TabIndex = 1;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Right;
            this.splitContainer1.Location = new System.Drawing.Point(1132, 24);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.splitContainer1.Panel1.Controls.Add(this.ContentTabControl);
            this.splitContainer1.Size = new System.Drawing.Size(357, 855);
            this.splitContainer1.SplitterDistance = 503;
            this.splitContainer1.TabIndex = 2;
            // 
            // ContentTabControl
            // 
            this.ContentTabControl.Controls.Add(this.AssetsTabPage);
            this.ContentTabControl.Controls.Add(this.ActorsTabPage);
            this.ContentTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ContentTabControl.Location = new System.Drawing.Point(0, 0);
            this.ContentTabControl.Name = "ContentTabControl";
            this.ContentTabControl.SelectedIndex = 0;
            this.ContentTabControl.Size = new System.Drawing.Size(357, 503);
            this.ContentTabControl.TabIndex = 0;
            // 
            // AssetsTabPage
            // 
            this.AssetsTabPage.AccessibleName = "Assets Tree";
            this.AssetsTabPage.Controls.Add(this.AssetsTreeView);
            this.AssetsTabPage.Location = new System.Drawing.Point(4, 22);
            this.AssetsTabPage.Name = "AssetsTabPage";
            this.AssetsTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.AssetsTabPage.Size = new System.Drawing.Size(349, 477);
            this.AssetsTabPage.TabIndex = 0;
            this.AssetsTabPage.Text = "Assets Tree";
            this.AssetsTabPage.UseVisualStyleBackColor = true;
            // 
            // AssetsTreeView
            // 
            this.AssetsTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AssetsTreeView.Location = new System.Drawing.Point(3, 3);
            this.AssetsTreeView.Name = "AssetsTreeView";
            this.AssetsTreeView.Size = new System.Drawing.Size(343, 471);
            this.AssetsTreeView.TabIndex = 0;
            // 
            // ActorsTabPage
            // 
            this.ActorsTabPage.Controls.Add(this.ActorsTreeView);
            this.ActorsTabPage.Location = new System.Drawing.Point(4, 22);
            this.ActorsTabPage.Name = "ActorsTabPage";
            this.ActorsTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.ActorsTabPage.Size = new System.Drawing.Size(349, 477);
            this.ActorsTabPage.TabIndex = 1;
            this.ActorsTabPage.Text = "Actors";
            this.ActorsTabPage.UseVisualStyleBackColor = true;
            // 
            // ActorsTreeView
            // 
            this.ActorsTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ActorsTreeView.Location = new System.Drawing.Point(3, 3);
            this.ActorsTreeView.Name = "ActorsTreeView";
            this.ActorsTreeView.Size = new System.Drawing.Size(343, 471);
            this.ActorsTreeView.TabIndex = 0;
            this.ActorsTreeView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.ActorsTreeView_MouseDoubleClick);
            // 
            // BombastEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1489, 879);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.EditorViewportPanel);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "BombastEditorForm";
            this.Text = "BombastEditorForm";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ContentTabControl.ResumeLayout(false);
            this.AssetsTabPage.ResumeLayout(false);
            this.ActorsTabPage.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openProjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.Panel EditorViewportPanel;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl ContentTabControl;
        private System.Windows.Forms.TabPage AssetsTabPage;
        private System.Windows.Forms.TabPage ActorsTabPage;
        private System.Windows.Forms.TreeView AssetsTreeView;
        private System.Windows.Forms.TreeView ActorsTreeView;
        private System.Windows.Forms.ToolStripMenuItem closeProjectToolStripMenuItem;
    }
}