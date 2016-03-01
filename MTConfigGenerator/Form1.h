#pragma once

#include "Windows.h"
#include "Config.h"
#include "AppInfo.h"
#include "InsertTestStep.h"
#include "DlgBootSectors.h"
#include <string.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using std::string;


namespace MTConfigGenerator {

	/// <summary>
	/// Summary for Form1
	/// </summary>
	enum	{	displayType1 = 1,	// sTestArgs = "a b c d e", print format: "a	-	-"
				displayType2,		// sTestArgs = "a b c d e", print format: "-	a	b"
				displayType3,		// sTestArgs = "a b c d e", print format: "-	-	a"
				displayType4,		// sTestArgs = "a b c d e", print format: "-	a	-"
				displayType5,		// sTestArgs = "a b c d e", print format: "-	c	a"
				displayType6,		// sTestArgs = "a b c d e", print format: "-	b	a"
			};

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
			:m_pOrgConfig(nullptr)
			,m_pConfig(nullptr)
		{
			int nRevMaj, nRevMin, nRevBuild, nRevision;
			AppInfo* pAppInfo = AppInfo::GetAppInfoPtr();
			pAppInfo->GetVersionInfo(nRevMaj, nRevMin, nRevBuild, nRevision);
			m_sRevision = nRevMaj + "." + nRevMin + "." + nRevBuild;

			m_pOrgConfig = new Config;
			m_pConfig = new Config;
			InitializeComponent();
			m_UIUpdateTimer->Interval = 100;
			m_UIUpdateTimer->Enabled = true;
			m_sOrgDialogTitle = this->Text + " (" + m_sRevision + ")";

			m_cboAutoController->Items->Add("LabJack");
			m_cboAutoController->Items->Add("MPC04Gpio");
			m_cboAutoController->Items->Add("ManualControl");

			m_cboDutController->Items->Add("MPC04");
			m_cboDutController->Items->Add("SPC");

			m_cboDutType->Items->Add("Metallica");
			m_cboDutType->Items->Add("Raptor");
			m_cboDutType->Items->Add("Viper");
			m_cboDutType->Items->Add("Viper2");
			m_cboDutType->Items->Add("Denali");

			m_cboDutCom->Items->Add("SPI");
			m_cboDutCom->Items->Add("USB");


			//Create a list of all known steps. Put them in the order in which they should be executed.
			m_nOrderIdxColWidth = 50;
			m_pAvailableStepsArr	= new(vector<TestStep>);

			m_pAvailableStepsArr->push_back(TestStep("OTPCheck",
				"TAG_ProjID;TAG_LNA;TAG_SNR;TAG_PGA_OOPR;TAG_FlexID;TAG_WOF_BOT;TAG_DutTempAdc;TAG_WOF_TOP;TAG_PGA_OOPP;TAG_SCM_WOF_BOT;TAG_PART_NUMBERS",		//args
				"1;1;1;1;1;1;1;1;1;1;1",		//default value
				"0=not check 1=check;0=not check 1=check;0=not check 1=check;0=not check 1=check;0=not check 1=check;0=not check 1=check;0=not check 1=check;0=not check 1=check;0=not check 1=check;0=not check 1=check;0=not check 1=check"));	//explanation
			//m_pAvailableStepsArr->push_back(TestStep("InitializationStep",
			//			"BGA;Flex ID;Trim with Stimulus (top);Trim with Stimulus (bottom);Trim with Stimulus (left);Trim with Stimulus (right);Build #;Trim without Stimulus (top);Trim without Stimulus (bottom);Trim without Stimulus (left);Trim without Stimulus (right);Max Log File Size;Verbose Log;Project ID;Has Flash;Engineering Security;Production Security;Verify ATE Pass bit",
			//			"0;0x08F3;0;0;1;0;132;0;0;1;0;100;0;0;0;0;0;0",
			//			"0=false, 1=true;;pixels (inclusive);pixels (inclusive);pixels (inclusive);pixels (inclusive);1-65535;pixels (inclusive);pixels (inclusive);pixels (inclusive);pixels (inclusive);MB (used in C++ only);0=false, 1=true (used in C++ only);32 bit value (0=don't write to OTP);0=false, 1=true;0 = disabled, 1 = enabled. Must be same as Security step setting.;0 = disabled, 1 = enabled. Must be same as Security step setting.;0 = do not check, 1 = check it."));

			//m_pAvailableStepsArr->push_back(TestStep("VerifyPackageType",
			//			"Package type",
			//			"0",
			//			"Use 'Extra arguments' to enter multiple package types"));

			//m_pAvailableStepsArr->push_back(TestStep("SpiFlashRam",
			//			"Delay",
			//			"500",
			//			"mSec"));

			//m_pAvailableStepsArr->push_back(TestStep("RAMTest",
			//			"Delay",
			//			"500",
			//			"mSec"));

			//m_pAvailableStepsArr->push_back(TestStep("LayerIdTest",
			//			"# Response Bytes",
			//			"1000",
			//			""));

			//m_pAvailableStepsArr->push_back(TestStep("FlexIdTest",
			//			"# Response Bytes",
			//			"1000",
			//			"used in C++ only"));

			//m_pAvailableStepsArr->push_back(TestStep("OscTrim",
			//			"Low Limit;High Limit;Initial Trim;Default Trim",
			//			"122000;123000;0x380;0",
			//			"Hz;Hz;;Used when frequency pin is not available"));

			//m_pAvailableStepsArr->push_back(TestStep("SlowOsc",
			//			"Low Limit;High Limit;Default HVOsc Trim;Default HVOsc Bias",
			//			"475000;501000;0;0",
			//			"Hz;Hz;Used when frequency pin is not available;Used when frequency pin is not available"));

			//m_pAvailableStepsArr->push_back(TestStep("OpensShortsTest",
			//			"# Response Bytes;Delay",
			//			"2056;500",
			//			";mSec"));

			//m_pAvailableStepsArr->push_back(TestStep("PixelPatchTest",
			//			"# response bytes;Delay",
			//			"3000;500",
			//			";mSec"));

			//m_pAvailableStepsArr->push_back(TestStep("WoVarTest",
			//			"# Response Bytes;Delay",
			//			"1000;500",
			//			";mSec"));

			//m_pAvailableStepsArr->push_back(TestStep("AFETest",
			//			"# Response Bytes;Delay",
			//			"500;500",
			//			""));

			//m_pAvailableStepsArr->push_back(TestStep("RetainModeCurrentTest",
			//			"Gain;Max Current;Min Current;Delay",
			//			"2;0.5;0;500",
			//			"0=1, 1=10, 2=100 or 3=1000 (for Viper only);mA (for Viper only);mA (for Viper only);mSec (for Viper only)"));

			//m_pAvailableStepsArr->push_back(TestStep("WOF-LowPower",
			//			"Gain;Max Current;Min Current;Delay",
			//			"2;50;0;200",
			//			"0=1, 1=10, 2=100 or 3=1000;uA;uA;mSec"));

			//m_pAvailableStepsArr->push_back(TestStep("WakeOnFinger",
			//	"With Stimulus;Minimum Delta;Trigger Min Threshold;Trigger Max Threshold;# Response Bytes; # Noise Samples;Threshold Offset High;Threshold Offset Low;Standared Dev;Minimum Delta2;Minimum Delta3;VCC",
			//			"0;7;25;60;2000;10;3;3;0.9;20;13;3.7",
			//			"1=with stimulus, 0=without stimulus;;;;;for Metallica only;for Metallica only;for Metallica only;for Metallica only;for Viper only;With alternate VCC (for Viper only);Alternate VCC (V) (for Viper only)"));

			//m_pAvailableStepsArr->push_back(TestStep("SCM_WOFWithoutStimulus",
			//	"Minimum Delta;Trigger Min Threshold;Trigger Max Threshold;# Response Bytes; # Noise Samples;Threshold Offset High;Threshold Offset Low;Standared Dev;Minimum Delta2;Minimum Delta3;VCC",
			//			"7;25;60;5000;10;3;3;0.9;20;13;3.7",
			//			"for Viper only;for Viper only;for Viper only;for Viper only;not used;not used;not used;not used;for Viper only;With alternate VCC (for Viper only);Alternate VCC (V) (for Viper only)"));

			//m_pAvailableStepsArr->push_back(TestStep("Calibrate",
			//			"LNA Print File Index;PGA Print File Index;Force Calibration;Discard Images;LNA Offset (Low);LNA Offset (High);PGA Limit (Low);PGA Limit (High);Cal Type;PGA Ratio;Number of PGA samples;Stage2 OTP Variance Threshold;HPF blob offset;PGA Fine Tuning",
			//			"1028;1140;0;20;108;148;108;148;0;0.3;4;90;0;0",
			//			";;0=Do not force, 1=force (used in C++ only);# images to discard;;;;;0= 1 offset/row, 1= 1 offset/pixel;PGA Offset Ratio;;> fails;HPF offset (0 = disabled);0 = Don't tune, 1 = tune"));

			//m_pAvailableStepsArr->push_back(TestStep("ReadDutAdc",
			//			"",
			//			"",
			//			""));

			//m_pAvailableStepsArr->push_back(TestStep("ButtonTestWithoutStimulus",
			//			"Port ID;Pin Mask;Expected State",
			//			"8;0x10;1",
			//			"0=Port A, 1=Port B... (for Viper C++ only);e.g. 0x8000 (for Viper C++ only);0=low, 1=high (for Viper C++ only)"));

			//m_pAvailableStepsArr->push_back(TestStep("AcquireFPSSamples",
			//			"Discard Images;Images w/o Stimulus;Images with Stimulus",
			//			"20;30;30",
			//			"# images to discard;# image acquisitions without stimulus;0 = no stimulus involved"));

			//m_pAvailableStepsArr->push_back(TestStep("ButtonTestWithStimulus",
			//			"Port ID;Pin Mask;Expected State",
			//			"8;0x10;0",
			//			"0=Port A, 1=Port B... (for Viper C++ only);e.g. 0x8000 (for Viper C++ only);0=low, 1=high (for Viper C++ only)"));

			//m_pAvailableStepsArr->push_back(TestStep("SCM_WOFWithStimulus",
			//	"Minimum Delta;Trigger Min Threshold;Trigger Max Threshold;# Response Bytes;# Noise Samples;Threshold Offset High;Threshold Offset Low;Standared Dev;Minimum Delta2;Minimum Delta3;VCC",
			//			"7;25;60;5000;10;3;3;0.9;20;13;3.7",
			//			"copy from SCM_WOFWithoutStimulus-Minimum Delta arg;copy from SCM_WOFWithoutStimulus-Trigger Min Threshold arg;copy from SCM_WOFWithoutStimulus-Trigger Max Threshold arg;copy from SCM_WOFWithoutStimulus-# Response Bytes arg;not used;not used;not used;not used;copy from SCM_WOFWithoutStimulus-Minimum Delta2 arg;With alternate VCC (copy from SCM_WOFWithoutStimulus-Minimum Delta3 arg);Alternate VCC (V) (copy from SCM_WOFWithoutStimulus-VCC arg)"));

			//m_pAvailableStepsArr->push_back(TestStep("PeggedPixelsTest",
			//			"Image Count;MaxPegged/row;MaxPegged/col;Pegged Threshold",
			//			"30;9;28;225",
			//			"copy from AcquireFPSSamples-Images w/o Stimulus arg;;;0-255 (inclusive)"));

			//m_pAvailableStepsArr->push_back(TestStep("FlooredPixelsTest",
			//			"Image Count;MaxFloored/row;MaxFloored/col;Floored Threshold",
			//			"30;9;28;25",
			//			"copy from AcquireFPSSamples-Images w/o Stimulus arg;;;0-255 (inclusive)"));

			//m_pAvailableStepsArr->push_back(TestStep("DRdyTest",
			//			"Port ID;Pin Mask",
			//			"1;0x10",
			//			"0=Port A, 1=Port B... (used in C++ only);e.g. 0x8000 (used in C++ only)"));

			//m_pAvailableStepsArr->push_back(TestStep("ConsecutivePixels",
			//			"Pegged Threshold;Floored Threshold;Max Adjacent;Trim (top, bottom, left, right);Image Count",
			//			"225;25;4;7;30",
			//			"0-255 (copy from PeggedPixelsTest-Pegged Threshold arg);0-255 (copy from FlooredPixelsTest-Floored Threshold arg);pixels;not used (pixels);copy from AcquireFPSSamples-Images w/o Stimulus arg"));

			//m_pAvailableStepsArr->push_back(TestStep("SDKBaselineTest",
			//			"Trim top;Trim bottom;Trim left;Trim right;Truncate bits;Stride width;Max sum allowed;Percent fail",
			//			"4;5;0;0;3;3;250;25",
			//			"pixels;pixels;pixels;pixels;bits;pixels;value;percent"));

			//m_pAvailableStepsArr->push_back(TestStep("CurrentTest",
			//			"Low Gain;High Gain;Image Acq Analog Max;Image Acq Digital Max;Image Acq Analog Min;Image Acq Digital Min;WOE Sleep current Max;WOE Sleep Current Min;WOE Sleep Current Delay",
			//			"2;3;50;5;1;0.01;0.150;0;500",
			//			"0=1, 1=10, 2=100 or 3=1000;0=1, 1=10, 2=100 or 3=1000;mA;mA;mA (Viper only);mA (Viper only);mA (Viper only);mA (Viper only);mSec (Viper only)"));

			//m_pAvailableStepsArr->push_back(TestStep("SNRTest",
			//			"Image Count;Trim Left;Trim Right;Trim Top;Trim Bottom;Percent Ignored;SNR Hi Limit / image;SNR Lo Limit / image;Signal Hi Limit / image;Signal Lo Limit / image;Noise Hi Limit / image;Noise Lo Limit / image;SNR Hi / zone;SNR Lo / zone;Signal Hi / zone;Signal Lo / zone;Noise Hi / zone;Noise Lo / zone",
			//			"30;0;0;0;0;10;500;25;500;80;5;0;500;14;500;50;6;0",
			//			"copy from AcquireFPSSamples-Images with Stimulus arg;not used;not used;not used;not used;%;;;;;;;;;;;;"));

			//m_pAvailableStepsArr->push_back(TestStep("PixelTest",
			//	"Image Count;Threshold (row);Threshold (col);Trim (top, bottom, left, right);Threshold Range;# Failed Pixels Permitted",
			//			"30;50;50;0;35;0",
			//			"copy from AcquireFPSSamples-Images w/o Stimulus arg;not used (0-255);not used (0-255);not used (pixels);+/- from 0;"));

			//m_pAvailableStepsArr->push_back(TestStep("DeltaPixelTest",
			//			"Image Count;Delta Threshold (row);Delta Threshold (col);MaxFailed/image",
			//			"30;0;0;10",
			//			"copy from AcquireFPSSamples-Images w/o Stimulus arg;0-255;0-255;pixels (inclusive)"));

			//m_pAvailableStepsArr->push_back(TestStep("SetOTPBits",
			//			"",
			//			"",
			//			""));

			//m_pAvailableStepsArr->push_back(TestStep("HardwareIdTest",
			//			"Expected State",
			//			"1",
			//			"0 or 1 (used in C++ only)"));

			//m_pAvailableStepsArr->push_back(TestStep("LEDTest",
			//			"GPIO0;GPIO1;GPIO2;LED description;Detection Pin;LED Register;Value;Timeout",
			//			"0;0;0;Red;22;0x80001600;0x04;5000",
			//			"0 = disabled, 1 = enabled (LabView only);0 = disabled, 1 = enabled (LabView only);0 = disabled, 1 = enabled (LabView only);String to describe LED (Production only);Aux pin number (1-30) (Production only);Sensor's LED register address (Production only);0x02 = green, 0x04 = red (Production only);Input signal timeout (ms) (Production only)"));

			//m_pAvailableStepsArr->push_back(TestStep("SharpnessTest",
			//			"Percent Variation Limit",
			//			"20",
			//			"%  (>= fails)"));

			//m_pAvailableStepsArr->push_back(TestStep("LaplacianTest",
			//			"Percent Variation Limit",
			//			"20",
			//			"%  (>= fails)"));

			//m_pAvailableStepsArr->push_back(TestStep("RxStandardDev",
			//			"Variance of Standard Dev;Image Count",
			//			"30;30",
			//			"%  (> fails);copy from AcquireFPSSamples-Images w/ Stimulus arg"));

			//m_pAvailableStepsArr->push_back(TestStep("Imperfections",
			//			"High Threshold;Low Threshold;Consecutive Limit",
			//			"100;150;5",
			//			";;(> fails)"));

			//m_pAvailableStepsArr->push_back(TestStep("ProductIdTest",
			//			"",
			//			"",
			//			""));

			//m_pAvailableStepsArr->push_back(TestStep("UsbCommTest",
			//			"VID;PID;Maxim Revision.;Slave Select;MISO;MOSI;SCLK",
			//			"0x138A;0x0090;0x13;17;19;21;23",
			//			"Expected USB vendor ID;Expected USB product ID;Expected Maxim ID;Aux port pin (1-30);Aux port pin (1-30);Aux port pin (1-30);Aux port pin (1-30)"));

			//m_pAvailableStepsArr->push_back(TestStep("OTPExtTag",
			//			"Customer-specific OTP Byte",
			//			"0",
			//			"0 = Don't write to customer-specific OTP byte."));

			//m_pAvailableStepsArr->push_back(TestStep("Security",
			//	"Engineering Security;Production Security;USB Comm.;Ownership;# of Ownership runs;Verify Flash Clear;ECC Flag",
			//			"0;0;0;0;5;0;0",
			//			"0 = disabled, 1 = enabled. Must be same as Initialization step setting.;0 = disabled, 1 = enabled. Must be same as Initialization step setting.;0 = disabled, 1 = enabled.;0 = disabled, 1 = enabled.;value;0 = disabled, 1 = enabled.;0 = disabled, 1 = enabled."));

			//m_pAvailableStepsArr->push_back(TestStep("FinalizationStep",
			//			"",
			//			"",
			//			""));

			System::Drawing::Size size = m_lstTestSeq->ClientSize;
			this->m_colOrderIdx->Width = m_nOrderIdxColWidth;
			this->m_colTest->Width = (size.Width - m_nOrderIdxColWidth) / 2;
			this->m_colArgs->Width = (size.Width - m_nOrderIdxColWidth) / 2;
			this->StartPosition = FormStartPosition::CenterScreen;

			size = m_lstPatchFiles->ClientSize;
			this->m_colPatchDescription->Width = (size.Width*40) / 100;
			this->m_colPatchRev->Width = (size.Width*10) / 100;
			this->m_colPatchName->Width = (size.Width*50) / 100;
			this->StartPosition = FormStartPosition::CenterScreen;

			//Make "Write Boot Sector" groupbox transparent.
			//m_grpWrBootSecs->SetStyle(static_cast<ControlStyles>(ControlStyles::SupportsTransparentBackColor), true);
			m_grpWrBootSecs->BackColor = Color::Transparent;    //Set BackColour

			this->KeyPreview = true;

			m_lstTestSeq->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Right | AnchorStyles::Top | AnchorStyles::Left);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete m_pOrgConfig;
				delete m_pConfig;
				delete components;
				delete m_pAvailableStepsArr;
			}
		}

	enum class PrintState { PS_HEADER, PS_STEP_SEQ, PS_PATCHES, PS_ARG_DES };

	private: System::String^		m_sRevision;
	private: vector<TestStep>*		m_pAvailableStepsArr;
	private: int					m_nOrderIdxColWidth;
	private: int					m_nNumPageLinesPrinted;
	private: int					m_nNumStepsPrinted;
	private: int					m_nNumPatchesPrinted;
	private: int					m_nNumHdrLines;
	private: bool					m_bPrintHeading;
	private: PrintState				m_printState;


	protected: 











	private: System::Windows::Forms::OpenFileDialog^  m_openFileDialog;

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  m_fileMenu;

	private: System::Windows::Forms::ToolStripMenuItem^  m_menuNewXml;
	private: System::Windows::Forms::ToolStripMenuItem^  m_menuOpenXml;
	private: System::Windows::Forms::ToolStripMenuItem^  m_helpMenu;



	private: System::Windows::Forms::ToolStripMenuItem^  m_menuSaveXml;
	private: System::Windows::Forms::ToolStripMenuItem^  m_menuSaveAsXml;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;

	private: System::String^	m_sOrgDialogTitle;
	private: Config*			m_pOrgConfig;
	private: Config*			m_pConfig;
	private: System::Windows::Forms::Timer^  m_UIUpdateTimer;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::GroupBox^  groupBox1;




	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ComboBox^  m_cboAutoController;
	private: System::Windows::Forms::ComboBox^  m_cboDutController;
	private: System::Windows::Forms::ComboBox^  m_cboDutType;
	private: System::Windows::Forms::ComboBox^  m_cboDutCom;



	private: System::Windows::Forms::Button^  m_btnInsertStep;
	private: System::Windows::Forms::Button^  m_btnDelStep;
	private: System::Windows::Forms::Button^  m_btnModStep;


	private: System::Windows::Forms::NumericUpDown^  m_numVdd;
	private: System::Windows::Forms::NumericUpDown^  m_numVddh;
	private: System::Windows::Forms::NumericUpDown^  m_numVled;
	private: System::Windows::Forms::NumericUpDown^  m_numVio;

	private: System::Windows::Forms::ToolStripMenuItem^  m_MenuItemExit;

	private: System::Windows::Forms::SaveFileDialog^  m_saveFileDialog;
	private: System::Windows::Forms::ListView^  m_lstTestSeq;
	private: System::Windows::Forms::ColumnHeader^  m_colTest;
	private: System::Windows::Forms::ColumnHeader^  m_colArgs;
	private: System::Windows::Forms::NumericUpDown^  m_numNumRows;
	private: System::Windows::Forms::NumericUpDown^  m_numNumCols;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;

	private: System::Windows::Forms::ToolStripMenuItem^  m_defaultConfigurations;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ListView^  m_lstPatchFiles;
	private: System::Windows::Forms::ColumnHeader^  m_colPatchDescription;
	private: System::Windows::Forms::ColumnHeader^  m_colPatchName;
	private: System::Windows::Forms::Button^  m_btnSelectPatch;
	private: System::Windows::Forms::Button^  m_btnRemovePatch;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
