//windows api
#include "windows.h"

//Local
#include "Syn_Dut.h"
#include "Syn_Viper.h"
#include "Syn_Viper1.h"
#include "Syn_Viper2.h"
#include "Syn_Metallica.h"
#include "Syn_Exception.h"
#include "Syn_Vcsfw.h"

//std
#include <iostream>

//third-party
#include "easylogging++.h"

Syn_Dut::Syn_Dut()
:_pSyn_DutCtrl(NULL)
{
}


Syn_Dut::~Syn_Dut()
{
	if (NULL != _pSyn_DutCtrl)
	{
		delete _pSyn_DutCtrl;
		_pSyn_DutCtrl = NULL;
	}
}

bool Syn_Dut::CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance)
{
	opSyn_DutInstance = NULL;
	if (Viper1 == iType)
	{
		Syn_Viper1 *pSyn_Viper1 = new Syn_Viper1();
		opSyn_DutInstance = static_cast<Syn_Dut*>(pSyn_Viper1);
	}
	else if (Viper2 == iType)
	{
		opSyn_DutInstance = new Syn_Viper2();
	}
	else if (Metallica == iType)
	{
		opSyn_DutInstance = new Syn_Metallica();
	}
	else
	{
		LOG(ERROR) << "Can't retrieve the ProjectType!";
		return false;
	}

	return true;
}

bool Syn_Dut::SetDutCtrl(Syn_DutCtrl * ipSyn_DutCtrl)
{
	if (NULL != ipSyn_DutCtrl)
	{
		_pSyn_DutCtrl = ipSyn_DutCtrl;
		return true;
	}
	else
	{
		LOG(ERROR) << "ipSyn_DutCtrl is NULL!";
		return false;
	}
}

Syn_DutCtrl * Syn_Dut::GetDutCtrl()
{
	return _pSyn_DutCtrl;
}

void Syn_Dut::PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep)
{
	if (NULL == _pSyn_DutCtrl)
	{
		LOG(ERROR) << "_pSyn_DutCtrl is NULL!";
		return;
	}

	_pSyn_DutCtrl->SetVoltages(nPwrVdd, nPwrVio, nPwrVled, nPwrVddh);
	::Sleep(50);

	//If requested by the caller, disable the sensor's sleep feature.
	if (bDisableSleep)
	{
		//Wake sensor from sleep mode.
		_pSyn_DutCtrl->FpWaitDeviceReady();
		//Configure sensor not to go back to sleep.
		_pSyn_DutCtrl->FpDisableSleep();
	}
}

void Syn_Dut::PowerOff()
{
	try
	{
		_pSyn_DutCtrl->SetVoltages(0, 0, 0, 0);
		::Sleep(50);
	}
	catch (...)
	{
	}
}

bool Syn_Dut::ReadOTP(uint8_t* oarMS0, int iSize)
{
	if (NULL == _pSyn_DutCtrl)
	{
		LOG(ERROR) << "_pSyn_DutCtrl is NULL!";
		return false;
	}

	Syn_PatchInfo OtpReadWritePatchInfo;
	if (!FindPatch("OtpReadWritePatch", OtpReadWritePatchInfo))
	{
		LOG(ERROR) << "Cannot find 'OtpReadWritePatch' in config file";
		return false;
	}
	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(OtpReadWritePatchInfo._pArrayBuf, OtpReadWritePatchInfo._uiArraySize);//OtpReadWritePatch
	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, oarMS0, BS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 1, &oarMS0[BS0_SIZE], BS1_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 0, &oarMS0[BS0_SIZE + BS1_SIZE], MS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 1, &oarMS0[BS0_SIZE + BS1_SIZE + MS0_SIZE], MS1_SIZE);

	//::Sleep(100);

	return true;
}

