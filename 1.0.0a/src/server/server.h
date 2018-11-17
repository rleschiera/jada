#pragma once

namespace jadaserver {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Riepilogo per server
	/// </summary>
	public ref class server : public System::Windows::Forms::Form
	{
	public:
		server(void)
		{
			InitializeComponent();
			//
			//TODO: aggiungere qui il codice del costruttore.
			//
		}

	protected:
		/// <summary>
		/// Liberare le risorse in uso.
		/// </summary>
		~server()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Variabile di progettazione necessaria.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metodo necessario per il supporto della finestra di progettazione. Non modificare
		/// il contenuto del metodo con l'editor di codice.
		/// </summary>
		void InitializeComponent(void)
		{
            this->SuspendLayout();
            // 
            // server
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(284, 262);
            this->Name = L"server";
            this->Text = L"server";
            this->Load += gcnew System::EventHandler(this, &server::server_Load);
            this->ResumeLayout(false);

        }
#pragma endregion
    private: System::Void server_Load(System::Object^  sender, System::EventArgs^  e) {
             }
    };
}
