#pragma once
#include "Syn_FingerprintTest.h"
class Ts_DeepSleepCurrent : public Syn_FingerprintTest
{
public:
	Ts_DeepSleepCurrent(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	~Ts_DeepSleepCurrent();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

