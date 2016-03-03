#pragma once
#include "Syn_FingerprintTest.h"

class Ts_AcqImgFinger : public Syn_FingerprintTest
{
public:
	Ts_AcqImgFinger(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_AcqImgFinger();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

};

