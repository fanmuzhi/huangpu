#pragma once
#include "Syn_FingerprintTest.h"

class Ts_Imperfections : public Syn_FingerprintTest
{
public:
	Ts_Imperfections(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_Imperfections();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

