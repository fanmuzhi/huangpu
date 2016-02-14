#pragma once

//Local
#include "Syn_TestStep.h"

class Syn_FingerprintTest :public Syn_TestStep
{

public:

	Syn_FingerprintTest(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);

	virtual ~Syn_FingerprintTest();
	

	void PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep);

	void PowerOff();

	void GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols);

};

