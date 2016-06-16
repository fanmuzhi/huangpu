//Local
#include "Syn_MetallicaModule.h"
#include <vector>

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

bool Syn_MetallicaModule::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
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
	int arRowNums[NUM_PGA_OOPP_OTP_ROWS];
	//int8_t* pTempOffsets = new int8_t[nNumRows * nNumCols];
	std::vector<int8_t> vPixelError;
	std::vector<int8_t> vPGAOffsets;
	std::vector<int8_t> vPGAFineOffsets;
	int8_t* pTmp;
	int8_t* pPrtFileOffsets;
	int8_t* pOtpOffsets;
    int32_t min_corr_limit = (int32_t) (-128 / nConfigRatio);
	min_corr_limit = min_corr_limit < -128 ? -128 : min_corr_limit;
    int32_t max_corr_limit = (int32_t) (127 / nConfigRatio);
	max_corr_limit = max_corr_limit > 127 ? 127 : max_corr_limit;

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
			calFrameZeroOffsets->arr[nRow][nCol] = nTotal / nNumFrames;
		}
	}

	//Calculate the PixelError.
	for (int nRow = 0; nRow<nNumRows; nRow++)
	{
		for (int nCol = HEADER; nCol < nNumCols; nCol++)
			//*pTmp++ = CalcPgaOffset(calFrameZeroOffsets->arr[nRow][nCol], nConfigRatio, nConfigRatio);
			vPixelError.push_back(calFrameZeroOffsets->arr[nRow][nCol] - 128);
	}

	//Calculate the PGA offsets (no fine tuning).
	for (int i = 0; i < nNumRows * (nNumCols - HEADER); i++)
	{
		vPGAOffsets.push_back(vPixelError[i] / nConfigRatio);
	}

	//Put the PGA offsets into the print file. The ordering is a bit strange.
	pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
	for (int nBigCol = 0; nBigCol<(nNumCols - HEADER); nBigCol = nBigCol + 4)
	{
		for (int nRow = 0; nRow<nNumRows; nRow++)
		{
			for (int nColIdx = 0; nColIdx<4; nColIdx++)
				*pPrtFileOffsets++ = vPGAOffsets[(nRow * (nNumCols - HEADER)) + (nBigCol + nColIdx)];
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
				if (new_ratio <= 0)
					new_ratio = nConfigRatio;
				if ((((calFrameNonZeroOffsets->arr[nRow][nCol] - 128) * (calFrameZeroOffsets->arr[nRow][nCol] - 128)) > 0)
					&& (vPGAOffsets[index] <= min_corr_limit || vPGAOffsets[index] >= max_corr_limit))
					new_ratio = nConfigRatio;
				if (abs(delta) < 10)
					new_ratio = nConfigRatio;

				int temp = vPixelError[index] / new_ratio;
				temp = temp > 127 ? 127 : temp;
				temp = temp < -128 ? -128 : temp;
				vPGAFineOffsets.push_back(temp);
				index++;
			}
		}

		//Put the PGA offsets into the print file. The ordering is a bit strange.
		pPrtFileOffsets = (int8_t*)&calResult.m_pPrintPatch[nPgaIdx];
		for (int nBigCol = 0; nBigCol<(nNumCols - HEADER); nBigCol = nBigCol + 4)
		{
			for (int nRow = 0; nRow<nNumRows; nRow++)
			{
				for (int nColIdx = 0; nColIdx<4; nColIdx++)
					*pPrtFileOffsets++ = vPGAFineOffsets[(nRow * (nNumCols - HEADER)) + (nBigCol + nColIdx)];
			}
		}

		//Save selected rows of PGA offsets for OTP (used in variance score) and log.
		pOtpOffsets = (int8_t*)calResult.m_arPgaOffsets;
		for (int nSelRowIdx = 0; nSelRowIdx < NUM_PGA_OOPP_OTP_ROWS; nSelRowIdx++)
		{
			for (int nCol = 0; nCol < nNumCols - HEADER; nCol++)
				*pOtpOffsets++ = vPGAFineOffsets[(arRowNums[nSelRowIdx] * (nNumCols - HEADER)) + nCol];
		}

		//print file
		std::string sFileName("PixelValue.csv");
		FILE *pFile = fopen(sFileName.c_str(), "a");
		if (NULL != pFile)
		{
			int k = 0;
			fprintf(pFile, "vPixelError\n");
			for (int nRow = 0; nRow < nNumRows; nRow++)
			{
				for (int nCol = HEADER; nCol < nNumCols; nCol++)
				{
					fprintf(pFile, "%d,", vPixelError[k]);
					k++;
				}
				fprintf(pFile, "\n");
			}

			k = 0;
			fprintf(pFile, "\nvPGAOffsets\n");
			for (int nRow = 0; nRow < nNumRows; nRow++)
			{
				for (int nCol = HEADER; nCol < nNumCols; nCol++)
				{
					fprintf(pFile, "%d,", vPGAOffsets[k]);
					k++;
				}
				fprintf(pFile, "\n");
			}

			k = 0;
			fprintf(pFile, "\nvPGAFineOffsets\n");
			for (int nRow = 0; nRow < nNumRows; nRow++)
			{
				for (int nCol = HEADER; nCol < nNumCols; nCol++)
				{
					fprintf(pFile, "%d,", vPGAFineOffsets[k]);
					k++;
				}
				fprintf(pFile, "\n");
			}

			fprintf(pFile, "\ncalFrameZeroOffsets\n");
			for (int nRow = 0; nRow < nNumRows; nRow++)
			{
				for (int nCol = HEADER; nCol < nNumCols; nCol++)
				{
					fprintf(pFile, "%d,", calFrameZeroOffsets->arr[nRow][nCol]);
				}
				fprintf(pFile, "\n");
			}

			fprintf(pFile, "\ncalFrameNonZeroOffsets\n");
			for (int nRow = 0; nRow < nNumRows; nRow++)
			{
				for (int nCol = HEADER; nCol < nNumCols; nCol++)
				{
					fprintf(pFile, "%d,", calFrameNonZeroOffsets->arr[nRow][nCol]);
				}
				fprintf(pFile, "\n");
			}

			fclose(pFile);
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

void Syn_MetallicaModule::TrimOsc(OscTrimInfo &iOscTrimInfo, OscTrimResults &ioOscTrimResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV)
{
	uint8_t		pDst[4];
	uint8_t		arHWRegAddr[5] = { 0x34, 0x03, 0x00, 0x80, 0x04 };
	uint32_t	nFreq_Hz;
	uint32_t	nTrimValue;
	int			timeout;

	ioOscTrimResults.m_bPass = 0;
	iOscTrimInfo.m_bDefaultValueUsed = 0;

	PowerOff();
	PowerOn(Vdd_mV, Vio_mV, Vled_mV, Vddh_mV, true);
	::Sleep(100);

	//Check the Osc Value before writing.
	_pSyn_DutCtrl->FpPeekRegister(0x80000334, pDst);

	//Make sure the state of the sensor is such that, it will accept Osc to be trimmed
	if (((pDst[1] & 0x3F) == 0) || ((pDst[1] & 0x3F) == 10))
	{
		//Set port as an input to read in the divided OSC Freq
		_pSyn_DutCtrl->GpioDirModSet(6, 0x80, 0);//Set pin as direct input
		_pSyn_DutCtrl->GpioSetPinType(6, 0x80, 5);//Set the pin to input.        

		//Poke - note that there is an array of data values that are being poked.
		_pSyn_DutCtrl->FpPokeRegister(0x8000034C, 0x02);
		_pSyn_DutCtrl->FpPokeRegister(0x80000384, 0x00);
		_pSyn_DutCtrl->FpPokeRegister(0x80001500, 0x01);
		_pSyn_DutCtrl->FpPokeRegister(0x80001508, 0x7F);
		_pSyn_DutCtrl->FpPokeRegister(0x80001510, 0x1F);
		_pSyn_DutCtrl->FpPokeRegister(0x80001504, 0x02);
		_pSyn_DutCtrl->FpPokeRegister(0x8000000B, 0x02);
		_pSyn_DutCtrl->FpPokeRegister(0x80000334, 0x0E);

		::Sleep(25);

		//Calculate the optimal Trim value
		nTrimValue = iOscTrimInfo.nInitialTrim;
		timeout = 100;
		_pSyn_DutCtrl->FpPokeRegister(0x80000B08, nTrimValue);
		_pSyn_DutCtrl->FpGpioGetFreq(6, 0x80, &nFreq_Hz);
		while (timeout && ((nFreq_Hz < iOscTrimInfo.nLowerLimit_Hz) || ((nFreq_Hz >= iOscTrimInfo.nUpperLimit_Hz))))
		{
			//Is the frequency less than approximately 1.5% of the lower limit.
			if (nFreq_Hz < (iOscTrimInfo.nLowerLimit_Hz - (iOscTrimInfo.nLowerLimit_Hz >> 6)))
				nTrimValue += 20;
			else
				nTrimValue++;

			_pSyn_DutCtrl->FpPokeRegister(0x80000B08, nTrimValue);
			_pSyn_DutCtrl->FpGpioGetFreq(6, 0x80, &nFreq_Hz);
			timeout--;
		}

		//If trimming was successful
		if (timeout != 0)
		{
			ioOscTrimResults.m_nOscTrim = nTrimValue;
			//FpWriteOscTrim(ioOscTrimResults.m_nOscTrim);
			ioOscTrimResults.m_bPass = 1;
		}
		else //Trimming unsuccessful
		{
			ioOscTrimResults.m_bPass = 0;

			//If default value specified in config file, use it.
			//if (0!=iOscTrimInfo.m_OscTrimDefault)
			//{
			//	iOscTrimInfo.m_bDefaultValueUsed = 1;
			//	ioOscTrimResults.m_nOscTrim = iOscTrimInfo.m_OscTrimDefault;
			//	FpWriteOscTrim(ioOscTrimResults.m_nOscTrim);
			//	ioOscTrimResults.m_bPass = 1;
			//}
		}
	}
}

void Syn_MetallicaModule::TrimSlowOsc(SlowOscInfo &iSlowOscInfo, SlowOscResults &ioSlowOscResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV)
{
	uint8_t		pDst[4] = {0};
	uint32_t	nFreq_Hz = 0;
	uint16_t	nHvOscBias, nHvOscTrim;
	int			timeout;
	ioSlowOscResults.m_bPass = 0;
	ioSlowOscResults.m_bDefaultValueUsed = 0;

	PowerOff();
	PowerOn(Vdd_mV, Vio_mV, Vled_mV, Vddh_mV, true);

	_pSyn_DutCtrl->GpioDirModSet(6, 0x80, 0);//Set pin as direct input
	_pSyn_DutCtrl->GpioSetPinType(6, 0x80, 5);

	//Poke - note that there is an array of data values that are being poked.
	_pSyn_DutCtrl->FpPokeRegister(0x80002050, 0x17);
	_pSyn_DutCtrl->FpPokeRegister(0x8000036C, 0x0A);
	_pSyn_DutCtrl->FpPokeRegister(0x80000338, 0x20);
	_pSyn_DutCtrl->FpPokeRegister(0x8000034C, 0x20);
	_pSyn_DutCtrl->FpPokeRegister(0x80000384, 0x03);
	_pSyn_DutCtrl->FpPokeRegister(0x80001604, 0x02);

	::Sleep(50);

	timeout = 10;
	nHvOscBias = 3;
	nHvOscTrim = 0;
	//Calculate optimal Trim and Bias values.
	uint32_t nData = (((0x1F & nHvOscTrim) << 8) | nHvOscBias);
	_pSyn_DutCtrl->FpPokeRegister(0x80000330, nData);

	_pSyn_DutCtrl->FpGpioGetFreq(6, 0x80, &nFreq_Hz);
	while (timeout && ((nFreq_Hz < iSlowOscInfo.nLowerLimit_Hz) || ((nFreq_Hz >= iSlowOscInfo.nUpperLimit_Hz))))
	{
		nHvOscTrim++;
		nData = (((0x1F & nHvOscTrim) << 8) | nHvOscBias);
		_pSyn_DutCtrl->FpPokeRegister(0x80000330, nData);
		_pSyn_DutCtrl->FpGpioGetFreq(6, 0x80, &nFreq_Hz);
		timeout--;
	}

	//If trimming was successful
	if (timeout != 0)
	{
		ioSlowOscResults.m_nTrim = nHvOscTrim;
		ioSlowOscResults.m_nBias = nHvOscBias;
		//FpWriteSlowOscFreq(nHvOscTrim, nHvOscBias);
		ioSlowOscResults.m_bPass = 1;
	}
	else //Trimming unsuccessful
	{
		ioSlowOscResults.m_bPass = 0;

		//If default value specified in config file, use it.
		//if ((iSlowOscInfo.m_nDefaultTrim != 0) && (iSlowOscInfo.m_nDefaultBias != 0))
		//{
		//	ioSlowOscResults.m_bDefaultValueUsed = 1;
		//	ioSlowOscResults.m_nTrim = iSlowOscInfo.m_nDefaultTrim;
		//	ioSlowOscResults.m_nBias = iSlowOscInfo.m_nDefaultBias;
		//	FpWriteSlowOscFreq(ioSlowOscResults.m_nTrim, ioSlowOscResults.m_nBias);
		//	ioSlowOscResults.m_bPass = 1;
		//}
	}
}