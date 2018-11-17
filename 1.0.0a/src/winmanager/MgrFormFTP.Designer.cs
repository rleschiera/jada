namespace winmanager
{
    partial class MgrFormFTP
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MgrFormFTP));
            this.lstAgent = new System.Windows.Forms.ListBox();
            this.lstManager = new System.Windows.Forms.ListBox();
            this.txtManagerCurrDir = new System.Windows.Forms.TextBox();
            this.txtAgentCurrDir = new System.Windows.Forms.TextBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.btnReceive = new System.Windows.Forms.Button();
            this.progGetFile = new System.Windows.Forms.ProgressBar();
            this.progPutFile = new System.Windows.Forms.ProgressBar();
            this.btnChiudi = new System.Windows.Forms.Button();
            this.txtReceivingFile = new System.Windows.Forms.TextBox();
            this.txtSendingFile = new System.Windows.Forms.TextBox();
            this.btnManagerRefresh = new System.Windows.Forms.Button();
            this.btnAgentRefresh = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lstAgent
            // 
            this.lstAgent.FormattingEnabled = true;
            this.lstAgent.Location = new System.Drawing.Point(12, 38);
            this.lstAgent.Name = "lstAgent";
            this.lstAgent.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lstAgent.Size = new System.Drawing.Size(283, 251);
            this.lstAgent.TabIndex = 3;
            this.lstAgent.DoubleClick += new System.EventHandler(this.lstAgent_DoubleClick);
            // 
            // lstManager
            // 
            this.lstManager.FormattingEnabled = true;
            this.lstManager.Location = new System.Drawing.Point(384, 38);
            this.lstManager.Name = "lstManager";
            this.lstManager.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lstManager.Size = new System.Drawing.Size(283, 251);
            this.lstManager.TabIndex = 4;
            this.lstManager.DoubleClick += new System.EventHandler(this.lstManager_DoubleClick);
            // 
            // txtManagerCurrDir
            // 
            this.txtManagerCurrDir.Location = new System.Drawing.Point(384, 12);
            this.txtManagerCurrDir.Name = "txtManagerCurrDir";
            this.txtManagerCurrDir.ReadOnly = true;
            this.txtManagerCurrDir.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal;
            this.txtManagerCurrDir.Size = new System.Drawing.Size(283, 20);
            this.txtManagerCurrDir.TabIndex = 3;
            this.txtManagerCurrDir.TabStop = false;
            // 
            // txtAgentCurrDir
            // 
            this.txtAgentCurrDir.Location = new System.Drawing.Point(12, 12);
            this.txtAgentCurrDir.Name = "txtAgentCurrDir";
            this.txtAgentCurrDir.ReadOnly = true;
            this.txtAgentCurrDir.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal;
            this.txtAgentCurrDir.Size = new System.Drawing.Size(283, 20);
            this.txtAgentCurrDir.TabIndex = 4;
            this.txtAgentCurrDir.TabStop = false;
            // 
            // btnSend
            // 
            this.btnSend.Location = new System.Drawing.Point(315, 124);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(50, 23);
            this.btnSend.TabIndex = 5;
            this.btnSend.Text = "<<";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // btnReceive
            // 
            this.btnReceive.Location = new System.Drawing.Point(315, 86);
            this.btnReceive.Name = "btnReceive";
            this.btnReceive.Size = new System.Drawing.Size(50, 23);
            this.btnReceive.TabIndex = 6;
            this.btnReceive.Text = ">>";
            this.btnReceive.UseVisualStyleBackColor = true;
            this.btnReceive.Click += new System.EventHandler(this.btnReceive_Click);
            // 
            // progGetFile
            // 
            this.progGetFile.Location = new System.Drawing.Point(384, 321);
            this.progGetFile.Name = "progGetFile";
            this.progGetFile.Size = new System.Drawing.Size(283, 23);
            this.progGetFile.TabIndex = 7;
            // 
            // progPutFile
            // 
            this.progPutFile.Location = new System.Drawing.Point(12, 321);
            this.progPutFile.Name = "progPutFile";
            this.progPutFile.Size = new System.Drawing.Size(283, 23);
            this.progPutFile.TabIndex = 8;
            // 
            // btnChiudi
            // 
            this.btnChiudi.Location = new System.Drawing.Point(302, 243);
            this.btnChiudi.Name = "btnChiudi";
            this.btnChiudi.Size = new System.Drawing.Size(77, 23);
            this.btnChiudi.TabIndex = 7;
            this.btnChiudi.Text = "Chiudi";
            this.btnChiudi.UseVisualStyleBackColor = true;
            this.btnChiudi.Click += new System.EventHandler(this.btnChiudi_Click);
            // 
            // txtReceivingFile
            // 
            this.txtReceivingFile.Location = new System.Drawing.Point(384, 295);
            this.txtReceivingFile.Name = "txtReceivingFile";
            this.txtReceivingFile.ReadOnly = true;
            this.txtReceivingFile.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal;
            this.txtReceivingFile.Size = new System.Drawing.Size(283, 20);
            this.txtReceivingFile.TabIndex = 10;
            this.txtReceivingFile.TabStop = false;
            // 
            // txtSendingFile
            // 
            this.txtSendingFile.Location = new System.Drawing.Point(12, 295);
            this.txtSendingFile.Name = "txtSendingFile";
            this.txtSendingFile.ReadOnly = true;
            this.txtSendingFile.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal;
            this.txtSendingFile.Size = new System.Drawing.Size(283, 20);
            this.txtSendingFile.TabIndex = 11;
            this.txtSendingFile.TabStop = false;
            // 
            // btnManagerRefresh
            // 
            this.btnManagerRefresh.Image = global::winmanager.Properties.Resources.refresh;
            this.btnManagerRefresh.ImageAlign = System.Drawing.ContentAlignment.BottomRight;
            this.btnManagerRefresh.Location = new System.Drawing.Point(355, 12);
            this.btnManagerRefresh.Name = "btnManagerRefresh";
            this.btnManagerRefresh.Size = new System.Drawing.Size(25, 25);
            this.btnManagerRefresh.TabIndex = 2;
            this.btnManagerRefresh.UseVisualStyleBackColor = true;
            this.btnManagerRefresh.Click += new System.EventHandler(this.btnManagerRefresh_Click);
            // 
            // btnAgentRefresh
            // 
            this.btnAgentRefresh.Image = global::winmanager.Properties.Resources.refresh;
            this.btnAgentRefresh.ImageAlign = System.Drawing.ContentAlignment.BottomRight;
            this.btnAgentRefresh.Location = new System.Drawing.Point(301, 12);
            this.btnAgentRefresh.Name = "btnAgentRefresh";
            this.btnAgentRefresh.Size = new System.Drawing.Size(25, 25);
            this.btnAgentRefresh.TabIndex = 1;
            this.btnAgentRefresh.UseVisualStyleBackColor = true;
            this.btnAgentRefresh.Click += new System.EventHandler(this.btnAgentRefresh_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Enabled = false;
            this.btnCancel.Location = new System.Drawing.Point(314, 164);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(50, 23);
            this.btnCancel.TabIndex = 12;
            this.btnCancel.Text = "Annulla";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // FormFTP
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(679, 349);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnManagerRefresh);
            this.Controls.Add(this.btnAgentRefresh);
            this.Controls.Add(this.txtSendingFile);
            this.Controls.Add(this.txtReceivingFile);
            this.Controls.Add(this.btnChiudi);
            this.Controls.Add(this.progPutFile);
            this.Controls.Add(this.progGetFile);
            this.Controls.Add(this.btnReceive);
            this.Controls.Add(this.btnSend);
            this.Controls.Add(this.txtAgentCurrDir);
            this.Controls.Add(this.txtManagerCurrDir);
            this.Controls.Add(this.lstManager);
            this.Controls.Add(this.lstAgent);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FormFTP";
            this.Text = "Scambia files con il PC remoto";
            this.Load += new System.EventHandler(this.FormFTP_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox lstAgent;
        private System.Windows.Forms.ListBox lstManager;
        private System.Windows.Forms.TextBox txtManagerCurrDir;
        private System.Windows.Forms.TextBox txtAgentCurrDir;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.Button btnReceive;
        private System.Windows.Forms.ProgressBar progGetFile;
        private System.Windows.Forms.ProgressBar progPutFile;
        private System.Windows.Forms.Button btnChiudi;
        private System.Windows.Forms.TextBox txtReceivingFile;
        private System.Windows.Forms.TextBox txtSendingFile;
        private System.Windows.Forms.Button btnAgentRefresh;
        private System.Windows.Forms.Button btnManagerRefresh;
        private System.Windows.Forms.Button btnCancel;

    }
}