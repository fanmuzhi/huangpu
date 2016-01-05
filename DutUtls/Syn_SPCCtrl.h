#pragma once

//ATL
#include <atlstr.h>
#include <stdio.h> 

//local
#include "Syn_DutCtrl.h"

class Syn_SPCCtrl :public Syn_DutCtrl
{
public:
	Syn_SPCCtrl(uint32_t iSerialNumber);
	virtual ~Syn_SPCCtrl();

	virtual bool Init();

	virtual void SetVoltages(uint16_t vdd_mV, uint16_t vio_mV, uint16_t vled_mV, uint16_t vddh_mV);

	virtual void FpGetStatus(uint8_t* pDst, int numBytes);

	virtual void FpRead(uint16_t endpoint, uint16_t command, uint8_t* pDst, int numBytes);

	virtual void FpWrite(uint16_t endpoint, uint16_t command, uint8_t* pSrc, int numBytes);

	virtual void FpLoadPatch(uint8_t* pPatch, int numBytes);

	virtual void FpUnloadPatch();

	virtual void FpOtpRomRead(int section, int sector, uint8_t* pDst, int numBytes);

	virtual void FpOtpRomWrite(int section, int sector, uint8_t* pDst, int numBytes);

	virtual void FpWaitForCMDComplete();

	virtual void FpWaitDeviceReady();

	virtual void FpDisableSleep();

	virtual uint16_t FpWaitForCommandCompleteAndReturnErrorCode(uint32_t numBytes);

	virtual void FpGetVersion(uint8_t *pDst, int numBytes);

	//virtual void UpdateMPC04Firmware(uint16_t nDevType, uint32_t nRevBootLoader, uint32_t nRevApplication);

protected:

	static bool	_bDLLInitialized;
	const uint32_t TIMEOUT = 2000;
};