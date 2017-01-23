#pragma once

#include "Syn_FingerprintTest.h"

class Ts_IromChecksumTest : public Syn_FingerprintTest
{
public:
	Ts_IromChecksumTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_IromChecksumTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

