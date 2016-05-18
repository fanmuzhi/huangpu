//Local
#include "Syn_ViperModule.h"

Syn_ViperModule::Syn_ViperModule()
{
}

Syn_ViperModule::~Syn_ViperModule()
{
}

void Syn_ViperModule::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
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

void Syn_ViperModule::ModifySweepWofCmdData(uint8_t* pSweepCmd) 
{ 
	pSweepCmd[0x0C] = 100;	//Gain start.
	pSweepCmd[0x10] = 100;	//Gain increment.
	pSweepCmd[0x14] = 200;	//Gain end.
}

void Syn_ViperModule::ModifySweepSCMWofCmdData(uint8_t* pSweepCmd) 
{
	pSweepCmd[0x0B] = 100;	//Gain start.
	pSweepCmd[0x0F] = 100;	//Gain increment.
	pSweepCmd[0x13] = 200;	//Gain end.
}