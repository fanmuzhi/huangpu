#pragma once

#include "Syn_FingerprintTest.h"

class Ts_WOF : public Syn_FingerprintTest
{
public:

	Ts_WOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WOF();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	void GetZ0WofData(WofTestInfo &wofInfo, WofTestResults &wofResults);

	void GetZ1WofData(WofTestInfo &wofInfo, WofTestResults &wofResults);

	void SYN_WofTestExecute(WofTestInfo &Info, WofTestResults &Results);

	int CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf);
};

