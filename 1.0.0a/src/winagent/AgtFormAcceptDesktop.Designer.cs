namespace winagent
{
    partial class AgtFormAcceptDesktop
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AgtFormAcceptDesktop));
            this.btnAccetta = new System.Windows.Forms.Button();
            this.btnRifiuta = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnAccetta
            // 
            this.btnAccetta.Location = new System.Drawing.Point(127, 120);
            this.btnAccetta.Name = "btnAccetta";
            this.btnAccetta.Size = new System.Drawing.Size(75, 23);
            this.btnAccetta.TabIndex = 9;
            this.btnAccetta.Text = "Si";
            this.btnAccetta.UseVisualStyleBackColor = true;
            this.btnAccetta.Click += new System.EventHandler(this.btnAccetta_Click);
            // 
            // btnRifiuta
            // 
            this.btnRifiuta.Location = new System.Drawing.Point(228, 120);
            this.btnRifiuta.Name = "btnRifiuta";
            this.btnRifiuta.Size = new System.Drawing.Size(75, 23);
            this.btnRifiuta.TabIndex = 10;
            this.btnRifiuta.Text = "No";
            this.btnRifiuta.UseVisualStyleBackColor = true;
            this.btnRifiuta.Click += new System.EventHandler(this.btnRifiuta_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(16, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(391, 64);
            this.label1.TabIndex = 11;
            this.label1.Text = resources.GetString("label1.Text");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(58, 91);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(309, 16);
            this.label2.TabIndex = 12;
            this.label2.Text = "Accetti che l\'operatore si connetta al tuo computer?";
            // 
            // AgtFormAcceptDesktop
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(416, 157);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnRifiuta);
            this.Controls.Add(this.btnAccetta);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "AgtFormAcceptDesktop";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Richiesta di connessione al tuo computer";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnAccetta;
        private System.Windows.Forms.Button btnRifiuta;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}