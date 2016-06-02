#pragma once

//DutUtls
#include "Syn_DutCtrl.h"

#include "Syn_TestUtils.h"

//windows API
#include "windows.h"

class Syn_Module
{
public:
	Syn_Module();
	virtual ~Syn_Module();

	void SetDutCtrl(Syn_DutCtrl * &pDutCtrl);

	//comman function
	void PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep);

	void PowerOff();

	void GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols);

	void GetFingerprintImageForCurrentTest(CalibrationResults& pCalResults, FPSFrame* pFrame, int nNumRows, int nNumCols, uint32_t* pCurrentDrawVals, int nGain);

	void CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults);

	void GetRowAverages(FPSFrame* pFrame, int nColBegin, int nColEnd, uint8_t* pAverages, int nNumRows);

	bool CheckDUTexists();

	virtual void ImageDecode(FPSFrame *pDecodeFrame, FPSFrame *pEncodeFrame, int nNumRow, int nNumCol, int nNumFrames);

	int8_t CalcPgaOffset(uint8_t nPixelValue, float nRatio, float nConfigRatio);

	//virtual function
	virtual	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx) = 0;

	virtual bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult) = 0;

	virtual void TrimOsc(OscTrimInfo &iOscTrimInfo, OscTrimResults &ioOscTrimResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV) = 0;

	virtual void TrimSlowOsc(SlowOscInfo &iSlowOscInfo, SlowOscResults &ioSlowOscResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV) = 0;

	//virtual void FpWriteOscTrim(uint32_t nOscTrimValue);

	//virtual void WriteBootSector(int nSection, uint8_t* pOtpBootSector, uint8_t* pBootSectorMask);

	//virtual void FpWriteSlowOscFreq(uint32_t nHvOscTrim, uint32_t nHvOscBias);

	virtual void ModifySweepWofCmdData(uint8_t* pSweepCmd) {}

	virtual void ModifySweepSCMWofCmdData(uint8_t* pSweepCmd) {}


protected:

	Syn_DutCtrl * _pSyn_DutCtrl;
};