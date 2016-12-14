#pragma once

#include "Syn_FingerprintTest.h"

class Ts_AFETest : public Syn_FingerprintTest
{
public:
	Ts_AFETest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_AFETest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

