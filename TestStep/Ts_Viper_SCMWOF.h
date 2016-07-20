#pragma once
#include "Syn_FingerprintTest.h"
class Ts_Viper_SCMWOF : public Syn_FingerprintTest
{
public:
	Ts_Viper_SCMWOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_Viper_SCMWOF();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	bool ExecuteZone0SCMWofTest(SCM_WofTestInfo& info, SCM_WofTestResults& results, bool UseConfigVotage);

	void SYN_SCMWofTestExecute(const SCM_WofTestInfo& pInfo, SCM_WofTestResults& pResults);

	bool CalcScmWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx);
};

