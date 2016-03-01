#pragma once

//Local
#include "Syn_TestStep.h"

//
#include <string>

class Syn_TestStepFactory
{
public:
	//Syn_TestStepFactory();
	~Syn_TestStepFactory();

	static bool CreateTestStepInstance(std::string strTestStepName, std::string strTestArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut, Syn_TestStep * &opTestStepInstance);

private:
	Syn_TestStepFactory();
};

