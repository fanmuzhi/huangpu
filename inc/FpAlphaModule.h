#pragma once

#ifdef FPALPHAMODULE_EXPORTS
	#define SYNA_FPALPHAMODULE_API __declspec(dllexport)
#else
	#define SYNA_FPALPHAMODULE_API __declspec(dllimport)
#endif

#include "syn_bridge.h"

#define ROW_SIZE_MAX		201
#define COLUMN_SIZE_MAX		200	
#define ALPHA_HEADER		8

#define TIMEOUT_VALUE 500

//error code
#define FPMODULE_ERROR_TIME_OUT				0x1500
#define FPMODULE_ERROR_CRC_VERIFY			0x1501
#define FPMODULE_ERROR_BL_MODE				0x1502
#define FPMODULE_ERROR_PARAM_UNDEFINE		0x1503
#define FPMODULE_ERROR_TYPE					0x1504
#define FPMODULE_ERROR_IMAGE_MAXSIZE		0x1505
#define FPMODULE_ERROR_PARAM_NULL			0x1506

typedef enum{ OFF_REPLYSENT, CMDWAIT, CMDPROC, REPLY };

typedef enum{ UNKNOWN, ABSENT, STILL, MOVING };

typedef struct Alpha_Sensor_Status
{
	unsigned int SOFTSTATE;
	unsigned int SOFTSTATE_EPSTATE;
	unsigned int SOFTSTATE_FPSTATE;
	unsigned int SOFTSTATE_SSLSTATE;
	unsigned int EP2FLUSH;
	unsigned int JUSTWOKE;
	unsigned int STATECHANGED;
	unsigned int EP2INSIZE;
	unsigned int EP2INDONE;
	unsigned int RUNNING;
	unsigned int EP1OUT;
	unsigned int EP1IN;
	unsigned int DRDY;
	unsigned int JUSTRESET;
	unsigned int ALIVE;
}Alpha_Sensor_Status_t;

enum class ChecksumType{ XOR, CRC32, SHA256, SHA1 };

class SYNA_FPALPHAMODULE_API FpAlphaModule
{
public:

	enum AlphaSensorType{ Viper2, Metallica };

	static uint32_t CreateModuleInstance(AlphaSensorType Type, syn_bridge *ipSynBridge, FpAlphaModule * &opAlphaModule);

	FpAlphaModule();
	virtual ~FpAlphaModule();

	void SetBridge(syn_bridge *ipSynBridge);

	void GetBridge(syn_bridge * &opSynBridge);

	virtual uint32_t PowerOn(uint32_t vcc, uint32_t spivcc, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t PowerOff(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpTidleSet(uint16_t idletime = 0, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpReset(uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetVersion(uint8_t *arrVersion, uint32_t size = 38, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetStartInfo(uint8_t *arrStartInfo, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetPatchInfo(uint8_t *arrPatchInfo, uint32_t size, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpLoadPatch(uint8_t* pPatch, uint32_t numBytes, uint8_t* pDst = NULL, uint32_t numbytes  = NULL, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpUnloadPatch(uint32_t patchindex = 0, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpOtpRomTagRead(uint32_t nExtTag, uint8_t* pDst, int numBytes, int &ntags, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpOtpRomTagWrite(uint8_t* pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpOtpRomRead(int section, int sector, uint8_t* pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpOtpRomWrite(int section, int sector, uint8_t* pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpRunPatchTest(uint8_t *pDst, int numBytes, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetImage(uint16_t nRows, uint16_t nCols, uint16_t nBlobSize, uint8_t *pBlob, uint32_t *arrValue, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpGetImage2(uint16_t nRows, uint16_t nCols, uint8_t *pDst, uint16_t nBlobSize, uint8_t *pBlob, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpRunWOFPlot(uint8_t *wofCmd1, uint16_t wofCmd1Size, uint8_t *wofCmd2, uint16_t wofCmd2Size, uint8_t *pDst, uint16_t responseSize, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpRunSCMWOFPlot(uint8_t *wofPlot, uint16_t wofPlotSize, uint8_t *wofBin, uint16_t wofBinSize, uint8_t * wofSweep, uint16_t wofSweepSize, uint8_t *pDst, uint16_t responseSize, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpPokeRegister(uint32_t nHwRegAddr, uint32_t value, uint8_t opsize = 4, uint32_t timeout = TIMEOUT_VALUE);

	/*opsize default value : 4*/
	virtual uint32_t FpPeekRegister(uint32_t nHwRegAddr, uint8_t opsize, uint32_t &ovalue, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpROMChecksum(ChecksumType Type, uint32_t &oChecksumValue, uint32_t timeout = TIMEOUT_VALUE);

	virtual uint32_t FpUpdateADCOffsets(uint32_t arrAdcbaselines[4], uint32_t timeout = TIMEOUT_VALUE);

	virtual void ImageDecode(uint8_t arrDecodeFrame[][COLUMN_SIZE_MAX], uint8_t arrEncodeFrame[][COLUMN_SIZE_MAX], int nNumRow, int nNumCol) = 0;

protected:

	uint32_t getStatus(Alpha_Sensor_Status_t &oSensorStatus);

	uint32_t executeCmd(uint8_t cmdname, uint8_t *cmdbufp, uint32_t buflen, uint8_t *replybufp, uint32_t replyszie, uint16_t &replystatus, uint32_t timeout);

	uint32_t writeCmd(uint8_t cmdname, uint8_t *cmdbufp, uint32_t buflen, uint32_t timeout);

	uint32_t readCmd(uint8_t endpoint, uint8_t *arrRep, uint32_t size, uint32_t timeout);

	//virtual uint32_t modifyWofCmd(uint8_t *wofCmd) = 0;

	//virtual uint32_t modifySweepSCMWofCmdData(uint8_t *wofCmd) = 0;

protected:

	syn_bridge *_pSynBridge;
};
