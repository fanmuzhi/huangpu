#include "Syn_Metallica.h"


Syn_Metallica::Syn_Metallica()
{
}


Syn_Metallica::~Syn_Metallica()
{
}

void Syn_Metallica::GetProjectType(ProjectType &oProjectType)
{
	oProjectType = Metallica;
}

void Syn_Metallica::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	//For Metallica sensors, we copy once into Print Patch.
	memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
}