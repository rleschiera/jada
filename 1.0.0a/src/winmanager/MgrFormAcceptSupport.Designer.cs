namespace winmanager
{
    partial class MgrFormAcceptSupport
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MgrFormAcceptSupport));
            this.label1 = new System.Windows.Forms.Label();
            this.btnAnnulla = new System.Windows.Forms.Button();
            this.btnGestisci = new System.Windows.Forms.Button();
            this.grdRichieste = new System.Windows.Forms.DataGridView();
            this.label2 = new System.Windows.Forms.Label();
            this.cmbCoda = new System.Windows.Forms.ComboBox();
            this.Id = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Utente = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Cliente = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Telefono = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Cellulare = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Mail = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Descrizione = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.grdRichieste)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(12, 37);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(317, 16);
            this.label1.TabIndex = 8;
            this.label1.Text = "Seleziona la richiesta che vuoi prendere in gestione:";
            // 
            // btnAnnulla
            // 
            this.btnAnnulla.Location = new System.Drawing.Point(280, 244);
            this.btnAnnulla.Name = "btnAnnulla";
            this.btnAnnulla.Size = new System.Drawing.Size(110, 23);
            this.btnAnnulla.TabIndex = 4;
            this.btnAnnulla.Text = "Annulla";
            this.btnAnnulla.UseVisualStyleBackColor = true;
            this.btnAnnulla.Click += new System.EventHandler(this.btnAnnulla_Click);
            // 
            // btnGestisci
            // 
            this.btnGestisci.Location = new System.Drawing.Point(135, 244);
            this.btnGestisci.Name = "btnGestisci";
            this.btnGestisci.Size = new System.Drawing.Size(110, 23);
            this.btnGestisci.TabIndex = 3;
            this.btnGestisci.Text = "Gestisci";
            this.btnGestisci.UseVisualStyleBackColor = true;
            this.btnGestisci.Click += new System.EventHandler(this.btnGestisci_Click);
            // 
            // grdRichieste
            // 
            this.grdRichieste.AllowUserToAddRows = false;
            this.grdRichieste.AllowUserToDeleteRows = false;
            this.grdRichieste.AllowUserToOrderColumns = true;
            this.grdRichieste.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.grdRichieste.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Id,
            this.Utente,
            this.Cliente,
            this.Telefono,
            this.Cellulare,
            this.Mail,
            this.Descrizione});
            this.grdRichieste.Location = new System.Drawing.Point(15, 56);
            this.grdRichieste.Name = "grdRichieste";
            this.grdRichieste.ReadOnly = true;
            this.grdRichieste.RowHeadersVisible = false;
            this.grdRichieste.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.grdRichieste.Size = new System.Drawing.Size(496, 182);
            this.grdRichieste.TabIndex = 2;
            this.grdRichieste.DoubleClick += new System.EventHandler(this.grdRichieste_DoubleClick);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(12, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(234, 16);
            this.label2.TabIndex = 9;
            this.label2.Text = "Filtra le richieste per la seguente area:";
            // 
            // cmbCoda
            // 
            this.cmbCoda.DisplayMember = "AMC";
            this.cmbCoda.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbCoda.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmbCoda.FormattingEnabled = true;
            this.cmbCoda.Location = new System.Drawing.Point(263, 6);
            this.cmbCoda.Name = "cmbCoda";
            this.cmbCoda.Size = new System.Drawing.Size(248, 24);
            this.cmbCoda.TabIndex = 1;
            this.cmbCoda.SelectedIndexChanged += new System.EventHandler(this.cmbArea_SelectedIndexChanged);
            // 
            // Id
            // 
            this.Id.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Id.Frozen = true;
            this.Id.HeaderText = "Id";
            this.Id.Name = "Id";
            this.Id.ReadOnly = true;
            this.Id.Width = 41;
            // 
            // Utente
            // 
            this.Utente.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Utente.HeaderText = "Richiedente";
            this.Utente.Name = "Utente";
            this.Utente.ReadOnly = true;
            this.Utente.Width = 89;
            // 
            // Cliente
            // 
            this.Cliente.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Cliente.HeaderText = "Cliente";
            this.Cliente.Name = "Cliente";
            this.Cliente.ReadOnly = true;
            this.Cliente.Width = 64;
            // 
            // Telefono
            // 
            this.Telefono.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Telefono.HeaderText = "Telefono";
            this.Telefono.Name = "Telefono";
            this.Telefono.ReadOnly = true;
            this.Telefono.Width = 74;
            // 
            // Cellulare
            // 
            this.Cellulare.HeaderText = "Cellulare";
            this.Cellulare.Name = "Cellulare";
            this.Cellulare.ReadOnly = true;
            // 
            // Mail
            // 
            this.Mail.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Mail.HeaderText = "Mail";
            this.Mail.Name = "Mail";
            this.Mail.ReadOnly = true;
            this.Mail.Width = 51;
            // 
            // Descrizione
            // 
            this.Descrizione.HeaderText = "Descrizione";
            this.Descrizione.Name = "Descrizione";
            this.Descrizione.ReadOnly = true;
            // 
            // MgrFormAcceptSupport
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(523, 273);
            this.Controls.Add(this.cmbCoda);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.grdRichieste);
            this.Controls.Add(this.btnAnnulla);
            this.Controls.Add(this.btnGestisci);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MgrFormAcceptSupport";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Apri una nuova richiesta di supporto";
            this.Load += new System.EventHandler(this.FormAcceptRequest_Load);
            ((System.ComponentModel.ISupportInitialize)(this.grdRichieste)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnAnnulla;
        private System.Windows.Forms.Button btnGestisci;
        private System.Windows.Forms.DataGridView grdRichieste;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cmbCoda;
        private System.Windows.Forms.DataGridViewTextBoxColumn Id;
        private System.Windows.Forms.DataGridViewTextBoxColumn Utente;
        private System.Windows.Forms.DataGridViewTextBoxColumn Cliente;
        private System.Windows.Forms.DataGridViewTextBoxColumn Telefono;
        private System.Windows.Forms.DataGridViewTextBoxColumn Cellulare;
        private System.Windows.Forms.DataGridViewTextBoxColumn Mail;
        private System.Windows.Forms.DataGridViewTextBoxColumn Descrizione;
    }
}