bool Syn_Dut::Calibration(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
{
	if (NULL == _pSyn_DutCtrl)
	{
		LOG(ERROR) << "_pSyn_DutCtrl is NULL!";
		return false;
	}

	

	//get print file
	Syn_PatchInfo PrintFileInfo;
	if (!FindPatch("PrintFile", PrintFileInfo))
	{
		LOG(ERROR) << "Cannot find 'PrintFileInfo' in config file";
		return false;
	}

	//construct print file
	uint8_t* pPrintPatch = new uint8_t[PrintFileInfo._uiArraySize];		//put into CalibrationResult later
	uint32_t nLnaIdx = calInfo.m_nLnaIdx;			

	memcpy(pPrintPatch, PrintFileInfo._pArrayBuf, PrintFileInfo._uiArraySize);

	string debugInfo;
	for (auto i = 0; i < PrintFileInfo._uiArraySize; i++)
	{
		debugInfo += to_string(pPrintPatch[i]) + ",";
	}
	LOG(DEBUG) << debugInfo;

	//High Pass Filter(HPF)
	//TODO
	
	
	_pSyn_DutCtrl->FpUnloadPatch();
	////load OTPReadWritePatch
	//Syn_PatchInfo OtpReadWritePatchInfo;
	//if (!FindPatch("OtpReadWritePatch", OtpReadWritePatchInfo))
	//{
	//	LOG(ERROR) << "Cannot find 'OtpReadWritePatch' in config file";
	//	return false;
	//}
	//_pSyn_DutCtrl->FpLoadPatch(OtpReadWritePatchInfo._pArrayBuf, OtpReadWritePatchInfo._uiArraySize);//OtpReadWritePatch

	//load ImgAcqPatch
	Syn_PatchInfo ImgAcqPatchInfo;
	if (!FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		LOG(ERROR) << "Cannot find 'ImageAcqPatch' in config file";
		return false;
	}
	_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);



	//If the configuration file specifies the High Pass Filter (HPF) be disabled during calibration.
	//Will be re-enabled at end of calibration step.
	if (calInfo.m_nHpfOffset)
		calResult.m_pPrintPatch[calInfo.m_nHpfOffset] &= 0xFE;

	//check LNA tag in OTP
	uint8_t		pSrc[2] = { 0, 0 };
	uint8_t		pLnaValues[MS0_SIZE];
	uint8_t		pPgaValues[MS0_SIZE] = {0};
	bool		bSuccess(false);
	if (_pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pLnaValues, MS0_SIZE) > 0)
	{
		CopyToPrintPatch(&pLnaValues[4], pPrintPatch, numRows, nLnaIdx);	//skip LNA first 4 bytes 00 00 00 07
		debugInfo = "";
		for (auto i = 0; i < PrintFileInfo._uiArraySize; i++)
		{
			debugInfo += to_string(pPrintPatch[i]) + ",";
		}
		LOG(DEBUG) << debugInfo;
	}
	else
	{
		//Calculate the LNA values and put them into the print patch.
	}

	//load print file
	//_pSyn_DutCtrl->FpWritePrintFile(pPrintPatch, PrintFileInfo._uiArraySize);
	//_pSyn_DutCtrl->FpWritePrintFile(PrintFileInfo._pArrayBuf, PrintFileInfo._uiArraySize);

	//There is a bug in the FPS firmware. This bug shows when the number of cols times the
	//number of rows is a multiple of 64. To avoid this problem we add 1 to the number of rows
	//when the number of bytes in the image is a multiple of 64.
	if (((numRows * numCols) % 64) == 0)
		numRows++;

	if (calInfo.m_nHpfOffset)
		pPrintPatch[calInfo.m_nHpfOffset] &= 0xFE;

	
	uint8_t *pImgBuff = new uint8_t[numCols * numRows];

	::Sleep(100);
	//_pSyn_DutCtrl->FpGetImage(pImgBuff, numCols*numRows);
	_pSyn_DutCtrl->FpGetImage2(numRows, numCols, pImgBuff, PrintFileInfo._uiArraySize, pPrintPatch);
	//_pSyn_DutCtrl->FpGetImage2(numRows, numCols, pImgBuff, PrintFileInfo._uiArraySize, PrintFileInfo._pArrayBuf);
	//_pSyn_DutCtrl->FpGetImage2(numRows, numCols, pImgBuff, PrintFileInfo._uiArraySize-4,&PrintFileInfo._pArrayBuf[4]);

	for (int i = 0; i < numCols * numRows; i++)
	{
		calResult.m_pPrintPatch[i] = pImgBuff[i];
		std::string strTempValue = to_string(pImgBuff[i]);
		LOG(INFO) << i << " is " << strTempValue;
	}

	
	//test fill FPSFrame
	FPSFrame *pFirstFPSFrameArray = new FPSFrame[MAXFRAMES];
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			pFirstFPSFrameArray[0].arr[i][j] = pImgBuff[numCols*i + j];
		}
	}





	//If cal type is One Offset Per Pixel.
	if (calInfo.m_nCalType == kPgaCalTypeOneOffsetPerPixel)
	{
		//If the OOPP offsets exist in the OTP, put them in a more convenient location. Skip the 0x00000007 stored in 1st 4 bytes.
		//int nPgaRecCount = GetMS0RecordData(TAG_CAL, EXT_TAG_PGA_OOPP, pPgaValues, MS0_SIZE, site);
		int nPgaRecCount = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_PGA_OOPP, pPgaValues, MS0_SIZE);
		memcpy(calResult.m_pPGAOtpArray, &pPgaValues[4], NUM_PGA_OOPP_OTP_ROWS * numCols);

		bSuccess = CalculatePgaOffsets_OOPP(numCols, numRows, calInfo, calResult);
		if (!bSuccess)
		{
			//site.PushBinCodes(BinCodes::m_sStages1Or2CalFail);
			calResult.m_bPass = 0;
		}
		else
		{
			//If at least 1 PGA record exists in the OTP, and an error has NOT yet been detected.
			if ((nPgaRecCount != 0) && (calResult.m_bPass != 0))
			{
				//int32_t nVariance = OtpPgaVarianceTest(site, (int8_t*)site.m_calibrationResults.m_pPGAOtpArray, (int8_t*)site.m_calibrationResults.m_arPgaOffsets);
				//calResult.m_nStage2VarianceScore = nVariance;

				//If the variance is not within spec, record the error.
				if (calResult.m_nStage2VarianceScore > calInfo.m_nPgaVarianceLimit)
				{
					//site.PushBinCodes(BinCodes::m_sStage2VarianceFail);
					calResult.m_bPass = 0;
				}
			}
		}
	}

	for (int i = 0; i < numRows; i++)
	{
		std::string strTempRowValue;
		for (int j = 0; j < numCols; j++)
		{
			strTempRowValue += to_string(pImgBuff[i*numRows + j]) + std::string(",");
		}

		LOG(INFO) << "row " << to_string(i) << " is " << strTempRowValue;
	}

	delete[] pImgBuff;
	
	delete[]pFirstFPSFrameArray;
	pFirstFPSFrameArray = NULL;

	return true;

	
}

