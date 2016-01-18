#pragma once

//Local:DutCtrl
#include "Syn_DutCtrl.h"
#include "Syn_Config.h" 

extern "C" {
#include "SYN_TestUtils.h"
}

//std
#include <string>
#include <vector>
using namespace std;

enum ProjectType {
	Viper1 = 0x1000,
	Viper2,
	Metallica
};

class Syn_Dut
{
public:
	Syn_Dut();
	virtual ~Syn_Dut();

	//Gobal Function:Create a Dut Instance
	static bool CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance);
	//static bool CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance, uint32_t iSerialNumber, DutController iDutControllerType);

	//Set and Get DutCtrl
	bool SetDutCtrl(Syn_DutCtrl * ipSyn_DutCtrl);
	Syn_DutCtrl * GetDutCtrl();

	//virtual function
	virtual void GetProjectType(ProjectType &oProjectType) = 0;
	virtual	void CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx) = 0;


	//list all test itmes here
	void PowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep);

	void PowerOff();

	bool ReadOTP(uint8_t* oarMS0, int iSize);

	bool GetFPImage();

	inline void SetPatchInfo(vector<Syn_PatchInfo> ilistOfPatchInfo){ _listOfPatchInfo = ilistOfPatchInfo; };

private:

	bool FindPatch(std::string patchName, Syn_PatchInfo &patchInfo);

protected:
	
	Syn_DutCtrl *_pSyn_DutCtrl;

	vector<Syn_PatchInfo> _listOfPatchInfo;

};