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
#include "Syn_Thread.h"
#include "Syn_LocalSettingsDlg.h"
#include "Ui_Syn_LocalSettingsDlg.h"
#include "Syn_ThreadDebug.h"//for debug

//DutUtls
#include "Syn_Site.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"
#include "Syn_Config.h"

//std
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <thread>

//Construct Device Counts(large enough)
#define DeviceCounts 20

extern "C" {
#include "SYN_TestUtils.h"
};

using namespace std;

class FPS_TestExecutive : public QMainWindow
{
	Q_OBJECT

public:
	FPS_TestExecutive(QWidget *parent = 0);
	~FPS_TestExecutive();

	void Initialize();

	bool ConstructSiteList(QString strConfigFilePath, bool SendMsg = false);


public Q_SLOTS:

	void CreateLocalSettings();

	void CloseLocalSettingsDialog();
	
	void SelectConfigFile();

	bool UpdateSiteLocalSettings();

	void LocalSettingsOKAction();

	void RunningTest();

	//void ReceiveDutTestResultSlot(void * pDutTestResult);

	void ReceiveSiteInfoSlot(void * pSiteInfo);

	//Dubug
	//Dut dump
	void GetVersionForDutDump();

	void ReadOTPForDutDump();

	//Calibration,Fingerprint
	void PushCablicationImageButton();
	void ImageCalibration(void * pSiteInfo);
	void PushFigerprintImageButton();
	void FigerprintImage(void * pSiteInfo);

private:

	//private function
	void Display(uint8_t* pDst, int DstSize);

	void Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos);

	//variable
	Ui::FPS_TestExecutiveClass ui;

	vector<Syn_Site*> _ListOfSitePtr;
	Syn_Thread _SynThreadArray[DeviceCounts];
	unsigned int _iRealDeviceCounts;

	bool _bStopTag;

	//std::ofstream _logfile;

	Syn_LocalSettingsDlg *_pSyn_LocalSettingsDlg;

	Syn_LocalSettings _LocalSettingsInfo;

	//debug
	Syn_ThreadDebug _threadForDebug;

	Syn_ThreadDebug _threadForDebugCalibrate;

};

#endif // FPS_TESTEXECUTIVE_H