void Syn_Dut::GetFingerprintImage(CalibrationResults &CalResults, FPSFrame *pFrame, int nNumRows, int nNumCols, uint32_t* pCurrentDrawVals, int nGain)
{
	uint32_t	currentDrawSums[NUM_CURRENT_VALUES] = { 0 };
	uint32_t	arCurrentSenseVals[NUM_CURRENT_VALUES] = { 0 };
	int			nRows = nNumRows;
	int			nCols = nNumCols;
	uint8_t*	pImgBuf = NULL;

	//If present, write the get print patch. Must go right before image aquisition.
	/*if (GetSysConfig().GetSize("ImageAcqPatch") != 0)
	{
		GetDutCtrl().FpUnloadPatch();
		WriteImageAcqPatch(GetSysConfig().GetPtr("ImageAcqPatch"), GetSysConfig().GetSize("ImageAcqPatch"));
	}*/
	Syn_PatchInfo ImgAcqPatchInfo;
	if (FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		_pSyn_DutCtrl->FpUnloadPatch();
		_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);
	}

	//There is a bug in the FPS firmware. This bug shows when the number of cols times the
	//number of rows is a multiple of 64. To avoid this problem we add 1 to the number of rows
	//when the number of bytes in the image is a multiple of 64.
	if (((nRows * nCols) % 64) == 0)
		nRows++;

	//If the caller is requesting current draw readings.
	if (pCurrentDrawVals != NULL)
	{
		//	WritePrintFile(pCalResults);
		Syn_PatchInfo PrintPatchInfo;
		if (FindPatch("PrintFile", PrintPatchInfo))
		{
			_pSyn_DutCtrl->FpWrite(1, VCSFW_CMD::GET_PRINT, PrintPatchInfo._pArrayBuf, PrintPatchInfo._uiArraySize);
			//_pSyn_DutCtrl->FpWaitForCommandCompleteAndCheckErrorCode(2);
			_pSyn_DutCtrl->FpWaitForCMDComplete();
			_pSyn_DutCtrl->FpReadAndCheckStatus(0);
		}

		

		for (int i = 0; i<2; i++)
		{
			//Get the current draw reading while the part is active.
			_pSyn_DutCtrl->GetCurrentSenseValues(nGain, 64, arCurrentSenseVals);// DutCtrl::CURRENT_SENSE_BASE_OVERSAMPLE

			for (int curIdx = 0; curIdx<NUM_CURRENT_VALUES; curIdx++)
				currentDrawSums[curIdx] += arCurrentSenseVals[curIdx];
		}

		for (int curIdx = 0; curIdx<NUM_CURRENT_VALUES; curIdx++)
			pCurrentDrawVals[curIdx] = currentDrawSums[curIdx] / 2;


		//Wait for the sensor to generate complete image.
		int timeout = 300;
		uint8_t		pDst[4];
		_pSyn_DutCtrl->FpGetStatus(pDst, sizeof(pDst));
		while (timeout-- && ((pDst[3] & 0x02) == 0))
			_pSyn_DutCtrl->FpGetStatus(pDst, sizeof(pDst));

		if (timeout == 0)
		{
			Syn_Exception ex(0);
			ex.SetDescription("Timeout waiting for image.");
			throw ex;
		}

		//Pull sensor's image data.
		uint8_t* pImgBuf = new uint8_t[(nRows * nCols)];
		_pSyn_DutCtrl->FpGetImage(pImgBuf, nRows * nCols);
		delete[] pImgBuf;
	}
	else //if (pCurrentDrawVals == NULL)
	{
		try
		{
			//If this sensor's image is too large for it's internal buffer, use a semaphore.
			if ((nRows * nCols) > 18000)
			{
				//Use a lock.
				//CSingleLock myLock(&m_criticalSection, TRUE);

				//Get the image.
				pImgBuf = new uint8_t[(nRows * nCols)];
				_pSyn_DutCtrl->FpGetImage2(nRows, nCols, pImgBuf, CalResults.m_nPrintPatchSize - 4, &CalResults.m_pPrintPatch[4]);
				for (int i = 0; i<(nRows*nCols); i++)
					pFrame->arr[i / nCols][i % nCols] = pImgBuf[i];
				delete[] pImgBuf;
			}
			else //Don't use a lock.
			{
				//Get the image.
				pImgBuf = new uint8_t[(nRows * nCols)];
				_pSyn_DutCtrl->FpGetImage2(nRows, nCols, pImgBuf, CalResults.m_nPrintPatchSize - 4, &CalResults.m_pPrintPatch[4]);
				for (int i = 0; i<(nRows*nCols); i++)
					pFrame->arr[i / nCols][i % nCols] = pImgBuf[i];
				delete[] pImgBuf;
			}
		}
		catch (Syn_Exception e)
		{
			delete[] pImgBuf;
			throw e;
		}
	}
}

bool Syn_Dut::FindPatch(std::string patchName, Syn_PatchInfo &patchInfo)
{
	bool IsExists(false);
	for (size_t i = 1; i <= _listOfPatchInfo.size(); i++)
	{
		if (patchName == _listOfPatchInfo[i - 1]._strXepatchName)
		{
			patchInfo = _listOfPatchInfo[i - 1];
			IsExists = true;
			break;
		}
	}
	return IsExists;
}


bool Syn_Dut::CheckDUTexists()
{
	bool IsExists(true);
	_pSyn_DutCtrl->GpioSetPinType(8, 0x10, 5);

	uint32_t nPinState = 0;
	_pSyn_DutCtrl->GpioPinRead(8, 0x10, &nPinState);

	if (0x10 & nPinState)
	{
		IsExists = false;
	}

	return IsExists;
}