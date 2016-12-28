//local
#include "Syn_BinCodes.h"

/////////////////////////////////////////////////////////////////////
//Statics
/////////////////////////////////////////////////////////////////////
std::string	Syn_BinCodes::m_sPass = "1";
std::string	Syn_BinCodes::m_sDutCommFail = "100";
std::string	Syn_BinCodes::m_s101Unused = "101";
std::string	Syn_BinCodes::m_sOscTrimCalFail = "102";
std::string	Syn_BinCodes::m_sSlowOscTrimCalFail = "103";
std::string	Syn_BinCodes::m_sDRdyPinFail = "104";
std::string	Syn_BinCodes::m_sExtTagsInvalidation = "105";		//Not used.
std::string	Syn_BinCodes::m_sOtpReadWriteFail = "106";		//Not used.
std::string	Syn_BinCodes::m_sOpensShortsFail = "107";
std::string	Syn_BinCodes::m_sStages1Or2CalFail = "108";
std::string	Syn_BinCodes::m_sButtonFail = "109";
std::string	Syn_BinCodes::m_sPeggedPixFail = "110";
std::string	Syn_BinCodes::m_sFlooredPixFail = "111";
std::string	Syn_BinCodes::m_sConsecutivePeggedPixFail = "112";
std::string	Syn_BinCodes::m_sConsecutiveFlooredPixFail = "113";		//Not used.
std::string	Syn_BinCodes::m_sDeltaPixFail = "114";
std::string	Syn_BinCodes::m_sImagingCurrentFail = "115";
std::string	Syn_BinCodes::m_sIdleCurrentFail = "116";		//Not used.
std::string	Syn_BinCodes::m_sWofWovarCurrentFail = "117";
std::string	Syn_BinCodes::m_sWofLowPowerFail = "117.1";
std::string	Syn_BinCodes::m_sSleepCurrentFail = "118";		//Not used.
std::string	Syn_BinCodes::m_sSnrFail = "119";
std::string	Syn_BinCodes::m_sPixelUniformityFail = "120";
std::string	Syn_BinCodes::m_sSdkBaselineTestFail = "120.1";
std::string	Syn_BinCodes::m_sFwUpgradeFail = "121";		//Not used.
std::string	Syn_BinCodes::m_sSecurityPatchFail = "122";
std::string	Syn_BinCodes::m_sSerialNumFail = "123";		//Not used.
std::string	Syn_BinCodes::m_sProductIDTest = "124";
std::string	Syn_BinCodes::m_sMainOtpReadWriteFail = "125";		//Not used.
std::string	Syn_BinCodes::m_sSharpnessFail = "126";
std::string	Syn_BinCodes::m_sLaplacianFail = "127";		//Not used.
std::string	Syn_BinCodes::m_sStdDevOfRxFail = "128";
std::string	Syn_BinCodes::m_sImperfectionsFail = "129";
std::string	Syn_BinCodes::m_sDutCtrlCommFail = "130";		//Not used.
std::string	Syn_BinCodes::m_sWofTestFail = "131";
std::string	Syn_BinCodes::m_sExtTagAnomoly = "132";
std::string	Syn_BinCodes::m_sPixPatchFail = "133";
std::string	Syn_BinCodes::m_sDutTimeout = "134";
std::string	Syn_BinCodes::m_sWoVarTestFail = "135";
std::string	Syn_BinCodes::m_sRAMTestFail = "136";
std::string	Syn_BinCodes::m_sBuildNumMismatch = "137";		//Not used.
std::string	Syn_BinCodes::m_sLedTestFail = "138";
std::string	Syn_BinCodes::m_sUsbCommFail = "139";
std::string	Syn_BinCodes::m_sStage2VarianceFail = "140";
std::string	Syn_BinCodes::m_sMs0ChecksumFail = "141";
std::string	Syn_BinCodes::m_sAFEMatchPatchFail = "142";		//Not used.
std::string	Syn_BinCodes::m_sVoltageMismatch = "143";
std::string	Syn_BinCodes::m_sAutoJigCtlFail = "144";		//Not used.
std::string	Syn_BinCodes::m_sLayerIdFail = "145";		//Not used.
std::string	Syn_BinCodes::m_sFlexIdFail = "146";		//Not used.
std::string	Syn_BinCodes::m_sConfigFileAnomaly = "147";
std::string	Syn_BinCodes::m_sSpiFlashFail = "148";
std::string	Syn_BinCodes::m_sSpiOwnershipFail = "148.1";
std::string	Syn_BinCodes::m_sMpcVoltageFail = "149";
std::string	Syn_BinCodes::m_sPackageTypeFail = "150";		//Not used.
std::string	Syn_BinCodes::m_sAFETestFail = "151";
std::string Syn_BinCodes::m_sBootSectorFail = "152";
std::string Syn_BinCodes::m_sMainSectorFail = "153";
std::string Syn_BinCodes::m_sROMChecsum = "154";
