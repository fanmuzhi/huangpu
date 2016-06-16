#pragma once

#include "Syn_FingerprintTest.h"

class Ts_WOFFU : public Syn_FingerprintTest
{
public:

	Ts_WOFFU(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WOFFU();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	bool GetZone0FingerUpData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd1Patch, Syn_PatchInfo &WofCmd2Patch);

	bool GetZone1FingerUpData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd3Patch, Syn_PatchInfo &WofCmd4Patch);

	void SYN_WofTestExecute(const WofTestInfo &Info, WofTestResults &Results);

	int CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf);
};

