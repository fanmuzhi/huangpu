#pragma once
#include "Syn_FingerprintTest.h"

class Ts_AcqImgNoFinger : public Syn_FingerprintTest
{
public:
	Ts_AcqImgNoFinger(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_AcqImgNoFinger();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

};