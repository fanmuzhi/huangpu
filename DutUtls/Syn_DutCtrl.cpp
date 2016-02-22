//local
#include "Syn_DutCtrl.h"
#include "Syn_SPCCtrl.h"
#include "Syn_MPC04Ctrl.h"
#include "Syn_Exception.h"

//std
#include <iostream>

//third-party
#include "easylogging++.h"

Syn_DutCtrl::Syn_DutCtrl(uint32_t iSerialNumber)
:syn_SerialNumber(iSerialNumber)
{
}

Syn_DutCtrl::~Syn_DutCtrl()
{
}

uint32_t Syn_DutCtrl::CreateDutCtrlInstance(DutController iType, uint32_t iSerialNumber, Syn_DutCtrl * &opSyn_DutCtrlInstance)
{
	opSyn_DutCtrlInstance = NULL;
	if (Syn_SPC == iType)
	{
		try
		{
			opSyn_DutCtrlInstance = new Syn_SPCCtrl(iSerialNumber);
		}
		catch (Syn_Exception ex)
		{
			if (NULL != opSyn_DutCtrlInstance)
			{
				delete opSyn_DutCtrlInstance;
				opSyn_DutCtrlInstance = NULL;
			}
			return ex.GetError();
		}
	}
	else if (Syn_MPC04 == iType)
	{
		try
		{
			opSyn_DutCtrlInstance = new Syn_MPC04Ctrl(iSerialNumber);
		}
		catch (Syn_Exception ex)
		{
			if (NULL != opSyn_DutCtrlInstance)
			{
				delete opSyn_DutCtrlInstance;
				opSyn_DutCtrlInstance = NULL;
			}
			return ex.GetError();
		}
	}
	else
	{
		LOG(ERROR) << "Error:Syn_DutCtrl::CreateDutCtrlInstance() - Can't retrieve the DutController!";
		return false;
	}

	return 0;
}