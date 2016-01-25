#pragma once
#include "Syn_Viper.h"
class Syn_Viper2 :
	public Syn_Viper
{
public:
	Syn_Viper2();
	~Syn_Viper2();

	void GetProjectType(ProjectType &oProjectType);

	//just for test and debug,noc ontent
	inline virtual bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
	{
		return false;
	};
};

