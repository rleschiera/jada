namespace winagent
{
    partial class AgtFormCall
    {
        /// <summary>
        /// Variabile di progettazione necessaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Liberare le risorse in uso.
        /// </summary>
        /// <param name="disposing">ha valore true se le risorse gestite devono essere eliminate, false in caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Codice generato da Progettazione Windows Form

        /// <summary>
        /// Metodo necessario per il supporto della finestra di progettazione. Non modificare
        /// il contenuto del metodo con l'editor di codice.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AgtFormCall));
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.btnCallReset = new System.Windows.Forms.Button();
            this.btnCloseCall = new System.Windows.Forms.Button();
            this.btnMute = new System.Windows.Forms.Button();
            this.picVideo = new System.Windows.Forms.PictureBox();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picVideo)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.btnCallReset);
            this.groupBox5.Controls.Add(this.btnCloseCall);
            this.groupBox5.Controls.Add(this.trackBar1);
            this.groupBox5.Controls.Add(this.btnMute);
            this.groupBox5.Controls.Add(this.picVideo);
            this.groupBox5.Location = new System.Drawing.Point(5, 2);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(335, 306);
            this.groupBox5.TabIndex = 4;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Video";
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(201, 262);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(125, 42);
            this.trackBar1.TabIndex = 3;
            this.trackBar1.Value = 8;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // btnCallReset
            // 
            this.btnCallReset.Enabled = false;
            this.btnCallReset.Image = global::winagent.Properties.Resources.call_reset;
            this.btnCallReset.Location = new System.Drawing.Point(72, 265);
            this.btnCallReset.Name = "btnCallReset";
            this.btnCallReset.Size = new System.Drawing.Size(60, 35);
            this.btnCallReset.TabIndex = 21;
            this.btnCallReset.UseVisualStyleBackColor = true;
            this.btnCallReset.Click += new System.EventHandler(this.btnCallReset_Click);
            // 
            // btnCloseCall
            // 
            this.btnCloseCall.Enabled = false;
            this.btnCloseCall.Image = global::winagent.Properties.Resources.call_off;
            this.btnCloseCall.Location = new System.Drawing.Point(6, 265);
            this.btnCloseCall.Name = "btnCloseCall";
            this.btnCloseCall.Size = new System.Drawing.Size(60, 35);
            this.btnCloseCall.TabIndex = 1;
            this.btnCloseCall.UseVisualStyleBackColor = true;
            this.btnCloseCall.Click += new System.EventHandler(this.btnCloseCall_Click);
            // 
            // btnMute
            // 
            this.btnMute.Enabled = false;
            this.btnMute.Image = global::winagent.Properties.Resources.microphone_off;
            this.btnMute.Location = new System.Drawing.Point(139, 265);
            this.btnMute.Name = "btnMute";
            this.btnMute.Size = new System.Drawing.Size(60, 35);
            this.btnMute.TabIndex = 2;
            this.btnMute.UseVisualStyleBackColor = true;
            this.btnMute.Click += new System.EventHandler(this.btnMute_Click);
            // 
            // picVideo
            // 
            this.picVideo.BackColor = System.Drawing.Color.DimGray;
            this.picVideo.Location = new System.Drawing.Point(7, 20);
            this.picVideo.Name = "picVideo";
            this.picVideo.Size = new System.Drawing.Size(320, 240);
            this.picVideo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picVideo.TabIndex = 18;
            this.picVideo.TabStop = false;
            // 
            // AgtFormCall
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(343, 310);
            this.Controls.Add(this.groupBox5);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "AgtFormCall";
            this.Text = "Chiamata in corso";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormCall_FormClosing);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picVideo)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button btnMute;
        private System.Windows.Forms.PictureBox picVideo;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Button btnCloseCall;
        private System.Windows.Forms.Button btnCallReset;

    }
}

