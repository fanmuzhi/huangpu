#pragma once

//Local
#include "Syn_FingerprintTest.h"

class Ts_Calibrate :public Syn_FingerprintTest
{
public:

	enum { kPgaCalTypeOneOffsetPerRow, kPgaCalTypeOneOffsetPerPixel };

	Ts_Calibrate(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_Calibrate();


	virtual void	SetUp();

	virtual void	Excute();

	virtual void	ProcessData();

	virtual void	CleanUp();
};

