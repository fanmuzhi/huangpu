#pragma once
#include "Syn_FingerprintTest.h"

class Ts_CurrentTest : public Syn_FingerprintTest
{
public:
	Ts_CurrentTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_CurrentTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

