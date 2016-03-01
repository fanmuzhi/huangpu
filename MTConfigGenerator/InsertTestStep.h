#pragma once

#include "Config.h"
#include "TestStep.h"
#include <msclr/marshal_cppstd.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


namespace MTConfigGenerator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for InsertTestStep
	/// </summary>
	public ref class InsertTestStep : public System::Windows::Forms::Form
	{
	public:
		void		SetInsertFlg(bool b)						{m_bInsert = b;}
		bool		GetInsertFlg()								{return m_bInsert;}
		void		SetCancelFlg(bool b)						{m_bCancel = b;}
		bool		GetCancelFlg()								{return m_bCancel;}
		void		SetStepAndArgs(StepAndArgs sa)				{*m_pStepAndArgs = sa;}
		StepAndArgs	GetStepAndArgs()							{return *m_pStepAndArgs;}
		void		SetAvailableStepsPtr(vector<TestStep>* p)	{m_pAvailableStepsArr = p;}


		InsertTestStep(void)
		{
			InitializeComponent();
			this->StartPosition = FormStartPosition::CenterScreen;

			System::Drawing::Size size = m_lstArgs->ClientSize;
			this->m_colName->Width = size.Width / 2;
			this->m_colValue->Width = size.Width / 4;
			this->m_colDetails->Width = size.Width / 4;

			m_pStepAndArgs			= new StepAndArgs;
			m_edtArgValue->Parent	= m_lstArgs;

			m_lstArgs->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Right | AnchorStyles::Top | AnchorStyles::Left);
			m_btnInsertStep->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Right);
			m_btnInsertStepCancel->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Right);
			m_lblExtraArgs->Anchor = static_cast<AnchorStyles>(AnchorStyles::Left | AnchorStyles::Bottom | AnchorStyles::Right);
			m_txtExtraArgs->Anchor = static_cast<AnchorStyles>(AnchorStyles::Left | AnchorStyles::Bottom | AnchorStyles::Right);

//			m_cboStepName->MouseWheel += gcnew MouseEventHandler(&InsertTestStep::ChangedStepNameCbo_MouseWheel);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~InsertTestStep()
		{
			if (components)
			{
				delete components;
				delete m_pStepAndArgs;
			}
		}

	private: bool					m_bInsert;
	private: bool					m_bCancel;
	private: StepAndArgs*			m_pStepAndArgs;
	private: vector<TestStep>*		m_pAvailableStepsArr;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  m_cboStepName;

	private: System::Windows::Forms::Button^  m_btnInsertStep;
	private: System::Windows::Forms::Button^  m_btnInsertStepCancel;
	private: System::Windows::Forms::TextBox^  m_txtExtraArgs;
private: System::Windows::Forms::Label^  m_lblExtraArgs;



	private: System::Windows::Forms::ListView^  m_lstArgs;
	private: System::Windows::Forms::ListViewHitTestInfo^ m_hitinfo;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ColumnHeader^  m_colName;
	private: System::Windows::Forms::ColumnHeader^  m_colValue;
	private: System::Windows::Forms::ColumnHeader^  m_colDetails;
	private: System::Windows::Forms::TextBox^  m_edtArgValue;
