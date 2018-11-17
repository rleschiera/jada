namespace winmanager
{
    partial class MgrFormMain
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MgrFormMain));
            this.grpCommands = new System.Windows.Forms.GroupBox();
            this.btnExit = new System.Windows.Forms.Button();
            this.btnPendingSupports = new System.Windows.Forms.Button();
            this.btnSupports = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.txtStatus = new System.Windows.Forms.TextBox();
            this.lblDelay = new System.Windows.Forms.Label();
            this.picStatus = new System.Windows.Forms.PictureBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.txtChatList = new System.Windows.Forms.RichTextBox();
            this.btnSendChat = new System.Windows.Forms.Button();
            this.txtChat = new System.Windows.Forms.TextBox();
            this.btnCloseSupport = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.txtDescription = new System.Windows.Forms.TextBox();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.grpActions = new System.Windows.Forms.GroupBox();
            this.btnOpenCall = new System.Windows.Forms.Button();
            this.btnShowConfig = new System.Windows.Forms.Button();
            this.btnOpenDesktop = new System.Windows.Forms.Button();
            this.grpCommands.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picStatus)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.grpActions.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpCommands
            // 
            this.grpCommands.Controls.Add(this.btnExit);
            this.grpCommands.Controls.Add(this.btnPendingSupports);
            this.grpCommands.Controls.Add(this.btnSupports);
            this.grpCommands.Location = new System.Drawing.Point(4, 74);
            this.grpCommands.Name = "grpCommands";
            this.grpCommands.Size = new System.Drawing.Size(138, 145);
            this.grpCommands.TabIndex = 1;
            this.grpCommands.TabStop = false;
            this.grpCommands.Text = "Comandi";
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(7, 108);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(125, 30);
            this.btnExit.TabIndex = 4;
            this.btnExit.Text = "Esci";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // btnPendingSupports
            // 
            this.btnPendingSupports.Location = new System.Drawing.Point(7, 62);
            this.btnPendingSupports.Name = "btnPendingSupports";
            this.btnPendingSupports.Size = new System.Drawing.Size(125, 40);
            this.btnPendingSupports.TabIndex = 2;
            this.btnPendingSupports.Text = "Riprendi una richiesta pending (-1)";
            this.btnPendingSupports.UseVisualStyleBackColor = true;
            this.btnPendingSupports.Click += new System.EventHandler(this.btnConnectRequest_Click);
            // 
            // btnSupports
            // 
            this.btnSupports.Location = new System.Drawing.Point(8, 17);
            this.btnSupports.Name = "btnSupports";
            this.btnSupports.Size = new System.Drawing.Size(124, 40);
            this.btnSupports.TabIndex = 1;
            this.btnSupports.Text = "Accetta una richiesta in coda (-1)";
            this.btnSupports.UseVisualStyleBackColor = true;
            this.btnSupports.Click += new System.EventHandler(this.btnOpenRequest_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.txtStatus);
            this.groupBox2.Controls.Add(this.lblDelay);
            this.groupBox2.Controls.Add(this.picStatus);
            this.groupBox2.Location = new System.Drawing.Point(148, 347);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(350, 66);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Stato";
            // 
            // txtStatus
            // 
            this.txtStatus.Location = new System.Drawing.Point(36, 17);
            this.txtStatus.Multiline = true;
            this.txtStatus.Name = "txtStatus";
            this.txtStatus.ReadOnly = true;
            this.txtStatus.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtStatus.Size = new System.Drawing.Size(308, 41);
            this.txtStatus.TabIndex = 1;
            this.txtStatus.TabStop = false;
            // 
            // lblDelay
            // 
            this.lblDelay.AutoSize = true;
            this.lblDelay.Location = new System.Drawing.Point(6, 43);
            this.lblDelay.Name = "lblDelay";
            this.lblDelay.Size = new System.Drawing.Size(13, 13);
            this.lblDelay.TabIndex = 3;
            this.lblDelay.Text = "0";
            // 
            // picStatus
            // 
            this.picStatus.Location = new System.Drawing.Point(8, 19);
            this.picStatus.Name = "picStatus";
            this.picStatus.Size = new System.Drawing.Size(23, 22);
            this.picStatus.TabIndex = 2;
            this.picStatus.TabStop = false;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.txtChatList);
            this.groupBox3.Controls.Add(this.btnSendChat);
            this.groupBox3.Controls.Add(this.txtChat);
            this.groupBox3.Location = new System.Drawing.Point(148, 107);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(350, 235);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Chat";
            // 
            // txtChatList
            // 
            this.txtChatList.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.txtChatList.Location = new System.Drawing.Point(8, 20);
            this.txtChatList.Name = "txtChatList";
            this.txtChatList.ReadOnly = true;
            this.txtChatList.Size = new System.Drawing.Size(332, 150);
            this.txtChatList.TabIndex = 3;
            this.txtChatList.TabStop = false;
            this.txtChatList.Text = "";
            // 
            // btnSendChat
            // 
            this.btnSendChat.Enabled = false;
            this.btnSendChat.Location = new System.Drawing.Point(265, 191);
            this.btnSendChat.Name = "btnSendChat";
            this.btnSendChat.Size = new System.Drawing.Size(75, 23);
            this.btnSendChat.TabIndex = 5;
            this.btnSendChat.Text = "Invia";
            this.btnSendChat.UseVisualStyleBackColor = true;
            this.btnSendChat.Click += new System.EventHandler(this.btnSendChat_Click);
            // 
            // txtChat
            // 
            this.txtChat.Location = new System.Drawing.Point(8, 176);
            this.txtChat.MaxLength = 1000;
            this.txtChat.Multiline = true;
            this.txtChat.Name = "txtChat";
            this.txtChat.Size = new System.Drawing.Size(247, 51);
            this.txtChat.TabIndex = 4;
            this.txtChat.TextChanged += new System.EventHandler(this.txtChat_TextChanged);
            // 
            // btnCloseSupport
            // 
            this.btnCloseSupport.Enabled = false;
            this.btnCloseSupport.Location = new System.Drawing.Point(7, 135);
            this.btnCloseSupport.Name = "btnCloseSupport";
            this.btnCloseSupport.Size = new System.Drawing.Size(125, 30);
            this.btnCloseSupport.TabIndex = 5;
            this.btnCloseSupport.Text = "Chiudi la richiesta";
            this.btnCloseSupport.UseVisualStyleBackColor = true;
            this.btnCloseSupport.Click += new System.EventHandler(this.btnCloseSupport_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.txtDescription);
            this.groupBox4.Location = new System.Drawing.Point(148, 3);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(350, 98);
            this.groupBox4.TabIndex = 5;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Dati della richiesta";
            // 
            // txtDescription
            // 
            this.txtDescription.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.txtDescription.Location = new System.Drawing.Point(8, 16);
            this.txtDescription.Multiline = true;
            this.txtDescription.Name = "txtDescription";
            this.txtDescription.ReadOnly = true;
            this.txtDescription.Size = new System.Drawing.Size(332, 76);
            this.txtDescription.TabIndex = 0;
            this.txtDescription.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::winmanager.Properties.Resources.logo;
            this.pictureBox1.Location = new System.Drawing.Point(4, 8);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(138, 57);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // grpActions
            // 
            this.grpActions.Controls.Add(this.btnOpenCall);
            this.grpActions.Controls.Add(this.btnShowConfig);
            this.grpActions.Controls.Add(this.btnOpenDesktop);
            this.grpActions.Controls.Add(this.btnCloseSupport);
            this.grpActions.Location = new System.Drawing.Point(4, 218);
            this.grpActions.Name = "grpActions";
            this.grpActions.Size = new System.Drawing.Size(138, 195);
            this.grpActions.TabIndex = 6;
            this.grpActions.TabStop = false;
            this.grpActions.Text = "Azioni";
            // 
            // btnOpenCall
            // 
            this.btnOpenCall.Enabled = false;
            this.btnOpenCall.Location = new System.Drawing.Point(7, 98);
            this.btnOpenCall.Name = "btnOpenCall";
            this.btnOpenCall.Size = new System.Drawing.Size(125, 30);
            this.btnOpenCall.TabIndex = 8;
            this.btnOpenCall.Text = "Chiama via VOIP";
            this.btnOpenCall.UseVisualStyleBackColor = true;
            this.btnOpenCall.Click += new System.EventHandler(this.btnOpenCall_Click);
            // 
            // btnShowConfig
            // 
            this.btnShowConfig.Enabled = false;
            this.btnShowConfig.Location = new System.Drawing.Point(8, 18);
            this.btnShowConfig.Name = "btnShowConfig";
            this.btnShowConfig.Size = new System.Drawing.Size(125, 40);
            this.btnShowConfig.TabIndex = 7;
            this.btnShowConfig.Text = "Guarda configurazione del PC remoto";
            this.btnShowConfig.UseVisualStyleBackColor = true;
            this.btnShowConfig.Click += new System.EventHandler(this.btnShowConfig_Click);
            // 
            // btnOpenDesktop
            // 
            this.btnOpenDesktop.Enabled = false;
            this.btnOpenDesktop.Location = new System.Drawing.Point(7, 63);
            this.btnOpenDesktop.Name = "btnOpenDesktop";
            this.btnOpenDesktop.Size = new System.Drawing.Size(125, 30);
            this.btnOpenDesktop.TabIndex = 6;
            this.btnOpenDesktop.Text = "Apri desktop remoto";
            this.btnOpenDesktop.UseVisualStyleBackColor = true;
            this.btnOpenDesktop.Click += new System.EventHandler(this.btnOpenDesktop_Click);
            // 
            // MgrFormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(505, 420);
            this.Controls.Add(this.grpActions);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.grpCommands);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MgrFormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "JADA Manager";
            this.grpCommands.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picStatus)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.grpActions.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.GroupBox grpCommands;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnSupports;
        private System.Windows.Forms.TextBox txtStatus;
        private System.Windows.Forms.Button btnPendingSupports;
        private System.Windows.Forms.Button btnSendChat;
        private System.Windows.Forms.TextBox txtChat;
        private System.Windows.Forms.RichTextBox txtChatList;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.PictureBox picStatus;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox txtDescription;
        private System.Windows.Forms.Label lblDelay;
        private System.Windows.Forms.ToolTip toolTip;
        private System.Windows.Forms.Button btnCloseSupport;
        private System.Windows.Forms.GroupBox grpActions;
        private System.Windows.Forms.Button btnOpenCall;
        private System.Windows.Forms.Button btnShowConfig;
        private System.Windows.Forms.Button btnOpenDesktop;

    }
}

