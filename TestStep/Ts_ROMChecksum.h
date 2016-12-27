#pragma once
#include "Syn_FingerprintTest.h"

class Ts_ROMChecksum : public Syn_FingerprintTest
{
public:
	Ts_ROMChecksum(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	~Ts_ROMChecksum();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();
};

