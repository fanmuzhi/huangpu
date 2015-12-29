//local
#include "Syn_DutCtrl.h"
#include "Syn_SPCCtrl.h"
#include "Syn_MPC04Ctrl.h"

//std
#include <iostream>

Syn_DutCtrl::Syn_DutCtrl()
{
}


Syn_DutCtrl::~Syn_DutCtrl()
{
}

bool Syn_DutCtrl::CreateDutCtrlInstance(DutController iType, uint32_t iSerialNumber, Syn_DutCtrl * &opSyn_DutCtrlInstance)
{
	opSyn_DutCtrlInstance = NULL;
	if (Syn_SPC == iType)
	{
		Syn_SPCCtrl *pSyn_SPCCtrl = new Syn_SPCCtrl();
		opSyn_DutCtrlInstance = static_cast<Syn_DutCtrl*>(pSyn_SPCCtrl);

		//*opSyn_DutCtrlInstance = new Syn_SPCCtrl();
		opSyn_DutCtrlInstance->syn_SerialNumber = iSerialNumber;
	}
	else if (Syn_MPC04 == iType)
	{
		opSyn_DutCtrlInstance = new Syn_MPC04Ctrl();
		opSyn_DutCtrlInstance->syn_SerialNumber = iSerialNumber;
	}
	else
	{
		cout << "Error:Syn_DutCtrl::CreateDutCtrlInstance() - Can't retrieve the DutController!" << endl;
		return false;
	}

	return true;
}