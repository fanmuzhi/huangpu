#pragma once

#include "Syn_FingerprintTest.h"
class Ts_WOFLowPower : public Syn_FingerprintTest
{
public:

	Ts_WOFLowPower(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WOFLowPower();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	void FpPokeCmd(uint32_t nHwRegAddr, uint32_t nData);

};

