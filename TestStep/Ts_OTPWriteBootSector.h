#pragma once
#include "Syn_FingerprintTest.h"

class Ts_OTPWriteBootSector : public Syn_FingerprintTest
{
public:
	Ts_OTPWriteBootSector(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_OTPWriteBootSector();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

protected:

	void Create_SN(uint8_t* SN, uint32_t nDutdSerNum, int nSiteNum, int nTestLocationId);
};

