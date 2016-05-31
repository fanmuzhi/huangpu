//Local
#include "Syn_Module.h"

//
#include "Syn_Exception.h"

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

	/*for (int i = 0; i < nNumRows; i++)
	{
		std::string strTempRowValue;
		for (int j = HEADER; j < nNumCols; j++)		// HEADER defined the first 8 cols to ignore.
		{
			//strTempRowValue += to_string(pImgBuff[i*nRows + j]) + std::string(",");
			strTempRowValue += to_string(pFrame->arr[i][j]) + std::string(",");
		}
	}*/

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
	_pSyn_DutCtrl->FpWritePrintFile((uint8_t*)pCalResults.m_pPrintPatch, pCalResults.m_nPrintPatchSize);
	//_pSyn_DutCtrl->FpWrite(1, 2, (uint8_t*)pCalResults.m_pPrintPatch, pCalResults.m_nPrintPatchSize);
	//_pSyn_DutCtrl->FpWaitForCMDComplete();

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
	_pSyn_DutCtrl->FpWaitForCMDComplete();

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

int8_t Syn_Module::CalcPgaOffset(uint8_t nPixelValue, float nRatio, float nConfigRatio)
{
	float nOffset;
	float nDelta;

	//      nRatio = (nRatio < 0.5 || nRatio > 3) ? nConfigRatio : nRatio;
	nDelta = (float)nPixelValue - (float)128;
	nOffset = (float)nDelta / nRatio;

	if (nOffset > 127)
		nOffset = 127;

	if (nOffset < -128)
		nOffset = -128;

	return (int8_t)nOffset;
}


//void Syn_Module::FpWriteOscTrim(uint32_t nOscTrimValue)
//{
//	uint8_t pDataSector0[2] = { 0, 1 };
//	uint8_t pDataSector1[2] = { 1, 1 };
//	uint8_t arrOscTrimVal[2] = { ((nOscTrimValue & 0xFFFFF) >> 8) & 0xFF, ((nOscTrimValue & 0xFFFFF)) & 0xFF };
//	uint8_t arSiOscTrimValid[2] = { 0x0, 0x10 };
//	uint8_t arOscTrimWrite[68] = { 0 };
//	uint8_t	arBootSector0[BS0_SIZE];
//	for (int i = 8; i<sizeof(arOscTrimWrite); i = i + 8)
//		arOscTrimWrite[i] = arOscTrimWrite[i + 1] = arOscTrimWrite[i + 2] = arOscTrimWrite[i + 3] = 255;

//	//set upt the array to write.
//	arOscTrimWrite[1] = 0x1;
//	arOscTrimWrite[2] = 0xA;
//	arOscTrimWrite[20] = arOscTrimWrite[28] = arrOscTrimVal[0];
//	arOscTrimWrite[21] = arOscTrimWrite[29] = arrOscTrimVal[1];
//	arOscTrimWrite[22] = arOscTrimWrite[30] = arSiOscTrimValid[0];
//	arOscTrimWrite[23] = arOscTrimWrite[31] = arSiOscTrimValid[1];

//	//Modify boot sector to set SiOsc TC Trim Valid Secure bit
//	/*_pSyn_DutCtrl->FpUnloadPatch();
//	_pSyn_DutCtrl->FpLoadPatch(site.m_initResults.pModuleTestPatchPtr, site.m_initResults.nModuleTestPatchSize);*/
//	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, arBootSector0, BS0_SIZE);
//	arBootSector0[19] |= 0x20;
//	arBootSector0[27] |= 0x20;
//	WriteBootSector(0, arBootSector0, NULL);

//	//send test patch
//	/*GetDutCtrl().FpUnloadPatch();
//	GetDutCtrl().FpLoadPatch(site.m_initResults.pModuleTestPatchPtr, site.m_initResults.nModuleTestPatchSize);
//	//Work-around for av-reg brown-out 
//	if (GetSysConfig().GetSize("ImageAcqPatch") != 0)
//		WriteImageAcqPatch(GetSysConfig().GetPtr("ImageAcqPatch"), GetSysConfig().GetSize("ImageAcqPatch"));*/

