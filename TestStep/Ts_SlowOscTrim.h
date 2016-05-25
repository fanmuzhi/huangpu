#pragma once

#include "Syn_FingerprintTest.h"

class Ts_SlowOscTrim : public Syn_FingerprintTest
{

public:
	
	Ts_SlowOscTrim(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SlowOscTrim();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	void GetSlowOscValues(uint32_t &oSlowOscTrim);
};

