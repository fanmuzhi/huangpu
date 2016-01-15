#pragma once
#include "Syn_Dut.h"
class Syn_Metallica :
	public Syn_Dut
{
public:
	Syn_Metallica();
	~Syn_Metallica();

	void GetProjectType(ProjectType &oProjectType);
	virtual	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx);
};