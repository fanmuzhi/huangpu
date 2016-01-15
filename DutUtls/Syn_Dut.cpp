//windows api
#include "windows.h"

//Local
#include "Syn_Dut.h"
#include "Syn_Viper.h"
#include "Syn_Viper1.h"
#include "Syn_Viper2.h"
#include "Syn_Metallica.h"
#include "Syn_Exception.h"

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
	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 1, &oarMS0[64], BS1_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 0, &oarMS0[128], MS1_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 1, &oarMS0[2176], MS1_SIZE);

	//::Sleep(100);

	return true;
}

bool Syn_Dut::GetFPImage()
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
	uint8_t pPrintPatch[MAX_PRINTFILE_SIZE + 4];		//put into CalibrationResult later
	uint8_t NumCols = 104;							    //put into CalibrationInfo later	
	uint8_t NumRows = 96;								//put into CalibrationInfo later
	uint8_t nLnaIdx = 1228;								//put into CalibrationInfo later

	pPrintPatch[0] = NumCols;
	pPrintPatch[1] = NumCols >> 8;
	pPrintPatch[2] = NumRows;
	pPrintPatch[3] = NumRows >> 8;

	memcpy(&pPrintPatch[4], PrintFileInfo._pArrayBuf, PrintFileInfo._uiArraySize);

	//High Pass Filter(HPF)
	//TODO
	
	//check LNA tag in OTP
	uint8_t pLnaValues[MS0_SIZE];
	if (_pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_LNA, pLnaValues, MS0_SIZE) > 0)
	{
		CopyToPrintPatch(&pLnaValues[4], pPrintPatch, NumRows, nLnaIdx);
	}
	else
	{
		//calibration LNA
	}
	
	//load ImgAcqPatch
	Syn_PatchInfo ImgAcqPatchInfo;
	if (!FindPatch("ImageAcqPatch", ImgAcqPatchInfo))
	{
		LOG(ERROR) << "Cannot find 'ImageAcqPatch' in config file";
		return false;
	}
	_pSyn_DutCtrl->FpUnloadPatch();
	_pSyn_DutCtrl->FpLoadPatch(ImgAcqPatchInfo._pArrayBuf, ImgAcqPatchInfo._uiArraySize);

	//load print file
	//_pSyn_DutCtrl->FpWritePrintFile(pPrintPatch, PrintFileInfo._uiArraySize);

	uint8_t *pImgBuff = new uint8_t[NumCols * NumRows];

	::Sleep(100);
	_pSyn_DutCtrl->FpGetImage2(NumRows, NumCols, pImgBuff, PrintFileInfo._uiArraySize - 4, &pPrintPatch[4]);
	return true;
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