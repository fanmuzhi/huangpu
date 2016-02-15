#include "Syn_Calibrate.h"


Syn_Calibrate::Syn_Calibrate(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut, Syn_ModuleDecorator * &pSyn_ModuleDecorator)
:Syn_FingerprintTest(strName, pDutCtrl, pDut, pSyn_ModuleDecorator)
{
}


Syn_Calibrate::~Syn_Calibrate()
{
}

int Syn_Calibrate::Excute()
{
	if (NULL == _pSyn_DutCtrl)
	{
		return 0;
	}
	if (NULL == _pSyn_Dut)
	{
		return 0;
	}
	if (NULL == _pSyn_ModuleDecorator)
	{
		return 0;
	}

	uint16_t numRows = _pSyn_Dut->_RowNumber;
	uint16_t numCols = _pSyn_Dut->_ColumnNumber;

	//get print file
	Syn_PatchInfo PrintFileInfo;
	if (!_pSyn_Dut->FindPatch("PrintFile", PrintFileInfo))
	{
		return 0;
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

	_pSyn_DutCtrl->FpUnloadPatch();

	//load ImgAcqPatch
	Syn_PatchInfo ImgAcqPatchInfo;
	if (!_pSyn_Dut->FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		return false;
	}
	_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);

	//check LNA tag in OTP
	uint8_t		pSrc[2] = { 0, 0 };
	uint8_t		pLnaValues[MS0_SIZE];
	uint8_t		pPgaValues[MS0_SIZE] = { 0 };
	bool		bSuccess(false);
	uint32_t nLnaIdx = _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo.m_nLnaIdx;
	if (_pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pLnaValues, MS0_SIZE) > 0)
	{
		_pSyn_ModuleDecorator->CopyToPrintPatch(&pLnaValues[4], _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch, numRows, nLnaIdx);	//skip LNA first 4 bytes 00 00 00 07
	}
	else
	{
		//Calculate the LNA values and put them into the print patch.
		//FPSFrame *pFrame = site.m_acquireFpsResults.m_arImagesWithoutStimulus;
		FPSFrame *pLnaFrame = new FPSFrame();
		CalculateLnaOffsetsBinarySearch(pLnaFrame, pLnaValues, numRows, numCols, _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults);
		//CopyToPrintPatch(pLnaValues, &site.m_calibrationResults.m_pPrintPatch[4], GetSysConfig().GetNumRows(), site.m_calibrationInfo.m_nLnaIdx);
		_pSyn_ModuleDecorator->CopyToPrintPatch(pLnaValues, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_pPrintPatch, numRows, nLnaIdx);

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

		bSuccess = _pSyn_ModuleDecorator->CalculatePgaOffsets_OOPP(_pSyn_DutCtrl,numCols, numRows, _pSyn_Dut->_pSyn_DutTestInfo->_calibrationInfo, _pSyn_Dut->_pSyn_DutTestResult->_calibrationResults);
		if (!bSuccess)
		{
			//site.PushBinCodes(BinCodes::m_sStages1Or2CalFail);
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
					//site.PushBinCodes(BinCodes::m_sStage2VarianceFail);
					_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_bPass = 0;
				}
			}
		}
	}

	FPSFrame *pFrame = new FPSFrame();
	GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, pFrame, numRows, numCols);
	_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.arr_ImageFPSFrame = *pFrame;
	/*delete pFrame;
	pFrame = NULL;*/

	return 0;
}
