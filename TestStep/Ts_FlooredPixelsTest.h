#pragma once
#include "Syn_FingerprintTest.h"

class Ts_FlooredPixelsTest : public Syn_FingerprintTest
{
public:

	Ts_FlooredPixelsTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_FlooredPixelsTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