private: System::Windows::Forms::ToolStripMenuItem^  m_menuFilePrint;
private: System::Windows::Forms::PrintDialog^  m_printDialog;
private: System::Drawing::Printing::PrintDocument^  m_printDocument;
private: System::Windows::Forms::GroupBox^  m_grpWrBootSecs;
private: System::Windows::Forms::CheckBox^  m_chkWrBootSecsEnabled;
private: System::Windows::Forms::Button^  m_btnWrBootSecsEdit;
private: System::Windows::Forms::ColumnHeader^  m_colOrderIdx;
private: System::Windows::Forms::ColumnHeader^  m_colPatchRev;
private: System::Windows::Forms::ToolStripMenuItem^  m_menuSaveAsCSV;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::TextBox^  m_txtComment;














	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->m_openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->m_fileMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_menuNewXml = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_menuOpenXml = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_defaultConfigurations = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->m_menuSaveXml = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_menuSaveAsXml = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_menuSaveAsCSV = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->m_menuFilePrint = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->m_MenuItemExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_helpMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_UIUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->m_numVddh = (gcnew System::Windows::Forms::NumericUpDown());
			this->m_numVled = (gcnew System::Windows::Forms::NumericUpDown());
			this->m_numVio = (gcnew System::Windows::Forms::NumericUpDown());
			this->m_numVdd = (gcnew System::Windows::Forms::NumericUpDown());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->m_cboAutoController = (gcnew System::Windows::Forms::ComboBox());
			this->m_cboDutController = (gcnew System::Windows::Forms::ComboBox());
			this->m_cboDutType = (gcnew System::Windows::Forms::ComboBox());
			this->m_cboDutCom = (gcnew System::Windows::Forms::ComboBox());
			this->m_btnInsertStep = (gcnew System::Windows::Forms::Button());
			this->m_btnDelStep = (gcnew System::Windows::Forms::Button());
			this->m_btnModStep = (gcnew System::Windows::Forms::Button());
			this->m_saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->m_lstTestSeq = (gcnew System::Windows::Forms::ListView());
			this->m_colOrderIdx = (gcnew System::Windows::Forms::ColumnHeader());
			this->m_colTest = (gcnew System::Windows::Forms::ColumnHeader());
			this->m_colArgs = (gcnew System::Windows::Forms::ColumnHeader());
			this->m_numNumRows = (gcnew System::Windows::Forms::NumericUpDown());
			this->m_numNumCols = (gcnew System::Windows::Forms::NumericUpDown());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->m_lstPatchFiles = (gcnew System::Windows::Forms::ListView());
			this->m_colPatchDescription = (gcnew System::Windows::Forms::ColumnHeader());
			this->m_colPatchRev = (gcnew System::Windows::Forms::ColumnHeader());
			this->m_colPatchName = (gcnew System::Windows::Forms::ColumnHeader());
			this->m_btnSelectPatch = (gcnew System::Windows::Forms::Button());
			this->m_btnRemovePatch = (gcnew System::Windows::Forms::Button());
			this->m_printDialog = (gcnew System::Windows::Forms::PrintDialog());
			this->m_printDocument = (gcnew System::Drawing::Printing::PrintDocument());
			this->m_grpWrBootSecs = (gcnew System::Windows::Forms::GroupBox());
			this->m_chkWrBootSecsEnabled = (gcnew System::Windows::Forms::CheckBox());
			this->m_btnWrBootSecsEdit = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->m_txtComment = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numVddh))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numVled))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numVio))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numVdd))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numNumRows))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numNumCols))->BeginInit();
			this->m_grpWrBootSecs->SuspendLayout();
			this->SuspendLayout();
			// 
			// m_openFileDialog
			// 
			this->m_openFileDialog->FileName = L"openFileDialog1";
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->m_fileMenu, this->m_helpMenu});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(821, 24);
			this->menuStrip1->TabIndex = 15;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// m_fileMenu
			// 
			this->m_fileMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {this->m_menuNewXml, 
				this->m_menuOpenXml, this->m_defaultConfigurations, this->toolStripSeparator1, this->m_menuSaveXml, this->m_menuSaveAsXml, this->m_menuSaveAsCSV, 
				this->toolStripSeparator2, this->m_menuFilePrint, this->toolStripSeparator3, this->m_MenuItemExit});
			this->m_fileMenu->Name = L"m_fileMenu";
			this->m_fileMenu->Size = System::Drawing::Size(37, 20);
			this->m_fileMenu->Text = L"File";
			// 
			// m_menuNewXml
			// 
			this->m_menuNewXml->Name = L"m_menuNewXml";
			this->m_menuNewXml->Size = System::Drawing::Size(203, 22);
			this->m_menuNewXml->Text = L"New";
			this->m_menuNewXml->Click += gcnew System::EventHandler(this, &Form1::MenuNewXml_Click);
			// 
			// m_menuOpenXml
			// 
			this->m_menuOpenXml->Name = L"m_menuOpenXml";
			this->m_menuOpenXml->Size = System::Drawing::Size(203, 22);
			this->m_menuOpenXml->Text = L"Open";
			this->m_menuOpenXml->Click += gcnew System::EventHandler(this, &Form1::MenuOpenXml_Click);
			// 
			// m_defaultConfigurations
			// 
			this->m_defaultConfigurations->Name = L"m_defaultConfigurations";
			this->m_defaultConfigurations->Size = System::Drawing::Size(203, 22);
			this->m_defaultConfigurations->Text = L"Default Configurations...";
			this->m_defaultConfigurations->Click += gcnew System::EventHandler(this, &Form1::MenuDefaultConfigurations_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(200, 6);
			// 
			// m_menuSaveXml
			// 
			this->m_menuSaveXml->Name = L"m_menuSaveXml";
			this->m_menuSaveXml->Size = System::Drawing::Size(203, 22);
			this->m_menuSaveXml->Text = L"Save";
			this->m_menuSaveXml->Click += gcnew System::EventHandler(this, &Form1::MenuSaveXml_Click);
			// 
			// m_menuSaveAsXml
			// 
			this->m_menuSaveAsXml->Name = L"m_menuSaveAsXml";
			this->m_menuSaveAsXml->Size = System::Drawing::Size(203, 22);
			this->m_menuSaveAsXml->Text = L"Save As...";
			this->m_menuSaveAsXml->Click += gcnew System::EventHandler(this, &Form1::MenuSaveAsXml_Click);
			// 
			// m_menuSaveAsCSV
			// 
			this->m_menuSaveAsCSV->Name = L"m_menuSaveAsCSV";
			this->m_menuSaveAsCSV->Size = System::Drawing::Size(203, 22);
			this->m_menuSaveAsCSV->Text = L"Save As CSV...";
			this->m_menuSaveAsCSV->Click += gcnew System::EventHandler(this, &Form1::MenuSaveAsCSV_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(200, 6);
			// 
			// m_menuFilePrint
			// 
			this->m_menuFilePrint->Name = L"m_menuFilePrint";
			this->m_menuFilePrint->Size = System::Drawing::Size(203, 22);
			this->m_menuFilePrint->Text = L"Print...";
			this->m_menuFilePrint->Click += gcnew System::EventHandler(this, &Form1::MenuFilePrint_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(200, 6);
			// 
			// m_MenuItemExit
			// 
			this->m_MenuItemExit->Name = L"m_MenuItemExit";
			this->m_MenuItemExit->Size = System::Drawing::Size(203, 22);
			this->m_MenuItemExit->Text = L"Exit";
			this->m_MenuItemExit->Click += gcnew System::EventHandler(this, &Form1::MenuItemExit_Click);
			// 
			// m_helpMenu
			// 
			this->m_helpMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutToolStripMenuItem});
			this->m_helpMenu->Name = L"m_helpMenu";
			this->m_helpMenu->Size = System::Drawing::Size(44, 20);
			this->m_helpMenu->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->aboutToolStripMenuItem->Text = L"About...";
			// 
			// m_UIUpdateTimer
			// 
			this->m_UIUpdateTimer->Tick += gcnew System::EventHandler(this, &Form1::UIUpdateTimer_Tick);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(13, 83);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(60, 26);
			this->label5->TabIndex = 13;
			this->label5->Text = L"Automation\r\ncontroller:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(12, 153);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(56, 13);
			this->label6->TabIndex = 14;
			this->label6->Text = L"DUT type:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(13, 182);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(56, 13);
			this->label7->TabIndex = 15;
			this->label7->Text = L"DUT com:";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(12, 124);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(79, 13);
			this->label8->TabIndex = 16;
			this->label8->Text = L"DUT controller:";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->m_numVddh);
			this->groupBox1->Controls->Add(this->m_numVled);
			this->groupBox1->Controls->Add(this->m_numVio);
			this->groupBox1->Controls->Add(this->m_numVdd);
			this->groupBox1->Controls->Add(this->label10);
			this->groupBox1->Controls->Add(this->label9);
			this->groupBox1->Controls->Add(this->label12);
			this->groupBox1->Controls->Add(this->label11);
			this->groupBox1->Location = System::Drawing::Point(219, 83);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(243, 112);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Voltages (mV)";
			// 
			// m_numVddh
			// 
			this->m_numVddh->Location = System::Drawing::Point(162, 66);
			this->m_numVddh->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->m_numVddh->Name = L"m_numVddh";
			this->m_numVddh->Size = System::Drawing::Size(66, 20);
			this->m_numVddh->TabIndex = 3;
			this->m_numVddh->ValueChanged += gcnew System::EventHandler(this, &Form1::PwrVddhValueChanged);
			this->m_numVddh->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::PwrVddhKeyUp);
			// 
			// m_numVled
			// 
			this->m_numVled->Location = System::Drawing::Point(162, 30);
			this->m_numVled->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->m_numVled->Name = L"m_numVled";
			this->m_numVled->Size = System::Drawing::Size(66, 20);
			this->m_numVled->TabIndex = 1;
			this->m_numVled->ValueChanged += gcnew System::EventHandler(this, &Form1::PwrVledValueChanged);
			this->m_numVled->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::PwrVledKeyUp);
			// 
			// m_numVio
			// 
			this->m_numVio->Location = System::Drawing::Point(44, 66);
			this->m_numVio->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->m_numVio->Name = L"m_numVio";
			this->m_numVio->Size = System::Drawing::Size(66, 20);
			this->m_numVio->TabIndex = 2;
			this->m_numVio->ValueChanged += gcnew System::EventHandler(this, &Form1::PwrVioValueChanged);
			this->m_numVio->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::PwrVioKeyUp);
			// 
			// m_numVdd
			// 
			this->m_numVdd->Location = System::Drawing::Point(44, 29);
			this->m_numVdd->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->m_numVdd->Name = L"m_numVdd";
			this->m_numVdd->Size = System::Drawing::Size(66, 20);
			this->m_numVdd->TabIndex = 0;
			this->m_numVdd->ValueChanged += gcnew System::EventHandler(this, &Form1::PwrVddValueChanged);
			this->m_numVdd->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::PwrVddKeyUp);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(12, 73);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(25, 13);
			this->label10->TabIndex = 5;
			this->label10->Text = L"Vio:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(12, 36);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(29, 13);
			this->label9->TabIndex = 2;
			this->label9->Text = L"Vdd:";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(125, 73);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(35, 13);
			this->label12->TabIndex = 21;
			this->label12->Text = L"Vddh:";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(125, 37);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(31, 13);
			this->label11->TabIndex = 20;
			this->label11->Text = L"Vled:";
			// 
			// m_cboAutoController
			// 
			this->m_cboAutoController->FormattingEnabled = true;
			this->m_cboAutoController->Location = System::Drawing::Point(97, 87);
			this->m_cboAutoController->Name = L"m_cboAutoController";
			this->m_cboAutoController->Size = System::Drawing::Size(105, 21);
			this->m_cboAutoController->TabIndex = 1;
			this->m_cboAutoController->TextChanged += gcnew System::EventHandler(this, &Form1::AutoCtrlNameChanged);
			// 
			// m_cboDutController
			// 
			this->m_cboDutController->FormattingEnabled = true;
			this->m_cboDutController->Location = System::Drawing::Point(97, 116);
			this->m_cboDutController->Name = L"m_cboDutController";
			this->m_cboDutController->Size = System::Drawing::Size(105, 21);
			this->m_cboDutController->TabIndex = 2;
			this->m_cboDutController->TextChanged += gcnew System::EventHandler(this, &Form1::DutCtrlNameChanged);
			// 
			// m_cboDutType
			// 
			this->m_cboDutType->FormattingEnabled = true;
			this->m_cboDutType->Location = System::Drawing::Point(97, 145);
			this->m_cboDutType->Name = L"m_cboDutType";
			this->m_cboDutType->Size = System::Drawing::Size(105, 21);
			this->m_cboDutType->TabIndex = 3;
			this->m_cboDutType->TextChanged += gcnew System::EventHandler(this, &Form1::DutTypeNameChanged);
			// 
			// m_cboDutCom
			// 
			this->m_cboDutCom->FormattingEnabled = true;
			this->m_cboDutCom->Location = System::Drawing::Point(97, 174);
			this->m_cboDutCom->Name = L"m_cboDutCom";
			this->m_cboDutCom->Size = System::Drawing::Size(105, 21);
			this->m_cboDutCom->TabIndex = 4;
			this->m_cboDutCom->TextChanged += gcnew System::EventHandler(this, &Form1::DutComNameChanged);
			// 
			// m_btnInsertStep
			// 
			this->m_btnInsertStep->Location = System::Drawing::Point(589, 37);
			this->m_btnInsertStep->Name = L"m_btnInsertStep";
			this->m_btnInsertStep->Size = System::Drawing::Size(75, 23);
			this->m_btnInsertStep->TabIndex = 13;
			this->m_btnInsertStep->Text = L"Insert...";
			this->m_btnInsertStep->UseVisualStyleBackColor = true;
			this->m_btnInsertStep->Visible = false;
			this->m_btnInsertStep->Click += gcnew System::EventHandler(this, &Form1::InsertStepBtn_Click);
			// 
			// m_btnDelStep
			// 
			this->m_btnDelStep->Location = System::Drawing::Point(690, 37);
			this->m_btnDelStep->Name = L"m_btnDelStep";
			this->m_btnDelStep->Size = System::Drawing::Size(75, 23);
			this->m_btnDelStep->TabIndex = 14;
			this->m_btnDelStep->Text = L"Delete";
			this->m_btnDelStep->UseVisualStyleBackColor = true;
			this->m_btnDelStep->Visible = false;
			this->m_btnDelStep->Click += gcnew System::EventHandler(this, &Form1::DelStepBtn_Click);
			// 
			// m_btnModStep
			// 
			this->m_btnModStep->Location = System::Drawing::Point(488, 37);
			this->m_btnModStep->Name = L"m_btnModStep";
			this->m_btnModStep->Size = System::Drawing::Size(75, 23);
			this->m_btnModStep->TabIndex = 12;
			this->m_btnModStep->Text = L"Modify...";
			this->m_btnModStep->UseVisualStyleBackColor = true;
			this->m_btnModStep->Click += gcnew System::EventHandler(this, &Form1::ModifyStepBtn_Click);
			// 
			// m_lstTestSeq
			// 
			this->m_lstTestSeq->AllowDrop = true;
			this->m_lstTestSeq->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->m_colOrderIdx, 
				this->m_colTest, this->m_colArgs});
			this->m_lstTestSeq->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->m_lstTestSeq->FullRowSelect = true;
			this->m_lstTestSeq->HideSelection = false;
			this->m_lstTestSeq->Location = System::Drawing::Point(488, 66);
			this->m_lstTestSeq->MultiSelect = false;
			this->m_lstTestSeq->Name = L"m_lstTestSeq";
			this->m_lstTestSeq->Size = System::Drawing::Size(321, 468);
			this->m_lstTestSeq->TabIndex = 15;
			this->m_lstTestSeq->UseCompatibleStateImageBehavior = false;
			this->m_lstTestSeq->View = System::Windows::Forms::View::Details;
			this->m_lstTestSeq->ItemDrag += gcnew System::Windows::Forms::ItemDragEventHandler(this, &Form1::TestSeq_ItemDrag);
			this->m_lstTestSeq->SizeChanged += gcnew System::EventHandler(this, &Form1::TestSeq_SizeChanged);
			this->m_lstTestSeq->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::TestSeq_DragDrop);
			this->m_lstTestSeq->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::TestSeq_DragEnter);
			this->m_lstTestSeq->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::TestSeq_DragOver);
			this->m_lstTestSeq->DragLeave += gcnew System::EventHandler(this, &Form1::TestSeq_DragLeave);
			this->m_lstTestSeq->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::TestSeq_MouseDoubleClick);
			// 
			// m_colOrderIdx
			// 
			this->m_colOrderIdx->Text = L"Order";
			// 
			// m_colTest
			// 
			this->m_colTest->Text = L"Test";
			// 
			// m_colArgs
			// 
			this->m_colArgs->Text = L"Arguments";
			// 
			// m_numNumRows
			// 
			this->m_numNumRows->Enabled = false;
			this->m_numNumRows->Location = System::Drawing::Point(97, 202);
			this->m_numNumRows->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->m_numNumRows->Name = L"m_numNumRows";
			this->m_numNumRows->Size = System::Drawing::Size(105, 20);
			this->m_numNumRows->TabIndex = 5;
			this->m_numNumRows->ValueChanged += gcnew System::EventHandler(this, &Form1::NumRowsValueChanged);
			this->m_numNumRows->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::NumRowsKeyUp);
			// 
			// m_numNumCols
			// 
			this->m_numNumCols->Enabled = false;
			this->m_numNumCols->Location = System::Drawing::Point(97, 228);
			this->m_numNumCols->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->m_numNumCols->Name = L"m_numNumCols";
			this->m_numNumCols->Size = System::Drawing::Size(105, 20);
			this->m_numNumCols->TabIndex = 6;
			this->m_numNumCols->ValueChanged += gcnew System::EventHandler(this, &Form1::NumColsValueChanged);
			this->m_numNumCols->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::NumColsKeyUp);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(13, 209);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(37, 13);
			this->label13->TabIndex = 30;
			this->label13->Text = L"Rows:";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(13, 235);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(30, 13);
			this->label14->TabIndex = 32;
			this->label14->Text = L"Cols:";
			// 
			// m_lstPatchFiles
			// 
			this->m_lstPatchFiles->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->m_lstPatchFiles->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->m_colPatchDescription, 
				this->m_colPatchRev, this->m_colPatchName});
			this->m_lstPatchFiles->FullRowSelect = true;
			this->m_lstPatchFiles->GridLines = true;
			this->m_lstPatchFiles->HideSelection = false;
			this->m_lstPatchFiles->Location = System::Drawing::Point(15, 295);
			this->m_lstPatchFiles->MultiSelect = false;
			this->m_lstPatchFiles->Name = L"m_lstPatchFiles";
			this->m_lstPatchFiles->Size = System::Drawing::Size(447, 239);
			this->m_lstPatchFiles->TabIndex = 11;
			this->m_lstPatchFiles->UseCompatibleStateImageBehavior = false;
			this->m_lstPatchFiles->View = System::Windows::Forms::View::Details;
			// 
			// m_colPatchDescription
			// 
			this->m_colPatchDescription->Text = L"Patch";
			// 
			// m_colPatchRev
			// 
			this->m_colPatchRev->Text = L"Rev.";
			// 
			// m_colPatchName
			// 
			this->m_colPatchName->Text = L"Name";
			// 
			// m_btnSelectPatch
			// 
			this->m_btnSelectPatch->Location = System::Drawing::Point(17, 266);
			this->m_btnSelectPatch->Name = L"m_btnSelectPatch";
			this->m_btnSelectPatch->Size = System::Drawing::Size(75, 23);
			this->m_btnSelectPatch->TabIndex = 9;
			this->m_btnSelectPatch->Text = L"Select...";
			this->m_btnSelectPatch->UseVisualStyleBackColor = true;
			this->m_btnSelectPatch->Visible = false;
			this->m_btnSelectPatch->Click += gcnew System::EventHandler(this, &Form1::SelectPatch_Click);
			// 
			// m_btnRemovePatch
			// 
			this->m_btnRemovePatch->Location = System::Drawing::Point(106, 266);
			this->m_btnRemovePatch->Name = L"m_btnRemovePatch";
			this->m_btnRemovePatch->Size = System::Drawing::Size(75, 23);
			this->m_btnRemovePatch->TabIndex = 10;
			this->m_btnRemovePatch->Text = L"Remove";
			this->m_btnRemovePatch->UseVisualStyleBackColor = true;
			this->m_btnRemovePatch->Visible = false;
			this->m_btnRemovePatch->Click += gcnew System::EventHandler(this, &Form1::RemovePatch_Click);
			// 
			// m_printDialog
			// 
			this->m_printDialog->UseEXDialog = true;
			// 
			// m_printDocument
			// 
			this->m_printDocument->BeginPrint += gcnew System::Drawing::Printing::PrintEventHandler(this, &Form1::PrintDocument_BeginPrint);
			this->m_printDocument->EndPrint += gcnew System::Drawing::Printing::PrintEventHandler(this, &Form1::PrintDocument_EndPrint);
			this->m_printDocument->PrintPage += gcnew System::Drawing::Printing::PrintPageEventHandler(this, &Form1::PrintDocument_PrintPage);
			// 
			// m_grpWrBootSecs
			// 
			this->m_grpWrBootSecs->BackColor = System::Drawing::SystemColors::Control;
			this->m_grpWrBootSecs->Controls->Add(this->m_chkWrBootSecsEnabled);
			this->m_grpWrBootSecs->Controls->Add(this->m_btnWrBootSecsEdit);
			this->m_grpWrBootSecs->Location = System::Drawing::Point(219, 202);
			this->m_grpWrBootSecs->Name = L"m_grpWrBootSecs";
			this->m_grpWrBootSecs->Size = System::Drawing::Size(243, 46);
			this->m_grpWrBootSecs->TabIndex = 8;
			this->m_grpWrBootSecs->TabStop = false;
			this->m_grpWrBootSecs->Text = L"Write Boot Sectors";
			// 
			// m_chkWrBootSecsEnabled
			// 
			this->m_chkWrBootSecsEnabled->AutoSize = true;
			this->m_chkWrBootSecsEnabled->Enabled = false;
			this->m_chkWrBootSecsEnabled->Location = System::Drawing::Point(15, 23);
			this->m_chkWrBootSecsEnabled->Name = L"m_chkWrBootSecsEnabled";
			this->m_chkWrBootSecsEnabled->Size = System::Drawing::Size(65, 17);
			this->m_chkWrBootSecsEnabled->TabIndex = 0;
			this->m_chkWrBootSecsEnabled->Text = L"Enabled";
			this->m_chkWrBootSecsEnabled->UseVisualStyleBackColor = true;
			this->m_chkWrBootSecsEnabled->CheckedChanged += gcnew System::EventHandler(this, &Form1::ChkWrBootSecsEnabled_CheckedChanged);
			// 
			// m_btnWrBootSecsEdit
			// 
			this->m_btnWrBootSecsEdit->Location = System::Drawing::Point(128, 17);
			this->m_btnWrBootSecsEdit->Name = L"m_btnWrBootSecsEdit";
			this->m_btnWrBootSecsEdit->Size = System::Drawing::Size(100, 23);
			this->m_btnWrBootSecsEdit->TabIndex = 1;
			this->m_btnWrBootSecsEdit->Text = L"Edit...";
			this->m_btnWrBootSecsEdit->UseVisualStyleBackColor = true;
			this->m_btnWrBootSecsEdit->Visible = false;
			this->m_btnWrBootSecsEdit->Click += gcnew System::EventHandler(this, &Form1::BtnWrBootSecsEdit_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 34);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(54, 13);
			this->label1->TabIndex = 33;
			this->label1->Text = L"Comment:";
			// 
			// m_txtComment
			// 
			this->m_txtComment->Enabled = false;
			this->m_txtComment->Location = System::Drawing::Point(97, 37);
			this->m_txtComment->Multiline = true;
			this->m_txtComment->Name = L"m_txtComment";
			this->m_txtComment->Size = System::Drawing::Size(365, 33);
			this->m_txtComment->TabIndex = 0;
			this->m_txtComment->TextChanged += gcnew System::EventHandler(this, &Form1::TxtComment_TextChanged);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(821, 546);
			this->Controls->Add(this->m_txtComment);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->m_btnRemovePatch);
			this->Controls->Add(this->m_btnSelectPatch);
			this->Controls->Add(this->m_lstPatchFiles);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->m_numNumCols);
			this->Controls->Add(this->m_numNumRows);
			this->Controls->Add(this->m_lstTestSeq);
			this->Controls->Add(this->m_btnModStep);
			this->Controls->Add(this->m_btnDelStep);
			this->Controls->Add(this->m_btnInsertStep);
			this->Controls->Add(this->m_cboDutCom);
			this->Controls->Add(this->m_cboDutType);
			this->Controls->Add(this->m_cboDutController);
			this->Controls->Add(this->m_cboAutoController);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->m_grpWrBootSecs);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"MT Configuration Generator";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numVddh))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numVled))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numVio))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numVdd))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numNumRows))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->m_numNumCols))->EndInit();
			this->m_grpWrBootSecs->ResumeLayout(false);
			this->m_grpWrBootSecs->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void MenuNewXml_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//If there are changes, ask the user to save data.
		if (*m_pOrgConfig != *m_pConfig)
		{
			System::Windows::Forms::DialogResult result;

			// Initializes the variables to pass to the MessageBox::Show method.
			String^ message = "Would you like to save your changes?";
			String^ caption = "Unsaved changes";
			MessageBoxButtons buttons = MessageBoxButtons::YesNoCancel;
			result = MessageBox::Show( this, message, caption, buttons );

			if (result == System::Windows::Forms::DialogResult::Yes)
			{
				// Displays a SaveFileDialog so the user can save the changes.
				m_saveFileDialog->Filter = "XML Files|*.xml";
				m_saveFileDialog->Title = "Save File";
				m_saveFileDialog->CheckFileExists = false;
				if (m_saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				{
					String^ sTemp = m_sRevision;
					m_pConfig->SetFileFullPath(msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName));
					m_pConfig->SaveXML(	msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName),
										msclr::interop::marshal_as< std::string >(sTemp));
				}
				m_pConfig->SetNewValues();
				*m_pOrgConfig = *m_pConfig;
				UIUpdateControls();
			}
			else if (result == System::Windows::Forms::DialogResult::No)
			{
				m_pConfig->SetNewValues();
				*m_pOrgConfig = *m_pConfig;
				UIUpdateControls();
			}
		}
		else
		{
			m_pConfig->SetNewValues();
			*m_pOrgConfig = *m_pConfig;
			UIUpdateControls();
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void MenuOpenXml_Click(System::Object^  sender, System::EventArgs^  e)
	{
		OpenConfigFile(m_pConfig->GetFileFullPath());
	}
			 
	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void MenuDefaultConfigurations_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Get folder where all default configurations are stored.
		CHAR szEXEPath[ MAX_PATH ];
		DWORD nChars = GetModuleFileNameA( NULL, szEXEPath, MAX_PATH );
		string sEXEPath( szEXEPath );
		sEXEPath = sEXEPath.substr(0, sEXEPath.find_last_of('\\'));

		OpenConfigFile(sEXEPath);
	}
			 
	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void OpenConfigFile(string& sPath)
	{
		//If there are changes, ask the user to save data.
		bool bOpen = true;
		if (*m_pOrgConfig != *m_pConfig)
		{
			// Initializes the variables to pass to the MessageBox::Show method.
			System::Windows::Forms::DialogResult result;
			String^ message = "Would you like to save your changes?";
			String^ caption = "Unsaved changes";
			MessageBoxButtons buttons = MessageBoxButtons::YesNoCancel;
			result = MessageBox::Show( this, message, caption, buttons );

			if (result == System::Windows::Forms::DialogResult::Yes)
			{
				// Displays a SaveFileDialog so the user can save the changes.
				m_saveFileDialog->Filter = "XML Files|*.xml";
				m_saveFileDialog->Title = "Save File";
				m_saveFileDialog->CheckFileExists = false;
				if (m_saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				{
					String^ sTemp = m_sRevision;
					m_pConfig->SetFileFullPath(msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName));
					m_pConfig->SaveXML(	msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName),
										msclr::interop::marshal_as< std::string >(sTemp));
				}
				else
					bOpen = false;
			}
			else if (result == System::Windows::Forms::DialogResult::Cancel)
				bOpen = false;
		}

		if (bOpen)
		{
			// Displays an OpenFileDialog so the user can select a Cursor.
			m_openFileDialog->FileName = L"";
			String^ str = gcnew String(sPath.c_str());
			if (sPath.size() != 0)
				m_openFileDialog->InitialDirectory = str;
			else
				m_openFileDialog->InitialDirectory = "c:\\";
			m_openFileDialog->Filter = "XML Files|*.xml";
			m_openFileDialog->Title = "Select File";

			//Get the name of the new XML file.
			if (m_openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				bool bSuccess = m_pConfig->ReadXML(msclr::interop::marshal_as< std::string >(m_openFileDialog->FileName));
				if (!bSuccess)
				{
					String^ message = "A CRC error has been detected in this file. Opening will not be allowed.";
					String^ caption = "CRC error";
					MessageBoxButtons buttons = MessageBoxButtons::OK;
					MessageBox::Show( this, message, caption, buttons );
					Config tmp;
					*m_pConfig = tmp;
				}

				*m_pOrgConfig = *m_pConfig;
				UIUpdateControls();
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void MenuItemExit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Application::Exit();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void MenuSaveXml_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if ((m_pConfig->GetFilePath()).size() != 0)
		{
			String^ sTemp = m_sRevision;
			m_pConfig->SaveXML(m_pConfig->GetFileFullPath(), msclr::interop::marshal_as< std::string >(sTemp));
			*m_pOrgConfig = *m_pConfig;
		}
		else
		{
			SaveAsXml();
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void MenuSaveAsXml_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SaveAsXml();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void SaveAsXml()
	{
		// Displays an OpenFileDialog so the user can select a file.
		m_saveFileDialog->Filter = "XML Files|*.xml";
		m_saveFileDialog->Title = "Save File As";

		//Get the name of the new XML file.
		if (m_saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ sTemp = m_sRevision;
			m_pConfig->SetFileFullPath(msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName));
			m_pConfig->SaveXML(	msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName),
								msclr::interop::marshal_as< std::string >(sTemp));
			*m_pOrgConfig = *m_pConfig;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void MenuSaveAsCSV_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		// Displays an OpenFileDialog so the user can select a file.
		m_saveFileDialog->Filter = "CSV Files|*.csv";
		m_saveFileDialog->Title = "Save File As";
		string sDefaultFilename = m_pConfig->GetFileFullPath();
		
		if (sDefaultFilename != "")
		{
			int sz = sDefaultFilename.size();
			int found = sDefaultFilename.find_last_of("/\\");

			// Change the extension from "xml" to "csv"
			sDefaultFilename.replace(sz-3, 3, "csv");

			// Defaults to same filename with "csv" extension at the same location
			m_saveFileDialog->FileName = gcnew String((sDefaultFilename.substr(found+1)).c_str()); 

			// Get the name of the new CSV file or use default
			if (m_saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				String^ sTemp = m_sRevision;
				bool bSuccess = SaveCSV(	msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName),
											msclr::interop::marshal_as< std::string >(sTemp));
			
				if (bSuccess == false)
				{
					String^ message = "Please close CSV file and try again.";
					String^ caption = "Save As CSV";
					MessageBoxButtons buttons = MessageBoxButtons::OK;
					MessageBox::Show( this, message, caption, buttons );
				}
			}
		}
		else
		{
			String^ message = "Please open a configuration file and try again.";
			String^ caption = "Save As CSV";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBox::Show( this, message, caption, buttons );
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	private: bool SaveCSV(const string& sFileName, const string& sRevision)
	{
		bool		bSuccess = true;
		string		sTemp;
		String^		sValues;
		String^		sValues2;
		FILE*		m_pCsvFile;

		errno_t nErr = fopen_s(&m_pCsvFile, sFileName.c_str(),"w");

		// Check if file is open already.
		if (nErr != 0)
		{
			bSuccess = false;
		}
		else
		{
			fprintf(m_pCsvFile, "\nMT Configuration Generator (%s)\n\n", sRevision.c_str());

			sValues2 = gcnew String(m_pConfig->GetFileName().c_str());
			sValues = (m_pConfig->GetFileName().empty()) ? "Untitled" : sValues2;
			fprintf(m_pCsvFile, "File:,,%s\n", sValues);
	
			sValues = gcnew String(m_pConfig->GetAutoCtrlName().c_str());
			fprintf(m_pCsvFile, "Auto controller:,,%s\n", sValues);

			sValues = gcnew String(m_pConfig->GetDutCtrlName().c_str());
			fprintf(m_pCsvFile, "Dut controller:,,%s\n", sValues);

			sValues = gcnew String(m_pConfig->GetDutTypeName().c_str());
			fprintf(m_pCsvFile, "Dut type:,,%s\n", sValues);

			sValues = gcnew String(m_pConfig->GetDutComName().c_str());
			fprintf(m_pCsvFile, "Dut comm:,,%s\n", sValues);

			fprintf(m_pCsvFile, "Voltages (mV):,,Vdd = %d, Vio = %d, Vled = %d, Vddh = %d\n", 
					m_pConfig->GetPwrVdd(), m_pConfig->GetPwrVio(), m_pConfig->GetPwrVled(), m_pConfig->GetPwrVddh());

			fprintf(m_pCsvFile, "Rows:,,%d\n", m_pConfig->GetNumRows());

			fprintf(m_pCsvFile, "Cols:,,%d\n", m_pConfig->GetNumCols());

			//There are two boot sectors (0 & 1). Print both.
			if (m_pConfig->GetWriteBootSecsFlg())
			{
				fprintf(m_pCsvFile, "\nBoot sector 0 (enabled):,,,,,Boot sector 1 (enabled):\n");

				string sBootSec0 = m_pConfig->GetBootSec(0);
				string sBootSec1 = m_pConfig->GetBootSec(1);

				for (int nLine=0; nLine<4; nLine++)
				{
					sTemp = "0x" + sBootSec0.substr(((nLine*32)+ 0), 8);
					fprintf(m_pCsvFile, "%s,",	gcnew String(sTemp.c_str()));

					sTemp = "0x" + sBootSec0.substr(((nLine*32)+ 8), 8);
					fprintf(m_pCsvFile, "%s,",	gcnew String(sTemp.c_str()));

					sTemp = "0x" + sBootSec0.substr(((nLine*32)+16), 8);
					fprintf(m_pCsvFile, "%s,",	gcnew String(sTemp.c_str()));

					sTemp = "0x" + sBootSec0.substr(((nLine*32)+24), 8);
					fprintf(m_pCsvFile, "%s,,",	gcnew String(sTemp.c_str()));

					sTemp = "0x" + sBootSec1.substr(((nLine*32)+ 0), 8);
					fprintf(m_pCsvFile, "%s,",	gcnew String(sTemp.c_str()));

					sTemp = "0x" + sBootSec1.substr(((nLine*32)+ 8), 8);
					fprintf(m_pCsvFile, "%s,",	gcnew String(sTemp.c_str()));

					sTemp = "0x" + sBootSec1.substr(((nLine*32)+16), 8);
					fprintf(m_pCsvFile, "%s,",	gcnew String(sTemp.c_str()));

					sTemp = "0x" + sBootSec1.substr(((nLine*32)+24), 8);
					fprintf(m_pCsvFile, "%s\n",	gcnew String(sTemp.c_str()));
				}
			}
			else
			{
				fprintf(m_pCsvFile, "\nBoot sector 0 (disabled):\nBoot sector 1 (disabled):\n");
			}
	
			fprintf(m_pCsvFile, "\nTest sequence:\n");
			fprintf(m_pCsvFile, "Order:,,Test Name:,,,Test Arguments:\n");

			for (int i=0; i<m_pConfig->GetNumSteps(); i++)
			{
				StepAndArgs stepArgs = m_pConfig->GetTestStep(i);
				int nSeqNum = GetSequenceNumber(stepArgs.m_sStepName);
				fprintf(m_pCsvFile, "%d,,%s,,,%s\n", nSeqNum, gcnew String((stepArgs.m_sStepName).c_str()), gcnew String((stepArgs.m_sStepArgs).c_str()));
			}

			fprintf(m_pCsvFile, "\nPatches:\n");
			fprintf(m_pCsvFile, "Description:,,,Revision:,,Filename:\n");

			for (int i=0; i<m_pConfig->GetNumFpsFiles(); i++)
			{
				FpsFile* pFpsFile = m_pConfig->GetFpsFilePtr(i);
				sValues  = (pFpsFile->GetNumBytes() == 0) ? "None selected" : gcnew String((pFpsFile->GetName()).c_str());
				sValues2 = ((gcnew String((pFpsFile->GetVersion()).c_str()))->Trim() == "")  ? "-" : (gcnew String((pFpsFile->GetVersion()).c_str()))->Trim();
				fprintf(m_pCsvFile, "%s,,,%s,,%s\n", gcnew String((pFpsFile->GetDescription()).c_str()), 
													sValues2,
													sValues);
			}

			fprintf(m_pCsvFile, "\nKey Test Parameters:\n");
			fprintf(m_pCsvFile, "Test Type:,,Name:,,,Value:,,Min Parameter:,,Max Parameter:\n");

			sTemp = ReturnTestArguments("InitializationStep");
			int found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,BGA", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Flex ID", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Data Exclusion of Rows with Fake Finger (Min=Top; Max=Bottom)", found, displayType2);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Data Exclusion of Columns with Fake Finger (Min=Left; Max=Right)", found, displayType2);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Build #", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Data Exclusion of Rows with NO Finger (Min=Top; Max=Bottom)", found, displayType2);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Data Exclusion of Columns with NO Finger (Min=Left; Max=Right)", found, displayType2);
			}

			sTemp = ReturnTestArguments("AcquireFPSSamples");
			found = -1; 
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Images Discard", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Images No Finger", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Set-up,,Images w/ Fake Finger", found, displayType1);
			}

			sTemp = ReturnTestArguments("Calibrate");
			found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nCalibrate,,LNA Print File Index", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,PGA Print File Index", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,Force Calibrate (0 = No Force; 1 = Force)", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,Discard Images (for Calibration)", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,LNA Offset", found, displayType2);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,PGA Limit", found, displayType2);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,Cal Type (0 = No Force; 1 = Force)", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,PGA Ratio", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,# of PGA Samples", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Calibrate,,Stage2 OTP Variance (Score Test)", found, displayType3);
			}

			sTemp = ReturnTestArguments("WakeOnFinger");
			found = 1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nWOF,,Minimum Delta", found, displayType4);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "WOF,,Trigger Threshold", found, displayType2);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "WOF,,# of Response Bytes", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "WOF,,# of Noise Samples", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "WOF,,Threshold Offset", found, displayType2);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "WOF,,Standard Dev", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "WOF,,Min Delta 2", found, displayType4);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "WOF,,Min Delta 3", found, displayType4);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "WOF,,VCC", found, displayType1);
			}

			sTemp = ReturnTestArguments("PeggedPixelsTest");
			found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nPegged Pixel,,Image Count", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Pegged Pixel,,Max Pegged / row", found, displayType3);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Pegged Pixel,,Max Pegged / column", found, displayType3);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Pegged Pixel,,Pegged Threshold", found, displayType3);
			}

			sTemp = ReturnTestArguments("FlooredPixelsTest");
			found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nFloored Pixel,,Image Count", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Floored Pixel,,Max Floored / row", found, displayType3);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Floored Pixel,,Max Floored / column", found, displayType3);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Floored Pixel,,Floored Threshold", found, displayType4);
			}

			sTemp = ReturnTestArguments("ConsecutivePixels");
			found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nConsecutive Pixel,,Threshold", found, displayType6);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Consecutive Pixel,,Max Adjacent", found, displayType3);
			}

			sTemp = ReturnTestArguments("PixelTest");
			found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nPixel Test,,Image Count", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Pixel Test,,Threshold (row)", found, displayType3);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Pixel Test,,Threshold (col)", found, displayType3);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Pixel Test,,Trim (all sides)", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Pixel Test,,Pixel Uniformity Threshold (+/-)", found, displayType3);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Pixel Test,,Max Pixel Uniformity # of Failed Pixels allowed", found, displayType3);
			}

			sTemp = ReturnTestArguments("CurrentTest");
			found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nCurrent Test,,Low Gain", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Current Test,,High Gain", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Current Test,,Image Analog (mA)", found, displayType5);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Current Test,,Image Digital (mA)", found, displayType5);
			}

			sTemp = ReturnTestArguments("RetainModeCurrentTest");
			found = 1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nRetain Mode Current,,Retain Mode Current", found, displayType6);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Retain Mode Current,,Delay (ms)", found, displayType1);
			}

			sTemp = ReturnTestArguments("SNRTest");
			found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nSNR,,Image Count", found, displayType1);
				found += 8;
				PrintKeyTestParameters(m_pCsvFile, sTemp, "SNR,,% of Data Ignored", found, displayType1);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "SNR,,Overall SNR", found, displayType6);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "SNR,,Overall Signal", found, displayType6);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "SNR,,Overall Noise", found, displayType6);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "SNR,,Zone SNR", found, displayType6);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "SNR,,Zone Signal", found, displayType6);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "SNR,,Zone Noise", found, displayType6);
			}

			sTemp = ReturnTestArguments("Imperfections");
			found = -1;
			if (sTemp != "")
			{
				PrintKeyTestParameters(m_pCsvFile, sTemp, "\nImperfections,,Threshold", found, displayType2);
				PrintKeyTestParameters(m_pCsvFile, sTemp, "Imperfections,,Consecutive Limit", found, displayType3);
			}

			fclose(m_pCsvFile);
		}

		return bSuccess;
	}

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	private: string ReturnTestArguments(string sStepName)
	{
		StepAndArgs stepArgs;
		string sTemp;

		for (int i=0; i<m_pConfig->GetNumSteps(); i++)
		{
			stepArgs = m_pConfig->GetTestStep(i);
			if (stepArgs.m_sStepName == sStepName)
			{
				sTemp = stepArgs.m_sStepArgs;
				i = m_pConfig->GetNumSteps();
			}
		}

		return sTemp;
	}

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	private: void PrintKeyTestParameters(FILE* m_pCsvFile, string& sTestArgs, string sTestTypeName, signed int& found, unsigned displayType)
	{
		string sTemp;
		String^ sValues;
		String^ sValues2;

		if (displayType == displayType1)	// sTestArgs = "a b c d e", print format: "a	-	-"
		{
			sTestArgs = sTestArgs.substr(found+1); 
			found = sTestArgs.find_first_of(" ");
			sTemp = sTestArgs.substr(0,found);
			sValues = gcnew String(sTemp.c_str());
			sValues2 = gcnew String(sTestTypeName.c_str());

			fprintf(m_pCsvFile, "%s,,,%s,,-,,-\n", sValues2, sValues);
		}
		else if (displayType == displayType2)	// sTestArgs = "a b c d e", print format: "-	a	b"
		{
			sTestArgs = sTestArgs.substr(found+1); 
			found = sTestArgs.find_first_of(" ");
			sTemp = sTestArgs.substr(0,found);
			sValues = gcnew String(sTemp.c_str());
			sValues2 = gcnew String(sTestTypeName.c_str());

			fprintf(m_pCsvFile, "%s,,,-,,%s,,", sValues2, sValues);

			sTestArgs = sTestArgs.substr(found+1); 
			found = sTestArgs.find_first_of(" ");
			sTemp = sTestArgs.substr(0,found);
			sValues = gcnew String(sTemp.c_str());

			fprintf(m_pCsvFile, "%s\n", sValues);
		}
		else if (displayType == displayType3)	// sTestArgs = "a b c d e", print format: "-	-	a"
		{
			sTestArgs = sTestArgs.substr(found+1); 
			found = sTestArgs.find_first_of(" ");
			sTemp = sTestArgs.substr(0,found);
			sValues = gcnew String(sTemp.c_str());
			sValues2 = gcnew String(sTestTypeName.c_str());

			fprintf(m_pCsvFile, "%s,,,-,,-,,%s\n", sValues2, sValues);
		}
		else if (displayType == displayType4)	// sTestArgs = "a b c d e", print format: "-	a	-"
		{
			sTestArgs = sTestArgs.substr(found+1); 
			found = sTestArgs.find_first_of(" ");
			sTemp = sTestArgs.substr(0,found);
			sValues = gcnew String(sTemp.c_str());
			sValues2 = gcnew String(sTestTypeName.c_str());

			fprintf(m_pCsvFile, "%s,,,-,,%s,,-\n", sValues2, sValues);
		}
		else if (displayType == displayType5)	// sTestArgs = "a b c d e", print format: "-	c	a"
		{
			sTestArgs = sTestArgs.substr(found+1);  // sTestArgs = "25 5 5 0"
			found = sTestArgs.find_first_of(" "); // found = 2
			sTemp = sTestArgs.substr(0,found); // sTemp = "25"

			string sTemp2 = sTestArgs.substr(found+1); // sTemp2 = "5 5 0"
			signed int found2 = sTemp2.find_first_of(" "); // found2 = 1
			sTemp2 = sTemp2.substr(found2+1); // sTemp2 = "5 0"
			found2 = sTemp2.find_first_of(" "); // found2 = 1
			sTemp = sTemp2.substr(0,found2) + ",," + sTemp;

			sValues = gcnew String(sTemp.c_str());
			sValues2 = gcnew String(sTestTypeName.c_str());

			fprintf(m_pCsvFile, "%s,,,-,,%s\n", sValues2, sValues);
		}
		else if (displayType == displayType6)	// sTestArgs = "a b c d e", print format: "-	b	a"
		{
			sTestArgs = sTestArgs.substr(found+1);  // sTestArgs = "1 0.1 0"
			found = sTestArgs.find_first_of(" "); // found = 1
			sTemp = sTestArgs.substr(0,found); // sTemp = "1"

			sTestArgs = sTestArgs.substr(found+1); // sTestArgs = "0.1 0"
			found = sTestArgs.find_first_of(" "); // found2 = 3
			sTemp = sTestArgs.substr(0,found) + ",," + sTemp;

			sValues = gcnew String(sTemp.c_str());
			sValues2 = gcnew String(sTestTypeName.c_str());

			fprintf(m_pCsvFile, "%s,,,-,,%s\n", sValues2, sValues);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void UIUpdateTimer_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		//Update the application file name. If there is a change from the original data, append '*'.
		String^ sFileName = gcnew String((m_pConfig->GetFileName()).c_str());
		if (*m_pOrgConfig == *m_pConfig)
		{
			if (m_pConfig->GetFileName().empty())
			{
				if (this->Text != ("Untitled" + " - " + this->m_sOrgDialogTitle))
					this->Text = "Untitled" + " - " + this->m_sOrgDialogTitle;
			}
			else if (this->Text != (sFileName + " - " + this->m_sOrgDialogTitle))
				this->Text = sFileName + " - " + this->m_sOrgDialogTitle;
		}
		else
		{
			//Use '*' to indicate a change has been made.
			if (m_pConfig->GetFileName().empty())
			{
				if (this->Text != ("Untitled *" + " - " + this->m_sOrgDialogTitle))
					this->Text = "Untitled *" + " - " + this->m_sOrgDialogTitle;
			}
			else if (this->Text != (sFileName + " *" + " - " + this->m_sOrgDialogTitle))
				this->Text = sFileName + " *" + " - " + this->m_sOrgDialogTitle;
		}

		//Make sure that the number of row digits does not exheed 4 characters.
		string sTemp = msclr::interop::marshal_as< std::string >(m_numNumCols->Text);
		if (sTemp.size() > 4)
			m_numNumCols->Text = "0";
		int nTemp = (int)m_numNumCols->Value;
		m_pConfig->SetNumCols(nTemp);

		//If we're showing "Write Boot Sectors" controls, make sure they're in the correct state.
		if (m_chkWrBootSecsEnabled->Visible)
		{
			if (m_pConfig->GetWriteBootSecsFlg() && (!m_btnWrBootSecsEdit->Enabled))
				m_btnWrBootSecsEdit->Enabled = true;
			else if ((!m_pConfig->GetWriteBootSecsFlg()) && (m_btnWrBootSecsEdit->Enabled))
				m_btnWrBootSecsEdit->Enabled = false;
		}

		ServicePatchList();
		ServiceTestSeq();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void UIUpdateControls()
	{
		String^ str = gcnew String((m_pConfig->GetComment()).c_str());
		m_txtComment->Text = str;
		str = gcnew String((m_pConfig->GetAutoCtrlName()).c_str());
		m_cboAutoController->Text = str;
		str = gcnew String((m_pConfig->GetDutCtrlName()).c_str());
		m_cboDutController->Text = str;
		str = gcnew String((m_pConfig->GetDutTypeName()).c_str());
		m_cboDutType->Text = str;
		str = gcnew String((m_pConfig->GetDutComName()).c_str());
		m_cboDutCom->Text = str;

		m_numVdd->Text				= (m_pConfig->GetPwrVdd()).ToString();
		m_numVio->Text				= (m_pConfig->GetPwrVio()).ToString();
		m_numVled->Text				= (m_pConfig->GetPwrVled()).ToString();
		m_numVddh->Text				= (m_pConfig->GetPwrVddh()).ToString();
		m_numNumRows->Text			= (m_pConfig->GetNumRows()).ToString();
		m_numNumCols->Text			= (m_pConfig->GetNumCols()).ToString();

		m_chkWrBootSecsEnabled->CheckState	= m_pConfig->GetWriteBootSecsFlg() ?	CheckState::Checked :
																					CheckState::Unchecked;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void ServicePatchList()
	{
		bool	bChanged = false;

		//Save the currently selected step index.
		bool bSelected = false;
		int curSel = 0;
		if (m_lstPatchFiles->SelectedItems->Count != 0)
		{
			curSel = m_lstPatchFiles->SelectedIndices[0];
			bSelected = true;
		}
		else
		{
			if (m_lstPatchFiles->Items->Count != 0)
				(m_lstPatchFiles->Items[curSel])->Selected = true;
		}

		//Determine if any patch has changed.
		if (m_lstPatchFiles->Items->Count != 0)
		{
			for (int i=0; i<m_pConfig->GetNumFpsFiles(); i++)
			{
				FpsFile* pFpsFile = m_pConfig->GetFpsFilePtr(i);
				String^ sName = gcnew String((pFpsFile->GetName()).c_str());
				String^ sVer = gcnew String((pFpsFile->GetVersion()).c_str());
				if (pFpsFile->GetNumBytes() == 0)
				{
					if (((m_lstPatchFiles->Items[i])->SubItems[2])->Text != "None selected")
						bChanged = true;
				}
				else if ((sName != ((m_lstPatchFiles->Items[i])->SubItems[2])->Text) ||
						 (sVer != ((m_lstPatchFiles->Items[i])->SubItems[1])->Text))
				{
					bChanged = true;
				}
			}
		}
		else
			bChanged = true;

		//If any patch has changed.
		if (bChanged)
		{
			m_lstPatchFiles->BeginUpdate();
			m_lstPatchFiles->Items->Clear();
			for (int i=0; i<m_pConfig->GetNumFpsFiles(); i++)
			{
				FpsFile* pFpsFile = m_pConfig->GetFpsFilePtr(i);
				String^ str = gcnew String((pFpsFile->GetDescription()).c_str());
				String^ str2 = gcnew String((pFpsFile->GetName()).c_str());
				String^ str3 = gcnew String((pFpsFile->GetVersion()).c_str());
				if (pFpsFile->GetNumBytes() == 0)
					str2 = "None selected";
				ListViewItem^ listItem = gcnew ListViewItem( str );
				listItem->SubItems->Add( str3 );
				listItem->SubItems->Add( str2 );
				m_lstPatchFiles->Items->Add(listItem);
			}
		}

		//Make sure appropriate step stays selected.
		if (bSelected && bChanged)
		{
			if (curSel < m_lstPatchFiles->Items->Count)
				(m_lstPatchFiles->Items[curSel])->Selected = true;
			else
				(m_lstPatchFiles->Items[0])->Selected = true;
		}
		m_lstPatchFiles->EndUpdate();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void ServiceTestSeq()
	{
		bool	bChanged = false;

		//Save the currently selected step index.
		bool bSelected = false;
		int curSel = 0;
		if (m_lstTestSeq->SelectedItems->Count != 0)
		{
			curSel = m_lstTestSeq->SelectedIndices[0];
			bSelected = true;
		}
		else
		{
			if (m_lstTestSeq->Items->Count != 0)
				(m_lstTestSeq->Items[curSel])->Selected = true;
		}

		//Determine if any Test step name, arguments or sequence has changed.
		if (m_pConfig->GetNumSteps() == m_lstTestSeq->Items->Count)
		{
			for (int i=0; i<m_pConfig->GetNumSteps(); i++)
			{
				StepAndArgs stepArgs = m_pConfig->GetTestStep(i);
				String^ sTestName = gcnew String((stepArgs.m_sStepName).c_str());
				String^ sDetails = gcnew String((stepArgs.m_sStepArgs).c_str());
				if ((sTestName != m_lstTestSeq->Items[i]->SubItems[1]->Text) || (sDetails != m_lstTestSeq->Items[i]->SubItems[2]->Text))
					bChanged = true;
			}
		}
		else
			bChanged = true;

		//If any Tests or arguments have been changed.
		if (bChanged)
		{
			m_lstTestSeq->BeginUpdate();
			m_lstTestSeq->Items->Clear();
			for (int i=0; i<m_pConfig->GetNumSteps(); i++)
			{
				StepAndArgs stepArgs = m_pConfig->GetTestStep(i);
				int nSeqNum = GetSequenceNumber(stepArgs.m_sStepName);
				String^ sOrderIdx = "-";
				if (nSeqNum != 0)
					sOrderIdx = gcnew String(System::Convert::ToString(nSeqNum));

				String^ sTestName = gcnew String((stepArgs.m_sStepName).c_str());
				String^ sDetails = gcnew String((stepArgs.m_sStepArgs).c_str());
				ListViewItem^ listItem = gcnew ListViewItem( sOrderIdx );
				listItem->SubItems->Add( sTestName );
				listItem->SubItems->Add( sDetails );
				m_lstTestSeq->Items->Add(listItem);
			}
		}

		//Make sure appropriate step stays selected.
		if (bSelected && bChanged)
		{
			if (curSel < m_lstTestSeq->Items->Count)
				(m_lstTestSeq->Items[curSel])->Selected = true;
			else
				(m_lstTestSeq->Items[0])->Selected = true;
		}
		m_lstTestSeq->EndUpdate();
	}

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	private: int GetSequenceNumber(string& sStepName)
	{
		int nStepIdx = 0;

		for (int i = 0; i<(int)m_pAvailableStepsArr->size(); i++)
		{
			if (sStepName == (m_pAvailableStepsArr->at(i)).GetName())
				nStepIdx = i+1;
		}

		return nStepIdx;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void AutoCtrlNameChanged(System::Object^  sender, System::EventArgs^  e)
	{
		m_pConfig->SetAutoCtrlName(msclr::interop::marshal_as< std::string >(m_cboAutoController->Text));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void DutCtrlNameChanged(System::Object^  sender, System::EventArgs^  e)
	{
		m_pConfig->SetDutCtrlName(msclr::interop::marshal_as< std::string >(m_cboDutController->Text));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void DutTypeNameChanged(System::Object^  sender, System::EventArgs^  e)
	{
		m_pConfig->SetDutTypeName(msclr::interop::marshal_as< std::string >(m_cboDutType->Text));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void DutComNameChanged(System::Object^  sender, System::EventArgs^  e)
	{
		m_pConfig->SetDutComName(msclr::interop::marshal_as< std::string >(m_cboDutCom->Text));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PwrVddValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numVdd->Text);
		if (sTemp.size() > 4)
		{
			m_numVdd->Text = "0";
		}

		int nTemp = (int)m_numVdd->Value;
		m_pConfig->SetPwrVdd(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PwrVioValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numVio->Text);
		if (sTemp.size() > 4)
		{
			m_numVio->Text = "0";
		}

		int nTemp = (int)m_numVio->Value;
		m_pConfig->SetPwrVio(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PwrVledValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numVled->Text);
		if (sTemp.size() > 4)
		{
			m_numVled->Text = "0";
		}

		int nTemp = (int)m_numVled->Value;
		m_pConfig->SetPwrVled(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PwrVddhValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numVddh->Text);
		if ((sTemp.size() > 4) || (sTemp.size() == 0))
		{
			m_numVddh->Text = "0";
		}

		int nTemp = (int)m_numVddh->Value;
		m_pConfig->SetPwrVddh(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PwrVddKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numVdd->Text);
		if ((sTemp.size() > 4) || (sTemp.size() == 0))
		{
			m_numVdd->Text = "0";
		}

		int nTemp = (int)m_numVdd->Value;
		m_pConfig->SetPwrVdd(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PwrVioKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numVio->Text);
		if ((sTemp.size() > 4) || (sTemp.size() == 0))
		{
			m_numVio->Text = "0";
		}

		int nTemp = (int)m_numVio->Value;
		m_pConfig->SetPwrVio(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PwrVledKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numVled->Text);
		if ((sTemp.size() > 4) || (sTemp.size() == 0))
		{
			m_numVled->Text = "0";
		}

		int nTemp = (int)m_numVled->Value;
		m_pConfig->SetPwrVled(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PwrVddhKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		//Allow only 4 digits for this value.
		string sTemp = msclr::interop::marshal_as< std::string >(m_numVddh->Text);
		if ((sTemp.size() > 4) || (sTemp.size() == 0))
		{
			m_numVddh->Text = "0";
		}

		int nTemp = (int)m_numVddh->Value;
		m_pConfig->SetPwrVddh(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void DelStepBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Don't let user delete the first or last steps (Initialization, Finalization). 
		if ((m_lstTestSeq->SelectedIndices[0] != 0) && (m_lstTestSeq->SelectedIndices[0] != (m_lstTestSeq->Items->Count - 1)))
		{
			m_pConfig->DeleteTestStep(m_lstTestSeq->SelectedIndices[0]);
		}
		else
		{
			// Initializes the variables to pass to the MessageBox::Show method.
			String^ message = "You can delete neither the Initialization nor Finalization steps.";
			String^ caption = "Delete error";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBox::Show( this, message, caption, buttons );
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void ModifyStepBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		ModifySelectedStep();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void TestSeq_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		ModifySelectedStep();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void ModifySelectedStep()
	{
		InsertTestStep^ dlg = gcnew InsertTestStep;
		StepAndArgs stepAndArgs = m_pConfig->GetTestStep(m_lstTestSeq->SelectedIndices[0]);
		dlg->SetAvailableStepsPtr(m_pAvailableStepsArr);
		dlg->SetStepAndArgs(stepAndArgs);
		dlg->SetInsertFlg(false);
		dlg->ShowDialog();

		//If the user has not hit the cancel button.
		if (dlg->GetCancelFlg() == false)
		{
			StepAndArgs newStepAndArgs = dlg->GetStepAndArgs();
			m_pConfig->SetTestStep(m_lstTestSeq->SelectedIndices[0], newStepAndArgs);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void InsertStepBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Don't let user insert after the last step (Finalization). 
		if (m_lstTestSeq->SelectedIndices[0] < (m_lstTestSeq->Items->Count - 1))
		{
			InsertTestStep^ dlg = gcnew InsertTestStep;
			dlg->SetAvailableStepsPtr(m_pAvailableStepsArr);
			dlg->SetInsertFlg(true);
			dlg->ShowDialog();

			//If the user has not hit the cancel button.
			if (dlg->GetCancelFlg() == false)
			{
				StepAndArgs newStepAndArgs = dlg->GetStepAndArgs();
				if (!newStepAndArgs.m_sStepName.empty())
				{
					m_pConfig->InsertTestStepAfter(m_lstTestSeq->SelectedIndices[0], newStepAndArgs);
					(m_lstTestSeq->Items[m_lstTestSeq->SelectedIndices[0]+1])->Selected = true;
				}
			}
		}
		else
		{
			// Initializes the variables to pass to the MessageBox::Show method.
			String^ message = "You cannot insert after the Finalization step.";
			String^ caption = "Insert error";
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			MessageBox::Show( this, message, caption, buttons );
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void NumRowsValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numNumRows->Text);
		if (sTemp.size() > 4)
		{
			m_numNumRows->Text = "0";
		}

		int nTemp = (int)m_numNumRows->Value;
		m_pConfig->SetNumRows(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void NumRowsKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		//Allow only 4 digits for this value.
		string sTemp = msclr::interop::marshal_as< std::string >(m_numNumRows->Text);
		if ((sTemp.size() > 4) || (sTemp.size() == 0))
		{
			m_numNumRows->Text = "0";
		}

		int nTemp = (int)m_numNumRows->Value;
		m_pConfig->SetNumRows(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void NumColsValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		string sTemp = msclr::interop::marshal_as< std::string >(m_numNumCols->Text);
		if (sTemp.size() > 4)
		{
			m_numNumCols->Text = "0";
		}

		int nTemp = (int)m_numNumCols->Value;
		m_pConfig->SetNumCols(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void NumColsKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		//Allow only 4 digits for this value.
		string sTemp = msclr::interop::marshal_as< std::string >(m_numNumCols->Text);
		if ((sTemp.size() > 4) || (sTemp.size() == 0))
		{
			m_numNumCols->Text = "0";
		}

		int nTemp = (int)m_numNumCols->Value;
		m_pConfig->SetNumCols(nTemp);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if ((e->KeyCode == Keys::S) && (e->Alt) && (e->Shift))
		{
			//Toggle the protected controls.
			m_txtComment->Enabled			= !m_txtComment->Enabled;
			m_numNumRows->Enabled			= !m_numNumRows->Enabled;
			m_numNumCols->Enabled			= !m_numNumCols->Enabled;
			m_chkWrBootSecsEnabled->Enabled	= !m_chkWrBootSecsEnabled->Enabled;

			m_btnSelectPatch->Visible		= !m_btnSelectPatch->Visible;
			m_btnRemovePatch->Visible		= !m_btnRemovePatch->Visible;
			m_btnInsertStep->Visible		= !m_btnInsertStep->Visible;
			m_btnDelStep->Visible			= !m_btnDelStep->Visible;
			m_btnWrBootSecsEdit->Visible	= !m_btnWrBootSecsEdit->Visible;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
	{
		//If there are changes, ask the user to save data.
		if (*m_pOrgConfig != *m_pConfig)
		{
			// Initializes the variables to pass to the MessageBox::Show method.
			System::Windows::Forms::DialogResult result;
			String^ message = "Would you like to save your changes?";
			String^ caption = "Unsaved changes";
			MessageBoxButtons buttons = MessageBoxButtons::YesNoCancel;
			result = MessageBox::Show( this, message, caption, buttons );

			if (result == System::Windows::Forms::DialogResult::Cancel)
			{
				//Cancel the save request
				e->Cancel = true;
			}
			else
			{
				if (result == System::Windows::Forms::DialogResult::Yes)
				{
					// Displays a SaveFileDialog so the user can save the changes.
					m_saveFileDialog->Filter = "XML Files|*.xml";
					m_saveFileDialog->Title = "Save File";
					m_saveFileDialog->CheckFileExists = false;
					if (m_saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					{
						String^ sTemp = m_sRevision;
						m_pConfig->SetFileFullPath(msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName));
						m_pConfig->SaveXML(	msclr::interop::marshal_as< std::string >(m_saveFileDialog->FileName),
											msclr::interop::marshal_as< std::string >(sTemp));
					}
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void TestSeq_SizeChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::Drawing::Size size = m_lstTestSeq->ClientSize;
		m_colArgs->Width = size.Width - m_colTest->Width - m_colOrderIdx->Width - 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void SelectPatch_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int			nIdx = m_lstPatchFiles->SelectedIndices[0];	//Selected index.
		FpsFile*	pFpsFile = m_pConfig->GetFpsFilePtr(nIdx);

		// Displays an OpenFileDialog.
		String^ sStr1 = gcnew String((pFpsFile->GetDescription()).c_str());
		String^ sStr2 = gcnew String((pFpsFile->GetExtension()).c_str());
		OpenFileDialog ^ openFileDialog = gcnew OpenFileDialog();
		openFileDialog->Filter = sStr1 + " Files|*." + sStr2;
		openFileDialog->Title = "Select a " + sStr1 + " File";

		// Get the name of the new patch file.
		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			//If the user has specified a valid patch file, read it in. Otherwise, do nothing.
			string sSafeFileName = msclr::interop::marshal_as< std::string >(openFileDialog->SafeFileName);
			string sFileName = msclr::interop::marshal_as< std::string >(openFileDialog->FileName);
			bool bSuccess = pFpsFile->ProcessFpsFile(sFileName);
		
			if (bSuccess)
			{
				//If this is the print file, try to get the column count.
				if (pFpsFile->GetXmlTag() == "PrintFile")
				{
					FileStream^ fs = gcnew FileStream(openFileDialog->FileName, FileMode::Open, FileAccess::Read, FileShare::None);
					StreamReader^ sr = gcnew StreamReader(fs);
					String^ in = sr->ReadLine();
					while (in != nullptr)
					{
						int idx = in->IndexOf("LineLength =");
						if (idx != -1)
						{
							m_pConfig->SetNumCols(System::Int32::Parse(in->Substring(idx + sizeof("LineLength ="))));
							UIUpdateControls();
						}

						in = sr->ReadLine();
					}
					fs->Close( );
				}
			}
			else
			{
				// Initializes the variables to pass to the MessageBox::Show method.
				String^ message = "The specified file is unrecognized.";
				String^ caption = "Unrecognized File";
				MessageBoxButtons buttons = MessageBoxButtons::OK;
				MessageBox::Show( this, message, caption, buttons );
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void RemovePatch_Click(System::Object^  sender, System::EventArgs^  e)
	{
		m_pConfig->ClearFpsFile(m_lstPatchFiles->SelectedIndices[0]);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void MenuFilePrint_Click(System::Object^  sender, System::EventArgs^  e)
	{
		// Allow the user to choose the page range he or she would
		// like to print.
		m_printDialog->AllowSomePages = true;

		// Show the help button.
		m_printDialog->ShowHelp = true;

		// Set the Document property to the PrintDocument for 
		// which the PrintPage Event has been handled. To display the
		// dialog, either this property or the PrinterSettings property 
		// must be set 
		m_printDialog->Document = m_printDocument;
		if ( m_printDocument == nullptr )
			System::Windows::Forms::MessageBox::Show( "null" );

		if ( m_printDialog == nullptr )
			System::Windows::Forms::MessageBox::Show( "pnull" );

		// If the result is OK then print the document.
		System::Windows::Forms::DialogResult result = m_printDialog->ShowDialog();
		if ( result == ::DialogResult::OK )
			m_printDocument->Print();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PrintDocument_BeginPrint(System::Object^  sender, System::Drawing::Printing::PrintEventArgs^  e)
	{
		m_printState = PrintState::PS_HEADER;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PrintDocument_EndPrint(System::Object^  sender, System::Drawing::Printing::PrintEventArgs^  e)
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void PrintDocument_PrintPage(System::Object^  sender, System::Drawing::Printing::PrintPageEventArgs^  e)
	{
		String^ text;
		String^ sLabels;
		String^ sValues;
		String^ strTmp;
		string	sTemp;

		System::Drawing::Font^ printFont = gcnew System::Drawing::Font( "Courier New",11,System::Drawing::FontStyle::Regular );
		float linesPerPage = e->MarginBounds.Height / printFont->GetHeight( e->Graphics );

		if (m_printState == PrintState::PS_HEADER)
		{
			sLabels = this->m_sOrgDialogTitle + "\n\n";
			sLabels += "File:\nAuto controller:\nDut controller:\nDut type:\nDut comm:\nVoltages (mV):\nRows:\nCols:\n";
			e->Graphics->DrawString( sLabels, printFont, System::Drawing::Brushes::Black, 20, 20 );

			strTmp = gcnew String(m_pConfig->GetFileName().c_str());
			sValues += (m_pConfig->GetFileName().empty()) ? "Untitled\n" : strTmp + "\n";
			sValues += m_cboAutoController->Text + "\n";
			sValues += m_cboDutController->Text + "\n";
			sValues += m_cboDutType->Text + "\n";
			sValues += m_cboDutCom->Text + "\n";
			sValues +=	"Vdd = " + m_numVdd->Text + ", Vio = " + m_numVio->Text +
						", Vled = " + m_numVled->Text + ", Vddh = " + m_numVddh->Text + "\n";
			sValues += m_numNumRows->Text + "\n";
			sValues += m_numNumCols->Text + "\n";
			e->Graphics->DrawString( sValues, printFont, System::Drawing::Brushes::Black, 200, 20 + (printFont->GetHeight( e->Graphics )*2) );

			//There are two boot sectors (0 & 1). Print both.
			if (m_pConfig->GetWriteBootSecsFlg())
			{
				sTemp = "";
				string	sBootSec0 = m_pConfig->GetBootSec(0);
				string	sBootSec1 = m_pConfig->GetBootSec(1);
				text = "\nBoot sector 0 (enabled):\t\t\tBoot sector 1 (enabled):\n";
				for (int nLine=0; nLine<4; nLine++)
				{
					sTemp += sBootSec0.substr(((nLine*32)+ 0), 8) + " ";
					sTemp += sBootSec0.substr(((nLine*32)+ 8), 8) + " ";
					sTemp += sBootSec0.substr(((nLine*32)+16), 8) + " ";
					sTemp += sBootSec0.substr(((nLine*32)+24), 8) + " \t";
					sTemp += sBootSec1.substr(((nLine*32)+ 0), 8) + " ";
					sTemp += sBootSec1.substr(((nLine*32)+ 8), 8) + " ";
					sTemp += sBootSec1.substr(((nLine*32)+16), 8) + " ";
					sTemp += sBootSec1.substr(((nLine*32)+24), 8) + " \n";
				}
				text += gcnew String(sTemp.c_str());
				m_nNumPageLinesPrinted = 17;
			}
			else
			{
				text += "\nBoot sector 0 (disabled):\n";
				text += "Boot sector 1 (disabled):\n";
				m_nNumPageLinesPrinted = 14;
			}

			//Print test sequence next.
			m_nNumStepsPrinted = 0;
			m_printState = PrintState::PS_STEP_SEQ;
			e->Graphics->DrawString( text, printFont, System::Drawing::Brushes::Black, 20, 11 * printFont->GetHeight( e->Graphics ) );
		}

		if (m_printState == PrintState::PS_STEP_SEQ)
		{
			sLabels = "Test sequence:\n";
			m_nNumPageLinesPrinted += 2;
			sValues = "\n";
			int nOrgLineCount = m_nNumPageLinesPrinted;
			//If there are more steps to print and there is space for at least one more line.
			for (int i=m_nNumStepsPrinted; (i<m_pConfig->GetNumSteps() && ((m_nNumPageLinesPrinted+1) < (int)linesPerPage)); i++)
			{
				StepAndArgs stepArgs = m_pConfig->GetTestStep(i);
				sLabels += gcnew String((stepArgs.m_sStepName).c_str()) + "\n";
				sValues += gcnew String((stepArgs.m_sStepArgs).c_str()) + "\n";
				m_nNumPageLinesPrinted++;
				m_nNumStepsPrinted++;
			}
			e->Graphics->DrawString( sLabels, printFont, System::Drawing::Brushes::Black, 20, nOrgLineCount * printFont->GetHeight( e->Graphics ) );
			e->Graphics->DrawString( sValues, printFont, System::Drawing::Brushes::Black, 250, nOrgLineCount * printFont->GetHeight( e->Graphics ) );
			e->HasMorePages = true;
			m_nNumPageLinesPrinted = 0;

			//If we're done printing all steps.
			if (m_nNumStepsPrinted >= m_pConfig->GetNumSteps())
			{
				m_printState = PrintState::PS_PATCHES;
				m_bPrintHeading = true;
				m_nNumPatchesPrinted = 0;
			}
		}
		else if (m_printState == PrintState::PS_PATCHES)
		{
			m_nNumHdrLines = 0;
			if (m_bPrintHeading)
			{
				sLabels = "Patches:\n";
				m_bPrintHeading = false;
				m_nNumPageLinesPrinted++;
				m_nNumHdrLines = 1;
			}

			//If there are more Patch files to print and there is space for at least one more line.
			sValues = "";
			for (int i=m_nNumPatchesPrinted; (i<m_pConfig->GetNumFpsFiles()) && ((m_nNumPageLinesPrinted+1) < (int)linesPerPage); i++)
			{
				FpsFile* pFpsFile = m_pConfig->GetFpsFilePtr(i);
				sLabels += gcnew String((pFpsFile->GetDescription()).c_str()) + "\n";
				sValues += (pFpsFile->GetNumBytes() == 0) ? "None selected\n" : gcnew String((pFpsFile->GetName()).c_str()) + "\n";
				m_nNumPatchesPrinted++;
			}
			e->Graphics->DrawString( sLabels, printFont, System::Drawing::Brushes::Black, 20, 20 );
			e->Graphics->DrawString( sValues, printFont, System::Drawing::Brushes::Black, 350, (1+m_nNumHdrLines) * printFont->GetHeight( e->Graphics ) );
			m_nNumPageLinesPrinted = 0;
			e->HasMorePages = true;

			if (m_nNumPatchesPrinted >= m_pConfig->GetNumFpsFiles())
			{
				m_printState = PrintState::PS_ARG_DES;
				m_bPrintHeading = true;
				m_nNumPatchesPrinted = 0;
			}
		}
		else if (m_printState == PrintState::PS_ARG_DES)
		{
			m_nNumHdrLines = 0;
			if (m_bPrintHeading)
			{
				sLabels = "Key Test Parameters:\n";
				m_bPrintHeading = false;
				m_nNumPageLinesPrinted++;
				m_nNumHdrLines = 1;
			}
			e->Graphics->DrawString( sLabels, printFont, System::Drawing::Brushes::Black, 20, 20+ (0 * printFont->GetHeight( e->Graphics )) );
			e->HasMorePages = false;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void ChkWrBootSecsEnabled_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (m_chkWrBootSecsEnabled->CheckState == CheckState::Checked)
			m_pConfig->SetWriteBootSecsFlg(true);
		else
			m_pConfig->SetWriteBootSecsFlg(false);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void BtnWrBootSecsEdit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		DlgBootSectors^ dlg = gcnew DlgBootSectors();
		dlg->SetBootSec(0, m_pConfig->GetBootSec(0));
		dlg->SetBootSec(1, m_pConfig->GetBootSec(1));

		//If the user wants to save these changes.
		dlg->ShowDialog();
		if (dlg->GetOkClickedFlg())
		{
			m_pConfig->SetBootSec(0, dlg->GetBootSec(0));
			m_pConfig->SetBootSec(1, dlg->GetBootSec(1));
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void TestSeq_ItemDrag(System::Object^  sender, System::Windows::Forms::ItemDragEventArgs^  e)
	{

		//If the user has not enabled the insert feature, do not allow drag and drop.
		if (m_btnInsertStep->Visible == true)
			m_lstTestSeq->DoDragDrop( e->Item, DragDropEffects::Move );
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void TestSeq_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
	{
		e->Effect = e->AllowedEffect;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void TestSeq_DragLeave(System::Object^  sender, System::EventArgs^  e)
	{
		 m_lstTestSeq->InsertionMark->Index = -1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void TestSeq_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
	{
		// Retrieve the index of the insertion mark; 
		int targetIndex = m_lstTestSeq->InsertionMark->Index;

		// If the insertion mark is not visible, exit the method. 
		if ( targetIndex == -1 )
		{
			return;
		}

		// Retrieve the dragged item.
		ListViewItem^ draggedItem = dynamic_cast<ListViewItem^>(e->Data->GetData( ListViewItem::typeid ));

		// If the insertion mark is below the item with 
		// the corresponding index, increment the target index. 
		if ( !m_lstTestSeq->InsertionMark->AppearsAfterItem )
		{
			targetIndex--;
		}

		StepAndArgs	temp = m_pConfig->GetTestStep(draggedItem->Index);
		if (draggedItem->Index > targetIndex)
		{
			m_pConfig->DeleteTestStep(draggedItem->Index);
			m_pConfig->InsertTestStepAfter(targetIndex, temp);
		}
		else
		{
			m_pConfig->InsertTestStepAfter(targetIndex, temp);
			m_pConfig->DeleteTestStep(draggedItem->Index);
		}

		// Insert a copy of the dragged item at the target index. 
		// A copy must be inserted before the original item is removed 
		// to preserve item index values.
		m_lstTestSeq->Items->Insert( targetIndex, dynamic_cast<ListViewItem^>(draggedItem->Clone()) );

		// Remove the original copy of the dragged item.
		m_lstTestSeq->Items->Remove( draggedItem );
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void TestSeq_DragOver(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
	{
		// Retrieve the client coordinates of the mouse pointer.
		Point targetPoint = m_lstTestSeq->PointToClient( Point(e->X,e->Y) );

		// Retrieve the index of the item closest to the mouse pointer. 
		int targetIndex = m_lstTestSeq->InsertionMark->NearestIndex( targetPoint );

		// Confirm that the mouse pointer is not over the dragged item.
		if ( targetIndex > -1 )
		{
			// Determine whether the mouse pointer is to the left or 
			// the right of the midpoint of the closest item and set 
			// the InsertionMark.AppearsAfterItem property accordingly.
			System::Drawing::Rectangle itemBounds = m_lstTestSeq->GetItemRect( targetIndex );
			if ( targetPoint.Y > itemBounds.Top + (itemBounds.Height / 2) )
			{
				//Don't allow a drop after the last step (Finalization).
				if (targetIndex == m_pConfig->GetNumSteps()-1)
					targetIndex = -1;
				else
					m_lstTestSeq->InsertionMark->AppearsAfterItem = true;
			}
			else
			{
				//Don't allow a drop before the first step (Initialization).
				if (targetIndex == 0)
					targetIndex = -1;
				else
					m_lstTestSeq->InsertionMark->AppearsAfterItem = false;
			}
		}

		// Set the location of the insertion mark. If the mouse is 
		// over the dragged item, the targetIndex value is -1 and 
		// the insertion mark disappears.
		m_lstTestSeq->InsertionMark->Index = targetIndex;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	private: System::Void TxtComment_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		m_pConfig->SetComment(msclr::interop::marshal_as< std::string >(m_txtComment->Text));
	}
};
}
