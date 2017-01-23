#pragma once

#include "Syn_FingerprintTest.h"

class Ts_FlexIdTest : public Syn_FingerprintTest
{
public:
	Ts_FlexIdTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_FlexIdTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

