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
#include <fstream>
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



	bool ConstructSyn_SysConfig(const std::string &strConfigFilePath,Syn_SysConfig &oSyn_SysConfig);


public Q_SLOTS:
<<<<<<< HEAD
	
	void ThreadTest();

	//void receiveslot(QString strTime);
	//void receiveslot(Syn_St strTime);
=======

	void ThreadTest();

>>>>>>> master
	void receiveslot(void * strTime);

	void SelectFile();

	void csvFileAnalysis(QString &strFilePath);

private:

	void Display(uint8_t* pDst, int DstSize);

	void Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos);


	Syn_SysConfig _Config;

	uint32_t Init();
	Ui::FPS_TestExecutiveClass ui;
	uint32_t m_deviceHandle;

	vector<Syn_Site*> _ListOfSitePtr;

	Syn_Thread _synThread;

	bool _bStopTag;

	QImage *_pImage;
};

#endif // FPS_TESTEXECUTIVE_H
