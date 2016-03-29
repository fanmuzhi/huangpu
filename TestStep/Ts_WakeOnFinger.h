#pragma once

#include "Syn_FingerprintTest.h"

class Ts_WakeOnFinger : public Syn_FingerprintTest
{
public:

	Ts_WakeOnFinger(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_WakeOnFinger();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	void ExecuteWofTest();

	void WofTestProcessData();

	void GetOtpWofData(uint8_t pOtpData[MS0_SIZE],int OtpWofDataCount);
};

