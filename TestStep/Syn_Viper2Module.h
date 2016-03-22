#pragma once
#include "Syn_ViperModule.h"
class Syn_Viper2Module : public Syn_ViperModule
{
public:
	Syn_Viper2Module();
	virtual ~Syn_Viper2Module();

	bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult);
};

