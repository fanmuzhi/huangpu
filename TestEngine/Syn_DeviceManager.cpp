#include "Syn_DeviceManager.h"

#include "Syn_DutCtrl.h"
#include "Syn_Exception.h"

//MPC_API
#include "MpcApiDll.h"
#include "MpcApiError.h"
//#include "MPCErrors.h"

#include "easylogging++.h"

Syn_DeviceManager::Syn_DeviceManager()
:_pDeviceSerNumArray(NULL)
{
	_deviceCount = 0;
}


Syn_DeviceManager::~Syn_DeviceManager()
{
	if (NULL != _pDeviceSerNumArray)
	{
		delete[] _pDeviceSerNumArray;
		_pDeviceSerNumArray = NULL;
	}
}

uint32_t Syn_DeviceManager::Open()
{
	uint32_t error = 0;
	error = MPC_Initialize();
	if (error != MpcApiError::ERR_OK)
	{
		LOG(ERROR) << "MPC_Initialize is failed!";
		return error;
	}

	error = MPC_GetNumberOfDevices(&_deviceCount);
	if (error != MpcApiError::ERR_OK)
	{
		LOG(ERROR) << "MPC_GetNumberOfDevice is failed!";
		return error;
	}
	if (0 == _deviceCount)
	{
		LOG(ERROR) << "DeviceCounts is 0!";
		error = MpcApiError::ERR_DEVICE_NOT_CONNECTED;
		return error;
	}

	_pDeviceSerNumArray = new uint32_t[_deviceCount];
	error = MPC_GetDeviceSerialNumList(_pDeviceSerNumArray);
	if (error != MpcApiError::ERR_OK)
	{
		LOG(ERROR) << "MPC_GetDeviceSerialNumList is failed!";
		return error;
	}
	return error;
}

void Syn_DeviceManager::GetSerialNumberList(std::vector<uint32_t> &serialNumberList)
{
	if (NULL == _pDeviceSerNumArray)
		return;

	for (auto i = 0; i < _deviceCount; i++)
	{
		serialNumberList.push_back(_pDeviceSerNumArray[i]);
	}
}

uint32_t Syn_DeviceManager::UpdateFirmware()
{
	uint32_t err = 0;
	try
	{
		for (auto i = 0; i < _deviceCount; i++)
		{
			Syn_DutCtrl * _pSyn_DutCtrl; 
			Syn_DutCtrl::CreateDutCtrlInstance(DutController::Syn_MPC04, _pDeviceSerNumArray[0], _pSyn_DutCtrl);
			_pSyn_DutCtrl->UpdateMPC04Firmware();
			delete _pSyn_DutCtrl;
			_pSyn_DutCtrl = NULL;
		}
	}
	catch (Syn_Exception ex)
	{
		err = ex.GetError();
	}
	return err;
}

uint32_t Syn_DeviceManager::UpdateADCOffsets()
{

	return 0;
}

uint32_t Syn_DeviceManager::Close()
{
	return 0;
}