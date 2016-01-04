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

	for (auto x = 0; x < numBytes; x++)
	{
		cout << std::hex << *(pDst + x);
		cout << ",";
	}
	cout << endl;

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

void Syn_SPCCtrl::FpWaitForCMDComplete(uint32_t numBytes, int nTimeout_ms)
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
	//uint8_t	pDst[MAX_PRINTFILE_SIZE];
	uint8_t	pDst[4];

	uint64_t timeout = GetTickCount() + nTimeout_ms;
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


void Syn_SPCCtrl::FpWaitDeviceReady()
{
	uint8_t pDst[4];
	uint32_t timeout = 2000;

	this->FpGetStatus(pDst, 4);
	while (timeout && ((pDst[0] != 0x01) || (pDst[1] != 0x00) || (pDst[2] != 0x00) || (pDst[3] != 0x08)))
	{
		this->FpGetStatus(pDst, 4);
		timeout--;
	}

	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("Timeout waiting for sensor to wake.");
		throw(ex);
	}
	
}

void Syn_SPCCtrl::FpDisableSleep()
{
	uint8_t pSrc[2] = { 0 };
	this->FpWrite(1, 0x0057, pSrc, sizeof(pSrc));
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


bool Syn_SPCCtrl::FpGetVersion(uint8_t *pDst, int numBytes)
{
	uint16_t errorCode;

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