#pragma once
#include "Syn_FingerprintTest.h"

class Ts_PixelTest : public Syn_FingerprintTest
{
public:

	Ts_PixelTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_PixelTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

