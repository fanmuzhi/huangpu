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

uint32_t Syn_DeviceManager::UpdateFirmware(std::vector<Syn_Site*> &listOfSite)
{
	uint32_t err = 0;

	/*if (NULL != _pDeviceSerNumArray)
	{
		Syn_DutCtrl * _pSyn_DutCtrl = NULL;
		try
		{
			for (int i = 0; i < _deviceCount; i++)
			{
<<<<<<< HEAD
				uint32_t rc = Syn_DutCtrl::CreateDutCtrlInstance(DutController::Syn_MPC04, _pDeviceSerNumArray[i], _pSyn_DutCtrl);
				if (_pSyn_DutCtrl == NULL)
				{
					return rc;
				}

				_pSyn_DutCtrl->UpdateMPC04Firmware();

				delete _pSyn_DutCtrl;
				_pSyn_DutCtrl = NULL;
=======
				Syn_DutCtrl * pSyn_DutCtrl = NULL;
				Syn_DutCtrl::CreateDutCtrlInstance(DutController::Syn_MPC04, _pDeviceSerNumArray[i], pSyn_DutCtrl);
				if (NULL != pSyn_DutCtrl)
				{
					pSyn_DutCtrl->UpdateMPC04Firmware();
					delete pSyn_DutCtrl;
					pSyn_DutCtrl = NULL;
				}
				else
				{
					err = 0x1004;//Syn_DutCtrlNull:device in used
				}
>>>>>>> zhangrui_dev
			}
		}
		catch (Syn_Exception ex)
		{
			if (_pSyn_DutCtrl != NULL)
			{
				delete _pSyn_DutCtrl;
				_pSyn_DutCtrl = NULL;
			}
			err = ex.GetError();
		}
<<<<<<< HEAD
		catch (...)
		{
			if (_pSyn_DutCtrl != NULL)
			{
				delete _pSyn_DutCtrl;
				_pSyn_DutCtrl = NULL;
			}
			err = Syn_ExceptionCode::Syn_UnknownError;
		}
=======
	}*/

	try
	{
		for (size_t i = 1; i <= listOfSite.size(); i++)
		{
			Syn_Site *pSiteInstance = listOfSite[i - 1];
			if (NULL != pSiteInstance)
			{
				Syn_DutCtrl *pCtrl = NULL;
				pSiteInstance->GeDutCtrl(pCtrl);
				if (NULL != pCtrl)
				{
					pCtrl->UpdateMPC04Firmware();
				}
			}
		}
	}
	catch (Syn_Exception ex)
	{
		err = ex.GetError();
	}

	
	return err;
}

uint32_t Syn_DeviceManager::UpdateADCOffsets(Syn_Site* &pSiteInstance, uint32_t nVdd, uint32_t nVio, uint32_t nVled, uint32_t nVddh, uint32_t arAdcBaseLines[NUM_CURRENT_VALUES][KNUMGAINS])
{
	uint32_t err = 0;

	if (NULL == pSiteInstance)
	{
		return 0x1000;
	}

	try
	{
		Syn_DutCtrl *pCtrl = NULL;
		pSiteInstance->GeDutCtrl(pCtrl);
		if (NULL != pCtrl)
		{
			pCtrl->SetVoltages(nVdd, nVio, nVled, nVddh);

			for (int gainIdx = 0; gainIdx < KNUMGAINS; gainIdx++)
			{
				uint32_t arValues[NUM_CURRENT_VALUES] = {0};
				pCtrl->GetCurrentSenseValues(gainIdx, 64, arValues);
				for (int adcIdx = 0; adcIdx<NUM_CURRENT_VALUES; adcIdx++)
					arAdcBaseLines[adcIdx][gainIdx] = arValues[adcIdx];
			}
		}
	}
	catch (Syn_Exception ex)
	{
		err = ex.GetError();
	}

	return err;
}

uint32_t Syn_DeviceManager::SetLeds(Syn_Site* &pSiteInstance)
{
	uint32_t err = 0;

	if (NULL == pSiteInstance)
	{
		return 0x1000;
	}

	try
	{
		Syn_DutCtrl *pCtrl = NULL;
		pSiteInstance->GeDutCtrl(pCtrl);
		if (NULL != pCtrl)
		{
			for (int i = 0; i<10; i++)
			{
				pCtrl->SetLeds(true, false, false);
				::Sleep(50);
				pCtrl->SetLeds(false, false, false);
				::Sleep(50);
			}
		}
	}
	catch (Syn_Exception ex)
	{
		err = ex.GetError();
	}

	return err;
}

uint32_t Syn_DeviceManager::Close()
{
	return 0;
}