#pragma once
#include "Syn_FingerprintTest.h"

class Ts_PeggedPixelsTest : public Syn_FingerprintTest
{
public:

	Ts_PeggedPixelsTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_PeggedPixelsTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

