#pragma once
#include "Syn_FingerprintTest.h"
class Ts_SNRTest : public Syn_FingerprintTest
{
public:
	Ts_SNRTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SNRTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

