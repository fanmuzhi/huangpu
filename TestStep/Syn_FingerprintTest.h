#pragma once

//Local
#include "Syn_TestStep.h"
#include "Syn_Exception.h"

//std
#include <time.h>

//windows
#include <windows.h>

//third-party
#include "easylogging++.h"

class Syn_FingerprintTest :public Syn_TestStep
{

public:

	Syn_FingerprintTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut);

	virtual ~Syn_FingerprintTest();
	
	void PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep);

	void PowerOff();

	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx);

	bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);

	void GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols);

	void GetFingerprintImageForCurrentTest(CalibrationResults& pCalResults, FPSFrame* pFrame, int nNumRows, int nNumCols, uint32_t* pCurrentDrawVals, int nGain);

	void CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults);

	bool CheckDUTexists();

	bool ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol = std::string(" "));

	void ImageDecode(FPSFrame *pDecodeFrame, FPSFrame *pEncodeFrame, int nNumRow, int nNumCol, int nNumFrames);

	void ComputeRunningTime(double &ioRunningTime);

	void ModifySweepWofCmdData(uint8_t* pSweepCmd);

	void ModifySweepSCMWofCmdData(uint8_t* pSweepCmd);

	void RemoveBaseline(FPSFrame *pImgFingerArr, FPSFrame *pImgNoFingerArr, int nNumRow, int nNumCol);

	int max_array(int a[], int num_elements);

	void TrimOsc(OscTrimInfo &iOscTrimInfo, OscTrimResults &ioOscTrimResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV);

	void TrimSlowOsc(SlowOscInfo &iSlowOscInfo, SlowOscResults &ioSlowOscResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV);

protected:

	Syn_Module *_pSyn_Module;

	DWORD _starttime, _finishtime;
	//time_t _starttime, _finishtime;
};