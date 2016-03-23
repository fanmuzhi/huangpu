#pragma once

#include "Syn_FingerprintTest.h"

class Ts_RAMTest : public Syn_FingerprintTest
{
public:
	Ts_RAMTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_RAMTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
	
	void LoadRAMPatch(std::string sPatchName, uint8_t* pPatchResults, uint16_t nSize);
};

