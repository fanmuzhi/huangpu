//Local
#include "Syn_FingerprintTest.h"

//windows
#include "windows.h" 

Syn_FingerprintTest::Syn_FingerprintTest(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_TestStep(strName, pDutCtrl, pDut)
{
}

Syn_FingerprintTest::~Syn_FingerprintTest()
{
}

void Syn_FingerprintTest::PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep)
{
	if (NULL == _pSyn_DutCtrl)
	{
		return;
	}

	_pSyn_DutCtrl->SetVoltages(nPwrVdd, nPwrVio, nPwrVled, nPwrVddh);
	::Sleep(50);

	//If requested by the caller, disable the sensor's sleep feature.
	if (bDisableSleep)
	{
		//Wake sensor from sleep mode.
		_pSyn_DutCtrl->FpWaitDeviceReady();
		//Configure sensor not to go back to sleep.
		_pSyn_DutCtrl->FpDisableSleep();
	}
}

void Syn_FingerprintTest::PowerOff()
{
	if (NULL == _pSyn_DutCtrl)
	{
		return;
	}

	try
	{
		_pSyn_DutCtrl->SetVoltages(0, 0, 0, 0);
		::Sleep(50);
	}
	catch (...)
	{
	}
}

void Syn_FingerprintTest::GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols)
{
	int			nRows = nNumRows;
	int			nCols = nNumCols;

	if (((nRows * nCols) % 64) == 0)
		nRows++;

	uint8_t *pImgBuff = new uint8_t[nCols * nRows];

	//_pSyn_DutCtrl->FpGetImage(pImgBuff, numCols*numRows);
	_pSyn_DutCtrl->FpGetImage2(nRows, nCols, pImgBuff, pCalResults.m_nPrintPatchSize, pCalResults.m_pPrintPatch);


	for (int i = 0; i < (nRows * nCols); i++)
	{
		pFrame->arr[i / nCols][i%nCols] = pImgBuff[i];
	}

	//for (int i = 0; i < nRows; i++)
	for (int i = 0; i < nNumRows; i++)
	{
		std::string strTempRowValue;
		for (int j = HEADER; j < nNumCols; j++)		// HEADER defined the first 8 cols to ignore.
		{
			//strTempRowValue += to_string(pImgBuff[i*nRows + j]) + std::string(",");
			strTempRowValue += to_string(pFrame->arr[i][j]) + std::string(",");
		}
	}

	delete[] pImgBuff;
	pImgBuff = NULL;
}