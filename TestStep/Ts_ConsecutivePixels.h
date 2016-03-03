#pragma once
#include "Syn_FingerprintTest.h"

class Ts_ConsecutivePixels : public Syn_FingerprintTest
{
public:

	Ts_ConsecutivePixels(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_ConsecutivePixels();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

