#pragma once
#include "Syn_FingerprintTest.h"

class Ts_DRdyTest : public Syn_FingerprintTest
{
public:
	Ts_DRdyTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_DRdyTest();
	
	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

};

