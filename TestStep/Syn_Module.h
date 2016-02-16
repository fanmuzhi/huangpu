#pragma once

//DutUtls
#include "Syn_DutCtrl.h"

extern "C" {
#include "SYN_TestUtils.h"
#include <string.h>
}

//enum Syn_ProjectType {
//	Syn_Viper1 = 0x1000,
//	Syn_Viper2,
//	Syn_Metallica
//};

class Syn_Module
{
public:
	Syn_Module();
	virtual ~Syn_Module();

	void GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols);

	void SetDutCtrl(Syn_DutCtrl * &pDutCtrl);

	//
	void PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep);

	void PowerOff();

	//virtual function
	virtual	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx) = 0;

	virtual bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult) = 0;

protected:

	Syn_DutCtrl * _pSyn_DutCtrl;
};