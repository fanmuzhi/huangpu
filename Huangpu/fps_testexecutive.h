#ifndef FPS_TESTEXECUTIVE_H
#define FPS_TESTEXECUTIVE_H

//Qt
#include <QtWidgets/QMainWindow>
#include <QVector>

//UI
#include "ui_fps_testexecutive.h"

//MPC DLL
//#include "MpcApiDll.h"

//Local
#include "Syn_LocalSettingConfig.h"
#include "Syn_Thread.h"
#include "Syn_LocalSettingsDlg.h"
#include "Ui_Syn_LocalSettingsDlg.h"
#include "Syn_SerialNumberManageDlg.h"
#include "Ui_Syn_SerialNumberManageDlg.h"
#include "Syn_UpdateADCOffsetsDlg.h"
#include "Ui_Syn_UpdateADCOffsetsDlg.h"
#include "Syn_BinCodesDlg.h"
#include "Syn_ThreadDebug.h"//for debug

//DutUtls
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"
#include "Syn_Exception.h"

//TestEngine
#include "Syn_Site.h"
#include "Syn_DeviceManager.h"

extern "C" {
#include "SYN_TestUtils.h"
};

//std
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

//Construct Device Counts(large enough)
#define DeviceCounts 20

struct Syn_VoltagesValue
{
	uint32_t nVdd;
	uint32_t nVio;
	uint32_t nVled;
	uint32_t nVddh;
};

class FPS_TestExecutive : public QMainWindow
{
	Q_OBJECT

public:
	FPS_TestExecutive(QWidget *parent = 0);
	~FPS_TestExecutive();

	void Initialize();

	//bool ConstructSiteList(QString strConfigFilePath, bool SendMsg = false);
	bool ConstructSiteList(const Syn_LocalSettings &LocalSettingsInfo);


public Q_SLOTS:

	//exit program
	void Exit();

	//LocalSettings Dialog slots
	void CreateLocalSettings();
	void CloseLocalSettingsDialog();
	void SelectConfigFile();
	void SelectLogFilePath();
	void ModifySiteCounts();
	void ModifySerialNumber();
	void SetLeds(int rowNumber, int columnNumber);

	void ConfirmSite();

	//SerialNumber Manage Dialog slots
	void ConfirmSerialNumberForSite();
	void CloseSiteManageDialog();

	void CreateUpdateADCOffsetsDlg();

	void UpdateADCOffsets();
	void CloseUpdateADCOffsetsDialog();

	void Run();
	void ReceiveSiteInfo(unsigned int iSiteNumber);

	void ReceiveTest(unsigned int iSiteNumber, const QString strTestStepName, const QString strPassResults);

	//Debug
	//Dut dump
	void GetVersionForDutDump();

	void ReadOTPForDutDump();

	//Calibration,Fingerprint
	void PushCablicationImageButton();
	void ImageCalibration(unsigned int iSiteNumber);
	void PushFigerprintImageButton();
	void FigerprintImage(unsigned int iSiteNumber);

	//write log
	void WriteLog(std::string strFolderPath, Syn_DutTestInfo * DutInfo, Syn_DutTestResult * DutResults,int RowNumber,int ColumnNumber);

	//bincode display
	void DisplayBinCodes();
	void CloseBinCodesDlg();

	//
	void ManageButtonStatus(int iFlag);

private:

	//private function
	void Display(uint8_t* pDst, int DstSize);
	void Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos);

	//variable
	//UI
	Ui::FPS_TestExecutiveClass ui;

	//Sites&thread
	vector<Syn_Site*> _ListOfSitePtr;
	Syn_Thread _SynThreadArray[DeviceCounts];
	unsigned int _iRealDeviceCounts;

	//Device Manage(for Construct Sites)
	Syn_DeviceManager *_pSyn_DeviceManager;

	//Son Dialog
	Syn_LocalSettingsDlg *_pSyn_LocalSettingsDlg;
	Syn_SerialNumberManageDlg *_pSyn_SerialNumberManageDlg;
	Syn_UpdateADCOffsetsDlg *_pSyn_UpdateADCOffsetsDlg;
	Syn_BinCodesDlg *_pSyn_BinCodesDlg;

	//LocalSetting config Operation
	Syn_LocalSettings _LocalSettingsInfo;

	//Temp Voltages,save VoltagesValue from user define or configfile
	Syn_VoltagesValue _TempVoltagesValue;

	bool _bStopTag;

	//debug
	Syn_ThreadDebug _threadForDebug;

	Syn_ThreadDebug _threadForDebugCalibrate;

	unsigned int _FinishedSiteCounts;


	QMenuBar *m_MenuBar;
};

#endif // FPS_TESTEXECUTIVE_H
