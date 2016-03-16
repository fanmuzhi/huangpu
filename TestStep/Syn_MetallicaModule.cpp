#include "Syn_MetallicaModule.h"

Syn_MetallicaModule::Syn_MetallicaModule()
{
}


Syn_MetallicaModule::~Syn_MetallicaModule()
{
}

void Syn_MetallicaModule::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	//For Metallica sensors, we copy once into Print Patch.
	memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
}

//bool Syn_MetallicaModule::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
//{
//	//FPSFrame*			arFrames = ArrAayFPSFrame;//site.m_acquireFpsResults.m_arImagesWithoutStimulus;
//	//FPSFrame*			arFrames = calResult.arr_calibration;//site.m_acquireFpsResults.m_arImagesWithoutStimulus;
//	FPSFrame*			arFrames = new FPSFrame[MAXFRAMES];
//	//CalibrationInfo*	pCalInfo = &calInfo;
//	//CalibrationResults* pCalResults = &calResult;
//	int					nNumRows = numRows;
//	int					nNumCols = numCols;
//	int					nPgaIdx = calInfo.m_nPgaIdx;
//	FPSFrame			calFrameZeroOffsets;
//	FPSFrame			calFrameNonZeroOffsets;
//	uint8_t*			pOffsetArr = &(calResult.m_pPrintPatch[nPgaIdx]);
//	float				nOffset;
//	float				nRatio;
//	float				nDistance;
//	float				nSetRatio = calInfo.m_nPgaOffsetRatio;
//	int					nNumFrames = calInfo.m_nNumPgaSamples;
//	int					nStage2SizeCounter = 0;
//	uint16_t			nTotal;
//	int					nOffsetDelta = 60;
//
//	//Get user-specified number of images with the PGA offsets set to 0 (no offset).
//	for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
//		GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols);
//	//Get the average.
//	for (int nRow = 0; nRow<nNumRows; nRow++)
//	{
//		for (int nCol = 0; nCol<nNumCols; nCol++)
//		{
//			nTotal = 0;
//			for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
//				nTotal += arFrames[nFrame].arr[nRow][nCol];
//
//			calFrameZeroOffsets.arr[nRow][nCol] = (uint8_t)(nTotal / nNumFrames);
//		}
//	}
//
//	//Add specified delta to offsets
//	for (int i = 0; i<nNumRows * (nNumCols - HEADER); i++)
//		pOffsetArr[i] = pOffsetArr[i] + nOffsetDelta;
//
//	//Get user-specified number of images with the PGA offsets set according to config. file.
//	for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
//		GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols);
//	//Get the average.
//	for (int nRow = 0; nRow<nNumRows; nRow++)
//	{
//		for (int nCol = 0; nCol<nNumCols; nCol++)
//		{
//			nTotal = 0;
//			for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
//				nTotal += arFrames[nFrame].arr[nRow][nCol];
//
//			calFrameNonZeroOffsets.arr[nRow][nCol] = (uint8_t)(nTotal / nNumFrames);
//		}
//	}
//
//	//Save these values for writing to the OTP and the LOG.
//	int	arRowNums[NUM_PGA_OOPP_OTP_ROWS] = { PGA_OOPP_OTP_ROW1, PGA_OOPP_OTP_ROW2, PGA_OOPP_OTP_ROW3, PGA_OOPP_OTP_ROW4, PGA_OOPP_OTP_ROW5 };
//
//	//New method of selecting rows, NOTE: Metallica auto-offsets all the time.
//	for (int i = 0; i < NUM_PGA_OOPP_OTP_ROWS; i++)
//		arRowNums[i] = ((i + 1)*(nNumRows / 6)) - 1;
//
//	uint8_t* pBuf = calResult.m_arPgaOffsets;
//	for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
//	{
//		int nRow = arRowNums[nSelRowIdx];
//		for (int nCol = 8; nCol < nNumCols; nCol++)
//		{
//			if (calInfo.m_bPgaFineTuning)
//				nRatio = (calFrameZeroOffsets.arr[nRow][nCol] - calFrameNonZeroOffsets.arr[nRow][nCol]) / (float)nOffsetDelta;
//			else
//				nRatio = calInfo.m_nPgaOffsetRatio;
//
//			nRatio = (nRatio < 0.1 || nRatio > 1.5) ? nSetRatio : nRatio;
//			nDistance = ((float)(calFrameZeroOffsets.arr[nRow][nCol]) - 128);
//			nOffset = nDistance / nRatio;
//			if (nOffset < -128)
//				nOffset = -128;
//			if (nOffset > 127)
//				nOffset = 127;
//			*pBuf++ = (uint8_t)nOffset;
//		}
//	}
//	/////////////////////   ///////////////////////////   ////////////////////////   ///////////////////////
//	for (int nBigCol = 8; nBigCol<nNumCols; nBigCol = nBigCol + 4)
//	{
//		for (int nRow = 0; nRow<nNumRows; nRow++)
//		{
//			for (int nCol = 0; nCol<4; nCol++)
//			{
//				if (calInfo.m_bPgaFineTuning)
//					nRatio = (calFrameZeroOffsets.arr[nRow][nBigCol + nCol] - calFrameNonZeroOffsets.arr[nRow][nBigCol + nCol]) / (float)nOffsetDelta;
//				else
//					nRatio = calInfo.m_nPgaOffsetRatio;
//
//				nRatio = (nRatio < 0.1 || nRatio > 1.5) ? nSetRatio : nRatio;
//				nDistance = ((float)(calFrameZeroOffsets.arr[nRow][nBigCol + nCol]) - 128);
//				nOffset = nDistance / nRatio;
//				if (nOffset < -128)
//					nOffset = -128;
//				if (nOffset > 127)
//					nOffset = 127;
//				*pOffsetArr++ = (uint8_t)nOffset;
//			}
//		}
//	}
//
//	//Check if Stage 2 array is all the same.
//	bool bStage2AllEqual = true;
//	pOffsetArr = &calResult.m_pPrintPatch[nPgaIdx];
//	for (int i = 1; i<(nNumRows * (nNumCols - 8)); i++)
//	{
//		if (pOffsetArr[i - 1] != pOffsetArr[i])
//			bStage2AllEqual = false;
//	}
//
//	delete[] arFrames;
//	arFrames = NULL;
//
//	return !bStage2AllEqual;
//}

