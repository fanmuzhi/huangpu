#include "Ts_Calibrate.h"


Ts_Calibrate::Ts_Calibrate(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, pDutCtrl, pDut)
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
}

void Ts_Calibrate::ProcessData()
{
	//uint16_t numRows = _pSyn_Dut->_RowNumber;
	//uint16_t numCols = _pSyn_Dut->_ColumnNumber;

	//GetFingerprintImage(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults, &(_pSyn_Dut->_pSyn_DutTestResult->_acquireFpsResults.arr_ImageFPSFrame), numRows, numCols);
}

void Ts_Calibrate::CleanUp()
{
	_pSyn_DutCtrl->FpUnloadPatch();
	PowerOff();
}
