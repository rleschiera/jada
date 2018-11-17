namespace winmanager
{
    partial class MgrFormParameters
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MgrFormParameters));
            this.btnChiudi = new System.Windows.Forms.Button();
            this.grdParametri = new System.Windows.Forms.DataGridView();
            this.Sezione = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Id = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Utente = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.grdParametri)).BeginInit();
            this.SuspendLayout();
            // 
            // btnChiudi
            // 
            this.btnChiudi.Location = new System.Drawing.Point(213, 244);
            this.btnChiudi.Name = "btnChiudi";
            this.btnChiudi.Size = new System.Drawing.Size(110, 23);
            this.btnChiudi.TabIndex = 2;
            this.btnChiudi.Text = "Chiudi";
            this.btnChiudi.UseVisualStyleBackColor = true;
            this.btnChiudi.Click += new System.EventHandler(this.btnAnnulla_Click);
            // 
            // grdParametri
            // 
            this.grdParametri.AllowUserToAddRows = false;
            this.grdParametri.AllowUserToDeleteRows = false;
            this.grdParametri.AllowUserToOrderColumns = true;
            this.grdParametri.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.grdParametri.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Sezione,
            this.Id,
            this.Utente});
            this.grdParametri.Location = new System.Drawing.Point(15, 12);
            this.grdParametri.Name = "grdParametri";
            this.grdParametri.ReadOnly = true;
            this.grdParametri.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.grdParametri.Size = new System.Drawing.Size(496, 226);
            this.grdParametri.TabIndex = 1;
            // 
            // Sezione
            // 
            this.Sezione.HeaderText = "Sezione";
            this.Sezione.Name = "Sezione";
            this.Sezione.ReadOnly = true;
            // 
            // Id
            // 
            this.Id.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Id.HeaderText = "Nome";
            this.Id.Name = "Id";
            this.Id.ReadOnly = true;
            this.Id.Width = 60;
            // 
            // Utente
            // 
            this.Utente.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Utente.HeaderText = "Valore";
            this.Utente.Name = "Utente";
            this.Utente.ReadOnly = true;
            this.Utente.Width = 62;
            // 
            // MgrFormParameters
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(523, 273);
            this.Controls.Add(this.grdParametri);
            this.Controls.Add(this.btnChiudi);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MgrFormParameters";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Guarda i parametri della richiesta";
            this.Load += new System.EventHandler(this.FormAcceptRequest_Load);
            ((System.ComponentModel.ISupportInitialize)(this.grdParametri)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnChiudi;
        private System.Windows.Forms.DataGridView grdParametri;
        private System.Windows.Forms.DataGridViewTextBoxColumn Sezione;
        private System.Windows.Forms.DataGridViewTextBoxColumn Id;
        private System.Windows.Forms.DataGridViewTextBoxColumn Utente;
    }
}