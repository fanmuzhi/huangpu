//Local
#include "Syn_SPCCtrl.h"

//MPC_API
#include "MpcApiDll.h"

//std
#include <iostream>

Syn_SPCCtrl::Syn_SPCCtrl()
:Syn_DutCtrl()
{
}


Syn_SPCCtrl::~Syn_SPCCtrl()
{
}

bool Syn_SPCCtrl::Init()
{
	MPC_Connect(syn_DeviceHandle);
	::Sleep(2);
	if (MPC_IsConnected(syn_DeviceHandle))
	{
		// Set SPI port
		MPC_SetPortFpSensor(syn_DeviceHandle, 0, 0x0008, 8000, 0, 0, 2000);

		return true;
	}
	else
	{
		cout << "Error:Syn_SPCCtrl::Init() - MPC_IsConnected is failed!" << endl;
		return false;
	}

	return true;
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
	uint16_t error;

	//Test


	error = MPC_FpGetVersion(syn_DeviceHandle, pDst, numBytes, 2000);
	/*ExTestStep	ex(error);
	if (error == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		ex.SetDescription("FpGetVersion() Controller communication failure.");
		throw ex;
	}
	else if (error != Errors::NO_MPC_ERROR)
	{
		ex.SetDescription("FpGetVersion() DUT communication failure.");
		throw ex;
	}*/

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
