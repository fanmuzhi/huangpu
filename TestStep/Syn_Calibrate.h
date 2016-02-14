#pragma once

//Local
#include "Syn_FingerprintTest.h"

class Syn_Calibrate :public Syn_FingerprintTest
{
public:

	Syn_Calibrate(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Syn_Calibrate();

	int Excute();
};

