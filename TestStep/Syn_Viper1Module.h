#pragma once
#include "Syn_ViperModule.h"
class Syn_Viper1Module : public Syn_ViperModule
{
public:
	Syn_Viper1Module();
	virtual ~Syn_Viper1Module();

	bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);
};

