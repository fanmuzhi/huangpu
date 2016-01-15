//Local
#include "Syn_SPCCtrl.h"
#include "Syn_Exception.h"
#include "Syn_Vcsfw.h"

//MPC_API
#include "MpcApiDll.h"
#include "MpcApiError.h"

//std
#include <iostream>

//third-party
#include "easylogging++.h"

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
	uint32_t	err;

	uint16_t	uiDevType;
	uint32_t	uiRevBootLoader;
	uint32_t	uiRevApplication;

	//SetValidFlg(false);

	//We want to init the DLL only once.
	if (false == _bDLLInitialized)
	{
		err = MPC_Initialize();
		if (err != MpcApiError::ERR_OK)
			_bDLLInitialized = true;
	}

	//Get the handle associated to this SPC.
	err = MPC_GetMpcDeviceHandle(syn_SerialNumber, &syn_DeviceHandle);
	::Sleep(2);

	if (err != MpcApiError::ERR_OK)
	{
		syn_DeviceHandle = NULL;
	}
	else
	{
		err = MPC_Connect(syn_DeviceHandle);
		if (err != MpcApiError::ERR_OK)
		{
			syn_DeviceHandle = NULL;
			//cout << "ERROR:Syn_SPCCtrl::Init() - cannot connect to MPC04 " << syn_SerialNumber << endl;
			LOG(ERROR) << "Cannot connect to MPC04: " << syn_SerialNumber;
		}
		else
		{
			err = MPC_GetIdentity(syn_DeviceHandle, &uiDevType, &uiRevBootLoader, &uiRevApplication, TIMEOUT);
			if (err != MpcApiError::ERR_OK)
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
				err = MPC_SetVoltages(syn_DeviceHandle, 0, 0, 0, 0, TIMEOUT);
				if (err != MpcApiError::ERR_OK)
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
	if (0!=syn_SerialNumber && NULL==syn_DeviceHandle)
	{
		//cout << "ERROR:Syn_SPCCtrl::Init() - syn_DeviceHandle is NULL!" << endl;
		LOG(ERROR) << "syn_DeviceHandle is NULL";
		return false;
	}

	if (MPC_IsConnected(syn_DeviceHandle))
	{
		// Set SPI port
		err = MPC_SetPortFpSensor(syn_DeviceHandle, 0, 0x0008, 8000, 0, 0, TIMEOUT);
		if (err != MpcApiError::ERR_OK)
		{
			syn_DeviceHandle = NULL;
			//cout << "ERROR:Syn_SPCCtrl::Init() - MPC_SetPortFpSensor failed with MPC04 " << syn_SerialNumber << endl;
			LOG(ERROR) << "MPC_SetPortFpSensor failed with MPC04: " << syn_SerialNumber;
			return false;
		}
		return true;
	}
	else
	{
		//cout << "ERROR:Syn_SPCCtrl::Init() - ::MPC_IsConnected is failed!" << endl;
		LOG(ERROR) << "MPC_IsConnected is failed!";
		return false;
	}
}

//--------------------
//Basic Function: Read
//--------------------
void Syn_SPCCtrl::FpRead(uint16_t endpoint, uint16_t command, uint8_t* pDst, int numBytes)
{
	uint16_t err;

	err = MPC_FpRead(syn_DeviceHandle, endpoint, command, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == err)
	{
		ex.SetDescription("FpRead() Controller communication failure.");
		throw ex;
	}
	else if (MpcApiError::ERR_OK != err)
	{
		ex.SetDescription("FpRead() DUT communication failure.");
		throw ex;
	}
}

//--------------------
//Basic Function: Write
//--------------------
void Syn_SPCCtrl::FpWrite(uint16_t endpoint, uint16_t command, uint8_t* pSrc, int numBytes)
{
	uint16_t err;

	err = MPC_FpWrite(syn_DeviceHandle, endpoint, command, pSrc, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == err)
	{
		ex.SetDescription("FpWrite() Controller communication failure.");
		throw ex;
	}
	else if (MpcApiError::ERR_OK != err)
	{
		ex.SetDescription("FpWrite() DUT communication failure.");
		throw ex;
	}
}


void Syn_SPCCtrl::SetVoltages(uint16_t vdd_mV, uint16_t vio_mV, uint16_t vled_mV, uint16_t vddh_mV)
{
	uint16_t err = MPC_SetVoltages(syn_DeviceHandle, vdd_mV, vio_mV, vled_mV, vddh_mV, TIMEOUT);
	Syn_Exception ex(err);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == err)
	{
		ex.SetDescription("SetVoltages() Controller communication failure.");
		throw ex;
	}
	else if (MpcApiError::ERR_OK != err)
	{
		ex.SetDescription("SetVoltages() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpGetStatus(uint8_t* pDst, int numBytes)
{
	uint16_t endpoint = 0x00;
	uint16_t cmd = 0x00;

	this->FpRead(endpoint, cmd, pDst, numBytes);

	//cout << "FpGetStatus(): 0x" << hex << *((uint32_t*)pDst) << endl;
	LOG(DEBUG) << "0x" << hex << *((uint32_t*)pDst);
}

void Syn_SPCCtrl::FpWaitForCMDComplete()
{
	uint32_t timeout = TIMEOUT;
	uint8_t	pDst[4];

	this->FpGetStatus(pDst, 4);
	while (timeout && (*((uint32_t*)pDst) != 0x30000003))
	{
		this->FpGetStatus(pDst, 4);
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("FpWaitForCommandCompleteAndReturnErrorCode() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpReadBuff(uint8_t *pDst, int numBytes)
{
	uint32_t timeout = TIMEOUT;

	this->FpRead(1, 0xFF, pDst, numBytes);
	while (timeout && (*((uint16_t*)pDst) == 0xFFFF))
	{
		this->FpRead(1, 0xFF, pDst, numBytes);
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("FpReadBuff() DUT communication failure.");
		throw ex;
	}

	//return ((pDst[0] << 8) + pDst[1]);
}


void Syn_SPCCtrl::FpReadAndCheckStatus(uint16_t statusIgnore)
{
	uint8_t numbytes = 2;
	uint8_t pDst[4] = { 0 };
	this->FpReadBuff(pDst, numbytes);
	
	//uint16_t returnValue = (pDst[0] << 8) + pDst[1];
	uint16_t returnValue = *((uint16_t*)pDst);
	LOG(DEBUG) << "0x" << hex << returnValue;

	if (statusIgnore != returnValue && returnValue != 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("FpReadAndCheckBuff() DUT return value unmatch.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpWaitDeviceReady()
{
	//cout << "FpWaitDeviceReady():" << endl;
	LOG(INFO) << "Wait Device Ready";

	uint8_t pDst[4];
	uint32_t timeout = TIMEOUT;

	this->FpGetStatus(pDst, 4);
	while (timeout && (*((uint32_t*)pDst) != 0x08000001))
	{
		this->FpGetStatus(pDst, 4);
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(0);
		ex.SetDescription("FpWaitDeviceReady() Timeout waiting for sensor to wake.");
		throw(ex);
	}
}

void Syn_SPCCtrl::FpDisableSleep()
{
	//cout << "FpDisableSleep():" << endl;
	LOG(INFO) << "Disable Sleep";

	uint8_t pSrc[2] = { 0 };
	this->FpWrite(1, VCSFW_CMD::TIDLE_SET, pSrc, sizeof(pSrc));
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0);
}

void Syn_SPCCtrl::FpLoadPatch(uint8_t* pPatch, int numBytes)
{
	//cout << "FpLoadPatch():" << endl;
	LOG(INFO) << "Load Patch";

	this->FpWrite(1, VCSFW_CMD::PATCH, pPatch, numBytes);
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0);
}

void Syn_SPCCtrl::FpUnloadPatch()
{
	//cout << "FpUnLoadPatch():" << endl;
	LOG(INFO) << "Unload Patch";

	this->FpWrite(1, VCSFW_CMD::UNLOAD_PATCH, (uint8_t*)0, 0);
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0x0491);
}

void Syn_SPCCtrl::FpOtpRomRead(int section, int sector, uint8_t* pDst, int numBytes)
{
	LOG(INFO) << "OTPRom Read";

	uint16_t err;

	err = MPC_FpOtpRomRead(syn_DeviceHandle, section, sector, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetDescription("FpOtpRomRead() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetDescription("FpOtpRomRead() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpOtpRomWrite(int section, int sector, uint8_t* pDst, int numBytes)
{
	LOG(INFO) << "OTPRom Write";

	uint16_t err;
	err = MPC_FpOtpRomWrite(syn_DeviceHandle, section, sector, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetDescription("FpOtpRomWrite() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetDescription("FpOtpRomWrite() DUT communication failure.");
		throw ex;
	}
}

uint8_t Syn_SPCCtrl::FpOtpRomTagRead(uint32_t nExtTag, uint8_t* pDst, int numBytes)
{
	uint8_t nRecCount = 0;
	LOG(INFO) << "OTPRom Tag Read";

	uint8_t arReadMs0Args[8] = { 0 };
	arReadMs0Args[1] = TAG_CAL;
	*((uint32_t*)(&arReadMs0Args[4])) = nExtTag;

	this->FpWrite(1, VCSFW_CMD::OTPROM_TAG_FIND, arReadMs0Args, sizeof(arReadMs0Args));
	this->FpWaitForCMDComplete();
	this->FpReadBuff(pDst, numBytes);
	
	nRecCount = (uint8_t)(*(uint32_t*)&pDst[6]);
	memcpy(pDst, &pDst[22], numBytes - 22);

	return nRecCount;
}


void Syn_SPCCtrl::FpGetVersion(uint8_t *pDst, int numBytes)
{
	LOG(INFO) << "Get Version";

	uint16_t err;
	err = MPC_FpGetVersion(syn_DeviceHandle, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetDescription("FpGetVersion() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetDescription("FpGetVersion() DUT communication failure.");
		throw ex;
	}
}


void Syn_SPCCtrl::FpWritePrintFile(uint8_t *pPrintPatch, int numBytes)
{
	LOG(INFO) << "Write Print File";

	this->FpWrite(1, VCSFW_CMD::GET_PRINT, pPrintPatch, numBytes);
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0);
}


	
void Syn_SPCCtrl::FpGetImage2(uint16_t nRows, uint16_t nCols, uint8_t *pDst, uint16_t nBlobSize, uint8_t *pBlob)
{
	LOG(INFO) << "Get Image 2";
	
	uint16_t err;
	//this->FpWaitDeviceReady();
	
	err = MPC_FpGetImage2(syn_DeviceHandle, nRows, nCols, pDst, nBlobSize, pBlob, NULL, NULL, NULL, TIMEOUT);
}
	
