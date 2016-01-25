#pragma once
#include "Syn_Dut.h"
class Syn_Viper :public Syn_Dut
{
public:
	Syn_Viper();
	virtual ~Syn_Viper();

	//void InitializeForRun();
	virtual void GetProjectType(ProjectType &oProjectType);
	virtual	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx);

};

