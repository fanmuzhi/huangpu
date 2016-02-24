#pragma once
#include "Syn_FingerprintTest.h"
class Ts_OTPCheck :
	public Syn_FingerprintTest
{
public:
	Ts_OTPCheck(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_OTPCheck();

	virtual int	SetUp();

	virtual int	Excute();

	virtual int ProcessData();

	virtual int	CleanUp();
};