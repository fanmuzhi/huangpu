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

	//bool GetZ0WofData(WofTestInfo &wofInfo, WofTestResults &wofResults);
	bool GetZ0WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd1Patch, Syn_PatchInfo &WofCmd2Patch);

	//bool GetZ1WofData(WofTestInfo &wofInfo, WofTestResults &wofResults);
	bool GetZ1WofData(WofTestInfo &wofInfo, WofTestResults &wofResults, Syn_PatchInfo &WofCmd3Patch, Syn_PatchInfo &WofCmd4Patch);

	void SYN_WofTestExecute(const WofTestInfo &Info, WofTestResults &Results);

	int CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf);
};

