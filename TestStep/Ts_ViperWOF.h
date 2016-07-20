#pragma once
#include "Syn_FingerprintTest.h"

class Ts_ViperWOF : public Syn_FingerprintTest
{
public:
	Ts_ViperWOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_ViperWOF();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	bool GetZ0WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, bool UseConfigVoltage);

	void SYN_WofTestExecute(const WofTestInfo &Info, WofTestResults &Results);

	bool CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx);
};

