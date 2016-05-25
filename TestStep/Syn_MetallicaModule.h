#pragma once

//Local
#include "Syn_Module.h"

class Syn_MetallicaModule :public Syn_Module
{

public:
	Syn_MetallicaModule();
	virtual ~Syn_MetallicaModule();

	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx);

	bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);

	void TrimOsc(OscTrimInfo &iOscTrimInfo, OscTrimResults &ioOscTrimResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV);

	void TrimSlowOsc(SlowOscInfo &iSlowOscInfo, SlowOscResults &ioSlowOscResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV);
};

