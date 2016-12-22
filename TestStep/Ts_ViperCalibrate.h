#pragma once

#include "Ts_Calibrate.h"

class Ts_ViperCalibrate : public Ts_Calibrate
{
public:
	Ts_ViperCalibrate(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut);
	~Ts_ViperCalibrate();

	bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);

	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx);
};