private: System::Windows::Forms::Label^  label2;

	protected: 


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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->m_cboStepName = (gcnew System::Windows::Forms::ComboBox());
			this->m_btnInsertStep = (gcnew System::Windows::Forms::Button());
			this->m_btnInsertStepCancel = (gcnew System::Windows::Forms::Button());
			this->m_txtExtraArgs = (gcnew System::Windows::Forms::TextBox());
			this->m_lblExtraArgs = (gcnew System::Windows::Forms::Label());
			this->m_lstArgs = (gcnew System::Windows::Forms::ListView());
			this->m_colName = (gcnew System::Windows::Forms::ColumnHeader());
			this->m_colValue = (gcnew System::Windows::Forms::ColumnHeader());
			this->m_colDetails = (gcnew System::Windows::Forms::ColumnHeader());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->m_edtArgValue = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(142, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Select or type the test name:";
			// 
			// m_cboStepName
			// 
			this->m_cboStepName->FormattingEnabled = true;
			this->m_cboStepName->Location = System::Drawing::Point(12, 25);
			this->m_cboStepName->Name = L"m_cboStepName";
			this->m_cboStepName->Size = System::Drawing::Size(322, 21);
			this->m_cboStepName->Sorted = true;
			this->m_cboStepName->TabIndex = 0;
			this->m_cboStepName->TextChanged += gcnew System::EventHandler(this, &InsertTestStep::ChangedStepNameCbo_TextChanged);
			// 
			// m_btnInsertStep
			// 
			this->m_btnInsertStep->Location = System::Drawing::Point(194, 295);
			this->m_btnInsertStep->Name = L"m_btnInsertStep";
			this->m_btnInsertStep->Size = System::Drawing::Size(67, 23);
			this->m_btnInsertStep->TabIndex = 3;
			this->m_btnInsertStep->Text = L"OK";
			this->m_btnInsertStep->UseVisualStyleBackColor = true;
			this->m_btnInsertStep->Click += gcnew System::EventHandler(this, &InsertTestStep::InsertStepBtn_Click);
			// 
			// m_btnInsertStepCancel
			// 
			this->m_btnInsertStepCancel->Location = System::Drawing::Point(267, 295);
			this->m_btnInsertStepCancel->Name = L"m_btnInsertStepCancel";
			this->m_btnInsertStepCancel->Size = System::Drawing::Size(67, 23);
			this->m_btnInsertStepCancel->TabIndex = 4;
			this->m_btnInsertStepCancel->Text = L"Cancel";
			this->m_btnInsertStepCancel->UseVisualStyleBackColor = true;
			this->m_btnInsertStepCancel->Click += gcnew System::EventHandler(this, &InsertTestStep::CancelBtn_Click);
			// 
			// m_txtExtraArgs
			// 
			this->m_txtExtraArgs->Location = System::Drawing::Point(12, 252);
			this->m_txtExtraArgs->Name = L"m_txtExtraArgs";
			this->m_txtExtraArgs->Size = System::Drawing::Size(322, 20);
			this->m_txtExtraArgs->TabIndex = 2;
			// 
			// m_lblExtraArgs
			// 
			this->m_lblExtraArgs->AutoSize = true;
			this->m_lblExtraArgs->Location = System::Drawing::Point(9, 236);
			this->m_lblExtraArgs->Name = L"m_lblExtraArgs";
			this->m_lblExtraArgs->Size = System::Drawing::Size(86, 13);
			this->m_lblExtraArgs->TabIndex = 5;
			this->m_lblExtraArgs->Text = L"Extra arguments:";
			// 
			// m_lstArgs
			// 
			this->m_lstArgs->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->m_colName, this->m_colValue, 
				this->m_colDetails});
			this->m_lstArgs->FullRowSelect = true;
			this->m_lstArgs->Location = System::Drawing::Point(12, 82);
			this->m_lstArgs->MultiSelect = false;
			this->m_lstArgs->Name = L"m_lstArgs";
			this->m_lstArgs->Size = System::Drawing::Size(319, 137);
			this->m_lstArgs->TabIndex = 1;
			this->m_lstArgs->UseCompatibleStateImageBehavior = false;
			this->m_lstArgs->View = System::Windows::Forms::View::Details;
			this->m_lstArgs->SizeChanged += gcnew System::EventHandler(this, &InsertTestStep::LstArgs_SizeChanged);
			this->m_lstArgs->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &InsertTestStep::LstArgs_MouseDoubleClick);
			// 
			// m_colName
			// 
			this->m_colName->Text = L"Name";
			// 
			// m_colValue
			// 
			this->m_colValue->Text = L"Value";
			// 
			// m_colDetails
			// 
			this->m_colDetails->Text = L"Details";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(9, 66);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(83, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Test arguments:";
			// 
			// m_edtArgValue
			// 
			this->m_edtArgValue->Location = System::Drawing::Point(38, 295);
			this->m_edtArgValue->Name = L"m_edtArgValue";
			this->m_edtArgValue->Size = System::Drawing::Size(100, 20);
			this->m_edtArgValue->TabIndex = 8;
			this->m_edtArgValue->TabStop = false;
			this->m_edtArgValue->Visible = false;
			this->m_edtArgValue->Leave += gcnew System::EventHandler(this, &InsertTestStep::EdtArgValue_Leave);
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(191, 222);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(142, 12);
			this->label2->TabIndex = 9;
			this->label2->Text = L"(To modify, double-click on value)";
			// 
			// InsertTestStep
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(346, 330);
			this->ControlBox = false;
			this->Controls->Add(this->label2);
			this->Controls->Add(this->m_edtArgValue);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->m_lstArgs);
			this->Controls->Add(this->m_lblExtraArgs);
			this->Controls->Add(this->m_txtExtraArgs);
			this->Controls->Add(this->m_btnInsertStepCancel);
			this->Controls->Add(this->m_btnInsertStep);
			this->Controls->Add(this->m_cboStepName);
			this->Controls->Add(this->label1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"InsertTestStep";
			this->Text = L"Test Step";
			this->Shown += gcnew System::EventHandler(this, &InsertTestStep::InsertTestStep_Shown);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void CancelBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		m_bCancel = true;
		Close();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void InsertStepBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		m_bCancel = false;
		UpdateInternalStructs(*m_pStepAndArgs);
		Close();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void InsertTestStep_Shown(System::Object^  sender, System::EventArgs^  e)
	{
		//Put the name of the known tests in the drop down menu.
		for (int i=0; i<(int)m_pAvailableStepsArr->size(); i++)
		{
			String^ str = gcnew String(((m_pAvailableStepsArr->at(i)).GetName()).c_str());
			m_cboStepName->Items->Add(str);
		}

		UpdateControls(*m_pStepAndArgs);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void UpdateControls(StepAndArgs& stepAndArgs)
	{
		string	sStepName = stepAndArgs.GetStepName();
		string	sStepArgs = stepAndArgs.GetStepArgs();

		String^ str = gcnew String(sStepName.c_str());
		m_cboStepName->Text = str;

		//If this is not an insert operation, it must be a modify operation. Do
		//not let the user change the selected step in case of a modify operation.
		if (m_bInsert == false)
			m_cboStepName->Enabled = false;

		TestStep info("", "", "", "");
		bool bFound = GetTestStepInfo(sStepName, &info);
		if (bFound)
		{
			m_lstArgs->Items->Clear();
			m_lstArgs->Enabled = false;
			for (int i=0; i<info.GetNumArgs(); i++)
			{
				m_lstArgs->Enabled = true;
				String^ sArgName = gcnew String((info.GetArgName(i)).c_str());
				String^ sArgValue = gcnew String((GetArgValue(i, sStepArgs)).c_str());
				String^ sArgDetails = gcnew String((info.GetArgDetails(i)).c_str());

				//Add the new item to the list. Use the 1,2,3 values so that the
				//LstArgs_MouseDoubleClick routine can determine which colomn was clicked.
				ListViewItem^ listItem = gcnew ListViewItem( sArgName );
				listItem->SubItems->Add( sArgValue );
				listItem->SubItems->Add( sArgDetails );
				listItem->SubItems[0]->Name = "1";
				listItem->SubItems[1]->Name = "2";
				listItem->SubItems[2]->Name = "3";
				m_lstArgs->Items->Add(listItem);
			}

			str = gcnew String(GetArgsRemaining(info.GetNumArgs(), sStepArgs).c_str());
			m_txtExtraArgs->Text = str;
		}
		else
		{
			m_lstArgs->Enabled = false;
			m_lstArgs->Items->Clear();
			str = gcnew String(sStepArgs.c_str());
			m_txtExtraArgs->Text = str;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void UpdateInternalStructs(StepAndArgs& stepAndArgs)
	{
		stepAndArgs.m_sStepName = msclr::interop::marshal_as< std::string >(m_cboStepName->Text);

		stepAndArgs.m_sStepArgs = "";
		TestStep info("", "", "", "");
		bool bFound = GetTestStepInfo(stepAndArgs.m_sStepName, &info);
		if (bFound)
		{
			//Get the latest arguments.
			for (int i=0; i<info.GetNumArgs(); i++)
			{
				string sTemp = msclr::interop::marshal_as< std::string >(m_lstArgs->Items[i]->SubItems[1]->Text);
				if (!sTemp.empty())
				{
					if (i != 0)
						m_pStepAndArgs->m_sStepArgs += " ";
					m_pStepAndArgs->m_sStepArgs +=sTemp;
				}
			}
		}

		//Take care of any extra arguments.
		String^ str = gcnew String(((m_txtExtraArgs->Text)->TrimStart())->TrimEnd());
		if (!String::IsNullOrEmpty(str))
			stepAndArgs.m_sStepArgs +=  " " + msclr::interop::marshal_as< std::string >(str);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: string GetArgValue(int idx, string sArgs)
	{
		string	sArgValue = "";
		string buf; // Have a buffer string
		stringstream ss(sArgs); // Insert the string into a stream

		vector<string> tokens; // Create vector to hold our words

		while (ss >> buf)
			tokens.push_back(buf);

		if ((int)tokens.size() > idx)
			sArgValue = tokens[idx];

		return sArgValue;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: string GetArgsRemaining(int idx, string sArgs)
	{
		string	sArgsRemaining = "";
		string buf; // Have a buffer string
		stringstream ss(sArgs); // Insert the string into a stream

		vector<string> tokens; // Create vector to hold our words

		while (ss >> buf)
			tokens.push_back(buf);

		if ((int)tokens.size() > idx)
		{
			for (int i=idx; i<(int)tokens.size(); i++)
				sArgsRemaining += tokens[i] + ' ';
		}

		return sArgsRemaining;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: bool GetTestStepInfo(string& sStepName, TestStep* pInfo)
	{
		bool bFound = false;
		for (int i=0; i<(int)m_pAvailableStepsArr->size(); i++)
		{
			if ((m_pAvailableStepsArr->at(i)).GetName() == sStepName)
			{
				bFound = true;
				*pInfo = m_pAvailableStepsArr->at(i);
			}
		}
		return bFound;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void ChangedStepNameCbo_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		//If the user has changed the step name.
		string sTemp = msclr::interop::marshal_as< std::string >(((m_cboStepName->Text)->TrimStart())->TrimEnd());
		TestStep info("", "", "", "");
		bool bFound = GetTestStepInfo(sTemp, &info);
		if (bFound)
		{
			string sArgDefaults = "";
			for (int i=0; i<info.GetNumArgs(); i++)
			{
				if (i > 0)
					sArgDefaults += " ";
				sArgDefaults += info.GetArgDefaults(i);
			}

			m_pStepAndArgs->SetStepArgs(sArgDefaults);
			m_pStepAndArgs->SetStepName(sTemp);
			UpdateControls(*m_pStepAndArgs);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void LstArgs_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		//If the user has double-clicked on a value in the 2nd column.
		m_hitinfo = m_lstArgs->HitTest(e->X, e->Y);
		if ((m_hitinfo->SubItem != nullptr) && (m_hitinfo->SubItem->Name == "2"))
		{
			m_edtArgValue->Bounds = m_hitinfo->SubItem->Bounds;
			m_edtArgValue->Text = m_hitinfo->SubItem->Text;
			m_edtArgValue->Select();
			m_edtArgValue->Show();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void EdtArgValue_Leave(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_hitinfo->SubItem != nullptr )
		{
			m_hitinfo->SubItem->Text = ((m_edtArgValue->Text)->TrimStart())->TrimEnd();
			m_edtArgValue->Hide();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void LstArgs_SizeChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::Drawing::Size size = m_lstArgs->ClientSize;
		m_colDetails->Width = size.Width - (m_colName->Width + m_colValue->Width);
	}
};
}
