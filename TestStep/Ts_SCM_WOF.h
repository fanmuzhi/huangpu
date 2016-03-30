#pragma once

#include "Syn_FingerprintTest.h"

class Ts_SCM_WOF :public Syn_FingerprintTest
{
public:
	Ts_SCM_WOF(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SCM_WOF();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	virtual void SCM_WofTestExecute();//for SCM_WOF

	virtual void SCM_WofTestProcessData();//for SCM_WOF

	virtual void GetOtpScmWofData(uint8_t pOtpData[MS0_SIZE], int OtpScmWofDataCount);//for SCM_WOF
};

