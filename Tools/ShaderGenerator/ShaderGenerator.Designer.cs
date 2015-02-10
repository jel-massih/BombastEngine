namespace ShaderGenerator
{
    partial class ShaderGenerator
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
            this.generateBtn = new MetroFramework.Controls.MetroButton();
            this.shaderModelSelect = new MetroFramework.Controls.MetroComboBox();
            this.shaderTypeLabel = new MetroFramework.Controls.MetroLabel();
            this.metroLabel1 = new MetroFramework.Controls.MetroLabel();
            this.metroTextBox1 = new MetroFramework.Controls.MetroTextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.metroLabel2 = new MetroFramework.Controls.MetroLabel();
            this.presetSelect = new MetroFramework.Controls.MetroComboBox();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // generateBtn
            // 
            this.generateBtn.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.generateBtn.BackColor = System.Drawing.Color.Black;
            this.generateBtn.ForeColor = System.Drawing.Color.White;
            this.generateBtn.Location = new System.Drawing.Point(209, 2);
            this.generateBtn.Name = "generateBtn";
            this.generateBtn.Size = new System.Drawing.Size(147, 35);
            this.generateBtn.TabIndex = 0;
            this.generateBtn.Text = "Generate Shader";
            this.generateBtn.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.generateBtn.UseSelectable = true;
            // 
            // shaderModelSelect
            // 
            this.shaderModelSelect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.shaderModelSelect.FormattingEnabled = true;
            this.shaderModelSelect.ItemHeight = 23;
            this.shaderModelSelect.Location = new System.Drawing.Point(184, 39);
            this.shaderModelSelect.Name = "shaderModelSelect";
            this.shaderModelSelect.Size = new System.Drawing.Size(380, 29);
            this.shaderModelSelect.TabIndex = 1;
            this.shaderModelSelect.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.shaderModelSelect.UseSelectable = true;
            // 
            // shaderTypeLabel
            // 
            this.shaderTypeLabel.AutoSize = true;
            this.shaderTypeLabel.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.shaderTypeLabel.Location = new System.Drawing.Point(22, 45);
            this.shaderTypeLabel.Name = "shaderTypeLabel";
            this.shaderTypeLabel.Size = new System.Drawing.Size(94, 19);
            this.shaderTypeLabel.TabIndex = 2;
            this.shaderTypeLabel.Text = "Shader Model";
            this.shaderTypeLabel.Theme = MetroFramework.MetroThemeStyle.Dark;
            // 
            // metroLabel1
            // 
            this.metroLabel1.AutoSize = true;
            this.metroLabel1.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel1.Location = new System.Drawing.Point(22, 7);
            this.metroLabel1.Name = "metroLabel1";
            this.metroLabel1.Size = new System.Drawing.Size(91, 19);
            this.metroLabel1.TabIndex = 3;
            this.metroLabel1.Text = "Shader Name";
            this.metroLabel1.Theme = MetroFramework.MetroThemeStyle.Dark;
            // 
            // metroTextBox1
            // 
            this.metroTextBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.metroTextBox1.Lines = new string[] {
        "metroTextBox1"};
            this.metroTextBox1.Location = new System.Drawing.Point(184, 7);
            this.metroTextBox1.MaxLength = 32767;
            this.metroTextBox1.Name = "metroTextBox1";
            this.metroTextBox1.PasswordChar = '\0';
            this.metroTextBox1.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.metroTextBox1.SelectedText = "";
            this.metroTextBox1.Size = new System.Drawing.Size(377, 23);
            this.metroTextBox1.TabIndex = 4;
            this.metroTextBox1.Text = "metroTextBox1";
            this.metroTextBox1.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.metroTextBox1.UseSelectable = true;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.generateBtn);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(20, 627);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(564, 38);
            this.panel1.TabIndex = 5;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.metroLabel2);
            this.panel2.Controls.Add(this.presetSelect);
            this.panel2.Controls.Add(this.shaderTypeLabel);
            this.panel2.Controls.Add(this.shaderModelSelect);
            this.panel2.Controls.Add(this.metroTextBox1);
            this.panel2.Controls.Add(this.metroLabel1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(20, 60);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(564, 567);
            this.panel2.TabIndex = 6;
            // 
            // metroLabel2
            // 
            this.metroLabel2.AutoSize = true;
            this.metroLabel2.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel2.Location = new System.Drawing.Point(22, 87);
            this.metroLabel2.Name = "metroLabel2";
            this.metroLabel2.Size = new System.Drawing.Size(93, 19);
            this.metroLabel2.TabIndex = 6;
            this.metroLabel2.Text = "Shader Preset";
            this.metroLabel2.Theme = MetroFramework.MetroThemeStyle.Dark;
            // 
            // presetSelect
            // 
            this.presetSelect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.presetSelect.FormattingEnabled = true;
            this.presetSelect.ItemHeight = 23;
            this.presetSelect.Location = new System.Drawing.Point(184, 81);
            this.presetSelect.Name = "presetSelect";
            this.presetSelect.Size = new System.Drawing.Size(380, 29);
            this.presetSelect.TabIndex = 5;
            this.presetSelect.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.presetSelect.UseSelectable = true;
            // 
            // ShaderGenerator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(604, 685);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Name = "ShaderGenerator";
            this.Style = MetroFramework.MetroColorStyle.Purple;
            this.Text = "Shader Generator";
            this.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private MetroFramework.Controls.MetroButton generateBtn;
        private MetroFramework.Controls.MetroComboBox shaderModelSelect;
        private MetroFramework.Controls.MetroLabel shaderTypeLabel;
        private MetroFramework.Controls.MetroLabel metroLabel1;
        private MetroFramework.Controls.MetroTextBox metroTextBox1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private MetroFramework.Controls.MetroLabel metroLabel2;
        private MetroFramework.Controls.MetroComboBox presetSelect;

    }
}

