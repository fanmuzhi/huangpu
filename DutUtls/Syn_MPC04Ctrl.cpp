
//local
#include "Syn_MPC04Ctrl.h"
#include "Syn_Exception.h"

//MPC DLL
#include "MpcApiDll.h"
#include "MpcApiError.h"
#include "MPCErrors.h"

Syn_MPC04Ctrl::Syn_MPC04Ctrl(uint32_t iSerialNumber)
:Syn_SPCCtrl(iSerialNumber)
{
}


Syn_MPC04Ctrl::~Syn_MPC04Ctrl()
{
}

void Syn_MPC04Ctrl::GetCurrentSenseValues(uint16_t gainIdx, uint16_t oversample, uint32_t arCurrentSenseValues[4])
{
	uint16_t error(1);

	error = MPC_GetCurrentSenseValues(syn_DeviceHandle, gainIdx, oversample, arCurrentSenseValues, 2000);
	Syn_Exception Exception(error);
	if (error == MpcApiError::ERR_COMMUNICATION_FAILED)
	{
		Exception.SetDescription("Controller communication failure.");
		throw Exception;
	}
	else if (error != Errors::NO_MPC_ERROR)
	{
		Exception.SetDescription("DUT communication failure.");
		throw Exception;
	}
}
