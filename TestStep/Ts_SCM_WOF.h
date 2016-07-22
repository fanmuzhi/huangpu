#pragma once

#include "Syn_FingerprintTest.h"

class Ts_SCM_WOF :public Syn_FingerprintTest
{
public:
	Ts_SCM_WOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SCM_WOF();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	bool ExecuteZone1SCMWofTest(SCM_WofTestInfo& info, SCM_WofTestResults& results);

	bool ExecuteZone0SCMWofTest(SCM_WofTestInfo& info, SCM_WofTestResults& results);

	void SYN_SCMWofTestExecute(const SCM_WofTestInfo& pInfo, SCM_WofTestResults& pResults);

	bool CalcScmWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx);
};

