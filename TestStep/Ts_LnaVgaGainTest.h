#pragma once

#include "Syn_FingerprintTest.h"

class Ts_LnaVgaGainTest : public Syn_FingerprintTest
{
public:
	Ts_LnaVgaGainTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_LnaVgaGainTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

