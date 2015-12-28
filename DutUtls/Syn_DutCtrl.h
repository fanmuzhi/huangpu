#pragma once

//std C
#include "stdint.h"

//std
#include <string>
using namespace std;

enum DutController
{
	Syn_SPC = 0x10FF,
	Syn_MPC04
};

class Syn_DutCtrl
{
public:

	//Gobal Function:Create DutCtrl Instance
	static bool CreateDutCtrlInstance(DutController iType, uint32_t iDeviceHandle, Syn_DutCtrl * &opSyn_DutCtrlInstance);

	//
	Syn_DutCtrl();

	virtual ~Syn_DutCtrl();

	virtual bool Init() = 0;

	virtual bool PowerOn(uint8_t *pDst, int numBytes) = 0;

	virtual bool PowerOff() = 0;

	virtual bool FpGetVersion(uint8_t *pDst, int numBytes) = 0;

protected:

	uint32_t syn_DeviceHandle;
};

