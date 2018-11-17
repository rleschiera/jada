namespace winagent
{
    partial class AgtFormMain
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AgtFormMain));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnSolicitSupport = new System.Windows.Forms.Button();
            this.btnExit = new System.Windows.Forms.Button();
            this.btnCloseSupport = new System.Windows.Forms.Button();
            this.btnConnectSupport = new System.Windows.Forms.Button();
            this.btnOpenSupport = new System.Windows.Forms.Button();
            this.btnTest = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.txtStatus = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.txtChatList = new System.Windows.Forms.RichTextBox();
            this.btnSendChat = new System.Windows.Forms.Button();
            this.txtChat = new System.Windows.Forms.TextBox();
            this.picStatus = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picStatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnSolicitSupport);
            this.groupBox1.Controls.Add(this.btnExit);
            this.groupBox1.Controls.Add(this.btnCloseSupport);
            this.groupBox1.Controls.Add(this.btnConnectSupport);
            this.groupBox1.Controls.Add(this.btnOpenSupport);
            this.groupBox1.Location = new System.Drawing.Point(4, 71);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(129, 235);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Comandi";
            // 
            // btnSolicitSupport
            // 
            this.btnSolicitSupport.Enabled = false;
            this.btnSolicitSupport.Location = new System.Drawing.Point(7, 107);
            this.btnSolicitSupport.Name = "btnSolicitSupport";
            this.btnSolicitSupport.Size = new System.Drawing.Size(115, 40);
            this.btnSolicitSupport.TabIndex = 3;
            this.btnSolicitSupport.Text = "Sollecita una tua richiesta in attesa";
            this.btnSolicitSupport.UseVisualStyleBackColor = true;
            this.btnSolicitSupport.Click += new System.EventHandler(this.btnSolicitSupport_Click);
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(7, 198);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(116, 30);
            this.btnExit.TabIndex = 5;
            this.btnExit.Text = "Esci";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // btnCloseSupport
            // 
            this.btnCloseSupport.Enabled = false;
            this.btnCloseSupport.Location = new System.Drawing.Point(7, 152);
            this.btnCloseSupport.Name = "btnCloseSupport";
            this.btnCloseSupport.Size = new System.Drawing.Size(115, 40);
            this.btnCloseSupport.TabIndex = 4;
            this.btnCloseSupport.Text = "Chiudi la tua richiesta di supporto attuale";
            this.btnCloseSupport.UseVisualStyleBackColor = true;
            this.btnCloseSupport.Click += new System.EventHandler(this.btnCloseSupport_Click);
            // 
            // btnConnectSupport
            // 
            this.btnConnectSupport.Location = new System.Drawing.Point(7, 62);
            this.btnConnectSupport.Name = "btnConnectSupport";
            this.btnConnectSupport.Size = new System.Drawing.Size(115, 40);
            this.btnConnectSupport.TabIndex = 2;
            this.btnConnectSupport.Text = "Riprendi una tua richiesta precedente";
            this.btnConnectSupport.UseVisualStyleBackColor = true;
            this.btnConnectSupport.Click += new System.EventHandler(this.btnConnectSupport_Click);
            // 
            // btnOpenSupport
            // 
            this.btnOpenSupport.Location = new System.Drawing.Point(8, 17);
            this.btnOpenSupport.Name = "btnOpenSupport";
            this.btnOpenSupport.Size = new System.Drawing.Size(114, 40);
            this.btnOpenSupport.TabIndex = 1;
            this.btnOpenSupport.Text = "Richiedi supporto ad un operatore";
            this.btnOpenSupport.UseVisualStyleBackColor = true;
            this.btnOpenSupport.Click += new System.EventHandler(this.btnOpenSupport_Click);
            // 
            // btnTest
            // 
            this.btnTest.Location = new System.Drawing.Point(424, 230);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(39, 23);
            this.btnTest.TabIndex = 5;
            this.btnTest.Text = "Test";
            this.btnTest.UseVisualStyleBackColor = true;
            this.btnTest.Visible = false;
            this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.picStatus);
            this.groupBox2.Controls.Add(this.txtStatus);
            this.groupBox2.Location = new System.Drawing.Point(139, 242);
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
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.groupBox4);
            this.groupBox3.Controls.Add(this.txtChatList);
            this.groupBox3.Controls.Add(this.btnSendChat);
            this.groupBox3.Controls.Add(this.txtChat);
            this.groupBox3.Location = new System.Drawing.Point(139, 2);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(350, 235);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Chat";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.button1);
            this.groupBox4.Location = new System.Drawing.Point(410, -3);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(405, 235);
            this.groupBox4.TabIndex = 4;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Video";
            // 
            // button1
            // 
            this.button1.Enabled = false;
            this.button1.Location = new System.Drawing.Point(321, 194);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 2;
            this.button1.Text = "Invia";
            this.button1.UseVisualStyleBackColor = true;
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
            this.txtChatList.TextChanged += new System.EventHandler(this.txtChatList_TextChanged);
            // 
            // btnSendChat
            // 
            this.btnSendChat.Enabled = false;
            this.btnSendChat.Location = new System.Drawing.Point(265, 191);
            this.btnSendChat.Name = "btnSendChat";
            this.btnSendChat.Size = new System.Drawing.Size(75, 23);
            this.btnSendChat.TabIndex = 7;
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
            this.txtChat.TabIndex = 6;
            this.txtChat.TextChanged += new System.EventHandler(this.txtChat_TextChanged);
            // 
            // picStatus
            // 
            this.picStatus.Location = new System.Drawing.Point(8, 25);
            this.picStatus.Name = "picStatus";
            this.picStatus.Size = new System.Drawing.Size(23, 22);
            this.picStatus.TabIndex = 2;
            this.picStatus.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::winagent.Properties.Resources.logo;
            this.pictureBox1.Location = new System.Drawing.Point(4, 8);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(129, 57);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // AgtFormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(493, 310);
            this.Controls.Add(this.btnTest);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pictureBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "AgtFormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "JADA";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picStatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnOpenSupport;
        private System.Windows.Forms.TextBox txtStatus;
        private System.Windows.Forms.Button btnCloseSupport;
        private System.Windows.Forms.Button btnConnectSupport;
        private System.Windows.Forms.Button btnSendChat;
        private System.Windows.Forms.TextBox txtChat;
        private System.Windows.Forms.RichTextBox txtChatList;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button btnSolicitSupport;
        private System.Windows.Forms.PictureBox picStatus;
        private System.Windows.Forms.Button btnTest;

    }
}

