#pragma once

#include "Syn_FingerprintTest.h"

class Ts_WOF : public Syn_FingerprintTest
{
public:

	Ts_WOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WOF();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	void ExecuteWofTest();

	void WofTestProcessData();

	void GetOtpWofData(uint8_t pOtpData[MS0_SIZE],int OtpWofDataCount);
};

