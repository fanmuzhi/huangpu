#pragma once

//Local
#include "Syn_Module.h"

class Syn_MetallicaModule :public Syn_Module
{

public:
	Syn_MetallicaModule();
	virtual ~Syn_MetallicaModule();

	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx);

	bool CalculatePgaOffsets_OOPP(Syn_DutCtrl * &pDutCtrl,uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);

};

