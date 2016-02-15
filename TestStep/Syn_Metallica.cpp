#include "Syn_Metallica.h"


Syn_Metallica::Syn_Metallica()
{
}


Syn_Metallica::~Syn_Metallica()
{
}

void Syn_Metallica::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	//For Metallica sensors, we copy once into Print Patch.
	memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
}

bool Syn_Metallica::CalculatePgaOffsets_OOPP(Syn_DutCtrl * &pDutCtrl,uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
{
	//FPSFrame*			arFrames = ArrAayFPSFrame;//site.m_acquireFpsResults.m_arImagesWithoutStimulus;
	//FPSFrame*			arFrames = calResult.arr_calibration;//site.m_acquireFpsResults.m_arImagesWithoutStimulus;
	FPSFrame*			arFrames = new FPSFrame[MAXFRAMES];
	//CalibrationInfo*	pCalInfo = &calInfo;
	//CalibrationResults* pCalResults = &calResult;
	int					nNumRows = numRows;
	int					nNumCols = numCols;
	int					nPgaIdx = calInfo.m_nPgaIdx;
	FPSFrame			calFrameZeroOffsets;
	FPSFrame			calFrameNonZeroOffsets;
	uint8_t*			pOffsetArr = &(calResult.m_pPrintPatch[nPgaIdx]);
	float				nOffset;
	float				nRatio;
	float				nDistance;
	float				nSetRatio = calInfo.m_nPgaOffsetRatio;
	int					nNumFrames = calInfo.m_nNumPgaSamples;
	int					nStage2SizeCounter = 0;
	uint16_t			nTotal;
	int					nOffsetDelta = 60;

	//Get user-specified number of images with the PGA offsets set to 0 (no offset).
	for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
		GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols, pDutCtrl);
	//Get the average.
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = 0; nCol<nNumCols; nCol++)
		{
			nTotal = 0;
			for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
				nTotal += arFrames[nFrame].arr[nRow][nCol];

			calFrameZeroOffsets.arr[nRow][nCol] = (uint8_t)(nTotal / nNumFrames);
		}
	}

	//Add specified delta to offsets
	for (int i = 0; i<nNumRows * (nNumCols - HEADER); i++)
		pOffsetArr[i] = pOffsetArr[i] + nOffsetDelta;

	//Get user-specified number of images with the PGA offsets set according to config. file.
	for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
		GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols, pDutCtrl);
	//Get the average.
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = 0; nCol<nNumCols; nCol++)
		{
			nTotal = 0;
			for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
				nTotal += arFrames[nFrame].arr[nRow][nCol];

			calFrameNonZeroOffsets.arr[nRow][nCol] = (uint8_t)(nTotal / nNumFrames);
		}
	}

	//Save these values for writing to the OTP and the LOG.
	int	arRowNums[NUM_PGA_OOPP_OTP_ROWS] = { PGA_OOPP_OTP_ROW1, PGA_OOPP_OTP_ROW2, PGA_OOPP_OTP_ROW3, PGA_OOPP_OTP_ROW4, PGA_OOPP_OTP_ROW5 };

	//New method of selecting rows, NOTE: Metallica auto-offsets all the time.
	for (int i = 0; i < NUM_PGA_OOPP_OTP_ROWS; i++)
		arRowNums[i] = ((i + 1)*(nNumRows / 6)) - 1;

	uint8_t* pBuf = calResult.m_arPgaOffsets;
	for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
	{
		int nRow = arRowNums[nSelRowIdx];
		for (int nCol = 8; nCol < nNumCols; nCol++)
		{
			if (calInfo.m_bPgaFineTuning)
				nRatio = (calFrameZeroOffsets.arr[nRow][nCol] - calFrameNonZeroOffsets.arr[nRow][nCol]) / (float)nOffsetDelta;
			else
				nRatio = calInfo.m_nPgaOffsetRatio;

			nRatio = (nRatio < 0.1 || nRatio > 1.5) ? nSetRatio : nRatio;
			nDistance = ((float)(calFrameZeroOffsets.arr[nRow][nCol]) - 128);
			nOffset = nDistance / nRatio;
			if (nOffset < -128)
				nOffset = -128;
			if (nOffset > 127)
				nOffset = 127;
			*pBuf++ = (uint8_t)nOffset;
		}
	}
	/////////////////////   ///////////////////////////   ////////////////////////   ///////////////////////
	for (int nBigCol = 8; nBigCol<nNumCols; nBigCol = nBigCol + 4)
	{
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nCol = 0; nCol<4; nCol++)
			{
				if (calInfo.m_bPgaFineTuning)
					nRatio = (calFrameZeroOffsets.arr[nRow][nBigCol + nCol] - calFrameNonZeroOffsets.arr[nRow][nBigCol + nCol]) / (float)nOffsetDelta;
				else
					nRatio = calInfo.m_nPgaOffsetRatio;

				nRatio = (nRatio < 0.1 || nRatio > 1.5) ? nSetRatio : nRatio;
				nDistance = ((float)(calFrameZeroOffsets.arr[nRow][nBigCol + nCol]) - 128);
				nOffset = nDistance / nRatio;
				if (nOffset < -128)
					nOffset = -128;
				if (nOffset > 127)
					nOffset = 127;
				*pOffsetArr++ = (uint8_t)nOffset;
			}
		}
	}

	//Check if Stage 2 array is all the same.
	bool bStage2AllEqual = true;
	pOffsetArr = &calResult.m_pPrintPatch[nPgaIdx];
	for (int i = 1; i<(nNumRows * (nNumCols - 8)); i++)
	{
		if (pOffsetArr[i - 1] != pOffsetArr[i])
			bStage2AllEqual = false;
	}

	delete[] arFrames;
	arFrames = NULL;

	return !bStage2AllEqual;
}