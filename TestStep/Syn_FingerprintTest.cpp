//Local
#include "Syn_FingerprintTest.h"

//windows
#include "windows.h" 

Syn_FingerprintTest::Syn_FingerprintTest(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut, Syn_Module * &pSyn_Module)
:Syn_TestStep(strName, pDutCtrl, pDut, pSyn_Module)
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

void Syn_FingerprintTest::CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults)
{
	if (NULL == pFrame)
	{
		//LOG(ERROR) << "Syn_Dut::CalculateLnaOffsetsBinarySearch() - pFrame is null!";
		return;
	}

	uint8_t arBestDiff[MAXROW];
	uint8_t arMid[MAXROW] = { 0 };
	uint8_t arHi[MAXROW] = { 0 };
	uint8_t arLo[MAXROW] = { 0 };
	uint8_t row_avgs[MAXROW];
	int		cal_colbegin = HEADER + 1;
	int		cal_colend = nNumCols - 1;

	//Initialize arrays that will ,hold place of indices for binary searching
	for (int i = 0; i<nNumRows; i++)
	{
		arBestDiff[i] = 0xFF;
		arLo[i] = CalInfo.m_nLnaOffsetLow;
		arHi[i] = CalInfo.m_nLnaOffsetHigh + 1;
	}

	//Calculate the number of iterations to find best offset
	int nNumIterations = 0;
	int nSpread = arHi[0] - arLo[0];
	for (int i = 1; i <= 8; i++)
	{
		if (nSpread & 0x01)
			nNumIterations = i;
		nSpread = nSpread >> 1;
	}

	//Use binary search to find best LNA offsets between high and low.
	for (int nLoopCnt = 0; nLoopCnt < nNumIterations; nLoopCnt++)
	{
		//Prepare print-file with latest LNA offsets.
		for (int i = 0; i<nNumRows; i++)
			arMid[i] = ((arHi[i] - arLo[i]) / 2) + arLo[i];
		//CopyToPrintPatch(arMid, CalResults.m_pPrintPatch, nNumRows, CalInfo.m_nLnaIdx);
		_pSyn_Module->CopyToPrintPatch(arMid, CalResults.m_pPrintPatch, nNumRows, CalInfo.m_nLnaIdx);

		GetFingerprintImage(CalResults, pFrame, nNumRows, nNumCols);
		GetRowAverages(pFrame, cal_colbegin, cal_colend, row_avgs, nNumRows);

		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			//Determine if the row average is greater or less than 128
			if (row_avgs[nRow] >= 128)
			{
				arLo[nRow] = arMid[nRow];
				if ((row_avgs[nRow] - 128) < arBestDiff[nRow])
				{
					arBestDiff[nRow] = row_avgs[nRow] - 128;
					pLnaValues[nRow] = arMid[nRow];
				}
			}
			else //if (row_avgs[nRow] < 128)
			{
				arHi[nRow] = arMid[nRow];
				if ((128 - row_avgs[nRow]) < arBestDiff[nRow])
				{
					arBestDiff[nRow] = 128 - row_avgs[nRow];
					pLnaValues[nRow] = arMid[nRow]; // - hint : log file should display pLnaValues array.
				}
			}
		}
	}
	memcpy(pLnaValues, arMid, nNumRows);
}

void Syn_FingerprintTest::GetRowAverages(FPSFrame* pFrame, int nColBegin, int nColEnd, uint8_t* pAverages, int nNumRows)
{
	if (NULL == pFrame)
	{
		//LOG(ERROR) << "Syn_Dut::GetRowAverages() - pFrame is null!";
		return;
	}

	int nRow; int nCol;
	int temp;

	for (nRow = 0; nRow<nNumRows; nRow++)
	{
		temp = 0;
		for (nCol = nColBegin; nCol<nColEnd; nCol++)
			temp += pFrame->arr[nRow][nCol];

		temp = temp / (nColEnd - nColBegin + 1);
		pAverages[nRow] = temp;
	}
}