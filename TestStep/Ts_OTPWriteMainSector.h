#pragma once
#include "Syn_FingerprintTest.h"

class Ts_OTPWriteMainSector : public Syn_FingerprintTest
{
public:
	Ts_OTPWriteMainSector(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_OTPWriteMainSector();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};
