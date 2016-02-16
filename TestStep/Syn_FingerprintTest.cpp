//Local
#include "Syn_FingerprintTest.h"

//windows
#include "windows.h" 

Syn_FingerprintTest::Syn_FingerprintTest(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_TestStep(strName, pDutCtrl, pDut)
, _pSyn_Module(NULL)
{
	switch (_pSyn_Dut->_eProjectType)
	{
		case Metallica:
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;

		case Viper1:
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;
			
		case Viper2:
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;

		default:
			_pSyn_Module = new Syn_MetallicaModule();
			_pSyn_Module->SetDutCtrl(_pSyn_DutCtrl);
			break;		
	}
}

Syn_FingerprintTest::~Syn_FingerprintTest()
{
	if (NULL != _pSyn_Module)
	{
		delete _pSyn_Module;
		_pSyn_Module = NULL;
	}
}

void Syn_FingerprintTest::PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->PowerOn(nPwrVdd, nPwrVio, nPwrVled, nPwrVddh, bDisableSleep);
}

void Syn_FingerprintTest::PowerOff()
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->PowerOff();
}

void Syn_FingerprintTest::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->CopyToPrintPatch(pSrc, pPrintPatch, nNumBytes, nPatchIdx);
}

bool Syn_FingerprintTest::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
{
	if (NULL == _pSyn_Module)
	{
		return false;
	}

	return _pSyn_Module->CalculatePgaOffsets_OOPP(numCols, numRows, calInfo, calResult);
}

void Syn_FingerprintTest::GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols)
{
	if (NULL == _pSyn_Module)
	{
		return;
	}

	_pSyn_Module->GetFingerprintImage(pCalResults, pFrame, nNumRows, nNumCols);
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