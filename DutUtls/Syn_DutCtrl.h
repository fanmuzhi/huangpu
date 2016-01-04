#pragma once

//std C
#include "stdint.h"

//std
#include <string>
using namespace std;

extern "C" {
#include "SYN_TestUtils.h"
};


enum DutController
{
	Syn_SPC = 0x10FF,
	Syn_MPC04
};

class Syn_DutCtrl
{
public:

	//Gobal Function:Create DutCtrl Instance
	static bool CreateDutCtrlInstance(DutController iType, uint32_t iSerialNumber, Syn_DutCtrl * &opSyn_DutCtrlInstance);

	//
	Syn_DutCtrl(uint32_t iSerialNumber);

	virtual ~Syn_DutCtrl();

	virtual bool Init() = 0;

	virtual void SetVoltages(uint16_t vdd_mV, uint16_t vio_mV, uint16_t vled_mV, uint16_t vddh_mV) = 0;

	virtual void FpGetStatus(uint8_t* pDst, int numBytes) = 0;

	virtual void FpRead(uint16_t endpoint, uint16_t command, uint8_t* pDst, int numBytes) = 0;

	virtual void FpWrite(uint16_t endpoint, uint16_t command, uint8_t* pSrc, int numBytes) = 0;

	virtual void FpLoadPatch(uint8_t* pPatch, int numBytes) = 0;

	virtual void FpUnloadPatch() = 0;

	virtual void FpOtpRomRead(int section, int sector, uint8_t* pDst, int numBytes) = 0;

	virtual void FpOtpRomWrite(int section, int sector, uint8_t* pDst, int numBytes) = 0;

	virtual void FpWaitForCommandCompleteAndCheckErrorCode(uint32_t numBytes, int nTimeout_ms = 2000) = 0;

	virtual uint16_t FpWaitForCommandCompleteAndReturnErrorCode(uint32_t numBytes, int nTimeout_ms = 2000) = 0;

	virtual bool FpGetVersion(uint8_t *pDst, int numBytes) = 0;

protected:

	uint32_t syn_SerialNumber;
	uint32_t syn_DeviceHandle;
};