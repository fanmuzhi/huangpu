#include "Ts_ViperCalibrate.h"

Ts_ViperCalibrate::Ts_ViperCalibrate(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Ts_Calibrate(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_ViperCalibrate::~Ts_ViperCalibrate()
{
}

bool Ts_ViperCalibrate::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
{
	FPSFrame * arFrames = new FPSFrame[MAXFRAMES];
	int nNumRows = numRows;
	int nNumCols = numCols;
	int nPgaIdx = calInfo.m_nPgaIdx;
	FPSFrame *calFrameZeroOffsets = new FPSFrame[MAXFRAMES];
	FPSFrame *calFrameNonZeroOffsets = new FPSFrame[MAXFRAMES];
	float nConfigRatio = calInfo.m_nPgaOffsetRatio;
	int nNumFrames = calInfo.m_nNumPgaSamples;
	int16_t nTotal;
	//int arRowNums[NUM_PGA_OOPP_OTP_ROWS];
	//int8_t* pTempOffsets = new int8_t[nNumRows * nNumCols];
	std::vector<int8_t> vPixelError;
	std::vector<int8_t> vPGAOffsets;
	std::vector<int8_t> vPGAFineOffsets;
	//int8_t* pTmp;
	int8_t* pPrtFileOffsets;
	int8_t* pOtpOffsets;
	int32_t min_corr_limit = (int32_t)(-128 / nConfigRatio);
	min_corr_limit = min_corr_limit < -128 ? -128 : min_corr_limit;
	int32_t max_corr_limit = (int32_t)(127 / nConfigRatio);
	max_corr_limit = max_corr_limit > 127 ? 127 : max_corr_limit;

	//These are the rows we save to OTP for calculating the variance score.
	int	arRowNums[NUM_PGA_OOPP_OTP_ROWS] = { PGA_OOPP_OTP_ROW1, PGA_OOPP_OTP_ROW2, PGA_OOPP_OTP_ROW3, PGA_OOPP_OTP_ROW4, PGA_OOPP_OTP_ROW5 };
	if (nNumRows != 56)
	{
		for (int i = 0; i < NUM_PGA_OOPP_OTP_ROWS; i++)
			arRowNums[i] = ((i + 1)*(nNumRows / 6)) - 1;
	}

	////These are the rows we save to OTP for calculating the variance score.
	//for (int i = 0; i < NUM_PGA_OOPP_OTP_ROWS; i++)
	//	arRowNums[i] = ((i + 1)*(nNumRows / 6)) - 1;

	//Get user-specified number of images with the PGA offsets set to 0 (no offset), then
	//calculate the average.
	for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
		GetFingerprintImage(calResult, &arFrames[nFrame], nNumRows, nNumCols);
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = HEADER; nCol < nNumCols; nCol++)
		{
			nTotal = 0;
			for (int nFrame = 0; nFrame<nNumFrames; nFrame++)
				nTotal += (int16_t)arFrames[nFrame].arr[nRow][nCol];
			calFrameZeroOffsets->arr[nRow][nCol] = nTotal / nNumFrames;
		}
	}

	//Calculate the PixelError.
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = HEADER; nCol < nNumCols; nCol++)
		{
			vPixelError.push_back(calFrameZeroOffsets->arr[nRow][nCol] - 128);
		}
	}

	//Calculate the PGA offsets (no fine tuning).
	for (int i = 0; i < nNumRows * (nNumCols - HEADER); i++)
	{
		float temp = (float)vPixelError[i] / nConfigRatio;
		temp = temp > 127 ? 127 : temp;
		temp = temp < -128 ? -128 : temp;
		vPGAOffsets.push_back(temp);
	}

	//Put the PGA offsets into the print file. The ordering is a bit strange.
	//pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx + 4];
	pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = 0; nCol<4; nCol++)
			*pPrtFileOffsets++ = vPGAOffsets[(nRow * (nNumCols - HEADER)) + nCol];
	}

	//Put the PGA offsets into the print file. The ordering is a bit strange.
	for (int nBigCol = HEADER + 4; nBigCol<nNumCols; nBigCol = nBigCol + 4)
	{
		for (int nCol = 0; nCol<4; nCol++)
			*pPrtFileOffsets++ = vPGAOffsets[((nNumRows - 1) * (nNumCols - HEADER)) + ((nBigCol - HEADER) + nCol)];

		for (int nRow = 0; nRow<nNumRows - 1; nRow++)
		{
			for (int nCol = 0; nCol<4; nCol++)
				*pPrtFileOffsets++ = vPGAOffsets[(nRow * (nNumCols - HEADER)) + ((nBigCol - HEADER) + nCol)];
		}
	}

	//Save selected rows of PGA offsets for OTP (used in variance score) and log.
	pOtpOffsets = (int8_t*)calResult.m_arPgaOffsets;
	for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
	{
		for (int nCol = 0; nCol < nNumCols - HEADER; nCol++)
			*pOtpOffsets++ = vPGAOffsets[(arRowNums[nSelRowIdx] * (nNumCols - HEADER)) + nCol];
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
				calFrameNonZeroOffsets->arr[nRow][nCol] = nTotal / nNumFrames;
			}
		}

		//Get per pixel ratio
		int index = 0;
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nCol = HEADER; nCol < nNumCols; nCol++)
			{
				float new_ratio = nConfigRatio;
				float delta = (float)(calFrameZeroOffsets->arr[nRow][nCol] - calFrameNonZeroOffsets->arr[nRow][nCol]);
				if (vPGAOffsets[index] != 0)
				{
					new_ratio = delta / vPGAOffsets[index];
				}
				if (abs(delta) < 10)
					new_ratio = nConfigRatio;
				if (new_ratio <= 0)
					new_ratio = nConfigRatio;
				if ((((calFrameNonZeroOffsets->arr[nRow][nCol] - 128) * (calFrameZeroOffsets->arr[nRow][nCol] - 128)) > 0)
					&& (vPGAOffsets[index] <= min_corr_limit || vPGAOffsets[index] >= max_corr_limit))
				{
					new_ratio = nConfigRatio;

					//calculate new pGA offsets with Dane's formula
					float temp = ((float)vPGAOffsets[index] - (delta / nConfigRatio)) + (float)vPGAOffsets[index];
					//int temp = (vPGAOffsets[index] - (delta / nConfigRatio)) + vPGAOffsets[index];
					temp = temp > 127 ? 127 : temp;
					temp = temp < -128 ? -128 : temp;
					vPGAFineOffsets.push_back(temp);
				}
				else
				{
					int temp = vPixelError[index] / new_ratio;
					temp = temp > 127 ? 127 : temp;
					temp = temp < -128 ? -128 : temp;
					vPGAFineOffsets.push_back(temp);
				}
				index++;
			}
		}

		//Put the PGA offsets into the print file. The ordering is a bit strange.
		//pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx + 4];
		pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nCol = 0; nCol<4; nCol++)
				*pPrtFileOffsets++ = vPGAOffsets[(nRow * (nNumCols - HEADER)) + nCol];
		}

		//Put the PGA offsets into the print file. The ordering is a bit strange.
		for (int nBigCol = HEADER + 4; nBigCol<nNumCols; nBigCol = nBigCol + 4)
		{
			for (int nCol = 0; nCol<4; nCol++)
				*pPrtFileOffsets++ = vPGAOffsets[((nNumRows - 1) * (nNumCols - HEADER)) + ((nBigCol - HEADER) + nCol)];

			for (int nRow = 0; nRow<nNumRows - 1; nRow++)
			{
				for (int nCol = 0; nCol<4; nCol++)
					*pPrtFileOffsets++ = vPGAOffsets[(nRow * (nNumCols - HEADER)) + ((nBigCol - HEADER) + nCol)];
			}
		}

		//Save selected rows of PGA offsets for OTP (used in variance score) and log.
		pOtpOffsets = (int8_t*)calResult.m_arPgaOffsets;
		for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
		{
			for (int nCol = 0; nCol < nNumCols - HEADER; nCol++)
				*pOtpOffsets++ = vPGAFineOffsets[(arRowNums[nSelRowIdx] * (nNumCols - HEADER)) + nCol];
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

	delete[] arFrames;
	arFrames = NULL;
	delete[] calFrameZeroOffsets;
	calFrameZeroOffsets = NULL;
	delete[] calFrameNonZeroOffsets;
	calFrameNonZeroOffsets = NULL;

	return !bStage2AllEqual;

}

void Ts_ViperCalibrate::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	//For Viper sensors, we copy twice into Print Patch. Make sure 2nd copy is alligned on
	//a four byte boundary.
	if ((nNumBytes % 4) == 0)
	{
		memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
		memcpy(&pPrintPatch[nPatchIdx + nNumBytes], pSrc, nNumBytes);
	}
	else
	{
		//Make sure the space between the two copies of data are set to zero.
		memset(&pPrintPatch[nPatchIdx], 0, nNumBytes + (4 - (nNumBytes % 4)));
		memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
		memcpy(&pPrintPatch[nPatchIdx + (nNumBytes + (4 - (nNumBytes % 4)))], pSrc, nNumBytes);
	}
}