bool Syn_MetallicaModule::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
{
	FPSFrame * arFrames = new FPSFrame[MAXFRAMES];
	int nNumRows = numRows;
	int nNumCols = numCols;
	int nPgaIdx = calInfo.m_nPgaIdx;
	FPSFrame calFrameZeroOffsets;
	FPSFrame calFrameNonZeroOffsets;
	float nConfigRatio = calInfo.m_nPgaOffsetRatio;
	int nNumFrames = calInfo.m_nNumPgaSamples;
	int16_t nTotal;
	int arRowNums[NUM_PGA_OOPP_OTP_ROWS];
	int8_t* pTempOffsets = new int8_t[nNumRows * nNumCols];
	int8_t* pTmp;
	int8_t* pPrtFileOffsets;
	int8_t* pOtpOffsets;

	//These are the rows we save to OTP for calculating the variance score.
	for (int i = 0; i < NUM_PGA_OOPP_OTP_ROWS; i++)
		arRowNums[i] = ((i + 1)*(nNumRows / 6)) - 1;

	//Get user-specified number of images with the PGA offsets set to 0 (no offset), then
	//calculate the average.
	for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
		GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols);
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = 0; nCol<nNumCols; nCol++)
		{
			nTotal = 0;
			for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
				nTotal += (int16_t)arFrames[nFrame].arr[nRow][nCol];
			calFrameZeroOffsets.arr[nRow][nCol] = nTotal / nNumFrames;
		}
	}

	//Calculate the PGA offsets (no fine tuning).
	pTmp = pTempOffsets;
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = HEADER; nCol<nNumCols; nCol++)
			*pTmp++ = CalcPgaOffset(calFrameZeroOffsets.arr[nRow][nCol], nConfigRatio, nConfigRatio);
	}

	//Put the PGA offsets into the print file. The ordering is a bit strange.
	pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
	for (int nBigCol = 0; nBigCol<(nNumCols - HEADER); nBigCol = nBigCol + 4)
	{
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nColIdx = 0; nColIdx<4; nColIdx++)
				*pPrtFileOffsets++ = pTempOffsets[(nRow * (nNumCols - HEADER)) + (nBigCol + nColIdx)];
		}
	}

	//Save selected rows of PGA offsets for OTP (used in variance score) and log.
	pOtpOffsets = (int8_t*)calResult.m_arPgaOffsets;
	for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
	{
		for (int nCol = 0; nCol < nNumCols - HEADER; nCol++)
			*pOtpOffsets++ = pTempOffsets[(arRowNums[nSelRowIdx] * (nNumCols - HEADER)) + nCol];
	}

	if (calInfo.m_bPgaFineTuning)
	{
		//Get user-specified number of images with new PGA offsets, then calculate the average.
		for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
			GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols);
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nCol = 0; nCol<nNumCols; nCol++)
			{
				nTotal = 0;
				for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
					nTotal += (int16_t)arFrames[nFrame].arr[nRow][nCol];
				calFrameNonZeroOffsets.arr[nRow][nCol] = nTotal / nNumFrames;
			}
		}

		//Put the fine tuned PGA offsets into the print file. The order is a bit strange.
		pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
		pOtpOffsets = (int8_t*)calResult.m_arPgaOffsets;
		for (int nBigCol = HEADER; nBigCol<nNumCols; nBigCol = nBigCol + 4)
		{
			for (int nRow = 0; nRow<nNumRows; nRow++)
			{
				for (int nColIdx = 0; nColIdx<4; nColIdx++)
				{
					//Calculate fine tuned PGA offset.
					int8_t nAdjustment = CalcPgaOffset(calFrameNonZeroOffsets.arr[nRow][nBigCol + nColIdx], nConfigRatio, nConfigRatio);
					if (((float)(*pTmp) + (float)nAdjustment) > 127)
						*pPrtFileOffsets = 127;
					else if (((float)(*pTmp) + (float)nAdjustment) < -128)
						*pPrtFileOffsets = -128;
					else
						*pPrtFileOffsets += nAdjustment;
					pOtpOffsets[nRow + (nBigCol + nColIdx)] = *pPrtFileOffsets++;
				}
			}
		}

		//Save selected rows of PGA offsets for OTP (used in variance score) and log.
		pOtpOffsets = (int8_t*)calResult.m_arPgaOffsets;
		for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
		{
			int nRow = arRowNums[nSelRowIdx];
			for (int nCol = 0; nCol < nNumCols - HEADER; nCol++)
				*pOtpOffsets++ = pTempOffsets[(nRow * (nNumCols - HEADER)) + nCol];
		}
	}

	//Check if Stage 2 array is all the same.
	bool bStage2AllEqual = true;
	pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
	for (int i = 1; i<(nNumRows * (nNumCols - HEADER)); i++)
	{
		if (pPrtFileOffsets[i - 1] != pPrtFileOffsets[i])
			bStage2AllEqual = false;
	}

	delete[] pTempOffsets;
	return !bStage2AllEqual;
}