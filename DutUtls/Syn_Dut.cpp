//Local
#include "Syn_Dut.h"
#include "Syn_Viper.h"
#include "Syn_Viper1.h"
#include "Syn_Viper2.h"
#include "Syn_Metallica.h"

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

bool Syn_Dut::CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance, uint32_t isyn_DeviceHandle, DutController iDutControllerType)
{
	opSyn_DutInstance = NULL;
	if (Viper1 == iType)
	{
		Syn_Viper1 *pSyn_Viper1 = new Syn_Viper1();
		opSyn_DutInstance = static_cast<Syn_Dut*>(pSyn_Viper1);

		//*opSyn_DutInstance = new Syn_Viper1();
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


	//just for test
	Syn_DutCtrl *pSyn_DutCtrl = NULL;
	bool bResult = Syn_DutCtrl::CreateDutCtrlInstance(iDutControllerType, isyn_DeviceHandle, pSyn_DutCtrl);
	if (!bResult || NULL == pSyn_DutCtrl)
	{
		cout << "Error:Syn_Dut::CreateDutInstance() - pSyn_DutCtrl is NULL!" << endl;
		return false;
	}

	opSyn_DutInstance->SetDutCtrl(pSyn_DutCtrl);

	//Init test

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