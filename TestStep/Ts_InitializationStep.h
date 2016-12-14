#pragma once
#include "Syn_FingerprintTest.h"

class Ts_InitializationStep : public Syn_FingerprintTest
{
public:
	Ts_InitializationStep(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_InitializationStep();

	bool CheckMPCVoltages();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

protected:

	void Create_SN(uint8_t* SN, uint32_t nDutdSerNum, int nSiteNum, int nTestLocationId);

	void SerialNumReverseBitFields(uint8_t* pSrc, uint8_t* pDst);
};

