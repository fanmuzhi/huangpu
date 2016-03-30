#pragma once

//c
#include <stdio.h> 

//local
#include "Syn_DutCtrl.h"


class Syn_SPCCtrl :public Syn_DutCtrl
{
public:
	Syn_SPCCtrl(uint32_t iSerialNumber);
	virtual ~Syn_SPCCtrl();

	virtual void Init();

	virtual void SetVoltages(uint16_t vdd_mV, uint16_t vio_mV, uint16_t vled_mV, uint16_t vddh_mV);

	virtual void FpGetStatus(uint8_t* pDst, int numBytes);

	virtual void FpRead(uint16_t endpoint, uint16_t command, uint8_t* pDst, int numBytes);

	virtual void FpWrite(uint16_t endpoint, uint16_t command, uint8_t* pSrc, int numBytes);

	virtual void FpLoadPatch(uint8_t* pPatch, int numBytes);

	virtual void FpUnloadPatch();

	virtual void FpOtpRomRead(int section, int sector, uint8_t* pDst, int numBytes);

	virtual void FpOtpRomWrite(int section, int sector, uint8_t* pDst, int numBytes);

	virtual void FpPokeRegister(uint32_t nHwRegAddr, uint32_t nData);

	virtual void FpPeekRegister(uint32_t nHwRegAddr, uint8_t pDst[]);

	virtual uint8_t FpOtpRomTagRead(uint32_t nExtTag, uint8_t* pDst, int numBytes);

	virtual void FpOtpRomTagWrite(uint8_t* pDst, int numBytes);

	virtual void FpWaitForCMDComplete(const uint32_t timeout = 500);

	virtual void FpReadBuff(uint8_t *pDst, int numBytes);

	virtual void FpReadAndCheckStatus(uint16_t statusIgnore);

	virtual void FpRunPatchTest(uint8_t *pDst, int numBytes);

	virtual void FpLoadRamPatch(uint8_t* pPatch, int patchSize, uint8_t* pDst, int numBytes);

	virtual void FpWaitDeviceReady();

	virtual void FpDisableSleep();

	virtual void FpEnterSleep();

	virtual void FpNoop();

	virtual void FpGetVersion(uint8_t *pDst, int numBytes);

	virtual void FpWritePrintFile(uint8_t *pPrintPatch, int numBytes);

	virtual void FpGetImage(uint8_t *pDst, int numBytes);

	virtual void FpGetImage2(uint16_t nRows, uint16_t nCols, uint8_t *pDst, uint16_t nBlobSize, uint8_t *pBlob);

	virtual void GpioSetPinType(uint16_t portId, uint32_t mskPins, uint16_t pinType);

	virtual void GpioPinRead(uint16_t portId, uint32_t mskPins, uint32_t* pMskPinState);

	virtual void GpioPinWrite(uint16_t portId, uint32_t mskPins, uint32_t pMskPinState);

	virtual void UpdateMPC04Firmware();

	virtual void FpMpcGetSelfTestResults(uint16_t overSamples, uint32_t arValues[MPC_SELF_TEST_BUFFER]);

protected:

	static bool	_bDLLInitialized;
	const uint32_t TIMEOUT = 500;
};