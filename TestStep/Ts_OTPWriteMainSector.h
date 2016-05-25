#pragma once
#include "Syn_FingerprintTest.h"

class Ts_OTPWriteMainSector : public Syn_FingerprintTest
{
public:
	Ts_OTPWriteMainSector(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_OTPWriteMainSector();

	virtual void	SetUp();

	virtual void	Execute();

	virtual void	ProcessData();

	virtual void	CleanUp();

	void BurnToOTP(long nRecType, uint8_t* pSrc, int numBytes);

	bool RegCheckBitSet();

	bool GetMtAndConfigPartNumbers(MtAndConfigPnInfo* pInfo);

	bool GetConfigFileArray(string sConfigFileName, uint8_t *pConfigFilerArray, int arrSize = MAX_PART_NUMBER_LENGTH);

	void TranslatePartNum(const string& sPN, uint8_t* pDst);

	bool FindGainInPrintFile(uint8_t iFirstValue, uint8_t iSecondValue, uint8_t &oGainValue);

private:
	bool bBurnPGA_OOPP;
};

