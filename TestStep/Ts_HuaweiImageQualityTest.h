#pragma once

#include "Syn_FingerprintTest.h"

#include "snrtest.h"
//extern "C" __declspec(dllimport) float testSNR(unsigned char* pImagewhite30, unsigned char* pImagespoof30, int width, int height, int iNumofimg);

class Ts_HuaweiImageQualityTest : public Syn_FingerprintTest
{
public:
	Ts_HuaweiImageQualityTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_HuaweiImageQualityTest();


	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();
};

