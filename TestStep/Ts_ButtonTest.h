#pragma once
#include "Syn_FingerprintTest.h"

class Ts_ButtonTest : public Syn_FingerprintTest
{
public:
	Ts_ButtonTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_ButtonTest();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	void SYN_ProcessBtnTestData(BtnTestInfo &pInfo, BtnTestResults &pResults);
};

