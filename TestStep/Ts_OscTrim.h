#pragma once

#include "Syn_FingerprintTest.h"

class Ts_OscTrim : public Syn_FingerprintTest
{
public:
	
	Ts_OscTrim(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_OscTrim();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	void GetOscValues(uint32_t &oOscTrim);
};

