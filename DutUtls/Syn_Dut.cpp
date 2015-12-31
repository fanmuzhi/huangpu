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
		cout << "Error:Syn_Dut::CreateDutInstance() - Can't retrieve the ProjectType!" << endl;
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
		cout << "Error:Syn_Dut::SetDutCtrl() - ipSyn_DutCtrl is NULL!" << endl;
		return false;
	}
}

Syn_DutCtrl * Syn_Dut::GetDutCtrl()
{
	return _pSyn_DutCtrl;
}

void Syn_Dut::CycleDutPowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep)
{
	int				timeout;
	uint8_t			pDst[10] = { 0 };
	uint8_t			pSrc[2] = { 0 };

	if (NULL == _pSyn_DutCtrl)
	{
		return;
	}

	//Cycle the power to the sensor.
	_pSyn_DutCtrl->SetVoltages(0, 0, nPwrVled, 0);
	::Sleep(50);
	_pSyn_DutCtrl->SetVoltages(nPwrVdd, nPwrVio, nPwrVled, nPwrVddh);
	::Sleep(50);

	//If requested by the caller, disable the sensor's sleep feature.
	if (bDisableSleep)
	{
		//Wake sensor from sleep mode.
		_pSyn_DutCtrl->FpGetStatus(pDst, 4);
		timeout = 200;
		while (timeout && ((pDst[0] != 0x01) || (pDst[1] != 0x00) || (pDst[2] != 0x00) || (pDst[3] != 0x08)))
		{
			_pSyn_DutCtrl->FpGetStatus(pDst, 4);
			timeout--;
		}

		if (timeout == 0)
		{
			Syn_Exception ex(0);
			ex.SetDescription("Timeout waiting for sensor to wake.");
			throw(ex);
		}

		//Configure sensor not to go back to sleep.
		_pSyn_DutCtrl->FpWrite(1, 0x0057, pSrc, sizeof(pSrc));
		//_pSyn_DutCtrl->FpGetStatus(pDst, 4);
		_pSyn_DutCtrl->FpWaitForCommandCompleteAndCheckErrorCode(2);
		
	}
}

bool Syn_Dut::ReadOTP(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep, uint8_t* pPatch, int numBytes, uint8_t *oarMS0,int iSize)
{
	if (NULL == _pSyn_DutCtrl)
	{
		cout << "Error:Syn_Dut::ReadOTP() - _pSyn_DutCtrl is NULL!" << endl;
		return false;
	}

	try
	{
		this->CycleDutPowerOn(nPwrVdd, nPwrVio, nPwrVled, nPwrVddh, bDisableSleep);
	}
	catch (Syn_Exception Exception)
	{
		cerr << "Error" + Exception.GetDescription() << endl;
		return false;
	}
	
	try
	{
		_pSyn_DutCtrl->FpUnloadPatch();
	}
	catch (Syn_Exception Exception)
	{
		cerr << "Error" + Exception.GetDescription() << endl;
		return false;
	}
	

	try
	{
		_pSyn_DutCtrl->FpLoadPatch(pPatch, numBytes);//OTPReadWritePatch
	}
	catch (Syn_Exception Exception)
	{
		cerr << "Error" + Exception.GetDescription() << endl;
		return false;
	}

	//_pSyn_DutCtrl->FpLoadPatch(pPatch, numBytes);//OTPReadWritePatch
	//_pSyn_DutCtrl->FpOtpRomRead();
	//_pSyn_DutCtrl->FpReadOTPROM()


	//bool FpSensor::ValidateMS0()
	//Read Main Sector 0 and Main Sector 1 to get the count of each record type.
	uint8_t	arMS0[MS0_SIZE] = {0};
	try
	{
		_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 0, arMS0, MS0_SIZE);
	}
	catch (Syn_Exception Exception)
	{
		cerr << "Error" + Exception.GetDescription() << endl;
		return false;
	}

	try
	{
		_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 1, &arMS0[2048], MS1_SIZE);
	}
	catch (Syn_Exception Exception)
	{
		cerr << "Error" + Exception.GetDescription() << endl;
		return false;
	}
	
	/*_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 0, arMS0, MS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(MAIN_SEC, 1, &arMS0[2048], MS1_SIZE);*/
	//CountOtpRecords(arMS0, &site.m_calibrationResults);
	

	oarMS0 = arMS0;

	return true;
}