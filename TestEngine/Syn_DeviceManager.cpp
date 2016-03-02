#include "Syn_DeviceManager.h"

#include "Syn_DutCtrl.h"
#include "Syn_Exception.h"

//MPC_API
#include "MpcApiDll.h"
#include "MpcApiError.h"
//#include "MPCErrors.h"

//third-part
#include "easylogging++.h"

Syn_DeviceManager::Syn_DeviceManager()
:_pDeviceSerNumArray(NULL)
,_OpenFlag(false)
,_deviceCount(0)
{
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

	if (!_OpenFlag)
	{
		error = MPC_Initialize();
		if (error != MpcApiError::ERR_OK)
		{
			LOG(ERROR) << "MPC_Initialize is failed!";
			return error;
		}

		_OpenFlag = true;
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

	if (NULL != _pDeviceSerNumArray)
	{
		delete[] _pDeviceSerNumArray;
		_pDeviceSerNumArray = NULL;
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

	if (NULL != _pDeviceSerNumArray)
	{
		
		try
		{
			for (int i = 0; i < _deviceCount; i++)
			{
				Syn_DutCtrl * pCtrl = NULL;
				uint32_t rc = Syn_DutCtrl::CreateDutCtrlInstance(DutController::Syn_MPC04, _pDeviceSerNumArray[i], pCtrl);
				if (pCtrl != NULL)
				{
					pCtrl->UpdateMPC04Firmware();

					delete pCtrl;
					pCtrl = NULL;
				}
			}
		}
		catch (Syn_Exception ex)
		{
			err = ex.GetError();
		}
		catch (...)
		{
			err = Syn_ExceptionCode::Syn_UnknownError;
		}
	}

	return err;
}

uint32_t Syn_DeviceManager::UpdateADCOffsets(uint32_t uiSerialNumber, uint32_t nVdd, uint32_t nVio, uint32_t nVled, uint32_t nVddh, uint32_t arAdcBaseLines[NUM_CURRENT_VALUES][KNUMGAINS])
{
	uint32_t err = 0;

	Syn_DutCtrl *pCtrl = NULL;
	uint32_t rc = Syn_DutCtrl::CreateDutCtrlInstance(DutController::Syn_MPC04, uiSerialNumber, pCtrl);
	if (NULL!=pCtrl)
	{
		try
		{
			pCtrl->SetVoltages(nVdd, nVio, nVled, nVddh);

			for (int gainIdx = 0; gainIdx < KNUMGAINS; gainIdx++)
			{
				uint32_t arValues[NUM_CURRENT_VALUES] = { 0 };
				pCtrl->GetCurrentSenseValues(gainIdx, 64, arValues);
				for (int adcIdx = 0; adcIdx<NUM_CURRENT_VALUES; adcIdx++)
					arAdcBaseLines[adcIdx][gainIdx] = arValues[adcIdx];
			}
		}
		catch (Syn_Exception ex)
		{
			err = ex.GetError();
		}
		
		delete pCtrl;
		pCtrl = NULL;
	}

	return err;
}

uint32_t Syn_DeviceManager::SetLeds(uint32_t uiSerialNumber)
{
	uint32_t err = 0;

	Syn_DutCtrl *pCtrl = NULL;
	uint32_t rc = Syn_DutCtrl::CreateDutCtrlInstance(DutController::Syn_MPC04, uiSerialNumber, pCtrl);
	if (NULL != pCtrl)
	{
		try
		{
			for (int i = 0; i<10; i++)
			{
				pCtrl->SetLeds(true, false, false);
				::Sleep(50);
				pCtrl->SetLeds(false, false, false);
				::Sleep(50);
			}
		}
		catch (Syn_Exception ex)
		{
			err = ex.GetError();
		}

		delete pCtrl;
		pCtrl = NULL;
	}

	return err;
}

uint32_t Syn_DeviceManager::Close()
{
	return 0;
}