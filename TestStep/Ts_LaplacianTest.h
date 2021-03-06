#pragma once
#include "Syn_FingerprintTest.h"

class Ts_LaplacianTest : public Syn_FingerprintTest
{
public:
	Ts_LaplacianTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_LaplacianTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

