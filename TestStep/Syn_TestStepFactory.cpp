//Local
#include "Syn_TestStepFactory.h"
#include "Syn_FingerprintTest.h"
#include "Ts_Calibrate.h"
#include "Ts_OTPCheck.h"
#include "Ts_AcqImgNoFinger.h"
#include "Ts_AcqImgFinger.h"
#include "Ts_DRdyTest.h"
#include "Ts_PeggedPixelsTest.h"
#include "Ts_FlooredPixelsTest.h"
#include "Ts_ConsecutivePixels.h"

Syn_TestStepFactory::Syn_TestStepFactory()
{
}

Syn_TestStepFactory::~Syn_TestStepFactory()
{
}

bool Syn_TestStepFactory::CreateTestStepInstance(std::string strTestStepName, std::string strTestArgs,Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut, Syn_TestStep * &opTestStepInstance)
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
		opTestStepInstance = new Ts_Calibrate(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("OTPCheck") == strTestStepName)
	{
		opTestStepInstance = new Ts_OTPCheck(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("AcqImgNoFinger") == strTestStepName)
	{
		opTestStepInstance = new Ts_AcqImgNoFinger(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("AcqImgFinger") == strTestStepName)
	{
		opTestStepInstance = new Ts_AcqImgFinger(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("DRdyTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_DRdyTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("PeggedPixelsTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_PeggedPixelsTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("FlooredPixelsTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_FlooredPixelsTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("ConsecutivePixels") == strTestStepName)
	{
		opTestStepInstance = new Ts_ConsecutivePixels(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else
	{
		rc = false;
	}

	return rc;
}
