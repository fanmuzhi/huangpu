#pragma once
#include "Syn_FingerprintTest.h"

class Ts_RxStandardDev : public Syn_FingerprintTest
{
public:
	Ts_RxStandardDev(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_RxStandardDev();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	float find_mean(float* arr, int S_COL);

	float std_dev(float pData[], int size);

};

