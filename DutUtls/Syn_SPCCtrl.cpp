//windows
//#include <afx.h>
////#include "stdafx.h"
//#include <windows.h>
//#include <stdio.h>
//#include <tchar.h>

//Local
#include "Syn_SPCCtrl.h"
#include "Syn_Exception.h"

//MPC_API
#include "MpcApiDll.h"
#include "MpcApiError.h"
#include "MPCErrors.h"

//std
#include <iostream>

bool Syn_SPCCtrl::_bDLLInitialized = false;

Syn_SPCCtrl::Syn_SPCCtrl(uint32_t iSerialNumber)
:Syn_DutCtrl(iSerialNumber)
{
	//syn_SerialNumber = iSerialNumber;
	bool bResult = Init();
	if (!bResult)
	{
		return;
	}
}

Syn_SPCCtrl::~Syn_SPCCtrl()
{
	if (NULL!=syn_DeviceHandle)
	{
		MPC_Disconnect(syn_DeviceHandle);
		MPC_CloseMpcDeviceHandle(syn_DeviceHandle);
	}
}

bool Syn_SPCCtrl::Init()
{
	uint32_t	error;

	uint16_t	uiDevType;
	uint32_t	uiRevBootLoader;
	uint32_t	uiRevApplication;

	//SetValidFlg(false);

	//We want to init the DLL only once.
	if (false == _bDLLInitialized)
	{
		error = MPC_Initialize();
		if (0 != error)
			_bDLLInitialized = true;
	}

	//Get the handle associated to this SPC.
	error = MPC_GetMpcDeviceHandle(syn_SerialNumber, &syn_DeviceHandle);

	MPC_Connect(syn_DeviceHandle);
	::Sleep(2);
	MPC_IsConnected(syn_DeviceHandle);

	if (0 != error)
	{
		syn_DeviceHandle = NULL;
	}
	else
	{
		error = MPC_Connect(syn_DeviceHandle);
		if (0 != error)
		{
			syn_DeviceHandle = NULL;
		}
		else
		{
			error = MPC_GetIdentity(syn_DeviceHandle, &uiDevType, &uiRevBootLoader, &uiRevApplication, 2000);
			if (0 != error)
			{
				MPC_Disconnect(syn_DeviceHandle);
				MPC_CloseMpcDeviceHandle(syn_DeviceHandle);
				syn_DeviceHandle = NULL;
			}
			else
			{
				//If necessary, update the firmware.
				//UpdateMPC04Firmware(uiDevType, uiRevBootLoader, uiRevApplication);
				//SetValidFlg(true);

				//Set the MPC04 voltages to zero.
				error = MPC_SetVoltages(syn_DeviceHandle, 0, 0, 0, 0, 2000);
				if (0!=error)
				{
					//SetValidFlg(false);
					MPC_Disconnect(syn_DeviceHandle);
					MPC_CloseMpcDeviceHandle(syn_DeviceHandle);
					syn_DeviceHandle = NULL;
				}
			}
		}
	}

	//If this site has been assigned a serial number and the connection was NOT successful.
	if (0!=syn_SerialNumber&&NULL==syn_DeviceHandle)
	{
		cout << "Error:Syn_SPCCtrl::Init() - syn_DeviceHandle is NULL!" << endl;
		return false;
	}

	if (MPC_IsConnected(syn_DeviceHandle))
	{
		// Set SPI port
		MPC_SetPortFpSensor(syn_DeviceHandle, 0, 0x0008, 8000, 0, 0, 2000);
		return true;
	}
	else
	{
		cout << "Error:Syn_SPCCtrl::Init() - ::MPC_IsConnected is failed!" << endl;
		return false;
	}

	return true;
}

void Syn_SPCCtrl::SetVoltages(uint16_t vdd_mV, uint16_t vio_mV, uint16_t vled_mV, uint16_t vddh_mV)
{
	uint16_t ErrorCode = MPC_SetVoltages(syn_DeviceHandle, vdd_mV, vio_mV, vled_mV, vddh_mV, 2000);
	Syn_Exception Exception(ErrorCode);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == ErrorCode)
	{
		Exception.SetDescription("SetVoltages() Controller communication failure.");
		throw Exception;
	}
	else if (Errors::NO_MPC_ERROR != ErrorCode)
	{
		Exception.SetDescription("SetVoltages() DUT communication failure.");
		throw Exception;
	}
	
}

