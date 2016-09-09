namespace BombastEditor
{
    partial class NewProjectDialog
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
            this.label1 = new System.Windows.Forms.Label();
            this.folderTextInput = new System.Windows.Forms.TextBox();
            this.nameTextInput = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.createProjectButton = new System.Windows.Forms.Button();
            this.browseFolderButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(328, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "Select a location for your project to be stored.";
            // 
            // folderTextInput
            // 
            this.folderTextInput.Location = new System.Drawing.Point(16, 52);
            this.folderTextInput.Name = "folderTextInput";
            this.folderTextInput.Size = new System.Drawing.Size(453, 26);
            this.folderTextInput.TabIndex = 1;
            // 
            // nameTextInput
            // 
            this.nameTextInput.Location = new System.Drawing.Point(475, 52);
            this.nameTextInput.Name = "nameTextInput";
            this.nameTextInput.Size = new System.Drawing.Size(230, 26);
            this.nameTextInput.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(212, 89);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(54, 20);
            this.label2.TabIndex = 3;
            this.label2.Text = "Folder";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(567, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(51, 20);
            this.label3.TabIndex = 4;
            this.label3.Text = "Name";
            // 
            // createProjectButton
            // 
            this.createProjectButton.Location = new System.Drawing.Point(571, 123);
            this.createProjectButton.Name = "createProjectButton";
            this.createProjectButton.Size = new System.Drawing.Size(134, 35);
            this.createProjectButton.TabIndex = 5;
            this.createProjectButton.Text = "Create Project";
            this.createProjectButton.UseVisualStyleBackColor = true;
            this.createProjectButton.Click += new System.EventHandler(this.createProjectButton_Click);
            // 
            // browseFolderButton
            // 
            this.browseFolderButton.Location = new System.Drawing.Point(387, 87);
            this.browseFolderButton.Name = "browseFolderButton";
            this.browseFolderButton.Size = new System.Drawing.Size(82, 31);
            this.browseFolderButton.TabIndex = 6;
            this.browseFolderButton.Text = "Browse...";
            this.browseFolderButton.UseVisualStyleBackColor = true;
            this.browseFolderButton.Click += new System.EventHandler(this.browseFolderButton_Click);
            // 
            // NewProjectDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(717, 170);
            this.Controls.Add(this.browseFolderButton);
            this.Controls.Add(this.createProjectButton);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.nameTextInput);
            this.Controls.Add(this.folderTextInput);
            this.Controls.Add(this.label1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NewProjectDialog";
            this.Text = "New Project";
            this.Load += new System.EventHandler(this.NewProjectDialog_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox folderTextInput;
        private System.Windows.Forms.TextBox nameTextInput;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button createProjectButton;
        private System.Windows.Forms.Button browseFolderButton;
    }
}