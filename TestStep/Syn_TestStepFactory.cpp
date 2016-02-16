//Local
#include "Syn_TestStepFactory.h"
#include "Syn_FingerprintTest.h"
#include "Syn_Calibrate.h"

Syn_TestStepFactory::Syn_TestStepFactory()
{
}


Syn_TestStepFactory::~Syn_TestStepFactory()
{
}

bool Syn_TestStepFactory::CreateTestStepInstance(std::string strTestStepName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut, Syn_TestStep * &opTestStepInstance)
{
	opTestStepInstance = NULL;

	if (NULL == pDutCtrl)
	{
		return false;
	}
	if (NULL == pDut)
	{
		return false;
	}

	bool rc = true;
	if (std::string("Calibrate") == strTestStepName)
	{
		opTestStepInstance = new Syn_Calibrate(strTestStepName, pDutCtrl, pDut);
	}
	else
	{
		rc = false;
	}

	return rc;
}