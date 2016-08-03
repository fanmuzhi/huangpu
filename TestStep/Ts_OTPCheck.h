#pragma once
#include "Syn_FingerprintTest.h"
class Ts_OTPCheck :
	public Syn_FingerprintTest
{
public:
	Ts_OTPCheck(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);
	virtual ~Ts_OTPCheck();

	virtual void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	string HexToString(uint8_t arrValue[], int stratPos, int endPos);

	bool GetConfigFileArray(string sConfigFileName, uint8_t *pConfigFilerArray, int arrSize = MAX_PART_NUMBER_LENGTH);

	void TranslatePartNum(const string& sPN, uint8_t* pDst);

private:
	bool bPass;
};