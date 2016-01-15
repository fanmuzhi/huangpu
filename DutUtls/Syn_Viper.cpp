//Local
#include "Syn_Viper.h"


Syn_Viper::Syn_Viper()
:Syn_Dut()
{
}


Syn_Viper::~Syn_Viper()
{
}

//void Syn_Viper::InitializeForRun()
//{
//
//}

void Syn_Viper::GetProjectType(ProjectType &oProjectType)
{

}

void Syn_Viper::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	//For Viper sensors, we copy twice into Print Patch. Make sure 2nd copy is alligned on
	//a four byte boundary.
	if ((nNumBytes % 4) == 0)
	{
		memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
		memcpy(&pPrintPatch[nPatchIdx + nNumBytes], pSrc, nNumBytes);
	}
	else
	{
		//Make sure the space between the two copies of data are set to zero.
		memset(&pPrintPatch[nPatchIdx], 0, nNumBytes + (4 - (nNumBytes % 4)));
		memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
		memcpy(&pPrintPatch[nPatchIdx + (nNumBytes + (4 - (nNumBytes % 4)))], pSrc, nNumBytes);
	}
}
