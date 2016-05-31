#pragma once

//Local
#include "Syn_Module.h"

class Syn_ViperModule :public Syn_Module
{
public:
	Syn_ViperModule();
	virtual ~Syn_ViperModule();

	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx);

	virtual void ModifySweepWofCmdData(uint8_t* pSweepCmd);

	virtual void ModifySweepSCMWofCmdData(uint8_t* pSweepCmd);

	virtual void TrimOsc(OscTrimInfo &iOscTrimInfo, OscTrimResults &ioOscTrimResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV);

	virtual void TrimSlowOsc(SlowOscInfo &iSlowOscInfo, SlowOscResults &ioSlowOscResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV);
};

