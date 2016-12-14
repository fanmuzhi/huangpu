#pragma once
#include "Syn_FingerprintTest.h"

class Ts_WoVarTest : public Syn_FingerprintTest
{
public:
	Ts_WoVarTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WoVarTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

