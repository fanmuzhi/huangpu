#pragma once

#include "Syn_FingerprintTest.h"

class Ts_DynOffCorrDacTest : public Syn_FingerprintTest
{
public:
	Ts_DynOffCorrDacTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_DynOffCorrDacTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

