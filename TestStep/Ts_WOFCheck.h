#pragma once

#include "Syn_FingerprintTest.h"

class Ts_WOFCheck : public Syn_FingerprintTest
{
public:
	Ts_WOFCheck(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WOFCheck();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

private:
	bool CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx);

	bool bPass;
};