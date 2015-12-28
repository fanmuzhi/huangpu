#ifndef FPS_TESTEXECUTIVE_H
#define FPS_TESTEXECUTIVE_H

#include <QtWidgets/QMainWindow>
#include "ui_fps_testexecutive.h"
#include "MpcApiDll.h"
#include "SysConfig.h"

//DutUtls
#include <Syn_DutOperation.h>
#include <Syn_Dut.h>
#include <Syn_DutCtrl.h>

//std
#include <sstream>
#include <vector>
#include <iostream>

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

public Q_SLOTS:
	int GetVersion();
	int DutPowerOn();
	int DutPowerOff();

private:
	void ConvertAsciiToBinary(const string& sAscii, int* pDst, int nDstSize);
	void Display(uint8_t* pDst, int DstSize);
	uint32_t Init();
	Ui::FPS_TestExecutiveClass ui;
	uint32_t m_deviceHandle;

	vector<Syn_Dut*> _ListOfDutPtr;
};

#endif // FPS_TESTEXECUTIVE_H
