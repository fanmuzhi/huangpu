#pragma once
#include "Syn_Viper.h"
class Syn_Viper1 :
	public Syn_Viper
{
public:
	Syn_Viper1();
	~Syn_Viper1();

	void GetProjectType(ProjectType &oProjectType);

	//just for test and debug,noc ontent
	inline virtual bool CalculatePgaOffsets_OOPP(uint16_t numCols, uint16_t numRows, CalibrationInfo &calInfo, CalibrationResults &calResult)
	{
		return false;
	};
};