void Syn_SPCCtrl::FpGetStatus(uint8_t* pDst, int numBytes)
{
	uint16_t ErrorCode;

	ErrorCode = MPC_FpGetStatus(syn_DeviceHandle, pDst, numBytes, 2000);
	Syn_Exception Exception(ErrorCode);
	if (MpcApiError::ERR_COMMUNICATION_FAILED==ErrorCode)
	{
		Exception.SetDescription("FpGetStatus() Controller communication failure.");
		throw Exception;
	}
	else if (Errors::NO_MPC_ERROR!=ErrorCode)
	{
		Exception.SetDescription("FpGetStatus() DUT communication failure.");
		throw Exception;
	}
}

void Syn_SPCCtrl::FpRead(uint16_t endpoint, uint16_t command, uint8_t* pDst, int numBytes)
{
	uint16_t ErrorCode;

	ErrorCode = MPC_FpRead(syn_DeviceHandle, endpoint, command, pDst, numBytes, 2000);
	Syn_Exception Exception(ErrorCode);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == ErrorCode)
	{
		Exception.SetDescription("FpRead() Controller communication failure.");
		throw Exception;
	}
	else if (Errors::NO_MPC_ERROR != ErrorCode)
	{
		Exception.SetDescription("FpRead() DUT communication failure.");
		throw Exception;
	}
}

void Syn_SPCCtrl::FpWrite(uint16_t endpoint, uint16_t command, uint8_t* pSrc, int numBytes)
{
	uint16_t ErrorCode;

	ErrorCode = MPC_FpWrite(syn_DeviceHandle, endpoint, command, pSrc, numBytes, 2000);
	Syn_Exception Exception(ErrorCode);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == ErrorCode)
	{
		Exception.SetDescription("FpWrite() Controller communication failure.");
		throw Exception;
	}
	else if (Errors::NO_MPC_ERROR != ErrorCode)
	{
		Exception.SetDescription("FpWrite() DUT communication failure.");
		throw Exception;
	}
}





void Syn_SPCCtrl::FpWaitForCommandCompleteAndCheckErrorCode(uint32_t numBytes, int nTimeout_ms)
{
	uint16_t ErrorCode = FpWaitForCommandCompleteAndReturnErrorCode(numBytes, nTimeout_ms);
	if (ErrorCode != 0 && ErrorCode != 0xA605)
	{
		Syn_Exception Exception(ErrorCode);
		Exception.SetDescription("FpWaitForCommandCompleteAndCheckErrorCode() DUT communication failure.");
		throw Exception;
	}
}

uint16_t Syn_SPCCtrl::FpWaitForCommandCompleteAndReturnErrorCode(uint32_t numBytes, int nTimeout_ms)
{
	uint8_t	pDst[MAX_PRINTFILE_SIZE];

	DWORD timeout = GetTickCount() + nTimeout_ms;
	FpGetStatus(pDst, 4);
	while ((timeout > GetTickCount()) && (*((uint32_t*)pDst) != 0x30000003))
		FpGetStatus(pDst, 4);

	if (timeout < GetTickCount())
	{
		Syn_Exception Exception(0);
		Exception.SetDescription("FpWaitForCommandCompleteAndReturnErrorCode() DUT communication failure.");
		throw Exception;
	}

	//Check error code. The 'command' value(0xFF) for FpRead are don't cares.
	FpRead(1, 0xFF, pDst, numBytes);
	while ((timeout > GetTickCount()) && (*((uint16_t*)pDst) == 0xFFFF))
		FpRead(1, 0xFF, pDst, numBytes);

	return ((pDst[0] << 8) + pDst[1]);
}

void Syn_SPCCtrl::FpLoadPatch(uint8_t* pPatch, int numBytes)
{
	uint16_t ErrorCode;

	ErrorCode = MPC_FpLoadPatch(syn_DeviceHandle, pPatch, numBytes, 2000);

	Syn_Exception Exception(ErrorCode);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == ErrorCode)
	{
		Exception.SetDescription("FpLoadPatch() Controller communication failure.");
		throw Exception;
	}
	else if (Errors::NO_MPC_ERROR != ErrorCode)
	{
		Exception.SetDescription("FpLoadPatch() DUT communication failure.");
		throw Exception;
	}
}

void Syn_SPCCtrl::FpUnloadPatch()
{
	uint16_t ErrorCode;

	ErrorCode = MPC_FpUnloadPatch(syn_DeviceHandle, 2000);

	Syn_Exception Exception(ErrorCode);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == ErrorCode)
	{
		Exception.SetDescription("FpUnloadPatch() Controller communication failure.");
		throw Exception;
	}
	else if (Errors::NO_MPC_ERROR != ErrorCode)
	{
		Exception.SetDescription("FpUnloadPatch() DUT communication failure.");
		throw Exception;
	}
}

