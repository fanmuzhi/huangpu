#pragma once

//Local
#include "Syn_FingerprintTest.h"

class Ts_Calibrate :public Syn_FingerprintTest
{
public:

	enum { kPgaCalTypeOneOffsetPerRow, kPgaCalTypeOneOffsetPerPixel };

	Ts_Calibrate(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_Calibrate();


	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	bool VerifyPixelRanges();

	int32_t OtpPgaVarianceTest(int8_t* pOtpPgaOffsets, int8_t* pCurPgaOffsets);
};