//	//Write Trim Value, sector 0
//	_pSyn_DutCtrl->FpWrite(1, 0x3B, pDataSector0, 2);
//	_pSyn_DutCtrl->FpWaitForCMDComplete(2);

//	//Write Trim Value, sector 1
//	_pSyn_DutCtrl->FpWrite(1, 0x3B, pDataSector1, 2);
//	_pSyn_DutCtrl->FpWaitForCMDComplete(10);

//	// VCSFW_CMD_TEST_OTP_WRITE_EX 
//	_pSyn_DutCtrl->FpWrite(1, 0x3C, arOscTrimWrite, sizeof(arOscTrimWrite));
//	::Sleep(50);
//	_pSyn_DutCtrl->FpWaitForCMDComplete(2);

//	//Cycle power at the end of write.
//	//CycleDutPower(GetSysConfig().GetPwrVdd(), GetSysConfig().GetPwrVio(),GetSysConfig().GetPwrVled(), GetSysConfig().GetPwrVddh(), true);
//}

//void Syn_Module::WriteBootSector(int nSection, uint8_t* pOtpBootSector, uint8_t* pBootSectorMask)
//{
//	uint8_t		arBootSectorWithCmd[BS0_SIZE + 4] = { 0 };
//	uint8_t		pSwapped[BS0_SIZE];
//	int			arrIncrement[4] = { 3, 1, -1, -3 };

//	//Put in 4 bytes of command-specific data.
//	arBootSectorWithCmd[0] = nSection;
//	arBootSectorWithCmd[1] = 1;
//	arBootSectorWithCmd[2] = 10;
//	arBootSectorWithCmd[3] = 0;
//	memcpy(&arBootSectorWithCmd[4], pOtpBootSector, BS0_SIZE);

//	if (pBootSectorMask != NULL)
//	{
//		//First, swap the bytes in the mask.
//		for (int i = 0; i < BS0_SIZE; i++)
//			pSwapped[i] = pBootSectorMask[i + arrIncrement[i % 4]];

//		//Remember, the boot sectors are OTP. For the even long words, we can only write a
//		//0 to a 1. For the odd long words, we can only write a 1 to a 0.
//		for (int i = 0; i<BS0_SIZE; i++)
//		{
//			//If this byte belongs to an odd long word.
//			if ((i / 4) & 0x01)
//				arBootSectorWithCmd[i + 4] = pSwapped[i] & pOtpBootSector[i];
//			else
//				arBootSectorWithCmd[i + 4] = pSwapped[i] | pOtpBootSector[i];
//		}
//	}

//	//Write the boot sector.  
//	_pSyn_DutCtrl->FpWrite(1, 0x3C, arBootSectorWithCmd, BS0_SIZE + 4);
//	_pSyn_DutCtrl->FpWaitForCMDComplete(2);
//}

//void Syn_Module::FpWriteSlowOscFreq(uint32_t nHvOscTrim, uint32_t nHvOscBias)
//{
//	uint8_t arBs0[BS0_SIZE] = { 0 };

//	//Read Boot Sector 0
//	/*GetDutCtrl().FpUnloadPatch();
//	GetDutCtrl().FpLoadPatch(site.m_initResults.pModuleTestPatchPtr, site.m_initResults.nModuleTestPatchSize);*/
//	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, arBs0, BS0_SIZE);

//	//Write HvOsc Trim and Bias values to BS0. 0x80 = Valid Trim, 0x40 = Valid Bias.
//	arBs0[18] = arBs0[26] = arBs0[18] | ((nHvOscTrim << 7) | (nHvOscBias << 4));
//	arBs0[19] = arBs0[27] = arBs0[19] | (0x0F & (nHvOscTrim >> 1)) | (0x80 | 0x40);
//	_pSyn_DutCtrl->FpOtpRomWrite(BOOT_SEC, 0, arBs0, BS0_SIZE);

//	//Read Boot Sector 0
//	/*GetDutCtrl().FpUnloadPatch();
//	GetDutCtrl().FpLoadPatch(site.m_initResults.pModuleTestPatchPtr, site.m_initResults.nModuleTestPatchSize);*/
//	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, arBs0, BS0_SIZE);
//}