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

	virtual uint8_t FpOtpRomTagRead(uint32_t nExtTag, uint8_t* pDst, int numBytes) = 0;

	virtual void FpWaitForCMDComplete() = 0;

	virtual void FpReadBuff(uint8_t *pDst, int numBytes) = 0;

	virtual void FpReadAndCheckStatus(uint16_t statusIgnore) = 0;		//check status of VCSFW_STATUS

	virtual void FpWaitDeviceReady() = 0;

	virtual void FpDisableSleep() = 0;

	virtual void FpGetVersion(uint8_t *pDst, int numBytes) = 0;

	virtual void FpWritePrintFile(uint8_t *pPrintPatch, int numBytes) = 0;

	virtual void FpGetImage(uint8_t *pDst, int numBytes) = 0;
	
	virtual void FpGetImage2(uint16_t nRows, uint16_t nCols, uint8_t *pDst, uint16_t nBlobSize, uint8_t *pBlob) = 0;

	virtual void GpioSetPinType(uint16_t portId, uint32_t mskPins, uint16_t pinType) = 0;

	virtual void GpioPinRead(uint16_t portId, uint32_t mskPins, uint32_t* pMskPinState) = 0;

	virtual void GpioPinWrite(uint16_t portId, uint32_t mskPins, uint32_t pMskPinState) = 0;

	virtual void UpdateMPC04Firmware() = 0;

	virtual void GetCurrentSenseValues(uint16_t gainIdx, uint16_t oversample, uint32_t arCurrentSenseValues[4]) {}

protected:

	uint32_t syn_SerialNumber;
	uint32_t syn_DeviceHandle;
};