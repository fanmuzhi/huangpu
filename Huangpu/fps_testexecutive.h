#ifndef FPS_TESTEXECUTIVE_H
#define FPS_TESTEXECUTIVE_H

//Qt
#include <QtWidgets/QMainWindow>
#include <QVector>

//UI
#include "ui_fps_testexecutive.h"

//Local
#include "Syn_LocalSettingConfig.h"
#include "Syn_Thread.h"
#include "Syn_MiniThread.h"
#include "Syn_LocalSettingsDlg.h"
#include "Syn_BinCodesDlg.h"
#include "Syn_DebugModeDlg.h"
#include "Syn_AboutDlg.h"

//DutUtls
#include "Syn_Dut.h"
#include "FpAlphaModule.h"
#include "Syn_Exception.h"
#include "Syn_TestUtils.h"

//TestEngine
#include "Syn_Site.h"

//std
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

//Construct Device Counts(large enough)
#define DeviceCounts 20

class FPS_TestExecutive : public QMainWindow
{
	Q_OBJECT

public:
	FPS_TestExecutive(QWidget *parent = 0);
	~FPS_TestExecutive();

	//bool ConstructSiteList(QString strConfigFilePath, bool SendMsg = false);
	bool ConstructSiteList(const Syn_LocalSettings &LocalSettingsInfo);

	void keyPressEvent(QKeyEvent * ev);

public Q_SLOTS:

	void Initialize();

	//exit program
	void Exit();

	//LocalSettings Dialog slots
	void CreateLocalSettings();
	//bincode display
	void CreateBinCodesDlg();

	void ShowAboutDlg();

	void Run();

	void ReceiveTestStep(unsigned int iSiteNumber, const QString strTestStepName, const QString strPassResults);

	void ReceiveTestResults(unsigned int iSiteNumber, const Syn_DutTestResult *pDutResult);

	void DisplayImageInTime(unsigned int iSiteNumber, const Syn_DutTestResult *pDutResult);

	void ManageButtonStatus(int iFlag);

	void GetErrorInfo(uint32_t iErrorCode, std::string &osErrorInfo);
	
	//Debug
	void GetVersion();
	void ReadOTP();
	void Invalidate();

	void CreateDebugModeDlg();
	void StartDebugMode();

private:

	//private function
	void Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos);
	void ClearSites();

	//UI
	Ui::FPS_TestExecutiveClass ui;

	//Sites&thread
	vector<Syn_Site*> _ListOfSitePtr;
	Syn_Thread _SynThreadArray[DeviceCounts];
	Syn_MiniThread _SynMiniThreadArray[DeviceCounts];
	unsigned int _iRealDeviceCounts;

	//LocalSetting config Operation
	Syn_LocalSettings _LocalSettingsInfo;

	unsigned int _FinishedSiteCounts;
};

#endif // FPS_TESTEXECUTIVE_H
