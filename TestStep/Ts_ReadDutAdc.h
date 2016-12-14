#pragma once

#include "Syn_FingerprintTest.h"

#define kSetupCmdLength 9
#define kSetupPeekCmdLength 5

class Ts_ReadDutAdc : public Syn_FingerprintTest
{
public:
	Ts_ReadDutAdc(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_ReadDutAdc();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	void ReadDutAdcSetup();

	int ReadDutAdc();

};

