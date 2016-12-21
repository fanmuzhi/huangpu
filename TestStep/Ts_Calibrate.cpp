#include "Ts_Calibrate.h"


Ts_Calibrate::Ts_Calibrate(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_Calibrate::~Ts_Calibrate()
{
}

void Ts_Calibrate::SetUp()
{
	Syn_Exception ex(0);
	if (NULL == _pSyn_DutCtrl)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutCtrlNull);
		ex.SetDescription("_pSyn_DutCtrl is NULL!");
		throw ex;
		return;
	}
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
		return;
	}

	//parse args
	std::vector<std::string> listOfArgValue;

	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx = 1028;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaIdx = 1140;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bForceCal = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nNumImagesToDiscard = 20;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaOffsetLow = 108;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaOffsetHigh = 148;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitLow = 108;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitHigh = 148;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nCalType = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaOffsetRatio = (float)0.3;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nNumPgaSamples = 4;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaVarianceLimit = 90;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset = 0;
	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bPgaFineTuning = 0;

	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 14)
	{
		for (size_t t = 1; t <= 14 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}

	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaIdx = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bForceCal = atoi(listOfArgValue[2].c_str()) ? 1 : 0;
	if (0 != listOfArgValue[3].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nNumImagesToDiscard = atoi(listOfArgValue[3].c_str());
	if (0 != listOfArgValue[4].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaOffsetLow = atoi(listOfArgValue[4].c_str());
	if (0 != listOfArgValue[5].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaOffsetHigh = atoi(listOfArgValue[5].c_str());
	if (0 != listOfArgValue[6].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitLow = atoi(listOfArgValue[6].c_str());
	if (0 != listOfArgValue[7].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitHigh = atoi(listOfArgValue[7].c_str());
	if (0 != listOfArgValue[8].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nCalType = atoi(listOfArgValue[8].c_str());
	if (0 != listOfArgValue[9].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaOffsetRatio = stof(listOfArgValue[9]);//(float)_tstof((LPCTSTR)listOfArgValue[9].c_str());
	if (0 != listOfArgValue[10].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nNumPgaSamples = atoi(listOfArgValue[10].c_str());
	if (0 != listOfArgValue[11].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaVarianceLimit = atoi(listOfArgValue[11].c_str());
	if (0 != listOfArgValue[12].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset = atoi(listOfArgValue[12].c_str());
	if (0 != listOfArgValue[13].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bPgaFineTuning = atoi(listOfArgValue[13].c_str());

	//load ImgAcqPatch
	Syn_PatchInfo ImgAcqPatchInfo;
	if (_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		if (0 != ImgAcqPatchInfo._uiArraySize)
		{
			_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);
		}
	}
}


void Ts_Calibrate::Execute()
{
	Syn_Exception ex(0);
	if (NULL == _pSyn_DutCtrl)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutCtrlNull);
		ex.SetDescription("_pSyn_DutCtrl is NULL!");
		throw ex;
		return;
	}
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
		return;
	}
	uint32_t rc(0);

	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted = true;
	
	uint16_t numRows = _pSyn_Dut->_RowNumber;
	uint16_t numCols = _pSyn_Dut->_ColumnNumber;

	//get print file
	Syn_PatchInfo PrintFileInfo;
	if (!_pSyn_Dut->FindPatch("PrintFile", PrintFileInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("PrintFile Patch is NULL!");
		throw ex;
		return;
	}

	_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass = 1;

	//construct print file
	//uint8_t* pPrintPatch = new uint8_t[PrintFileInfo._uiArraySize];		//put into CalibrationResult later
	memcpy(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch, PrintFileInfo._pArrayBuf, PrintFileInfo._uiArraySize);
	_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nPrintPatchSize = PrintFileInfo._uiArraySize;


	//High Pass Filter(HPF)
	//If the configuration file specifies the High Pass Filter (HPF) be disabled during calibration.
	//Will be re-enabled at end of calibration step.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset)
		(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch)[_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset] &= 0xFE;


	//check LNA tag in OTP
	uint8_t		pSrc[2] = { 0, 0 };
	uint8_t		pLnaValues[MS0_SIZE] = { 0 };
	uint8_t		pPgaValues[MS0_SIZE] = { 0 };
	bool		bSuccess(false);
	uint32_t nLnaIdx = _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx;

	int nTags(0);
	//rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pLnaValues, MS0_SIZE, nTags);
	//if (nTags > 0)
	//{
	//	_pSyn_DutCtrl->CopyToPrintPatch(&pLnaValues[4], _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch, numRows, nLnaIdx);	//skip LNA first 4 bytes 00 00 00 07
	//}
	//else
	//{
	//	//Calculate the LNA values and put them into the print patch.
	//	//FPSFrame *pFrame = site.m_acquireFpsResults.m_arImagesWithoutStimulus;
	//	FPSFrame *pLnaFrame = new FPSFrame();
	//	this->CalculateLnaOffsetsBinarySearch(pLnaFrame, pLnaValues, numRows, numCols, _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults);
	//	//CopyToPrintPatch(pLnaValues, &site.m_calibrationResults.m_pPrintPatch[4], GetSysConfig().GetNumRows(), site.m_calibrationInfo.m_nLnaIdx);
	//	_pSyn_DutCtrl->CopyToPrintPatch(pLnaValues, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch, numRows, nLnaIdx);

	//	delete pLnaFrame;
	//	pLnaFrame = NULL;
	//}

	//Calculate the LNA values and put them into the print patch.
	//FPSFrame *pFrame = site.m_acquireFpsResults.m_arImagesWithoutStimulus;
	FPSFrame *pLnaFrame = new FPSFrame();
	this->CalculateLnaOffsetsBinarySearch(pLnaFrame, pLnaValues, numRows, numCols, _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults);
	//CopyToPrintPatch(pLnaValues, &site.m_calibrationResults.m_pPrintPatch[4], GetSysConfig().GetNumRows(), site.m_calibrationInfo.m_nLnaIdx);
	CopyToPrintPatch(pLnaValues, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch, numRows, nLnaIdx);

	delete pLnaFrame;
	pLnaFrame = NULL;





	//If cal type is One Offset Per Pixel.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nCalType == kPgaCalTypeOneOffsetPerPixel)
	{
		//If the OOPP offsets exist in the OTP, put them in a more convenient location. Skip the 0x00000007 stored in 1st 4 bytes.
		//int nPgaRecCount = GetMS0RecordData(TAG_CAL, EXT_TAG_PGA_OOPP, pPgaValues, MS0_SIZE, site);
		int nPgaRecCount(0);
		rc = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, pPgaValues, MS0_SIZE, nPgaRecCount);
		memcpy(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPGAOtpArray, &pPgaValues[4], NUM_PGA_OOPP_OTP_ROWS * (numCols  - HEADER));

		bSuccess = CalculatePgaOffsets_OOPP(numCols, numRows, _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults);
		if (!bSuccess)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sStages1Or2CalFail);
			_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass = 0;
		}
		else
		{
			//If at least 1 PGA record exists in the OTP, and an error has NOT yet been detected.
			if ((nPgaRecCount != 0) && (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass != 0))
			{
				int32_t nVariance = OtpPgaVarianceTest((int8_t*)_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPGAOtpArray, 
													   (int8_t*)_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_arPgaOffsets);
				_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nStage2VarianceScore = nVariance;

				//If the variance is not within spec, record the error.
				if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nStage2VarianceScore > _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaVarianceLimit)
				{
					_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sStage2VarianceFail);
					_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass = 0;
				}
			}

			//If no error detected yet.
			if ((_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass != 0) && (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bPgaFineTuning != 0))
			{
				//If any of the pixels are out of range.
				if ((!VerifyPixelRanges()))
				{
					_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sStages1Or2CalFail);
					_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass = 0;
				}
			}
		}
	}

	//Calibration is done. If the High Pass Filter (HPF) was disabled during calibration, re-enable it.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset)
		(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch)[_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset] |= 0x01;

	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted = true;

}

void Ts_Calibrate::ProcessData()
{
	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_elapsedtime);

	if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("Calibrate", "Pass"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("Calibrate", "Fail"));
	}

	if (!_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted)
	{
		Syn_Exception ex(Syn_ExceptionCode::Syn_TestStepNotExecuted);
		ex.SetDescription("TestStep Calibrate is not executed!");
		throw ex;
	}
}

