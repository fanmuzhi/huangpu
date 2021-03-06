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
#include "Ts_SNRTest.h"
#include "Ts_PixelTest.h"
#include "Ts_SharpnessTest.h"
#include "Ts_Imperfections.h"
#include "Ts_RxStandardDev.h"
#include "Ts_OTPWriteMainSector.h"
#include "Ts_PixelPatchTest.h"
#include "Ts_WoVarTest.h"
#include "Ts_CurrentTest.h"
#include "Ts_RetainMode.h"
#include "Ts_InitializationStep.h"
#include "Ts_FinalizationStep.h"
#include "Ts_OpensShortsTest.h"
#include "Ts_RAMTest.h"
#include "Ts_AFETest.h"
#include "Ts_WOFFD.h"
#include "Ts_WOFFU.h"
#include "Ts_WOFLowPower.h"
#include "Ts_SCM_WOF.h"
#include "Ts_ReadDutAdc.h"
#include "Ts_OTPWriteBootSector.h"
#include "Ts_ButtonTest.h"
#include "Ts_WaitStimulus.h"
#include "Ts_Invalidate.h"
#include "Ts_HuaweiImageQualityTest.h"
#include "Ts_OscTrim.h"
#include "Ts_SlowOscTrim.h"
#include "Ts_Viper_SCMWOF.h"
#include "Ts_ViperWOF.h"
#include "Ts_DeepSleepCurrent.h"

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

	if (std::string("InitializationStep") == strTestStepName)
	{
		opTestStepInstance = new Ts_InitializationStep(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("Calibrate") == strTestStepName)
	{
		opTestStepInstance = new Ts_Calibrate(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("OTPWriteBootSector") == strTestStepName)
	{
		opTestStepInstance = new Ts_OTPWriteBootSector(strTestStepName, strTestArgs, pDutCtrl, pDut);
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
	else if (std::string("SNRTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_SNRTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("PixelTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_PixelTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("SharpnessTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_SharpnessTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("Imperfections") == strTestStepName)
	{
		opTestStepInstance = new Ts_Imperfections(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("RxStandardDev") == strTestStepName)
	{
		opTestStepInstance = new Ts_RxStandardDev(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("OTPWriteMainSector") == strTestStepName)
	{
		opTestStepInstance = new Ts_OTPWriteMainSector(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("PixelPatchTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_PixelPatchTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("WoVarTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_WoVarTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("CurrentTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_CurrentTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("RetainModeCurrentTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_RetainMode(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("OpensShortsTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_OpensShortsTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("RAMTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_RAMTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("AFETest") == strTestStepName)
	{
		opTestStepInstance = new Ts_AFETest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	/*else if (std::string("WOFWithoutStimulus") == strTestStepName)
	{
		opTestStepInstance = new Ts_WOF(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("WOFWithStimulus") == strTestStepName)
	{
		opTestStepInstance = new Ts_WOF(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}*/
	else if (std::string("WOF_FD_WithoutStimulus") == strTestStepName || std::string("WOF_FD_WithStimulus") == strTestStepName)
	{
		if (pDut->_eProjectType == ProjectType::Viper1 || pDut->_eProjectType == ProjectType::Viper2)
			//Viper WOF
			opTestStepInstance = new Ts_ViperWOF(strTestStepName, strTestArgs, pDutCtrl, pDut);
		else if (pDut->_eProjectType == ProjectType::Metallica)
			//Metallica WOF
			opTestStepInstance = new Ts_WOFFD(strTestStepName, strTestArgs, pDutCtrl, pDut);
		else
			return false;
	}
	else if (std::string("WOF_FU_WithoutStimulus") == strTestStepName || std::string("WOF_FU_WithStimulus") == strTestStepName)
	{
		//Metallica Only Now
		opTestStepInstance = new Ts_WOFFU(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("WOF-LowPower") == strTestStepName)
	{
		opTestStepInstance = new Ts_WOFLowPower(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("SCM_WOFWithoutStimulus") == strTestStepName || std::string("SCM_WOFWithStimulus") == strTestStepName)
	{
		if (pDut->_eProjectType == ProjectType::Viper1 || pDut->_eProjectType == ProjectType::Viper2)
			//Viper SCM WOF
			opTestStepInstance = new Ts_Viper_SCMWOF(strTestStepName, strTestArgs, pDutCtrl, pDut);
		else if (pDut->_eProjectType == ProjectType::Metallica)
			//Metallica WOF
			opTestStepInstance = new Ts_SCM_WOF(strTestStepName, strTestArgs, pDutCtrl, pDut);
		else
			return false;
	}
	else if (std::string("ReadDutAdc") == strTestStepName)
	{
		opTestStepInstance = new Ts_ReadDutAdc(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("HuaweiImageQualityTest") == strTestStepName)
	{
		opTestStepInstance = new Ts_HuaweiImageQualityTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("ButtonTestWithoutStimulus") == strTestStepName)
	{
		opTestStepInstance = new Ts_ButtonTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("ButtonTestWithStimulus") == strTestStepName)
	{
		opTestStepInstance = new Ts_ButtonTest(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("FinalizationStep") == strTestStepName)
	{
		opTestStepInstance = new Ts_FinalizationStep(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("WaitStimulus") == strTestStepName)
	{
		opTestStepInstance = new Ts_WaitStimulus(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("Invalidate") == strTestStepName)
	{
		opTestStepInstance = new Ts_Invalidate(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("DeepSleepCurrent") == strTestStepName)
	{
		opTestStepInstance = new Ts_DeepSleepCurrent(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("OscTrim") == strTestStepName)
	{
		opTestStepInstance = new Ts_OscTrim(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else if (std::string("SlowOsc") == strTestStepName)
	{
		opTestStepInstance = new Ts_SlowOscTrim(strTestStepName, strTestArgs, pDutCtrl, pDut);
	}
	else
	{
		rc = false; 
	}

	return rc;
}