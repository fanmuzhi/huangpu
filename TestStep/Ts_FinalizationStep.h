#pragma once
#include "Syn_FingerprintTest.h"
class Ts_FinalizationStep : public Syn_FingerprintTest
{
public:
	Ts_FinalizationStep(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_FinalizationStep();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	void WriteLog();

};