void Ts_Calibrate::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
}

bool Ts_Calibrate::VerifyPixelRanges()
{
	bool		bSuccess = true;
	int			nNumRows = _pSyn_Dut->_RowNumber - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	int			nNumCols = _pSyn_Dut->_ColumnNumber - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;

	int			nLastRow = nNumRows - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimBotWithoutStim;
	int			nLastCol = nNumCols - _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimRightWithoutStim;

	int			nFirstRow = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimTopWithoutStim;
	int			nFirstCol = _pSyn_Dut->_pSyn_DutTestInfo->_initInfo.m_nTrimLeftWithoutStim;

	FPSFrame	*arFrame = &_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.arr_ImageFPSFrame;

	GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, arFrame, _pSyn_Dut->_RowNumber, _pSyn_Dut->_ColumnNumber);

	//Scan through all rows and columns. Apply trim values specified in Initialization step.
	for (int nRow = nFirstRow; nRow < nLastRow; nRow++)
	{
		for (int nCol = (nFirstCol + HEADER); nCol < nLastCol; nCol++)
		{
			if ((arFrame->arr[nRow][nCol] < _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitLow) || (arFrame->arr[nRow][nCol] > _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaLimitHigh))
			{
				int failPixel = arFrame->arr[nRow][nCol];
				bSuccess = false;
			}
		}
	}

	return bSuccess;
}

