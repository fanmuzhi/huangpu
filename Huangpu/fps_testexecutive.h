#ifndef FPS_TESTEXECUTIVE_H
#define FPS_TESTEXECUTIVE_H

//Qt
#include <QtWidgets/QMainWindow>
#include <QVector>

//UI
#include "ui_fps_testexecutive.h"

//MPC DLL
#include "MpcApiDll.h"

//Local
#include "SysConfig.h"
#include "Syn_Thread.h"

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


public Q_SLOTS:

	void SelectConfigFile();

	void RunningTest();

	void ReceiveOTPTestSlot(void * pOTPTestInfo);

private:

	//private function
	void Display(uint8_t* pDst, int DstSize);

	void Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos);

	bool Init(QString strConfigFile);

	//variable
	Ui::FPS_TestExecutiveClass ui;

	vector<Syn_Site*> _ListOfSitePtr;
	Syn_Thread _SynThreadArray[DeviceCounts];
	unsigned int _iRealDeviceCounts;

	bool _bStopTag;

	std::ofstream _logfile;
};

#endif // FPS_TESTEXECUTIVE_H
