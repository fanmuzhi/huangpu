#pragma once
#include "Syn_FingerprintTest.h"

class Ts_SharpnessTest : public Syn_FingerprintTest
{
public:
	Ts_SharpnessTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SharpnessTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

