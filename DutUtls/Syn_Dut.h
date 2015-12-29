#pragma once

//Local:DutCtrl
#include "Syn_DutCtrl.h"

extern "C" {
#include "inc/SYN_TestUtils.h"
}

//std
#include <string>
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
	//static bool CreateDutInstance(ProjectType iType, Syn_Dut ** opSyn_DutInstance);
	static bool CreateDutInstance(ProjectType iType, Syn_Dut * &opSyn_DutInstance, uint32_t iSerialNumber, DutController iDutControllerType);

	//Set and Get DutCtrl
	bool SetDutCtrl(Syn_DutCtrl * ipSyn_DutCtrl);
	Syn_DutCtrl * GetDutCtrl();

	//virtual function
	//virtual void InitializeForRun() = 0;
	virtual void GetProjectType(ProjectType &oProjectType) = 0;

	//bool PowerOn();
	void CycleDutPowerOn(int nPwrVdd, int nPwrVio, int nPwrVled, int nPwrVddh, bool bDisableSleep);

	bool ReadOTP();


protected:

	Syn_DutCtrl *_pSyn_DutCtrl;

};

