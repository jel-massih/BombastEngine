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
            this.shaderNameInput = new MetroFramework.Controls.MetroTextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.generationStatusLabel = new MetroFramework.Controls.MetroLabel();
            this.PIXEL_TEX_SAMPLE = new MetroFramework.Controls.MetroCheckBox();
            this.PIXEL_COLOR_DIRECT = new MetroFramework.Controls.MetroCheckBox();
            this.metroLabel7 = new MetroFramework.Controls.MetroLabel();
            this.PIXEL_OUT_GBUFFER = new MetroFramework.Controls.MetroCheckBox();
            this.metroLabel6 = new MetroFramework.Controls.MetroLabel();
            this.PIXEL_OUT_FLOAT4 = new MetroFramework.Controls.MetroCheckBox();
            this.ENABLE_TEXTURE_INPUT = new MetroFramework.Controls.MetroCheckBox();
            this.ENABLE_CAMERA_BUFFER = new MetroFramework.Controls.MetroCheckBox();
            this.metroLabel5 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel4 = new MetroFramework.Controls.MetroLabel();
            this.PIXEL_INPUT_VIEWDIR = new MetroFramework.Controls.MetroCheckBox();
            this.VERT_INPUT_NORMAL = new MetroFramework.Controls.MetroCheckBox();
            this.VERT_INPUT_TEX = new MetroFramework.Controls.MetroCheckBox();
            this.metroLabel3 = new MetroFramework.Controls.MetroLabel();
            this.VERT_INPUT_COLOR = new MetroFramework.Controls.MetroCheckBox();
            this.metroLabel2 = new MetroFramework.Controls.MetroLabel();
            this.presetSelect = new MetroFramework.Controls.MetroComboBox();
            this.currentEventLabel = new MetroFramework.Controls.MetroLabel();
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
            this.generateBtn.Click += new System.EventHandler(this.generateBtn_Click);
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
            // shaderNameInput
            // 
            this.shaderNameInput.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.shaderNameInput.Lines = new string[0];
            this.shaderNameInput.Location = new System.Drawing.Point(184, 7);
            this.shaderNameInput.MaxLength = 32767;
            this.shaderNameInput.Name = "shaderNameInput";
            this.shaderNameInput.PasswordChar = '\0';
            this.shaderNameInput.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.shaderNameInput.SelectedText = "";
            this.shaderNameInput.Size = new System.Drawing.Size(377, 23);
            this.shaderNameInput.TabIndex = 4;
            this.shaderNameInput.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.shaderNameInput.UseSelectable = true;
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
            this.panel2.Controls.Add(this.currentEventLabel);
            this.panel2.Controls.Add(this.generationStatusLabel);
            this.panel2.Controls.Add(this.PIXEL_TEX_SAMPLE);
            this.panel2.Controls.Add(this.PIXEL_COLOR_DIRECT);
            this.panel2.Controls.Add(this.metroLabel7);
            this.panel2.Controls.Add(this.PIXEL_OUT_GBUFFER);
            this.panel2.Controls.Add(this.metroLabel6);
            this.panel2.Controls.Add(this.PIXEL_OUT_FLOAT4);
            this.panel2.Controls.Add(this.ENABLE_TEXTURE_INPUT);
            this.panel2.Controls.Add(this.ENABLE_CAMERA_BUFFER);
            this.panel2.Controls.Add(this.metroLabel5);
            this.panel2.Controls.Add(this.metroLabel4);
            this.panel2.Controls.Add(this.PIXEL_INPUT_VIEWDIR);
            this.panel2.Controls.Add(this.VERT_INPUT_NORMAL);
            this.panel2.Controls.Add(this.VERT_INPUT_TEX);
            this.panel2.Controls.Add(this.metroLabel3);
            this.panel2.Controls.Add(this.VERT_INPUT_COLOR);
            this.panel2.Controls.Add(this.metroLabel2);
            this.panel2.Controls.Add(this.presetSelect);
            this.panel2.Controls.Add(this.shaderTypeLabel);
            this.panel2.Controls.Add(this.shaderModelSelect);
            this.panel2.Controls.Add(this.shaderNameInput);
            this.panel2.Controls.Add(this.metroLabel1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(20, 60);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(564, 567);
            this.panel2.TabIndex = 6;
            // 
            // generationStatusLabel
            // 
            this.generationStatusLabel.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.generationStatusLabel.Location = new System.Drawing.Point(3, 535);
            this.generationStatusLabel.Name = "generationStatusLabel";
            this.generationStatusLabel.Size = new System.Drawing.Size(558, 23);
            this.generationStatusLabel.TabIndex = 22;
            this.generationStatusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.generationStatusLabel.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.generationStatusLabel.UseCustomForeColor = true;
            // 
            // PIXEL_TEX_SAMPLE
            // 
            this.PIXEL_TEX_SAMPLE.AutoSize = true;
            this.PIXEL_TEX_SAMPLE.Location = new System.Drawing.Point(153, 329);
            this.PIXEL_TEX_SAMPLE.Name = "PIXEL_TEX_SAMPLE";
            this.PIXEL_TEX_SAMPLE.Size = new System.Drawing.Size(115, 15);
            this.PIXEL_TEX_SAMPLE.TabIndex = 21;
            this.PIXEL_TEX_SAMPLE.Text = "Texture Sampling";
            this.PIXEL_TEX_SAMPLE.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.PIXEL_TEX_SAMPLE.UseSelectable = true;
            // 
            // PIXEL_COLOR_DIRECT
            // 
            this.PIXEL_COLOR_DIRECT.AutoSize = true;
            this.PIXEL_COLOR_DIRECT.Location = new System.Drawing.Point(48, 329);
            this.PIXEL_COLOR_DIRECT.Name = "PIXEL_COLOR_DIRECT";
            this.PIXEL_COLOR_DIRECT.Size = new System.Drawing.Size(105, 15);
            this.PIXEL_COLOR_DIRECT.TabIndex = 20;
            this.PIXEL_COLOR_DIRECT.Text = "Color Sampling";
            this.PIXEL_COLOR_DIRECT.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.PIXEL_COLOR_DIRECT.UseSelectable = true;
            // 
            // metroLabel7
            // 
            this.metroLabel7.AutoSize = true;
            this.metroLabel7.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel7.Location = new System.Drawing.Point(22, 307);
            this.metroLabel7.Name = "metroLabel7";
            this.metroLabel7.Size = new System.Drawing.Size(154, 19);
            this.metroLabel7.TabIndex = 19;
            this.metroLabel7.Text = "Pixel Shader Transforms";
            this.metroLabel7.Theme = MetroFramework.MetroThemeStyle.Dark;
            // 
            // PIXEL_OUT_GBUFFER
            // 
            this.PIXEL_OUT_GBUFFER.AutoSize = true;
            this.PIXEL_OUT_GBUFFER.Location = new System.Drawing.Point(159, 287);
            this.PIXEL_OUT_GBUFFER.Name = "PIXEL_OUT_GBUFFER";
            this.PIXEL_OUT_GBUFFER.Size = new System.Drawing.Size(148, 15);
            this.PIXEL_OUT_GBUFFER.TabIndex = 18;
            this.PIXEL_OUT_GBUFFER.Text = "GBuffer Diffuse/Normal";
            this.PIXEL_OUT_GBUFFER.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.PIXEL_OUT_GBUFFER.UseSelectable = true;
            // 
            // metroLabel6
            // 
            this.metroLabel6.AutoSize = true;
            this.metroLabel6.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel6.Location = new System.Drawing.Point(22, 260);
            this.metroLabel6.Name = "metroLabel6";
            this.metroLabel6.Size = new System.Drawing.Size(137, 19);
            this.metroLabel6.TabIndex = 17;
            this.metroLabel6.Text = "Pixel Shader Outputs";
            this.metroLabel6.Theme = MetroFramework.MetroThemeStyle.Dark;
            // 
            // PIXEL_OUT_FLOAT4
            // 
            this.PIXEL_OUT_FLOAT4.AutoSize = true;
            this.PIXEL_OUT_FLOAT4.Location = new System.Drawing.Point(48, 287);
            this.PIXEL_OUT_FLOAT4.Name = "PIXEL_OUT_FLOAT4";
            this.PIXEL_OUT_FLOAT4.Size = new System.Drawing.Size(105, 15);
            this.PIXEL_OUT_FLOAT4.TabIndex = 16;
            this.PIXEL_OUT_FLOAT4.Text = "Standard Float4";
            this.PIXEL_OUT_FLOAT4.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.PIXEL_OUT_FLOAT4.UseSelectable = true;
            // 
            // ENABLE_TEXTURE_INPUT
            // 
            this.ENABLE_TEXTURE_INPUT.AutoSize = true;
            this.ENABLE_TEXTURE_INPUT.Location = new System.Drawing.Point(153, 240);
            this.ENABLE_TEXTURE_INPUT.Name = "ENABLE_TEXTURE_INPUT";
            this.ENABLE_TEXTURE_INPUT.Size = new System.Drawing.Size(145, 15);
            this.ENABLE_TEXTURE_INPUT.TabIndex = 15;
            this.ENABLE_TEXTURE_INPUT.Text = "Single Texture/Sampler";
            this.ENABLE_TEXTURE_INPUT.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.ENABLE_TEXTURE_INPUT.UseSelectable = true;
            // 
            // ENABLE_CAMERA_BUFFER
            // 
            this.ENABLE_CAMERA_BUFFER.AutoSize = true;
            this.ENABLE_CAMERA_BUFFER.Location = new System.Drawing.Point(48, 240);
            this.ENABLE_CAMERA_BUFFER.Name = "ENABLE_CAMERA_BUFFER";
            this.ENABLE_CAMERA_BUFFER.Size = new System.Drawing.Size(99, 15);
            this.ENABLE_CAMERA_BUFFER.TabIndex = 14;
            this.ENABLE_CAMERA_BUFFER.Text = "Camera Buffer";
            this.ENABLE_CAMERA_BUFFER.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.ENABLE_CAMERA_BUFFER.UseSelectable = true;
            // 
            // metroLabel5
            // 
            this.metroLabel5.AutoSize = true;
            this.metroLabel5.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel5.Location = new System.Drawing.Point(22, 218);
            this.metroLabel5.Name = "metroLabel5";
            this.metroLabel5.Size = new System.Drawing.Size(111, 19);
            this.metroLabel5.TabIndex = 13;
            this.metroLabel5.Text = "Constant Buffers";
            this.metroLabel5.Theme = MetroFramework.MetroThemeStyle.Dark;
            // 
            // metroLabel4
            // 
            this.metroLabel4.AutoSize = true;
            this.metroLabel4.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel4.Location = new System.Drawing.Point(22, 169);
            this.metroLabel4.Name = "metroLabel4";
            this.metroLabel4.Size = new System.Drawing.Size(125, 19);
            this.metroLabel4.TabIndex = 12;
            this.metroLabel4.Text = "Pixel Shader Inputs";
            this.metroLabel4.Theme = MetroFramework.MetroThemeStyle.Dark;
            // 
            // PIXEL_INPUT_VIEWDIR
            // 
            this.PIXEL_INPUT_VIEWDIR.AutoSize = true;
            this.PIXEL_INPUT_VIEWDIR.Location = new System.Drawing.Point(48, 198);
            this.PIXEL_INPUT_VIEWDIR.Name = "PIXEL_INPUT_VIEWDIR";
            this.PIXEL_INPUT_VIEWDIR.Size = new System.Drawing.Size(155, 15);
            this.PIXEL_INPUT_VIEWDIR.TabIndex = 11;
            this.PIXEL_INPUT_VIEWDIR.Text = "viewDirection TexCoord1";
            this.PIXEL_INPUT_VIEWDIR.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.PIXEL_INPUT_VIEWDIR.UseSelectable = true;
            // 
            // VERT_INPUT_NORMAL
            // 
            this.VERT_INPUT_NORMAL.AutoSize = true;
            this.VERT_INPUT_NORMAL.Location = new System.Drawing.Point(220, 148);
            this.VERT_INPUT_NORMAL.Name = "VERT_INPUT_NORMAL";
            this.VERT_INPUT_NORMAL.Size = new System.Drawing.Size(96, 15);
            this.VERT_INPUT_NORMAL.TabIndex = 10;
            this.VERT_INPUT_NORMAL.Text = "normal Float3";
            this.VERT_INPUT_NORMAL.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.VERT_INPUT_NORMAL.UseSelectable = true;
            // 
            // VERT_INPUT_TEX
            // 
            this.VERT_INPUT_TEX.AutoSize = true;
            this.VERT_INPUT_TEX.Location = new System.Drawing.Point(141, 148);
            this.VERT_INPUT_TEX.Name = "VERT_INPUT_TEX";
            this.VERT_INPUT_TEX.Size = new System.Drawing.Size(73, 15);
            this.VERT_INPUT_TEX.TabIndex = 9;
            this.VERT_INPUT_TEX.Text = "tex Float2";
            this.VERT_INPUT_TEX.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.VERT_INPUT_TEX.UseSelectable = true;
            // 
            // metroLabel3
            // 
            this.metroLabel3.AutoSize = true;
            this.metroLabel3.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel3.Location = new System.Drawing.Point(22, 119);
            this.metroLabel3.Name = "metroLabel3";
            this.metroLabel3.Size = new System.Drawing.Size(137, 19);
            this.metroLabel3.TabIndex = 8;
            this.metroLabel3.Text = "Vertex Shader Inputs";
            this.metroLabel3.Theme = MetroFramework.MetroThemeStyle.Dark;
            // 
            // VERT_INPUT_COLOR
            // 
            this.VERT_INPUT_COLOR.AutoSize = true;
            this.VERT_INPUT_COLOR.Location = new System.Drawing.Point(48, 148);
            this.VERT_INPUT_COLOR.Name = "VERT_INPUT_COLOR";
            this.VERT_INPUT_COLOR.Size = new System.Drawing.Size(85, 15);
            this.VERT_INPUT_COLOR.TabIndex = 7;
            this.VERT_INPUT_COLOR.Text = "color Float4";
            this.VERT_INPUT_COLOR.Theme = MetroFramework.MetroThemeStyle.Dark;
            this.VERT_INPUT_COLOR.UseSelectable = true;
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
            // currentEventLabel
            // 
            this.currentEventLabel.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.currentEventLabel.Location = new System.Drawing.Point(0, 512);
            this.currentEventLabel.Name = "currentEventLabel";
            this.currentEventLabel.Size = new System.Drawing.Size(558, 23);
            this.currentEventLabel.TabIndex = 23;
            this.currentEventLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.currentEventLabel.Theme = MetroFramework.MetroThemeStyle.Dark;
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
        private MetroFramework.Controls.MetroTextBox shaderNameInput;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private MetroFramework.Controls.MetroLabel metroLabel2;
        private MetroFramework.Controls.MetroComboBox presetSelect;
        private MetroFramework.Controls.MetroCheckBox VERT_INPUT_NORMAL;
        private MetroFramework.Controls.MetroCheckBox VERT_INPUT_TEX;
        private MetroFramework.Controls.MetroLabel metroLabel3;
        private MetroFramework.Controls.MetroCheckBox VERT_INPUT_COLOR;
        private MetroFramework.Controls.MetroLabel metroLabel4;
        private MetroFramework.Controls.MetroCheckBox PIXEL_INPUT_VIEWDIR;
        private MetroFramework.Controls.MetroCheckBox ENABLE_TEXTURE_INPUT;
        private MetroFramework.Controls.MetroCheckBox ENABLE_CAMERA_BUFFER;
        private MetroFramework.Controls.MetroLabel metroLabel5;
        private MetroFramework.Controls.MetroCheckBox PIXEL_OUT_GBUFFER;
        private MetroFramework.Controls.MetroLabel metroLabel6;
        private MetroFramework.Controls.MetroCheckBox PIXEL_OUT_FLOAT4;
        private MetroFramework.Controls.MetroCheckBox PIXEL_COLOR_DIRECT;
        private MetroFramework.Controls.MetroLabel metroLabel7;
        private MetroFramework.Controls.MetroCheckBox PIXEL_TEX_SAMPLE;
        private MetroFramework.Controls.MetroLabel generationStatusLabel;
        private MetroFramework.Controls.MetroLabel currentEventLabel;

    }
}

