#pragma once
#include "Syn_FingerprintTest.h"

class Ts_RetainMode : public Syn_FingerprintTest
{
public:
	Ts_RetainMode(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_RetainMode();
	
	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

