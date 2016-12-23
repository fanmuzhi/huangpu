#pragma once

//Local
#include "Syn_TestStep.h"
#include "Syn_Exception.h"

//std
#include <time.h>

//windows
#include <windows.h>

//third-party
#ifdef _DEBUG
#include "easylogging++.h"
#endif

class Syn_FingerprintTest :public Syn_TestStep
{

public:

	Syn_FingerprintTest(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);

	virtual ~Syn_FingerprintTest();

	//void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx);

	//bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);

	void GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols);

	//void CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults);

	void ImageDecode(FPSFrame *pDecodeFrame, FPSFrame *pEncodeFrame, int nNumRow, int nNumCol, int nNumFrames);

	bool ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol = std::string(" "));

	void ComputeRunningTime(double &ioRunningTime);

	void RemoveBaseline(FPSFrame *pImgFingerArr, FPSFrame *pImgNoFingerArr, int nNumRow, int nNumCol);

	int max_array(int a[], int num_elements);

protected:

	ULONGLONG _startTime, _finishTime;
};