#pragma once
#include "Syn_FingerprintTest.h"

class Ts_PixelPatchTest : public Syn_FingerprintTest
{
public:
	Ts_PixelPatchTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_PixelPatchTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

