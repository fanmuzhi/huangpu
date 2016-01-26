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
	//uint8_t* pPrintPatch = new uint8_t[PrintFileInfo._uiArraySize];		//put into CalibrationResult later
	memcpy(calResult.m_pPrintPatch, PrintFileInfo._pArrayBuf, PrintFileInfo._uiArraySize);
	calResult.m_nPrintPatchSize = PrintFileInfo._uiArraySize;


	//High Pass Filter(HPF)
	//If the configuration file specifies the High Pass Filter (HPF) be disabled during calibration.
	//Will be re-enabled at end of calibration step.
	if (calInfo.m_nHpfOffset)
		calResult.m_pPrintPatch[calInfo.m_nHpfOffset] &= 0xFE;
	
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

	//check LNA tag in OTP
	uint8_t		pSrc[2] = { 0, 0 };
	uint8_t		pLnaValues[MS0_SIZE];
	uint8_t		pPgaValues[MS0_SIZE] = {0};
	bool		bSuccess(false);
	uint32_t nLnaIdx = calInfo.m_nLnaIdx;			
	if (_pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pLnaValues, MS0_SIZE) > 0)
	{
		CopyToPrintPatch(&pLnaValues[4], calResult.m_pPrintPatch, numRows, nLnaIdx);	//skip LNA first 4 bytes 00 00 00 07
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
	//_pSyn_DutCtrl->FpGetImage2(numRows, numCols, pImgBuff, PrintFileInfo._uiArraySize, PrintFileInfo._pArrayBuf);
	//_pSyn_DutCtrl->FpGetImage2(numRows, numCols, pImgBuff, PrintFileInfo._uiArraySize-4,&PrintFileInfo._pArrayBuf[4]);

	/*
	for (int i = 0; i < numCols * numRows; i++)
	{
		std::string strTempValue = to_string(pImgBuff[i]);
		LOG(INFO) << i << " is " << strTempValue;
	}*/
	
	
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

	FPSFrame *pFrame = new FPSFrame();
	GetFingerprintImage(calResult, pFrame, numRows, numCols);

	//calResult.testarr_calibration = *pFrame;

	delete pFrame;
	
	return true;
}

void Syn_Dut::GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols)
{
	int			nRows = nNumRows;
	int			nCols = nNumCols;

	if (((nRows * nCols) % 64) == 0)
		nRows++;
	
	uint8_t *pImgBuff = new uint8_t[nCols * nRows];

	//_pSyn_DutCtrl->FpGetImage(pImgBuff, numCols*numRows);
	_pSyn_DutCtrl->FpGetImage2(nRows, nCols, pImgBuff, pCalResults.m_nPrintPatchSize, pCalResults.m_pPrintPatch);


	for (int i = 0; i < (nRows * nCols); i++)
	{
		pFrame->arr[i / nCols][i%nCols] = pImgBuff[i];
	}

	for (int i = 0; i < nRows; i++)
	{
		std::string strTempRowValue;
		for (int j = 0; j < nCols; j++)
		{
			strTempRowValue += to_string(pImgBuff[i*nRows + j]) + std::string(",");
		}

		LOG(INFO) << "row " << to_string(i) << " is " << strTempRowValue;
	}
	
	delete[] pImgBuff;
	pImgBuff = NULL;
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