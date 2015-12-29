#pragma once

//local
#include "Syn_DutCtrl.h"


class Syn_SPCCtrl :public Syn_DutCtrl
{
public:
	Syn_SPCCtrl();
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



	virtual void FpWaitForCommandCompleteAndCheckErrorCode(uint32_t numBytes, int nTimeout_ms = 2000);

	virtual uint16_t FpWaitForCommandCompleteAndReturnErrorCode(uint32_t numBytes, int nTimeout_ms = 2000);




	virtual bool PowerOn(uint8_t *pDst, int numBytes);

	virtual bool FpGetVersion(uint8_t *pDst, int numBytes);

	virtual bool PowerOff();

protected:

	static bool	_bDLLInitialized;
};

