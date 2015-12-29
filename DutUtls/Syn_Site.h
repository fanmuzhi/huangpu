#pragma once

//local
#include "Syn_Config.h"
#include "Syn_Dut.h"

class Syn_Site
{
public:

	Syn_Site();
	~Syn_Site();

	//
	void Run();


private:

	Syn_SysConfig _SysConfig;

	Syn_Dut *_pSyn_Dut;

};

