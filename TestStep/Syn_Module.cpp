//Local
#include "Syn_Module.h"

//
#include "Syn_Exception.h"

//windows API
#include "windows.h"

Syn_Module::Syn_Module()
:_pSyn_DutCtrl(NULL)
{
}


Syn_Module::~Syn_Module()
{
}

void Syn_Module::SetDutCtrl(Syn_DutCtrl * &pDutCtrl)
{
	_pSyn_DutCtrl = pDutCtrl;
}

void Syn_Module::PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep)
{
	if (NULL == _pSyn_DutCtrl)
	{
		//LOG(ERROR) << "_pSyn_DutCtrl is NULL!";
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
		_pSyn_DutCtrl->FpWaitForCMDComplete();
		_pSyn_DutCtrl->FpReadAndCheckStatus(0);
	}
}

void Syn_Module::PowerOff()
{
	try
	{
		_pSyn_DutCtrl->SetVoltages(0, 0, 0, 0);
		::Sleep(50);
	}
	catch (...)
	{
	}
}

void Syn_Module::GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols)
{
	if (NULL == _pSyn_DutCtrl)
	{
		return;
	}

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

void Syn_Module::GetFingerprintImageForCurrentTest(CalibrationResults& pCalResults, FPSFrame* pFrame, int nNumRows, int nNumCols, uint32_t* pCurrentDrawVals, int nGain)
{
	uint32_t	currentDrawSums[NUM_CURRENT_VALUES] = { 0 };
	uint32_t	arCurrentSenseVals[NUM_CURRENT_VALUES];
	int			nRows = nNumRows;
	int			nCols = nNumCols;

	if (NULL == pCurrentDrawVals)
	{
		return;
	}

	//There is a bug in the FPS firmware. This bug shows when the number of cols times the
	//number of rows is a multiple of 64. To avoid this problem we add 1 to the number of rows
	//when the number of bytes in the image is a multiple of 64.
	if (((nRows * nCols) % 64) == 0)
		nRows++;

	//If the caller is requesting current draw readings.
	//WritePrintFile(pCalResults);
	_pSyn_DutCtrl->FpWrite(1, 2, (uint8_t*)pCalResults.m_pPrintPatch, pCalResults.m_nPrintPatchSize);
	_pSyn_DutCtrl->FpWaitForCMDComplete();

	for (int i = 0; i<2; i++)
	{
		//Get the current draw reading while the part is active.
		_pSyn_DutCtrl->GetCurrentSenseValues(nGain, 64, arCurrentSenseVals);

		for (int curIdx = 0; curIdx<NUM_CURRENT_VALUES; curIdx++)
			currentDrawSums[curIdx] += arCurrentSenseVals[curIdx];
	}

	for (int curIdx = 0; curIdx<NUM_CURRENT_VALUES; curIdx++)
		pCurrentDrawVals[curIdx] = currentDrawSums[curIdx] / 2;


	//Wait for the sensor to generate complete image.
	int timeout = 300;
	uint8_t		pDst[4];
	_pSyn_DutCtrl->FpGetStatus(pDst, sizeof(pDst));
	while (timeout-- && ((pDst[3] & 0x02) == 0))
		_pSyn_DutCtrl->FpGetStatus(pDst, sizeof(pDst));

	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("Timeout waiting for image.");
		throw ex;
	}

	//Pull sensor's image data.
	uint8_t* pImgBuf = new uint8_t[(nRows * nCols)];
	_pSyn_DutCtrl->FpGetImage(pImgBuf, nRows * nCols);
	delete[] pImgBuf;
	pImgBuf = NULL;
}


void Syn_Module::CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults)
{
	if (NULL == pFrame)
	{
		//LOG(ERROR) << "Syn_Module::CalculateLnaOffsetsBinarySearch() - pFrame is null!";
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

		CopyToPrintPatch(arMid, CalResults.m_pPrintPatch, nNumRows, CalInfo.m_nLnaIdx);

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

void Syn_Module::GetRowAverages(FPSFrame* pFrame, int nColBegin, int nColEnd, uint8_t* pAverages, int nNumRows)
{
	if (NULL == pFrame)
	{
		//LOG(ERROR) << "Syn_Module::GetRowAverages() - pFrame is null!";
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

bool Syn_Module::CheckDUTexists()
{
	bool IsExists(true);
	_pSyn_DutCtrl->GpioSetPinType(8, 0x10, 5);

	uint32_t nPinState = 0;
	_pSyn_DutCtrl->GpioPinRead(8, 0x10, &nPinState);

	if (0x10 & nPinState)
	{
		IsExists = false;
	}

	return IsExists;
}


void Syn_Module::ImageDecode(FPSFrame *pDecodedFrame, FPSFrame *pEncodedFrame, int nNumRow, int nNumCol, int nNumFrames)
{
	int layer;
	int row,col;
	int kernelM=144;
	float sumElements = 0;	

	for(layer = 0; layer < nNumFrames; layer++)
	{
		//copy data in Finger data into pFrameWith
		for(row = 0; row < nNumRow; row++)
		{
			for(col = 0; col < nNumCol; col++)
			{
				pDecodedFrame[layer].arr[row][col] = pEncodedFrame[layer].arr[row][col];
			}
		}
	}
}
