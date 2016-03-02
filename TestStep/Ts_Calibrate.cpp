#include "Ts_Calibrate.h"


Ts_Calibrate::Ts_Calibrate(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
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


	//Power on
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();

	//load ImgAcqPatch
	Syn_PatchInfo ImgAcqPatchInfo;
	if (!_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("ImageAcqPatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);
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

	//CheckDUTexists();
	
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

	if (NULL == _pSyn_Dut->_pSyn_DutTestResult)
	{
		_pSyn_Dut->_pSyn_DutTestResult = new Syn_DutTestResult();
	}

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
	uint8_t		pLnaValues[MS0_SIZE];
	uint8_t		pPgaValues[MS0_SIZE] = { 0 };
	bool		bSuccess(false);
	uint32_t nLnaIdx = _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx;
	if (_pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pLnaValues, MS0_SIZE) > 0)
	{
		CopyToPrintPatch(&pLnaValues[4], _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch, numRows, nLnaIdx);	//skip LNA first 4 bytes 00 00 00 07
	}
	else
	{
		//Calculate the LNA values and put them into the print patch.
		//FPSFrame *pFrame = site.m_acquireFpsResults.m_arImagesWithoutStimulus;
		FPSFrame *pLnaFrame = new FPSFrame();
		CalculateLnaOffsetsBinarySearch(pLnaFrame, pLnaValues, numRows, numCols, _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults);
		//CopyToPrintPatch(pLnaValues, &site.m_calibrationResults.m_pPrintPatch[4], GetSysConfig().GetNumRows(), site.m_calibrationInfo.m_nLnaIdx);
		CopyToPrintPatch(pLnaValues, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch, numRows, nLnaIdx);

		delete pLnaFrame;
		pLnaFrame = NULL;
	}

	//If cal type is One Offset Per Pixel.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nCalType == kPgaCalTypeOneOffsetPerPixel)
	{
		//If the OOPP offsets exist in the OTP, put them in a more convenient location. Skip the 0x00000007 stored in 1st 4 bytes.
		//int nPgaRecCount = GetMS0RecordData(TAG_CAL, EXT_TAG_PGA_OOPP, pPgaValues, MS0_SIZE, site);
		int nPgaRecCount = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, pPgaValues, MS0_SIZE);
		memcpy(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPGAOtpArray, &pPgaValues[4], NUM_PGA_OOPP_OTP_ROWS * numCols);

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
				//int32_t nVariance = OtpPgaVarianceTest(site, (int8_t*)site.m_calibrationResults.m_pPGAOtpArray, (int8_t*)site.m_calibrationResults.m_arPgaOffsets);
				//calResult.m_nStage2VarianceScore = nVariance;

				//If the variance is not within spec, record the error.
				if (_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nStage2VarianceScore > _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nPgaVarianceLimit)
				{
					_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sStage2VarianceFail);
					_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass = 0;
				}
			}
		}
	}

	/*FPSFrame *pFrame = new FPSFrame();
	GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, pFrame, numRows, numCols);
	delete pFrame;
	pFrame = NULL;*/

	//Calibration is done. If the High Pass Filter (HPF) was disabled during calibration, re-enable it.
	if (_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset)
		(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch)[_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nHpfOffset] |= 0x01;

	GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, &(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.arr_ImageFPSFrame), numRows, numCols);

	_pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_bExecuted = true;
}

void Ts_Calibrate::ProcessData()
{
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
	PowerOff();
}
