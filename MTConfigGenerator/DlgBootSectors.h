#pragma once

#include "Config.h"
#include <vector>
#include <msclr\marshal_cppstd.h>

namespace MTConfigGenerator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	using namespace msclr::interop;

	/// <summary>
	/// Summary for DlgBootSectors
	/// </summary>
	public ref class DlgBootSectors : public System::Windows::Forms::Form
	{
	public:
		void			SetBootSec(int nSec, string& s)		{m_pBootSecStrArr->at(nSec) = s;}
		string			GetBootSec(int nSec)				{return m_pBootSecStrArr->at(nSec);}
		bool			GetOkClickedFlg()					{return m_bOkClicked;}

		bool			m_bOkClicked;
		vector<string>*	m_pBootSecStrArr;
		List<TextBox^>^ m_bs0TextboxArr;
		List<TextBox^>^ m_bs1TextboxArr;

	public: 

	public:
		DlgBootSectors(void)
		{
			InitializeComponent();

			//
			//TODO: Add the constructor code here
			//
			m_bOkClicked			= false;
			m_pBootSecStrArr		= new(vector<string>);
			m_bs0TextboxArr			= gcnew(List<TextBox^>);
			m_bs1TextboxArr			= gcnew(List<TextBox^>);

			//Set default values for boot sector writes.
			string strBs0, strBs1;
			for (int i=0; i<Config::kBS0Size; i++)
			{
				strBs0 = strBs0 += (((i / 4) % 2) != 0) ? "FF" : "00";
				strBs1 = strBs1 += (((i / 4) % 2) != 0) ? "FF" : "00";
			}
			m_pBootSecStrArr->push_back(strBs0);
			m_pBootSecStrArr->push_back(strBs1);

			m_bs0TextboxArr->Add(m_txtBS0Word0);
			m_bs0TextboxArr->Add(m_txtBS0Word1);
			m_bs0TextboxArr->Add(m_txtBS0Word2);
			m_bs0TextboxArr->Add(m_txtBS0Word3);
			m_bs0TextboxArr->Add(m_txtBS0Word4);
			m_bs0TextboxArr->Add(m_txtBS0Word5);
			m_bs0TextboxArr->Add(m_txtBS0Word6);
			m_bs0TextboxArr->Add(m_txtBS0Word7);
			m_bs0TextboxArr->Add(m_txtBS0Word8);
			m_bs0TextboxArr->Add(m_txtBS0Word9);
			m_bs0TextboxArr->Add(m_txtBS0Word10);
			m_bs0TextboxArr->Add(m_txtBS0Word11);
			m_bs0TextboxArr->Add(m_txtBS0Word12);
			m_bs0TextboxArr->Add(m_txtBS0Word13);
			m_bs0TextboxArr->Add(m_txtBS0Word14);
			m_bs0TextboxArr->Add(m_txtBS0Word15);

			m_bs1TextboxArr->Add(m_txtBS1Word0);
			m_bs1TextboxArr->Add(m_txtBS1Word1);
			m_bs1TextboxArr->Add(m_txtBS1Word2);
			m_bs1TextboxArr->Add(m_txtBS1Word3);
			m_bs1TextboxArr->Add(m_txtBS1Word4);
			m_bs1TextboxArr->Add(m_txtBS1Word5);
			m_bs1TextboxArr->Add(m_txtBS1Word6);
			m_bs1TextboxArr->Add(m_txtBS1Word7);
			m_bs1TextboxArr->Add(m_txtBS1Word8);
			m_bs1TextboxArr->Add(m_txtBS1Word9);
			m_bs1TextboxArr->Add(m_txtBS1Word10);
			m_bs1TextboxArr->Add(m_txtBS1Word11);
			m_bs1TextboxArr->Add(m_txtBS1Word12);
			m_bs1TextboxArr->Add(m_txtBS1Word13);
			m_bs1TextboxArr->Add(m_txtBS1Word14);
			m_bs1TextboxArr->Add(m_txtBS1Word15);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DlgBootSectors()
		{
			if (components)
			{
				delete components;
			}

			delete m_pBootSecStrArr;
		}

	private: System::Windows::Forms::Button^  btnOK;
	protected: 
	private: System::Windows::Forms::Button^  btnCancel;

	private: System::Windows::Forms::GroupBox^  m_grpBootSec1;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word0;
	private: System::Windows::Forms::Label^  m_lblBS1Word4;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word4;


	private: System::Windows::Forms::Label^  m_lblBS1Word0;
	private: System::Windows::Forms::Label^  m_lblBS1Word12;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word12;


	private: System::Windows::Forms::Label^  m_lblBS1Word8;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word8;











	private: System::Windows::Forms::GroupBox^  groupBox1;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word12;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word8;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word4;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word0;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word15;

	private: System::Windows::Forms::TextBox^  m_txtBS0Word11;

	private: System::Windows::Forms::TextBox^  m_txtBS0Word7;

	private: System::Windows::Forms::TextBox^  m_txtBS0Word3;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word14;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word10;



	private: System::Windows::Forms::TextBox^  m_txtBS0Word6;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word2;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word13;

	private: System::Windows::Forms::TextBox^  m_txtBS0Word9;

	private: System::Windows::Forms::TextBox^  m_txtBS0Word5;
	private: System::Windows::Forms::TextBox^  m_txtBS0Word1;






















	private: System::Windows::Forms::TextBox^  m_txtBS1Word15;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word11;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word7;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word3;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word14;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word10;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word6;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word2;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word13;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word9;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word5;
	private: System::Windows::Forms::TextBox^  m_txtBS1Word1;
















	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->m_grpBootSec1 = (gcnew System::Windows::Forms::GroupBox());
			this->m_txtBS1Word15 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word11 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word7 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word3 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word14 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word10 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word6 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word2 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word13 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word9 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word5 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS1Word1 = (gcnew System::Windows::Forms::TextBox());
			this->m_lblBS1Word12 = (gcnew System::Windows::Forms::Label());
			this->m_txtBS1Word12 = (gcnew System::Windows::Forms::TextBox());
			this->m_lblBS1Word8 = (gcnew System::Windows::Forms::Label());
			this->m_txtBS1Word8 = (gcnew System::Windows::Forms::TextBox());
			this->m_lblBS1Word4 = (gcnew System::Windows::Forms::Label());
			this->m_txtBS1Word4 = (gcnew System::Windows::Forms::TextBox());
			this->m_lblBS1Word0 = (gcnew System::Windows::Forms::Label());
			this->m_txtBS1Word0 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->m_txtBS0Word15 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word11 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word7 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word3 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word14 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word10 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word6 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word2 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word13 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word9 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word5 = (gcnew System::Windows::Forms::TextBox());
			this->m_txtBS0Word1 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->m_txtBS0Word12 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->m_txtBS0Word8 = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->m_txtBS0Word4 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->m_txtBS0Word0 = (gcnew System::Windows::Forms::TextBox());
			this->m_grpBootSec1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(119, 357);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 0;
			this->btnOK->Text = L"OK";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &DlgBootSectors::btnOK_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->Location = System::Drawing::Point(216, 357);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->TabIndex = 1;
			this->btnCancel->Text = L"Cancel";
			this->btnCancel->UseVisualStyleBackColor = true;
			// 
			// m_grpBootSec1
			// 
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word15);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word11);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word7);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word3);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word14);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word10);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word6);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word2);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word13);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word9);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word5);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word1);
			this->m_grpBootSec1->Controls->Add(this->m_lblBS1Word12);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word12);
			this->m_grpBootSec1->Controls->Add(this->m_lblBS1Word8);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word8);
			this->m_grpBootSec1->Controls->Add(this->m_lblBS1Word4);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word4);
			this->m_grpBootSec1->Controls->Add(this->m_lblBS1Word0);
			this->m_grpBootSec1->Controls->Add(this->m_txtBS1Word0);
			this->m_grpBootSec1->Location = System::Drawing::Point(12, 182);
			this->m_grpBootSec1->Name = L"m_grpBootSec1";
			this->m_grpBootSec1->Size = System::Drawing::Size(389, 150);
			this->m_grpBootSec1->TabIndex = 3;
			this->m_grpBootSec1->TabStop = false;
			this->m_grpBootSec1->Text = L"Boot Sector 1";
			// 
			// m_txtBS1Word15
			// 
			this->m_txtBS1Word15->Enabled = false;
			this->m_txtBS1Word15->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word15->Location = System::Drawing::Point(303, 111);
			this->m_txtBS1Word15->Name = L"m_txtBS1Word15";
			this->m_txtBS1Word15->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word15->TabIndex = 15;
			this->m_txtBS1Word15->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word11
			// 
			this->m_txtBS1Word11->Enabled = false;
			this->m_txtBS1Word11->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word11->Location = System::Drawing::Point(303, 85);
			this->m_txtBS1Word11->Name = L"m_txtBS1Word11";
			this->m_txtBS1Word11->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word11->TabIndex = 11;
			this->m_txtBS1Word11->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word7
			// 
			this->m_txtBS1Word7->Enabled = false;
			this->m_txtBS1Word7->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word7->Location = System::Drawing::Point(303, 59);
			this->m_txtBS1Word7->Name = L"m_txtBS1Word7";
			this->m_txtBS1Word7->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word7->TabIndex = 7;
			this->m_txtBS1Word7->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word3
			// 
			this->m_txtBS1Word3->Enabled = false;
			this->m_txtBS1Word3->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word3->Location = System::Drawing::Point(303, 33);
			this->m_txtBS1Word3->Name = L"m_txtBS1Word3";
			this->m_txtBS1Word3->Size = System::Drawing::Size(74, 20);
			this->m_txtBS1Word3->TabIndex = 3;
			this->m_txtBS1Word3->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word14
			// 
			this->m_txtBS1Word14->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word14->Location = System::Drawing::Point(223, 111);
			this->m_txtBS1Word14->Name = L"m_txtBS1Word14";
			this->m_txtBS1Word14->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word14->TabIndex = 14;
			this->m_txtBS1Word14->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word10
			// 
			this->m_txtBS1Word10->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word10->Location = System::Drawing::Point(223, 85);
			this->m_txtBS1Word10->Name = L"m_txtBS1Word10";
			this->m_txtBS1Word10->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word10->TabIndex = 10;
			this->m_txtBS1Word10->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word6
			// 
			this->m_txtBS1Word6->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word6->Location = System::Drawing::Point(223, 59);
			this->m_txtBS1Word6->Name = L"m_txtBS1Word6";
			this->m_txtBS1Word6->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word6->TabIndex = 6;
			this->m_txtBS1Word6->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word2
			// 
			this->m_txtBS1Word2->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word2->Location = System::Drawing::Point(223, 33);
			this->m_txtBS1Word2->Name = L"m_txtBS1Word2";
			this->m_txtBS1Word2->Size = System::Drawing::Size(74, 20);
			this->m_txtBS1Word2->TabIndex = 2;
			this->m_txtBS1Word2->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word13
			// 
			this->m_txtBS1Word13->Enabled = false;
			this->m_txtBS1Word13->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word13->Location = System::Drawing::Point(143, 111);
			this->m_txtBS1Word13->Name = L"m_txtBS1Word13";
			this->m_txtBS1Word13->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word13->TabIndex = 13;
			this->m_txtBS1Word13->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word9
			// 
			this->m_txtBS1Word9->Enabled = false;
			this->m_txtBS1Word9->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word9->Location = System::Drawing::Point(143, 85);
			this->m_txtBS1Word9->Name = L"m_txtBS1Word9";
			this->m_txtBS1Word9->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word9->TabIndex = 9;
			this->m_txtBS1Word9->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word5
			// 
			this->m_txtBS1Word5->Enabled = false;
			this->m_txtBS1Word5->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word5->Location = System::Drawing::Point(143, 59);
			this->m_txtBS1Word5->Name = L"m_txtBS1Word5";
			this->m_txtBS1Word5->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word5->TabIndex = 5;
			this->m_txtBS1Word5->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS1Word1
			// 
			this->m_txtBS1Word1->Enabled = false;
			this->m_txtBS1Word1->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word1->Location = System::Drawing::Point(143, 33);
			this->m_txtBS1Word1->Name = L"m_txtBS1Word1";
			this->m_txtBS1Word1->Size = System::Drawing::Size(74, 20);
			this->m_txtBS1Word1->TabIndex = 1;
			this->m_txtBS1Word1->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_lblBS1Word12
			// 
			this->m_lblBS1Word12->AutoSize = true;
			this->m_lblBS1Word12->Location = System::Drawing::Point(6, 118);
			this->m_lblBS1Word12->Name = L"m_lblBS1Word12";
			this->m_lblBS1Word12->Size = System::Drawing::Size(56, 13);
			this->m_lblBS1Word12->TabIndex = 7;
			this->m_lblBS1Word12->Text = L"DWord 12";
			// 
			// m_txtBS1Word12
			// 
			this->m_txtBS1Word12->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word12->Location = System::Drawing::Point(62, 111);
			this->m_txtBS1Word12->Name = L"m_txtBS1Word12";
			this->m_txtBS1Word12->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word12->TabIndex = 12;
			this->m_txtBS1Word12->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_lblBS1Word8
			// 
			this->m_lblBS1Word8->AutoSize = true;
			this->m_lblBS1Word8->Location = System::Drawing::Point(6, 92);
			this->m_lblBS1Word8->Name = L"m_lblBS1Word8";
			this->m_lblBS1Word8->Size = System::Drawing::Size(50, 13);
			this->m_lblBS1Word8->TabIndex = 5;
			this->m_lblBS1Word8->Text = L"DWord 8";
			// 
			// m_txtBS1Word8
			// 
			this->m_txtBS1Word8->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word8->Location = System::Drawing::Point(62, 85);
			this->m_txtBS1Word8->Name = L"m_txtBS1Word8";
			this->m_txtBS1Word8->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word8->TabIndex = 8;
			this->m_txtBS1Word8->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_lblBS1Word4
			// 
			this->m_lblBS1Word4->AutoSize = true;
			this->m_lblBS1Word4->Location = System::Drawing::Point(6, 66);
			this->m_lblBS1Word4->Name = L"m_lblBS1Word4";
			this->m_lblBS1Word4->Size = System::Drawing::Size(50, 13);
			this->m_lblBS1Word4->TabIndex = 3;
			this->m_lblBS1Word4->Text = L"DWord 4";
			// 
			// m_txtBS1Word4
			// 
			this->m_txtBS1Word4->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word4->Location = System::Drawing::Point(62, 59);
			this->m_txtBS1Word4->Name = L"m_txtBS1Word4";
			this->m_txtBS1Word4->Size = System::Drawing::Size(75, 20);
			this->m_txtBS1Word4->TabIndex = 4;
			this->m_txtBS1Word4->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_lblBS1Word0
			// 
			this->m_lblBS1Word0->AutoSize = true;
			this->m_lblBS1Word0->Location = System::Drawing::Point(6, 40);
			this->m_lblBS1Word0->Name = L"m_lblBS1Word0";
			this->m_lblBS1Word0->Size = System::Drawing::Size(50, 13);
			this->m_lblBS1Word0->TabIndex = 1;
			this->m_lblBS1Word0->Text = L"DWord 0";
			// 
			// m_txtBS1Word0
			// 
			this->m_txtBS1Word0->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS1Word0->Location = System::Drawing::Point(62, 33);
			this->m_txtBS1Word0->Name = L"m_txtBS1Word0";
			this->m_txtBS1Word0->Size = System::Drawing::Size(74, 20);
			this->m_txtBS1Word0->TabIndex = 0;
			this->m_txtBS1Word0->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->m_txtBS0Word15);
			this->groupBox1->Controls->Add(this->m_txtBS0Word11);
			this->groupBox1->Controls->Add(this->m_txtBS0Word7);
			this->groupBox1->Controls->Add(this->m_txtBS0Word3);
			this->groupBox1->Controls->Add(this->m_txtBS0Word14);
			this->groupBox1->Controls->Add(this->m_txtBS0Word10);
			this->groupBox1->Controls->Add(this->m_txtBS0Word6);
			this->groupBox1->Controls->Add(this->m_txtBS0Word2);
			this->groupBox1->Controls->Add(this->m_txtBS0Word13);
			this->groupBox1->Controls->Add(this->m_txtBS0Word9);
			this->groupBox1->Controls->Add(this->m_txtBS0Word5);
			this->groupBox1->Controls->Add(this->m_txtBS0Word1);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->m_txtBS0Word12);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->m_txtBS0Word8);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->m_txtBS0Word4);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->m_txtBS0Word0);
			this->groupBox1->Location = System::Drawing::Point(12, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(389, 150);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Boot Sector 0";
			// 
			// m_txtBS0Word15
			// 
			this->m_txtBS0Word15->Enabled = false;
			this->m_txtBS0Word15->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word15->Location = System::Drawing::Point(302, 116);
			this->m_txtBS0Word15->Name = L"m_txtBS0Word15";
			this->m_txtBS0Word15->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word15->TabIndex = 15;
			this->m_txtBS0Word15->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word11
			// 
			this->m_txtBS0Word11->Enabled = false;
			this->m_txtBS0Word11->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word11->Location = System::Drawing::Point(302, 90);
			this->m_txtBS0Word11->Name = L"m_txtBS0Word11";
			this->m_txtBS0Word11->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word11->TabIndex = 11;
			this->m_txtBS0Word11->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word7
			// 
			this->m_txtBS0Word7->Enabled = false;
			this->m_txtBS0Word7->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word7->Location = System::Drawing::Point(302, 64);
			this->m_txtBS0Word7->Name = L"m_txtBS0Word7";
			this->m_txtBS0Word7->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word7->TabIndex = 7;
			this->m_txtBS0Word7->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word3
			// 
			this->m_txtBS0Word3->Enabled = false;
			this->m_txtBS0Word3->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word3->Location = System::Drawing::Point(302, 38);
			this->m_txtBS0Word3->Name = L"m_txtBS0Word3";
			this->m_txtBS0Word3->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word3->TabIndex = 3;
			this->m_txtBS0Word3->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word14
			// 
			this->m_txtBS0Word14->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word14->Location = System::Drawing::Point(222, 116);
			this->m_txtBS0Word14->Name = L"m_txtBS0Word14";
			this->m_txtBS0Word14->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word14->TabIndex = 14;
			this->m_txtBS0Word14->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word10
			// 
			this->m_txtBS0Word10->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word10->Location = System::Drawing::Point(222, 90);
			this->m_txtBS0Word10->Name = L"m_txtBS0Word10";
			this->m_txtBS0Word10->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word10->TabIndex = 10;
			this->m_txtBS0Word10->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word6
			// 
			this->m_txtBS0Word6->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word6->Location = System::Drawing::Point(222, 64);
			this->m_txtBS0Word6->Name = L"m_txtBS0Word6";
			this->m_txtBS0Word6->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word6->TabIndex = 6;
			this->m_txtBS0Word6->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word2
			// 
			this->m_txtBS0Word2->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word2->Location = System::Drawing::Point(222, 38);
			this->m_txtBS0Word2->Name = L"m_txtBS0Word2";
			this->m_txtBS0Word2->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word2->TabIndex = 2;
			this->m_txtBS0Word2->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word13
			// 
			this->m_txtBS0Word13->Enabled = false;
			this->m_txtBS0Word13->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word13->Location = System::Drawing::Point(142, 116);
			this->m_txtBS0Word13->Name = L"m_txtBS0Word13";
			this->m_txtBS0Word13->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word13->TabIndex = 13;
			this->m_txtBS0Word13->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word9
			// 
			this->m_txtBS0Word9->Enabled = false;
			this->m_txtBS0Word9->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word9->Location = System::Drawing::Point(142, 90);
			this->m_txtBS0Word9->Name = L"m_txtBS0Word9";
			this->m_txtBS0Word9->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word9->TabIndex = 9;
			this->m_txtBS0Word9->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word5
			// 
			this->m_txtBS0Word5->Enabled = false;
			this->m_txtBS0Word5->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word5->Location = System::Drawing::Point(142, 64);
			this->m_txtBS0Word5->Name = L"m_txtBS0Word5";
			this->m_txtBS0Word5->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word5->TabIndex = 5;
			this->m_txtBS0Word5->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// m_txtBS0Word1
			// 
			this->m_txtBS0Word1->Enabled = false;
			this->m_txtBS0Word1->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word1->Location = System::Drawing::Point(142, 38);
			this->m_txtBS0Word1->Name = L"m_txtBS0Word1";
			this->m_txtBS0Word1->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word1->TabIndex = 1;
			this->m_txtBS0Word1->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 123);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(56, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"DWord 12";
			// 
			// m_txtBS0Word12
			// 
			this->m_txtBS0Word12->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word12->Location = System::Drawing::Point(62, 116);
			this->m_txtBS0Word12->Name = L"m_txtBS0Word12";
			this->m_txtBS0Word12->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word12->TabIndex = 12;
			this->m_txtBS0Word12->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 97);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(50, 13);
			this->label4->TabIndex = 5;
			this->label4->Text = L"DWord 8";
			// 
			// m_txtBS0Word8
			// 
			this->m_txtBS0Word8->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word8->Location = System::Drawing::Point(62, 90);
			this->m_txtBS0Word8->Name = L"m_txtBS0Word8";
			this->m_txtBS0Word8->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word8->TabIndex = 8;
			this->m_txtBS0Word8->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 71);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(50, 13);
			this->label5->TabIndex = 3;
			this->label5->Text = L"DWord 4";
			// 
			// m_txtBS0Word4
			// 
			this->m_txtBS0Word4->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word4->Location = System::Drawing::Point(62, 64);
			this->m_txtBS0Word4->Name = L"m_txtBS0Word4";
			this->m_txtBS0Word4->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word4->TabIndex = 4;
			this->m_txtBS0Word4->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(6, 45);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(50, 13);
			this->label6->TabIndex = 1;
			this->label6->Text = L"DWord 0";
			// 
			// m_txtBS0Word0
			// 
			this->m_txtBS0Word0->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_txtBS0Word0->Location = System::Drawing::Point(62, 38);
			this->m_txtBS0Word0->Name = L"m_txtBS0Word0";
			this->m_txtBS0Word0->Size = System::Drawing::Size(74, 20);
			this->m_txtBS0Word0->TabIndex = 0;
			this->m_txtBS0Word0->TextChanged += gcnew System::EventHandler(this, &DlgBootSectors::BootSector_TextChange);
			// 
			// DlgBootSectors
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(411, 389);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->m_grpBootSec1);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnOK);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"DlgBootSectors";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Boot Sectors 0 & 1";
			this->Shown += gcnew System::EventHandler(this, &DlgBootSectors::DlgBootSectors_Shown);
			this->m_grpBootSec1->ResumeLayout(false);
			this->m_grpBootSec1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void UpdateTextBoxes(System::Collections::Generic::List<TextBox^>^ lstTextBoxes, string& sValues)
	{
		String^ str;
		int nOffset = 0;
		for each(TextBox^ thisBox in lstTextBoxes )
		{
			str = gcnew String((sValues.substr(nOffset, 8)).c_str());
			thisBox->Text = str;
			nOffset += 8;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void DlgBootSectors_Shown(System::Object^  sender, System::EventArgs^  e)
	{
		//There should be 64 bytes of ASCII for each boot sector. If not, go to default values.
		if (((m_pBootSecStrArr->at(0)).size() != Config::kBS0Size*2) || ((m_pBootSecStrArr->at(1)).size() != Config::kBS1Size*2))
		{
			m_pBootSecStrArr->at(0) = "00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF";
			m_pBootSecStrArr->at(1) = "00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF00000000FFFFFFFF";
		}
		UpdateTextBoxes(m_bs0TextboxArr, m_pBootSecStrArr->at(0));
		UpdateTextBoxes(m_bs1TextboxArr, m_pBootSecStrArr->at(1));
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void BootSector_TextChange(System::Object^  sender, System::EventArgs^  e)
	{
		TextBox^ thisBox = safe_cast<TextBox^>(sender);
		String^ str = thisBox->Text;
		str = thisBox->Text;

		//Get the cursor position.
		int nSelStart = thisBox->SelectionStart;

		//Make sure all values are 0-9 or a-f.
		int nNumRemoved = 0;
		for (int i=str->Length-1; i>=0; i--)
		{
			if ((!((str[i] >= '0') && (str[i] <= '9'))) &&
				(!((str[i] >= 'a') && (str[i] <= 'f'))) &&
				(!((str[i] >= 'A') && (str[i] <= 'F'))))
			{
				str = str->Remove(i, 1);
				nNumRemoved++;
			}
		}

		//4 bytes = a max of 8 hex characters.
		if (str->Length > 8)
			str = str->Remove(8);

		//Restore cursor position.
		if (thisBox->Text != str)
		{
			thisBox->Text = str;
			thisBox->SelectionStart = nSelStart - nNumRemoved;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void UpdateValues(System::Collections::Generic::List<TextBox^>^ lstTextBoxes, string& sValues)
	{
		sValues.clear();
		for (int i=0; i<lstTextBoxes->Count; i++)
			sValues += marshal_as<std::string>(lstTextBoxes[i]->Text);
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Make sure each BS0 Textbox has exactly 8 characters.
		bool bAllTextboxesValid = true;
		for (int i=0; (i<m_bs0TextboxArr->Count) && bAllTextboxesValid; i++)
		{
			if ((m_bs0TextboxArr[i]->Text)->Length != 8)
			{
				bAllTextboxesValid = false;	//Select the offending characters
				m_bs0TextboxArr[i]->SelectAll();
				m_bs0TextboxArr[i]->Focus();
			}
		}

		//Make sure each BS1 Textbox has exactly 8 characters.
		for (int i=0; (i<m_bs0TextboxArr->Count) && bAllTextboxesValid; i++)
		{
			if (((m_bs1TextboxArr[i]->Text)->Length != 8) && bAllTextboxesValid)
			{
				bAllTextboxesValid = false;	//Select the offending characters
				m_bs1TextboxArr[i]->SelectAll();
				m_bs1TextboxArr[i]->Focus();
			}
		}

		if (bAllTextboxesValid)
		{
			//Update the values that will be passed back to the creator of this dialog.
			UpdateValues(m_bs0TextboxArr, m_pBootSecStrArr->at(0));
			UpdateValues(m_bs1TextboxArr, m_pBootSecStrArr->at(1));
			m_bOkClicked = true;
			Close();
		}
		else
		{
			String^ message = "One or more of the text boxes was found to have more or less than 8 characters.\nBefore a save can be done this error must be fixed.";
			String^ caption = "More or less than 8 characters";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBox::Show( this, message, caption, buttons );
		}
	}
};
}
