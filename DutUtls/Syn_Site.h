#pragma once

//local
#include "Syn_Config.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"

//std
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

enum Syn_TestState{TestReady = 0xAEF0,TestRunning,TestOK,TestError,TestFailed};

struct Syn_OTPTestInfo
{
	uint8_t _BootSector0Array[BS0_SIZE];
	uint8_t _BootSector1Array[BS1_SIZE];
	
	uint8_t _MainSector0Array[MS1_SIZE];//MS0_SIZE
	uint8_t _MainSector1Array[MS1_SIZE];

	Syn_TestState _TestState;

	std::string _strErrorMessage;

	uint32_t _uiSerialNumber;
	unsigned int _uiSiteNumber;
};

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	void Init();

	static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(Syn_SysConfig &iSyn_SysConfigInfo, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);

	void Run();

	void GetOTPTestInfo(Syn_OTPTestInfo &oSyn_OTPTestInfo);

private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

	Syn_OTPTestInfo _OTPTestInfo;

	unsigned int _iSiteNumber;
};

