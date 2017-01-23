#pragma once

#include "Syn_FingerprintTest.h"


class Ts_Metallica_WOFLowPower : public Syn_FingerprintTest
{
public:

	Ts_Metallica_WOFLowPower(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_Metallica_WOFLowPower();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();
};

