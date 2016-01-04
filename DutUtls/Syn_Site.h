#pragma once

//local
#include "Syn_Config.h"
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"

//std
#include <vector>

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	void Init();

	static bool ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance);

	static bool ConstructSiteList(Syn_SysConfig &iSyn_SysConfigInfo, std::vector<Syn_Site*> &olistOfSyn_SiteInstance);


	//
	//void Run();
	void Run(uint8_t * &arMS0, int iSize);



private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

};

