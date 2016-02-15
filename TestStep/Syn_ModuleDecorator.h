#pragma once

//DutUtls
#include "Syn_DutCtrl.h"

extern "C" {
#include "SYN_TestUtils.h"
#include <string.h>
}

enum Syn_ProjectType {
	Syn_Viper1 = 0x1000,
	Syn_Viper2,
	Syn_Metallica
};


class Syn_ModuleDecorator
{
public:
	Syn_ModuleDecorator();
	virtual ~Syn_ModuleDecorator();

	//function
	virtual	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx) = 0;

	virtual bool CalculatePgaOffsets_OOPP(Syn_DutCtrl * &pDutCtrl,uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult) = 0;

protected:

	void GetFingerprintImage(CalibrationResults &pCalResults, FPSFrame *pFrame, int nNumRows, int nNumCols,Syn_DutCtrl * &pDutCtrl);

};