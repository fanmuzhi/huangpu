#pragma once

#include "Syn_FingerprintTest.h"

class Ts_WOFFD : public Syn_FingerprintTest
{
public:
	Ts_WOFFD(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WOFFD();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	bool GetZ0WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd1Patch, Syn_PatchInfo &WofCmd2Patch);

	bool GetZ1WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd3Patch, Syn_PatchInfo &WofCmd4Patch);

	void SYN_WofTestExecute(const WofTestInfo &Info, WofTestResults &Results);

	bool CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx);
};

