#ifndef FPS_TESTEXECUTIVE_H
#define FPS_TESTEXECUTIVE_H

#include <QtWidgets/QMainWindow>
#include "ui_fps_testexecutive.h"
#include "MpcApiDll.h"
#include "SysConfig.h"

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

public Q_SLOTS:
	int GetVersion();
	int DutPowerOn();
	int DutPowerOff();


private:
	void ConvertAsciiToBinary(const string& sAscii, int* pDst, int nDstSize);
	void Display(uint8_t* pDst, int DstSize);

	void Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos);



	uint32_t Init();
	Ui::FPS_TestExecutiveClass ui;
	uint32_t m_deviceHandle;

	vector<Syn_Dut*> _ListOfDutPtr;
};

#endif // FPS_TESTEXECUTIVE_H
