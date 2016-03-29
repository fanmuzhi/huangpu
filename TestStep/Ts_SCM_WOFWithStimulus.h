#pragma once

#include "Syn_FingerprintTest.h"

class Ts_SCM_WOFWithStimulus : public Syn_FingerprintTest
{
public:
	Ts_SCM_WOFWithStimulus(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SCM_WOFWithStimulus();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();
};

