#include "Syn_Viper1Module.h"


Syn_Viper1Module::Syn_Viper1Module()
{
}


Syn_Viper1Module::~Syn_Viper1Module()
{
}

bool Syn_Viper1Module::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
{
	FPSFrame * arFrames = new FPSFrame[MAXFRAMES];
	int	nNumRows = numRows;
	int	nNumCols = numCols;
	int	nPgaIdx = calInfo.m_nPgaIdx;
	FPSFrame * calFrameZeroOffsets = new FPSFrame[MAXFRAMES];
	FPSFrame * calFrameNonZeroOffsets = new FPSFrame[MAXFRAMES];
	//int8_t *pOffsetArr = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx + 4];
	int8_t *pOffsetArr = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
	float nRatio;
	float nConfigRatio = calInfo.m_nPgaOffsetRatio;
	int	nNumFrames = calInfo.m_nNumPgaSamples;
	int16_t	nTotal;
	int	nOffsetDelta = 20;

	//Get user-specified number of images with the PGA offsets set to 0 (no offset).
	for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
		GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols);
	//Get the average.
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = 0; nCol<nNumCols; nCol++)
		{
			nTotal = 0;
			for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
				nTotal += (int16_t)arFrames[nFrame].arr[nRow][nCol];

			calFrameZeroOffsets->arr[nRow][nCol] = nTotal / nNumFrames;
		}
	}

	if (calInfo.m_bPgaFineTuning)
	{
		//Add specified delta to offsets
		for (int i = 0; i<nNumRows * (nNumCols - HEADER); i++)
			pOffsetArr[i] = pOffsetArr[i] + nOffsetDelta;

		//Get user-specified number of images with the PGA offsets to nOffsetDelta.		
		for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
			GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols);
		//Get the average.
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nCol = 0; nCol<nNumCols; nCol++)
			{
				nTotal = 0;
				for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
					nTotal += (int16_t)arFrames[nFrame].arr[nRow][nCol];

				calFrameNonZeroOffsets->arr[nRow][nCol] = nTotal / nNumFrames;
			}
		}
	}

	//Row indentifiers of rows to be used in variance score.
	int	arRowNums[NUM_PGA_OOPP_OTP_ROWS] = { PGA_OOPP_OTP_ROW1, PGA_OOPP_OTP_ROW2, PGA_OOPP_OTP_ROW3, PGA_OOPP_OTP_ROW4, PGA_OOPP_OTP_ROW5 };

	//Save 5 rows of PGA offsets for OTP (used in variance score) and log.
	int8_t* pBuf = (int8_t*)calResult.m_arPgaOffsets;
	for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
	{
		int nRow = arRowNums[nSelRowIdx];
		for (int nCol = 8; nCol < nNumCols; nCol++)
		{
			nRatio = calInfo.m_nPgaOffsetRatio;
			if (calInfo.m_bPgaFineTuning)
			{
				nRatio = ((int16_t)calFrameZeroOffsets->arr[nRow][nCol] -
					(int16_t)calFrameNonZeroOffsets->arr[nRow][nCol]) / (float)nOffsetDelta;
			}
			*pBuf++ = CalcPgaOffset(calFrameZeroOffsets->arr[nRow][nCol], nRatio, nConfigRatio);
		}
	}

	//Put the PGA offsets into the print file in order specific to sensor type.
	for (int nBigCol = 8; nBigCol<nNumCols; nBigCol = nBigCol + 4)
	{
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nCol = 0; nCol<4; nCol++)
			{
				nRatio = calInfo.m_nPgaOffsetRatio;
				if (calInfo.m_bPgaFineTuning)
				{
					nRatio = ((int16_t)calFrameZeroOffsets->arr[nRow][nBigCol + nCol] -
						(int16_t)calFrameNonZeroOffsets->arr[nRow][nBigCol + nCol]) / (float)nOffsetDelta;
				}
				*pOffsetArr++ = CalcPgaOffset(calFrameZeroOffsets->arr[nRow][nBigCol + nCol], nRatio, nConfigRatio);
			}
		}
	}

	//Check if Stage 2 array is all the same.
	bool bStage2AllEqual = true;
	//pOffsetArr = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx + 4];
	pOffsetArr = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
	for (int i = 1; i<(nNumRows * (nNumCols - 8)); i++)
	{
		if (pOffsetArr[i - 1] != pOffsetArr[i])
			bStage2AllEqual = false;
	}

	delete[] arFrames;
	arFrames = NULL;

	delete[] calFrameNonZeroOffsets;
	calFrameNonZeroOffsets = NULL;

	delete[] calFrameZeroOffsets;
	calFrameZeroOffsets = NULL;

	return !bStage2AllEqual;
}