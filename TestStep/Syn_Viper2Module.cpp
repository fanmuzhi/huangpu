#include "Syn_Viper2Module.h"


Syn_Viper2Module::Syn_Viper2Module()
{
}


Syn_Viper2Module::~Syn_Viper2Module()
{
}

bool Syn_Viper2Module::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
{
	FPSFrame * arFrames = new FPSFrame[MAXFRAMES];
	int	nNumRows = numRows;
	int	nNumCols = numCols;
	int	nPgaIdx = calInfo.m_nPgaIdx;
	FPSFrame calFrameZeroOffsets;
	FPSFrame calFrameNonZeroOffsets;
	int8_t * pOffsetArr = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx + 4];
	float nSetRatio = calInfo.m_nPgaOffsetRatio;
	float nConfigRatio = calInfo.m_nPgaOffsetRatio;
	int nNumFrames = calInfo.m_nNumPgaSamples;
	int16_t nTotal;
	int8_t * pTempOffsets = new int8_t[nNumRows * nNumCols];
	int8_t * pTmp;
	int8_t * pPrtFileOffsets;
	int8_t * pOtpOffsets;

	//These are the rows we save to OTP for calculating the variance score.
	int	arRowNums[NUM_PGA_OOPP_OTP_ROWS] = { PGA_OOPP_OTP_ROW1, PGA_OOPP_OTP_ROW2, PGA_OOPP_OTP_ROW3, PGA_OOPP_OTP_ROW4, PGA_OOPP_OTP_ROW5 };
	if (nNumRows != 56)
	{
		for (int i = 0; i < NUM_PGA_OOPP_OTP_ROWS; i++)
			arRowNums[i] = ((i + 1)*(nNumRows / 6)) - 1;
	}

	/*	int8_t arPixVals[256] = {0};
	for (int nFrame=0; nFrame<256; nFrame++)
	{
	memset((void*)&pCalResults->m_pPrintPatch[nPgaIdx+4], (int8_t)nFrame, nNumRows * (nNumCols-HEADER));
	GetFingerprintImage(*pCalResults, &arFrames[0], nNumRows, nNumCols, NULL, 0);
	arPixVals[nFrame] = arFrames[0].arr[5][HEADER+137];
	}
	memset((void*)&pCalResults->m_pPrintPatch[nPgaIdx+4], 0, nNumRows * (nNumCols-HEADER));
	*/
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
		if (calFrameZeroOffsets.arr[nRow][nCol] >= 0xbb)
			*pTmp++ = CalcPgaOffset(calFrameZeroOffsets.arr[nRow][nCol], nConfigRatio, nConfigRatio);
		else
			*pTmp++ = CalcPgaOffset(calFrameZeroOffsets.arr[nRow][nCol], nConfigRatio, nConfigRatio);
	}

	//Put the PGA offsets into the print file. The ordering is a bit strange.
	pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx + 4];
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = 0; nCol<4; nCol++)
			*pPrtFileOffsets++ = pTempOffsets[(nRow * (nNumCols - HEADER)) + nCol];
	}

	//Put the PGA offsets into the print file. The ordering is a bit strange.
	for (int nBigCol = HEADER + 4; nBigCol<nNumCols; nBigCol = nBigCol + 4)
	{
		for (int nCol = 0; nCol<4; nCol++)
			*pPrtFileOffsets++ = pTempOffsets[((nNumRows - 1) * (nNumCols - HEADER)) + ((nBigCol - HEADER) + nCol)];

		for (int nRow = 0; nRow<nNumRows - 1; nRow++)
		{
			for (int nCol = 0; nCol<4; nCol++)
				*pPrtFileOffsets++ = pTempOffsets[(nRow * (nNumCols - HEADER)) + ((nBigCol - HEADER) + nCol)];
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

				calFrameNonZeroOffsets.arr[nRow][nCol] = nTotal / nNumFrames;
			}
		}

		//Adjust the PGA offsets (fine tuning).
		pTmp = pTempOffsets;
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nCol = HEADER; nCol<nNumCols; nCol++)
			{
				int8_t nAdjustment = CalcPgaOffset(calFrameNonZeroOffsets.arr[nRow][nCol], nConfigRatio, nConfigRatio);
				if (((float)(*pTmp) + (float)nAdjustment) > 127)
					*pTmp++ = 127;
				else if (((float)(*pTmp) + (float)nAdjustment) < -128)
					*pTmp++ = -128;
				else
					*pTmp++ += nAdjustment;
			}
		}

		//Put the PGA offsets into the print file. The ordering is a bit strange.
		pPrtFileOffsets = (int8_t*)calResult.m_pPrintPatch[nPgaIdx + 4];
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nCol = 0; nCol<4; nCol++)
				*pPrtFileOffsets++ = pTempOffsets[(nRow * (nNumCols - HEADER)) + nCol];
		}

		//Put the PGA offsets into the print file. The ordering is a bit strange.
		for (int nBigCol = HEADER + 4; nBigCol<nNumCols; nBigCol = nBigCol + 4)
		{
			for (int nCol = 0; nCol<4; nCol++)
				*pPrtFileOffsets++ = pTempOffsets[((nNumRows - 1) * (nNumCols - HEADER)) + ((nBigCol - HEADER) + nCol)];

			for (int nRow = 0; nRow<nNumRows - 1; nRow++)
			{
				for (int nCol = 0; nCol<4; nCol++)
					*pPrtFileOffsets++ = pTempOffsets[(nRow * (nNumCols - HEADER)) + ((nBigCol - HEADER) + nCol)];
			}
		}

		//Save selected rows of PGA offsets for OTP (used in variance score) and log.
		pOtpOffsets = (int8_t*)calResult.m_arPgaOffsets;
		for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
		{
			for (int nCol = 0; nCol < nNumCols - HEADER; nCol++)
				*pOtpOffsets++ = pTempOffsets[(arRowNums[nSelRowIdx] * (nNumCols - HEADER)) + nCol];
		}
	}

	//Check if Stage 2 array is all the same.
	bool bStage2AllEqual = true;
	pOffsetArr = (int8_t*)calResult.m_pPrintPatch[nPgaIdx + 4];
	for (int i = 1; i<(nNumRows * (nNumCols - 8)); i++)
	{
		if (pOffsetArr[i - 1] != pOffsetArr[i])
			bStage2AllEqual = false;
	}

	delete[] arFrames;
	arFrames = NULL;

	delete[] pTempOffsets;
	return !bStage2AllEqual;
}