int32_t Ts_Calibrate::OtpPgaVarianceTest(int8_t* pOtpPgaOffsets, int8_t* pCurPgaOffsets)
{
	int	nNumCols = _pSyn_Dut->_ColumnNumber;

	////Calculate the variance between OTP PGA offsets and current PGA offsets.
	//int32_t nVariance = 0, nDelta;
	//for (int i=0; i < NUM_PGA_OOPP_OTP_ROWS * (nNumCols-HEADER); i++)
	//{
	//	nDelta = (int32_t)(pOtpPgaOffsets[i] - pCurPgaOffsets[i]);
	//	nVariance += nDelta * nDelta;
	//}
	//nVariance /= 4096;

	//return nVariance;

	int nMAX_COL(0), LineDC_OTP[NUM_PGA_OOPP_OTP_ROWS] = { 0 }, LineDC_Cur[NUM_PGA_OOPP_OTP_ROWS] = {0};
	int LineTotal_OTP(0), LineAverage_OTP(0), LineTotal_Cur(0), LineAverage_Cur(0);

	//int nPixCount = _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nNumberPxlSteps;
	for (int nRow = 0; nRow < NUM_PGA_OOPP_OTP_ROWS; nRow++)
	{
		LineTotal_OTP = 0;
		LineTotal_Cur = 0;

		for (int i = 0; i < nNumCols; i++)
		{
			LineTotal_OTP += pOtpPgaOffsets[(nRow * nNumCols) + i];
			LineTotal_Cur += pCurPgaOffsets[(nRow * nNumCols) + i];
		}

		LineDC_OTP[nRow] = LineTotal_OTP / nNumCols;
		LineDC_Cur[nRow] = LineTotal_Cur / nNumCols;
	}

	//Calculate the variance between OTP PGA offsets and current PGA offsets.
	int32_t nVariance = 0, nDelta;
	for (int i = 0; i < NUM_PGA_OOPP_OTP_ROWS * nNumCols; i++)
	{
		nDelta = (int32_t)((pOtpPgaOffsets[i] - LineDC_OTP[i / nNumCols]) - (pCurPgaOffsets[i] - LineDC_Cur[i / nNumCols]));
		nVariance += nDelta * nDelta;
	}

	nVariance /= (4 * nNumCols * NUM_PGA_OOPP_OTP_ROWS);

	return nVariance;
}


void Ts_Calibrate::CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults)
{
	if (NULL == pFrame)
	{
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

		this->GetRowAverages(pFrame, cal_colbegin, cal_colend, row_avgs, nNumRows);

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


void Ts_Calibrate::GetRowAverages(FPSFrame* pFrame, int nColBegin, int nColEnd, uint8_t* pAverages, int nNumRows)
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

bool Ts_Calibrate::CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
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
	//int8_t* pTmp;
	int8_t* pPrtFileOffsets;
	int8_t* pOtpOffsets;
	int32_t min_corr_limit = (int32_t)(-128 / nConfigRatio);
	min_corr_limit = min_corr_limit < -128 ? -128 : min_corr_limit;
	int32_t max_corr_limit = (int32_t)(127 / nConfigRatio);
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
		float temp = (float)vPixelError[i] / nConfigRatio;
		temp = temp > 127 ? 127 : temp;
		temp = temp < -128 ? -128 : temp;
		vPGAOffsets.push_back(temp);
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

void Ts_Calibrate::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	//For Metallica sensors, we copy once into Print Patch.
	memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
}