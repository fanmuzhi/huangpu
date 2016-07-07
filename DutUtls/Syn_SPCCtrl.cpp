//Local
#include "Syn_SPCCtrl.h"
#include "Syn_Exception.h"
#include "Syn_Vcsfw.h"

//MPC_API
#include "MpcApiDll.h"
#include "MpcApiError.h"

//std
#include <iostream>

//#include "afx.h"

//third-party
//#include "easylogging++.h"


bool Syn_SPCCtrl::_bDLLInitialized = false;

Syn_SPCCtrl::Syn_SPCCtrl(uint32_t iSerialNumber)
:Syn_DutCtrl(iSerialNumber)
{
	this->Init();

	//UpdateMPC04Firmware();
}

Syn_SPCCtrl::~Syn_SPCCtrl()
{
	if (NULL!=syn_DeviceHandle)
	{
		try
		{
			MPC_Disconnect(syn_DeviceHandle);
			MPC_CloseMpcDeviceHandle(syn_DeviceHandle);
		}
		catch (...){}
	}
}

void Syn_SPCCtrl::Init()
{
	uint32_t	err(0);

	//uint16_t	uiDevType;
	//uint32_t	uiRevBootLoader;
	//uint32_t	uiRevApplication;

	Syn_Exception ex(err);

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
			ex.SetError(err);
			ex.SetDescription("Cannot connect to MPC04:" + to_string(syn_SerialNumber));
			throw ex;
		}
	}

	//If this site has been assigned a serial number and the connection was NOT successful.
	if (0!=syn_SerialNumber && NULL==syn_DeviceHandle)
	{
		ex.SetError(err);
		ex.SetDescription("syn_DeviceHandle is NULL:" + to_string(syn_SerialNumber));
		throw ex;
	}

	if (MPC_IsConnected(syn_DeviceHandle))
	{
		// Set SPI port
		err = MPC_SetPortFpSensor(syn_DeviceHandle, 0, 0x0008, 8000, 0, 0, TIMEOUT);
		if (err != MpcApiError::ERR_OK)
		{
			syn_DeviceHandle = NULL;

			ex.SetError(err);
			ex.SetDescription("MPC_SetPortFpSensor failed with MPC04:" + to_string(syn_SerialNumber));
			throw ex;
		}
	}
	else
	{
		ex.SetError(err);
		ex.SetDescription("MPC_IsConnected is failed:" + to_string(syn_SerialNumber));
		throw ex;
	}

	return;
}

//--------------------
//Basic Function: Read
//--------------------
void Syn_SPCCtrl::FpRead(uint16_t endpoint, uint16_t command, uint8_t* pDst, int numBytes)
{
	uint32_t err;

	err = MPC_FpRead(syn_DeviceHandle, endpoint, command, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == err)
	{
		ex.SetError(err);
		ex.SetDescription("FpRead() Controller communication failure.");
		throw ex;
	}
	else if (MpcApiError::ERR_OK != err)
	{
		ex.SetError(err);
		ex.SetDescription("FpRead() DUT communication failure.");
		throw ex;
	}
}

//--------------------
//Basic Function: Write
//--------------------
void Syn_SPCCtrl::FpWrite(uint16_t endpoint, uint16_t command, uint8_t* pSrc, int numBytes)
{
	uint32_t err;

	err = MPC_FpWrite(syn_DeviceHandle, endpoint, command, pSrc, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == err)
	{
		ex.SetError(err);
		ex.SetDescription("FpWrite() Controller communication failure.");
		throw ex;
	}
	else if (MpcApiError::ERR_OK != err)
	{
		ex.SetError(err);
		ex.SetDescription("FpWrite() DUT communication failure.");
		throw ex;
	}
}


