#pragma once

#include "Syn_FingerprintTest.h"

class Ts_SCM_WOFWithoutStimulus :public Syn_FingerprintTest
{
public:
	Ts_SCM_WOFWithoutStimulus(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_SCM_WOFWithoutStimulus();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	void SCM_WofTestExecute();

	void SCM_WofTestProcessData();

	void GetOtpScmWofData(uint8_t pOtpData[MS0_SIZE], int OtpScmWofDataCount);
};

