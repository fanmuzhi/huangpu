#pragma once

#include "Syn_FingerprintTest.h"

class Ts_ReadDutAdc : public Syn_FingerprintTest
{
public:
	Ts_ReadDutAdc(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_ReadDutAdc();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

};

