#pragma once

//DutUtls
#include "Syn_DutCtrl.h"

extern "C" {
#include "SYN_TestUtils.h"
#include <string.h>
}

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

	void CalculateLnaOffsetsBinarySearch(FPSFrame* pFrame, uint8_t* pLnaValues, int nNumRows, int nNumCols, CalibrationInfo &CalInfo, CalibrationResults &CalResults);

	void GetRowAverages(FPSFrame* pFrame, int nColBegin, int nColEnd, uint8_t* pAverages, int nNumRows);

	bool CheckDUTexists();


	//virtual function
	virtual	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx) = 0;

	virtual bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult) = 0;

protected:

	Syn_DutCtrl * _pSyn_DutCtrl;
};