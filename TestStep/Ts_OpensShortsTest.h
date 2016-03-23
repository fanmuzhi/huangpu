#pragma once

#include "Syn_FingerprintTest.h"

class Ts_OpensShortsTest : public Syn_FingerprintTest
{
public:
	Ts_OpensShortsTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_OpensShortsTest();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

