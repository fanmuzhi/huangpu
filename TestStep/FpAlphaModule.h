#pragma once

#ifdef FPALPHAMODULE_EXPORTS
	#define SYNA_FPALPHAMODULE_API __declspec(dllexport)
#else
	#define SYNA_FPALPHAMODULE_API __declspec(dllimport)
#endif

#include "syn_bridge.h"

#define TIMEOUT_VALUE 2000

//error code
#define ERROR_TIME_OUT				0x1500
#define ERROR_CRC_VERIFY			0x1501
#define ERROR_BL_MODE				0x1502
#define ERROR_PARAM_UNDEFINE		0x1502
#define ERROR_TYPE					0x1503

typedef enum{ OFF_REPLYSENT, CMDWAIT, CMDPROC, REPLY };

typedef enum{ UNKNOWN, ABSENT, STILL, MOVING };

typedef struct Alpha_Sensor_Status
{
	unsigned int SOFTSTATE;
	unsigned int SOFTSTATE_EPSTATE;
	unsigned int SOFTSTATE_FPSTATE;
	unsigned int SOFTSTATE_SSLSTATE;
	unsigned int EP2FLUSH;//Unused in Denali/Hayes
	unsigned int JUSTWOKE;
	unsigned int STATECHANGED;
	unsigned int EP2INSIZE;//Unused in Denali/Hayes
	unsigned int EP2INDONE;//Unused in Denali/Hayes
	unsigned int RUNNING;
	unsigned int EP1OUT;
	unsigned int EP1IN;
	unsigned int DRDY;
	unsigned int JUSTRESET;
	unsigned int ALIVE;
}Alpha_Sensor_Status_t;

class SYNA_FPALPHAMODULE_API FpAlphaModule
{

public:

	enum AlphaSensorType{ Viper2, Metallica };

	static uint32_t CreateModuleInstance(AlphaSensorType Type, syn_bridge *ipSynBridge, FpAlphaModule * &opAlphaModule);

	FpAlphaModule();
	virtual ~FpAlphaModule();

	void SetBridge(syn_bridge *ipSynBridge);

	virtual uint32_t PowerOn(uint32_t vcc, uint32_t spivcc, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t PowerOff(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpTidleSet(uint16_t idletime = 0, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetVersion(uint8_t *arrVersion, uint32_t size = 38, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetStartInfo(uint8_t *arrVersion, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpLoadPatch(uint8_t* pPatch, uint32_t numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetImage(uint8_t *arrImage, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpOtpRomTagRead(uint32_t nExtTag, uint8_t* pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpOtpRomTagWrite(uint8_t* pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpWritePrintFile(uint8_t *pPrintPatch, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpOtpRomRead(int section, int sector, uint8_t* pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpOtpRomWrite(int section, int sector, uint8_t* pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpRunPatchTest(uint8_t *pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpDisableSleep(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpEnterSleep(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetImage2(uint16_t nRows, uint16_t nCols, uint8_t *pDst, uint16_t nBlobSize, uint8_t *pBlob, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpUnloadPatch(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpPokeRegister(uint32_t nHwRegAddr, uint32_t nData, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpPeekRegister(uint32_t nHwRegAddr, uint8_t pDst[], uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpUpdateADCOffsets(uint32_t arrAdcbaselines[4], uint32_t timeout = TIMEOUT_VALUE);

protected:

	uint32_t getStatus(Alpha_Sensor_Status_t &oSensorStatus);

	uint32_t executeCmd(uint8_t cmdname, uint8_t *cmdbufp, uint32_t buflen, uint8_t *replybufp, uint32_t replyszie, uint16_t &replystatus, uint32_t timeout);

	uint32_t writeCmd(uint8_t cmdname, uint8_t *cmdbufp, uint32_t buflen, uint32_t timeout);

	uint32_t readCmd(uint8_t endpoint, uint8_t *arrRep, uint32_t size, uint32_t timeout);

protected:

	syn_bridge *_pSynBridge;
};

