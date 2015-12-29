//local
#include "Syn_DutOperation.h"

//MPC_API
//#include "MpcApiDll.h"
#include "inc/MpcApiDll.h"

//std
#include <iostream>

Syn_DutOperation::Syn_DutOperation()
{
}


Syn_DutOperation::~Syn_DutOperation()
{
}

bool Syn_DutOperation::StartDutOperation(vector<Syn_Dut*> &ListOfDuctPtr,std::string &strValue)
{
	ListOfDuctPtr.clear();

	uint32_t uResult(1);

	//Init Operation
	uResult = MPC_Initialize();
	if (0 != uResult)
	{
		cout << "Error:Syn_DutOperation::StartDutOperation() - ::MPC_Initialize is failed!" << endl;
		strValue = "Error:Syn_DutOperation::StartDutOperation() - ::MPC_Initialize is failed!";
		return false;
	}

	int iDeviceCounts(0);
	MPC_GetNumberOfDevices(&iDeviceCounts);
	if (0 == iDeviceCounts)
	{
		cout << "Error:Syn_DutOperation::StartDutOperation() - iDeviceCounts is 0!" << endl;
		strValue = "Error:Syn_DutOperation::StartDutOperation() - iDeviceCounts is 0!";

		return false;
	}

	//uint32_t *pDeviceSerialNumberArray = new uint32_t[iDeviceCounts];

	/*uint32_t pDeviceSerialNumberArray[8] = { 0 };
	MPC_GetDeviceSerialNumList(pDeviceSerialNumberArray);
	for (int i = 0; i<iDeviceCounts; i++)
	{
		uint32_t deviceHandle;
		MPC_GetMpcDeviceHandle(pDeviceSerialNumberArray[i], &deviceHandle);

		Syn_Dut *pSyn_Dut = NULL;
		bool bResult = Syn_Dut::CreateDutInstance(Viper1, pSyn_Dut, deviceHandle, Syn_SPC);
		if (!bResult || NULL == pSyn_Dut)
		{
			cout << "Error:Syn_DutOperation::StartDutOperation() - ::CreateDutInstance is failed,i is "<<i<<"!" << endl;
			strValue = "Error:Syn_DutOperation::StartDutOperation() - ::CreateDutInstance is failed,i is!";
			continue;
		}

		pSyn_Dut->GetDutCtrl()->Init();

		ListOfDuctPtr.push_back(pSyn_Dut);
	}

	strValue = ListOfDuctPtr.size();*/
	

	return true;
}