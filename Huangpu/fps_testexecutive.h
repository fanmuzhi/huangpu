#ifndef FPS_TESTEXECUTIVE_H
#define FPS_TESTEXECUTIVE_H

#include <QtWidgets/QMainWindow>
#include "ui_fps_testexecutive.h"
#include "MpcApiDll.h"
#include "SysConfig.h"

//Local
#include "Syn_Thread.h"

//DutUtls
#include "Syn_Site.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"
#include "Syn_Config.h"

//std
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

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
    void parse_config();


	bool ConstructSyn_SysConfig(const std::string &strConfigFilePath,Syn_SysConfig &oSyn_SysConfig);


	//threadTest
	static void SetTest(void * vpSite);
	static void GetTest(void *vpFPS_TestExecutive, void * vpSite);



public Q_SLOTS:
	int GetVersion();
	int DutPowerOn();
	int DutPowerOff();

	void ThreadTest();


	//void receiveslot(QString strTime);
	//void receiveslot(Syn_St strTime);
	void receiveslot(void * strTime);


private:
	void ConvertAsciiToBinary(const string& sAscii, int* pDst, int nDstSize);
	void Display(uint8_t* pDst, int DstSize);

	void Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos);



	uint32_t Init();
	Ui::FPS_TestExecutiveClass ui;
	uint32_t m_deviceHandle;

	vector<Syn_Dut*> _ListOfDutPtr;

	vector<Syn_Site*> _ListOfSitePtr;

	//vector<Syn_Thread> _ListOfThread;
	Syn_Thread _synThread;

	bool _bStopTag;
};

#endif // FPS_TESTEXECUTIVE_H