void Syn_SPCCtrl::FpOtpRomRead(int section, int sector, uint8_t* pDst, int numBytes)
{
	uint16_t ErrorCode;

	ErrorCode = MPC_FpOtpRomRead(syn_DeviceHandle, section, sector, pDst, numBytes, 2000);
	Syn_Exception Exception(ErrorCode);
	if (ErrorCode == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		Exception.SetDescription("FpOtpRomRead() Controller communication failure.");
		throw Exception;
	}
	else if (ErrorCode != Errors::NO_MPC_ERROR)
	{
		Exception.SetDescription("FpOtpRomRead() DUT communication failure.");
		throw Exception;
	}
}

void Syn_SPCCtrl::FpOtpRomWrite(int section, int sector, uint8_t* pDst, int numBytes)
{
	uint16_t ErrorCode;
	ErrorCode = MPC_FpOtpRomWrite(syn_DeviceHandle, section, sector, pDst, numBytes, 2000);
	Syn_Exception Exception(ErrorCode);
	if (ErrorCode == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		Exception.SetDescription("FpOtpRomWrite() Controller communication failure.");
		throw Exception;
	}
	else if (ErrorCode != Errors::NO_MPC_ERROR)
	{
		Exception.SetDescription("FpOtpRomWrite() DUT communication failure.");
		throw Exception;
	}
}




















bool Syn_SPCCtrl::PowerOn(uint8_t *pDst, int numBytes)
{
	uint16_t error;
	// Power on sensor
	error = MPC_SetVoltages(syn_DeviceHandle, 1800, 1800, 3300, 3300, 200);
	::Sleep(5);

	// Disable Sleep
	//uint8_t pDst[4] = { 0 };
	error = MPC_FpGetStatus(syn_DeviceHandle, pDst, numBytes, 2000);
	uint8_t timeout = 200;
	while (timeout && ((pDst[0] != 0x01) || (pDst[1] != 0x00) || (pDst[2] != 0x00) || (pDst[3] != 0x08)))
	{
		error = MPC_FpGetStatus(syn_DeviceHandle, pDst, numBytes, 2000);
		timeout--;
	}

	uint8_t pSrc[2] = { 0 };
	error = MPC_FpWrite(syn_DeviceHandle, 1, 0x0057, pSrc, sizeof(pSrc), 200);
	error = MPC_FpGetStatus(syn_DeviceHandle, pDst, numBytes, 2000);

	return true;
}



bool Syn_SPCCtrl::FpGetVersion(uint8_t *pDst, int numBytes)
{
	uint16_t errorCode;

	//Test


	errorCode = MPC_FpGetVersion(syn_DeviceHandle, pDst, numBytes, 2000);
	Syn_Exception Exception(errorCode);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == errorCode)
	{
		Exception.SetDescription("FpGetVersion() Controller communication failure.");
		throw Exception;
	}
	else if (Errors::NO_MPC_ERROR != errorCode)
	{
		Exception.SetDescription("FpGetVersion() DUT communication failure.");
		throw Exception;
	}

	return true;
}

bool Syn_SPCCtrl::PowerOff()
{
	uint16_t error;
	// Power on sensor
	error = MPC_SetVoltages(syn_DeviceHandle, 0, 0, 0, 0, 200);
	::Sleep(5);

	return true;
}




