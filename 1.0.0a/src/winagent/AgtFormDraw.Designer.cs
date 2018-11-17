namespace winagent
{
    partial class AgtFormDraw
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AgtFormDraw));
            this.picPointer = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.picPointer)).BeginInit();
            this.SuspendLayout();
            // 
            // picPointer
            // 
            this.picPointer.Image = global::winagent.Properties.Resources.hand;
            this.picPointer.Location = new System.Drawing.Point(12, 12);
            this.picPointer.Name = "picPointer";
            this.picPointer.Size = new System.Drawing.Size(80, 80);
            this.picPointer.TabIndex = 0;
            this.picPointer.TabStop = false;
            this.picPointer.Visible = false;
            // 
            // AgtFormDraw
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightPink;
            this.ClientSize = new System.Drawing.Size(593, 389);
            this.Controls.Add(this.picPointer);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "AgtFormDraw";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Lavagna sul desktop";
            this.TransparencyKey = System.Drawing.Color.LightPink;
            this.Activated += new System.EventHandler(this.FormDraw_Activate);
            this.Load += new System.EventHandler(this.FormDraw_Load);
            ((System.ComponentModel.ISupportInitialize)(this.picPointer)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox picPointer;


    }
}