void Syn_SPCCtrl::SetVoltages(uint16_t vdd_mV, uint16_t vio_mV, uint16_t vled_mV, uint16_t vddh_mV)
{
	uint32_t err = MPC_SetVoltages(syn_DeviceHandle, vdd_mV, vio_mV, vled_mV, vddh_mV, TIMEOUT);
	Syn_Exception ex(err);
	if (MpcApiError::ERR_COMMUNICATION_FAILED == err)
	{
		ex.SetError(err);
		ex.SetDescription("SetVoltages() Controller communication failure.");
		throw ex;
	}
	else if (MpcApiError::ERR_OK != err)
	{
		ex.SetError(err);
		ex.SetDescription("SetVoltages() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpGetStatus(uint8_t* pDst, int numBytes)
{
	uint16_t endpoint = 0x00;
	uint16_t cmd = 0x00;

	this->FpRead(endpoint, cmd, pDst, numBytes);

	//LOG(INFO) << "0x" << hex << *((uint32_t*)pDst);
}

void Syn_SPCCtrl::FpWaitForCMDComplete(const uint32_t timeout)
{
	uint8_t	pDst[4];
	uint32_t _timeout = timeout;

	this->FpGetStatus(pDst, 4);

	//status 10000003: 1 = reply ready +  3= processed command
	//status 30000003: 3 = DRDY deaaserted and reply ready +  3= processed command
	while (_timeout && ((*((uint32_t*)pDst) & 0x10000003) != 0x10000003))
	{
		this->FpGetStatus(pDst, 4);
		_timeout--;
	}
	if (_timeout == 0)
	{
		Syn_Exception ex(MpcApiError::ERR_COMMUNICATION_FAILED);
		ex.SetDescription("FpWaitForCommandCompleteAndR1eturnErrorCode() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpReadBuff(uint8_t *pDst, int numBytes)
{
	uint32_t timeout = TIMEOUT;

	this->FpRead(1, 0xFF, pDst, numBytes);
	uint16_t returnValue = *((uint16_t*)pDst);
	//LOG(INFO) << "0x" << hex << returnValue;
	while (timeout && (*((uint16_t*)pDst) == 0xFFFF))
	{
		this->FpRead(1, 0xFF, pDst, numBytes);
		timeout--;
	}
	if (timeout == 0)
	{
		Syn_Exception ex(MpcApiError::ERR_COMMUNICATION_FAILED);
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

	if (statusIgnore != returnValue && returnValue != 0)
	{
		Syn_Exception ex(MpcApiError::ERR_COMMUNICATION_FAILED);
		ex.SetDescription("FpReadAndCheckStatus() DUT return status: " + to_string(returnValue));
		throw ex;
	}
}

void Syn_SPCCtrl::FpRunPatchTest(uint8_t *pDst, int numBytes)
{
	uint32_t timeout = 1000;

	uint8_t numbytes = 2;
	uint8_t pSrc[10];
	uint8_t pResult[2];

	this->FpWrite(1, VCSFW_CMD::TEST_RUN, pSrc, 0);
	//::Sleep(2000);

	this->FpWaitForCMDComplete(timeout);
	//this->FpNoop();
	this->FpRead(1, 0xFF, pResult, sizeof(pResult));

	::Sleep(500);
	//this->FpWaitDeviceReady();

	this->FpWrite(1, VCSFW_CMD::TEST_READ, pSrc, 0);//253
	this->FpWaitForCMDComplete(timeout);
	this->FpRead(1, 0x00FF, pDst, numBytes);
}

void Syn_SPCCtrl::FpWaitDeviceReady()
{
	//LOG(INFO) << "Wait Device Ready";

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
		Syn_Exception ex(MpcApiError::ERR_COMMUNICATION_FAILED);
		ex.SetDescription("FpWaitDeviceReady() Timeout waiting for sensor to wake.");
		throw(ex);
	}
}

void Syn_SPCCtrl::FpDisableSleep()
{
	//LOG(INFO) << "Disable Sleep";

	uint8_t pSrc[2] = { 0 };
	this->FpWrite(1, VCSFW_CMD::TIDLE_SET, pSrc, sizeof(pSrc));
	//this->FpWaitForCMDComplete();
	//this->FpReadAndCheckStatus(0);
}

void Syn_SPCCtrl::FpEnterSleep()
{
	//LOG(INFO) << "Enable Sleep";

	uint8_t pSrc[2] = { 0xE8, 0x03 };	//delay time to enter into retain mode.
	this->FpWrite(1, VCSFW_CMD::TIDLE_SET, pSrc, sizeof(pSrc));
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0);
}


void Syn_SPCCtrl::FpNoop()
{
	//LOG(INFO) << "No Op";

	uint8_t pSrc[2] = { 0 };
	this->FpWaitDeviceReady();
	this->FpWrite(1, VCSFW_CMD::NOOP, pSrc, sizeof(pSrc));
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0);
}

void Syn_SPCCtrl::FpLoadRamPatch(uint8_t* pPatch, int patchSize, uint8_t* pDst, int numBytes)
{
	//LOG(INFO) << "Load RAM Patch";

	this->FpWrite(1, VCSFW_CMD::PATCH, pPatch, patchSize);
	this->FpWaitForCMDComplete();
	this->FpRead(1, 0xFF, pDst, numBytes);
}

void Syn_SPCCtrl::FpLoadPatch(uint8_t* pPatch, int numBytes)
{
	//LOG(INFO) << "Load Patch";

	this->FpWrite(1, VCSFW_CMD::PATCH, pPatch, numBytes);
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0);
}

void Syn_SPCCtrl::FpUnloadPatch()
{
	//LOG(INFO) << "Unload Patch";

	this->FpWrite(1, VCSFW_CMD::UNLOAD_PATCH, (uint8_t*)0, 0);
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0x0491);
}

void Syn_SPCCtrl::FpOtpRomRead(int section, int sector, uint8_t* pDst, int numBytes)
{
	//LOG(INFO) << "OTPRom Read";

	uint32_t err;

	err = MPC_FpOtpRomRead(syn_DeviceHandle, section, sector, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetError(err);
		ex.SetDescription("FpOtpRomRead() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("FpOtpRomRead() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpOtpRomWrite(int section, int sector, uint8_t* pDst, int numBytes)
{
	//section =  0 main section || 1 boot section
	//sector  =  1 sector1 ||  0 sector0
	//e.g. [0, 0] for main sector 0, [0, 1] for main sector 1 
	//     [1, 0] for boot sector 0, [1, 1] for boot sector 1

	//LOG(INFO) << "OTPRom Write";

	uint32_t err;
	err = MPC_FpOtpRomWrite(syn_DeviceHandle, section, sector, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetError(err);
		ex.SetDescription("FpOtpRomWrite() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("FpOtpRomWrite() DUT communication failure.");
		throw ex;
	}
}

uint8_t Syn_SPCCtrl::FpOtpRomTagRead(uint32_t nExtTag, uint8_t* pDst, int numBytes)
{
	uint8_t nRecCount = 0;
	//LOG(INFO) << "OTPRom Tag Read";

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


void Syn_SPCCtrl::FpOtpRomTagWrite(uint8_t* pDst, int numBytes)
{
	//LOG(INFO) << "OTPRom Tag Write";

	this->FpWrite(1, VCSFW_CMD::TEST_OTPROM_TAG_WRITE, pDst,numBytes);
	::Sleep(50);
	this->FpWaitForCMDComplete();
	this->FpReadAndCheckStatus(0);
}



void Syn_SPCCtrl::FpPokeRegister(uint32_t nHwRegAddr, uint32_t nData)
{
	uint8_t pDst[10];
	//set up data to write:
	uint32_t nAddrHi_Lo = (((nHwRegAddr >> 16) & 0xFFFF)) & 0xFF;
	uint32_t nAddrHi_Hi = ((((nHwRegAddr >> 16) & 0xFFFF)) >> 8) & 0xFF;
	uint32_t nAddrLo_Lo = ((nHwRegAddr & 0xFFFFF)) & 0xFF;
	uint32_t nAddrLo_Hi = ((nHwRegAddr & 0xFFFFF) >> 8) & 0xFF;    
	uint32_t nDataHi_Lo = (((nData >> 16) & 0xFFFF)) & 0xFF;
	uint32_t nDataHi_Hi = ((((nData >> 16) & 0xFFFF)) >> 8) & 0xFF;
	uint32_t nDataLo_Lo = ((nData & 0xFFFFF)) & 0xFF;
	uint32_t nDataLo_Hi = ((nData & 0xFFFFF) >> 8) & 0xFF;
	uint8_t pDataWrite[9] = {nAddrLo_Lo, nAddrLo_Hi, nAddrHi_Lo, nAddrHi_Hi, nDataLo_Lo, nDataLo_Hi, nDataHi_Lo, nDataHi_Hi, 8};

	//poke the register
	this->FpWrite(1,VCSFW_CMD::POKE,pDataWrite,9);
	this->FpWaitForCMDComplete();
	//this->FpRead(1, 0xFF, pDst, 2);
	//this->FpWaitDeviceReady();
	this->FpReadAndCheckStatus(0);
}

//void Syn_SPCCtrl::FpPeekRegister(uint32_t nHwRegAddr, uint32_t& nData)
void Syn_SPCCtrl::FpPeekRegister(uint32_t nHwRegAddr, uint8_t pDst[])
{
	//uint8_t pDst[10];
	//set up data to write:
	uint32_t nAddrHi_Lo = (((nHwRegAddr >> 16) & 0xFFFF)) & 0xFF;
	uint32_t nAddrHi_Hi = ((((nHwRegAddr >> 16) & 0xFFFF)) >> 8) & 0xFF;
	uint32_t nAddrLo_Lo = ((nHwRegAddr & 0xFFFFF)) & 0xFF;
	uint32_t nAddrLo_Hi = ((nHwRegAddr & 0xFFFFF) >> 8) & 0xFF;    
	uint8_t pDataWrite[9] = {nAddrLo_Lo, nAddrLo_Hi, nAddrHi_Lo, nAddrHi_Hi, 4};

	//poke the register
	this->FpWrite(1,VCSFW_CMD::PEEK,pDataWrite,5);
	this->FpWaitForCMDComplete();
	this->FpRead(1, 0xFF, pDst, 6);
	//this->FpWaitDeviceReady();

	//nData = *((uint32_t*)pDst);
}


void Syn_SPCCtrl::FpGetVersion(uint8_t *pDst, int numBytes)
{
	//LOG(INFO) << "Get Version";

	uint32_t err;
	err = MPC_FpGetVersion(syn_DeviceHandle, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetError(err);
		ex.SetDescription("FpGetVersion() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("FpGetVersion() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpWritePrintFile(uint8_t *pPrintPatch, int numBytes)
{
	//LOG(INFO) << "Write Print File";

	this->FpWrite(1, VCSFW_CMD::GET_PRINT, pPrintPatch, numBytes);
	this->FpWaitForCMDComplete();
	//this->FpReadAndCheckStatus(0);
}


//
//FpGetImage, used to current draw reading
//
void Syn_SPCCtrl::FpGetImage(uint8_t *pDst, int numBytes)
{
	//LOG(INFO) << "Get Image";

	uint32_t err;
	//this->FpNoop();
	err = MPC_FpGetImage(syn_DeviceHandle, pDst, numBytes, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetError(err);
		ex.SetDescription("FpGetImage() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("FpGetImage() DUT communication failure.");
		throw ex;
	}
}

//
//FpGetImage2, to capture image
//
void Syn_SPCCtrl::FpGetImage2(uint16_t nRows, uint16_t nCols, uint8_t *pDst, uint16_t nBlobSize, uint8_t *pBlob)
{
	//LOG(INFO) << "Get Image 2";
	
	uint32_t err;
	this->FpNoop();

	//err = MPC_FpGetImage2(syn_DeviceHandle, nRows, nCols, pDst, nBlobSize, pBlob, NULL, NULL, NULL, TIMEOUT);
	err = MPC_FpGetImage2(syn_DeviceHandle, nRows, nCols, pDst, nBlobSize, pBlob, 0, 0, 0, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetError(err);
		ex.SetDescription("FpGetImage2() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("FpGetImage2() DUT communication failure.");
		throw ex;
	}
}


//
//Set GPIO pin to in, out, timer and others.
//
void Syn_SPCCtrl::GpioSetPinType(uint16_t portId, uint32_t mskPins, uint16_t pinType)
{
	//LOG(INFO) << "GPIO Set Pin Type";

	uint32_t err;
	err = MPC_GpioSetPinType(syn_DeviceHandle, portId, mskPins, pinType, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetError(err);
		ex.SetDescription("GpioSetPinType() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("GpioSetPinType() DUT communication failure.");
		throw ex;
	}
}
	

//
//Read from GPIO pin, pMskPinState is the return state.
//
void Syn_SPCCtrl::GpioPinRead(uint16_t portID, uint32_t mskPins, uint32_t* pMskPinState)
{

	uint32_t err;
	err = MPC_GpioPinRead(syn_DeviceHandle, portID, mskPins, pMskPinState, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetError(err);
		ex.SetDescription("GpioPinRead() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("GpioPinRead() DUT communication failure.");
		throw ex;
	}

	int result = (*pMskPinState & mskPins) ? 1 : 0;
	//LOG(INFO) << "Read from GPIO:" << result;
}


//
//Write GPIO pin, value is mskPinState
//
void Syn_SPCCtrl::GpioPinWrite(uint16_t portID, uint32_t mskPins, uint32_t mskPinState)
{
	//LOG(INFO) << "Write GPIO";

	uint32_t err;
	err = MPC_GpioPinWrite(syn_DeviceHandle, portID, mskPins, mskPinState, TIMEOUT);
	Syn_Exception ex(err);
	if (err == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetError(err);
		ex.SetDescription("GpioPinWrite() Controller communication failure.");
		throw ex;
	}
	else if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("GpioPinWrite() DUT communication failure.");
		throw ex;
	}
}


void Syn_SPCCtrl::FpMpcGetSelfTestResults(uint16_t overSamples, uint32_t arValues[MPC_SELF_TEST_BUFFER])
{
	//LOG(INFO) << "MPC04 Self Test";
	
	uint32_t err(0);
	Syn_Exception ex(err);
	uint32_t gain, offset;
	err = MPC_GetTestValues(syn_DeviceHandle, overSamples, &gain, &offset, arValues, TIMEOUT);
	if (err != MpcApiError::ERR_OK)
	{
		ex.SetError(err);
		ex.SetDescription("FpMpcGetSelfTestResults DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::FpGpioGetFreq(uint16_t portId, uint32_t mskPins, uint32_t* pFreq_Hz)
{
	uint16_t error;
	error = MPC_GpioGetFreq(syn_DeviceHandle, portId, mskPins, pFreq_Hz, 2000);
	Syn_Exception ex(error);
	if (error == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetDescription("FpGpioGetFreq() Controller communication failure.");
		throw ex;
	}
	else if (error != MpcApiError::ERR_OK)
	{
		ex.SetDescription("FpGpioGetFreq() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::GpioDirModSet(uint16_t portId, uint32_t mskPins, uint32_t direction)
{
	uint16_t error;
	error = MPC_GpioDirModSet(syn_DeviceHandle, portId, mskPins, direction, 2000);
	Syn_Exception ex(error);
	if (error == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetDescription("GpioDirModSet() Controller communication failure.");
		throw ex;
	}
	else if (error != MpcApiError::ERR_OK)
	{
		ex.SetDescription("GpioDirModSet() DUT communication failure.");
		throw ex;
	}
}

void Syn_SPCCtrl::UpdateMPC04Firmware()
{
	uint32_t error = 0;
	Syn_Exception ex(error);

	std::string str("");
	int fileAppRev = 0, fileBootloaderRev = 0, bSuccess = 0;
	bool AppUpdate, BootloaderUpdate, sequenceFound;
	char filename_application[200];
	char filename_bootloader[200];
	FILE *pFileApplication, *pFileBootloader;
	char line[44] = { 0 };

	uint16_t nDevType;
	uint32_t nRevBootLoader;
	uint32_t nRevApplication;
	error = MPC_GetIdentity(syn_DeviceHandle, &nDevType, &nRevBootLoader, &nRevApplication, TIMEOUT);
	if (error != MpcApiError::ERR_OK)
	{
		MPC_Disconnect(syn_DeviceHandle);
		MPC_CloseMpcDeviceHandle(syn_DeviceHandle);
		syn_DeviceHandle = NULL;
		//LOG(ERROR) << "Cannot Get Identity of MPC04: " << syn_SerialNumber;
		ex.SetError(error);
		ex.SetDescription("Cannot Get Identity of MPC04: " + to_string(syn_SerialNumber));
		throw ex;
	}
	//LOG(INFO) << "MPC04 BootLoader Rev: " << nRevBootLoader << " Application Rev: " << nRevApplication;

	sprintf_s(filename_application, "Mpc04Application.hex");
	sprintf_s(filename_bootloader, "Mpc04BootloaderLoader.hex");
	pFileApplication = fopen(filename_application, "r");
	pFileBootloader = fopen(filename_bootloader, "r");
	if ((pFileApplication == NULL) || ((pFileBootloader == NULL)))
	{
		//LOG(ERROR) << "Mpc04Application.hex or Mpc04BootloaderLoader.hex is null: " << syn_SerialNumber;
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
					/*CString Tempstr;
					Tempstr.Format("%c%c%c%c%c%c%c%c", line[15], line[16], line[13], line[14], line[11], line[12], line[9], line[10]);*/
					//str = line[15] + line[16] + line[13] + line[14] + line[11] + line[12] + line[9] + line[10];
					str = "";
					str.push_back(line[15]); 
					str.push_back(line[16]);
					str.push_back(line[13]);
					str.push_back(line[14]);
					str.push_back(line[11]);
					str.push_back(line[12]);
					str.push_back(line[9]);
					str.push_back(line[10]);
					sscanf(str.c_str(), "%x", &fileBootloaderRev);
					//LOG(INFO) << "File BootLoader Rev: " << fileBootloaderRev;
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
					//str.Format("%c%c%c%c%c%c%c%c", line[15], line[16], line[13], line[14], line[11], line[12], line[9], line[10]);
					//str = line[15]+line[16]+line[13]+line[14]+line[11]+line[12]+line[9]+line[10];
					str = "";
					str.push_back(line[15]);
					str.push_back(line[16]);
					str.push_back(line[13]);
					str.push_back(line[14]);
					str.push_back(line[11]);
					str.push_back(line[12]);
					str.push_back(line[9]);
					str.push_back(line[10]);
					sscanf(str.c_str(), "%x", &fileAppRev);
					//LOG(INFO) << "File Application Rev: " << fileAppRev;
					if (((int)nRevApplication != fileAppRev))
						AppUpdate = true;
					break;
				}
			}
		}
	}

	uint32_t serNum;
	error = MPC_GetDeviceSerialNumber(syn_DeviceHandle, &serNum);
	if (error != MpcApiError::ERR_OK)
	{
		//LOG(ERROR) << "Cannot get device serial number";
	}
	if ((BootloaderUpdate))
	{
		//LOG(INFO) << "MPC04 Bootloader will be update,Please wait...: " << syn_SerialNumber;

		//Asynchronously, start the update.
		error = MPC_AsyncUpdateFirmware(syn_DeviceHandle, (LPCTSTR)filename_bootloader, NULL, NULL, NULL);

		//Update the modless progress dialog.
		uint32_t nPercent = 0;
		while ((nPercent < 100) && (error == MpcApiError::ERR_OK))
		{
			error = MPC_GetUpdateFirmwareProgress(syn_DeviceHandle, &nPercent);
			if (error != MpcApiError::ERR_OK)
			{
				//LOG(ERROR) << "MPC04 Bootloader update failure!: " << syn_SerialNumber;
				ex.SetError(error);
				ex.SetDescription("MPC04 Bootloader update failure!: " + to_string(syn_SerialNumber));
				throw ex;
			}
			//LOG(INFO) << "MPC04 Bootloader update process: " << nPercent;
		}

		//Wait for the Update command to complete.
		::Sleep(6000);

		if (error == MpcApiError::ERR_OK)
			error = MPC_GetMpcDeviceHandle(serNum, &syn_DeviceHandle);
		if (error == MpcApiError::ERR_OK)
			error = MPC_Connect(syn_DeviceHandle);

		//if (error == MpcApiError::ERR_OK)
			//LOG(INFO) << "Bootloader update completed sucessfully!: " << syn_SerialNumber;
		//else
			//LOG(ERROR) << "Bootloader update failure!: " << syn_SerialNumber;
	}

	if (AppUpdate || BootloaderUpdate)
	{
		//LOG(INFO) << "MPC04 Firmware will be updated.Please wait...: " << syn_SerialNumber;

		//Asynchronously, start the update.
		error = MPC_AsyncUpdateFirmware(syn_DeviceHandle, (LPCTSTR)filename_application, NULL, NULL, NULL);

		//When the percent goes to zero it means the API has started the download.
		//We want to wait until the download is started.
		uint32_t nPercent;
		error = MPC_GetUpdateFirmwareProgress(syn_DeviceHandle, &nPercent);
		while ((nPercent == 100) && (error == MpcApiError::ERR_OK))
			error = MPC_GetUpdateFirmwareProgress(syn_DeviceHandle, &nPercent);

		//Update the modeless progress dialog.
		while ((nPercent < 100) && (error == MpcApiError::ERR_OK))
		{
			error = MPC_GetUpdateFirmwareProgress(syn_DeviceHandle, &nPercent);
			if (error != MpcApiError::ERR_OK)
			{
				//LOG(ERROR) << "MPC04 Application update failure!: " << syn_SerialNumber;
				ex.SetError(error);
				ex.SetDescription("MPC04 Application update failure!: " + to_string(syn_SerialNumber));
				throw ex;
			}
			//LOG(INFO) << "MPC04 Firmware update process: " << nPercent;
			::Sleep(5);
		}

		//Wait for the Update command to complete.
		::Sleep(8000);

		if (error == MpcApiError::ERR_OK)
		{
			error = MPC_GetMpcDeviceHandle(serNum, &syn_DeviceHandle);
		}

		if (error == MpcApiError::ERR_OK)
		{
			error = MPC_Connect(syn_DeviceHandle);
		}

		if (error == MpcApiError::ERR_OK)
		{
			//LOG(INFO) << "Firmware update completed sucessfully!: " << syn_SerialNumber;
		}
		else
		{
			//LOG(ERROR) << "Firmware update failure!: " << syn_SerialNumber;
			ex.SetError(error);
			ex.SetDescription("Firmware update failure!: " + to_string(syn_SerialNumber));
			throw ex;
		}
	}
}