/*void Syn_SPCCtrl::UpdateMPC04Firmware(uint16_t nDevType, uint32_t nRevBootLoader, uint32_t nRevApplication)
{
	uint16_t error;
	CString str;
	int fileAppRev, fileBootloaderRev, bSuccess = 0;
	bool AppUpdate, BootloaderUpdate, sequenceFound;
	char filename_application[200];
	char filename_bootloader[200];
	FILE *pFileApplication, *pFileBootloader;
	char line[44] = { 0 };

	sprintf_s(filename_application, "Mpc04Application.hex");
	sprintf_s(filename_bootloader, "Mpc04BootloaderLoader.hex");
	pFileApplication = fopen(filename_application, "r");
	pFileBootloader = fopen(filename_bootloader, "r");
	if ((pFileApplication == NULL) || ((pFileBootloader == NULL)))
	{
		str.Format("MPC04 firmware files not found!\nTesting will continue without updating the MPC04.");
		//AfxMessageBox(str, MB_OK | MB_SYSTEMMODAL);
		return;
	}
	else
	{
		AppUpdate = false;
		BootloaderUpdate = false;
		sequenceFound = false;
		//Acquire the bootloader version from Mpc04BootloaderLoader.hex
		while ((fgets(line, sizeof line, pFileBootloader) != NULL) && (!sequenceFound))
		{
			for (int i = 0; i<44 - 7; i++)
			{
				if ((line[i + 0] == '4') && (line[i + 1] == 'C') && (line[i + 2] == '5') && (line[i + 3] == '0') && (line[i + 4] == '5') && (line[i + 5] == '0') && (line[i + 6] == '4') && (line[i + 7] == '1'))
				{
					fgets(line, sizeof line, pFileBootloader);
					fgets(line, sizeof line, pFileBootloader);
					sequenceFound = true;
					str.Format("%c%c%c%c%c%c%c%c", line[15], line[16], line[13], line[14], line[11], line[12], line[9], line[10]);
					sscanf(str, "%x", &fileBootloaderRev);

					if (((int)nRevBootLoader != fileBootloaderRev))
						BootloaderUpdate = true;

					break;
				}
			}
		}
		//Acquire the firmware version from Mpc04Application.hex
		sequenceFound = false;
		while ((fgets(line, sizeof line, pFileApplication) != NULL) && (!sequenceFound))
		{
			for (int i = 0; i<44 - 7; i++)
			{
				if ((line[i + 0] == '4') && (line[i + 1] == 'C') && (line[i + 2] == '5') && (line[i + 3] == '0') && (line[i + 4] == '5') && (line[i + 5] == '0') && (line[i + 6] == '4') && (line[i + 7] == '1'))
				{
					fgets(line, sizeof line, pFileApplication);
					fgets(line, sizeof line, pFileApplication);
					sequenceFound = true;
					str.Format("%c%c%c%c%c%c%c%c", line[15], line[16], line[13], line[14], line[11], line[12], line[9], line[10]);
					sscanf(str, "%x", &fileAppRev);

					if (((int)nRevApplication != fileAppRev))
						AppUpdate = true;

					break;
				}
			}
		}
	}

	uint32_t serNum;
	MPC_GetDeviceSerialNumber(syn_DeviceHandle, &serNum);
	if ((BootloaderUpdate))
	{
		str.Format("MPC04 Bootloader will be updated.\nPlease wait...");
		//AfxMessageBox(str, MB_OK);

		//Create a modless progress dialog.
		

		//Asynchronously, start the update.
		error = MPC_AsyncUpdateFirmware(syn_DeviceHandle, filename_bootloader, NULL, NULL, NULL);

		//Update the modless progress dialog.
		uint32_t nPercent = 0;
		while ((nPercent < 100) && (error == Errors::NO_MPC_ERROR))
		{
			error = MPC_GetUpdateFirmwareProgress(syn_DeviceHandle, &nPercent);
		}

		//Wait for the Update command to complete.
		::Sleep(6000);

		if (error == Errors::NO_MPC_ERROR)
			error = MPC_GetMpcDeviceHandle(serNum, &syn_DeviceHandle);
		if (error == Errors::NO_MPC_ERROR)
			error = MPC_Connect(syn_DeviceHandle);

		if (error == Errors::NO_MPC_ERROR)
		{
		}
		//else
	}

	if (AppUpdate || BootloaderUpdate)
	{
		str.Format("MPC04 Firmware will be updated.\nPlease wait...");
		//AfxMessageBox(str, MB_OK);

		//Create a modless progress dialog.

		//Asynchronously, start the update.
		error = MPC_AsyncUpdateFirmware(syn_DeviceHandle, filename_application, NULL, NULL, NULL);

		//When the percent goes to zero it means the API has started the download.
		//We want to wait until the download is started.
		uint32_t nPercent;
		error = MPC_GetUpdateFirmwareProgress(syn_DeviceHandle, &nPercent);
		while ((nPercent == 100) && (error == Errors::NO_MPC_ERROR))
			error = MPC_GetUpdateFirmwareProgress(syn_DeviceHandle, &nPercent);

		//Update the modeless progress dialog.
		while ((nPercent < 100) && (error == Errors::NO_MPC_ERROR))
		{
			error = MPC_GetUpdateFirmwareProgress(syn_DeviceHandle, &nPercent);
		}

		//Wait for the Update command to complete.
		::Sleep(8000);

		if (error == Errors::NO_MPC_ERROR)
			error = MPC_GetMpcDeviceHandle(serNum, &syn_DeviceHandle);
		if (error == Errors::NO_MPC_ERROR)
			error = MPC_Connect(syn_DeviceHandle);

		if (error == Errors::NO_MPC_ERROR)
		{
		}
		
		//else
	}
}*/