#pragma once
#include "Syn_FingerprintTest.h"

class Ts_WaitStimulus :	public Syn_FingerprintTest
{
public:
	Ts_WaitStimulus(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WaitStimulus();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

