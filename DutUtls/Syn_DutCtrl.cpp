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

bool Syn_DutCtrl::CreateDutCtrlInstance(DutController iType, uint32_t iDeviceHandle, Syn_DutCtrl * &opSyn_DutCtrlInstance)
{
	opSyn_DutCtrlInstance = NULL;
	if (Syn_SPC == iType)
	{
		Syn_SPCCtrl *pSyn_SPCCtrl = new Syn_SPCCtrl();
		opSyn_DutCtrlInstance = static_cast<Syn_DutCtrl*>(pSyn_SPCCtrl);

		//*opSyn_DutCtrlInstance = new Syn_SPCCtrl();
		opSyn_DutCtrlInstance->syn_DeviceHandle = iDeviceHandle;
	}
	else if (Syn_MPC04 == iType)
	{
		opSyn_DutCtrlInstance = new Syn_MPC04Ctrl();
		opSyn_DutCtrlInstance->syn_DeviceHandle = iDeviceHandle;
	}
	else
	{
		cout << "Error:Can't retrieve the DutController!" << endl;
		return false;
	}

	//MPC_Initialize();
	//int deviceNum;
	//MPC_GetNumberOfDevices(&deviceNum);
	//QString s = "DeviceNum: " + QString::number(deviceNum);
	//ui.textBrowser->append(s);
	////parse_config();
	//uint32_t DeviceSN[8];
	//MPC_GetDeviceSerialNumList(DeviceSN);

	//for (int i = 0; i<deviceNum; i++){
	//	QString s = "Device SN: " + QString::number(DeviceSN[i]);
	//	ui.textBrowser->append(s);
	//}
	//uint32_t deviceHandle;
	//MPC_GetMpcDeviceHandle(DeviceSN[0], &deviceHandle);

	//MPC_Connect(deviceHandle);
	//::Sleep(2);
	//if (MPC_IsConnected(deviceHandle)){
	//	ui.textBrowser->append("Device Connected.");

	//	// Set SPI port
	//	MPC_SetPortFpSensor(deviceHandle, 0, 0x0008, 8000, 0, 0, 2000);

	//	return deviceHandle;
	//}
	//else{
	//	ui.textBrowser->append("Device Not Connected.");
	//	return -1;
	//}

	return true;
}