#pragma once

#include <string>

class Syn_BinCodes
{
public:
	
	static std::string	m_sPass;						//1
	static std::string	m_sDutCommFail;					//100
	static std::string	m_s101Unused;					//101
	static std::string	m_sOscTrimCalFail;				//102	
	static std::string	m_sSlowOscTrimCalFail;			//103	
	static std::string	m_sDRdyPinFail;					//104
	static std::string	m_sExtTagsInvalidation;			//105
	static std::string	m_sOtpReadWriteFail;			//106
	static std::string	m_sOpensShortsFail;				//107
	static std::string	m_sStages1Or2CalFail;			//108
	static std::string	m_sButtonFail;					//109
	static std::string	m_sPeggedPixFail;				//110
	static std::string	m_sFlooredPixFail;				//111
	static std::string	m_sConsecutivePeggedPixFail;	//112
	static std::string	m_sConsecutiveFlooredPixFail;	//113
	static std::string	m_sDeltaPixFail;				//114
	static std::string	m_sImagingCurrentFail;			//115
	static std::string	m_sIdleCurrentFail;				//116		//Not used.
	static std::string	m_sWofWovarCurrentFail;			//117
	static std::string	m_sWofLowPowerFail;				//117.1
	static std::string	m_sSleepCurrentFail;			//118		//Not used.
	static std::string	m_sSnrFail;						//119
	static std::string	m_sPixelUniformityFail;			//120 
	static std::string  m_sSdkBaselineTestFail;		    //120.1
	static std::string	m_sFwUpgradeFail;				//121		//Not used.
	static std::string	m_sSecurityPatchFail;			//122		//Not used.
	static std::string	m_sSerialNumFail;				//123 
	static std::string	m_sProductIDTest;				//124
	static std::string	m_sMainOtpReadWriteFail;		//125
	static std::string	m_sSharpnessFail;				//126 
	static std::string	m_sLaplacianFail;				//127		//Not used.
	static std::string	m_sStdDevOfRxFail;				//128 
	static std::string	m_sImperfectionsFail;			//129
	static std::string	m_sDutCtrlCommFail;				//130
	static std::string	m_sWofTestFail;					//131
	static std::string	m_sExtTagAnomoly;				//132
	static std::string	m_sPixPatchFail;				//133
	static std::string	m_sDutTimeout;					//134
	static std::string	m_sWoVarTestFail;				//135
	static std::string	m_sRAMTestFail;					//136
	static std::string	m_sBuildNumMismatch;			//137
	static std::string	m_sLedTestFail;					//138
	static std::string	m_sUsbCommFail;					//139
	static std::string	m_sStage2VarianceFail;			//140
	static std::string	m_sMs0ChecksumFail;				//141
	static std::string	m_sAFEMatchPatchFail;			//142
	static std::string	m_sVoltageMismatch;				//143
	static std::string	m_sAutoJigCtlFail;				//144
	static std::string	m_sLayerIdFail;					//145
	static std::string	m_sFlexIdFail;					//146
	static std::string	m_sConfigFileAnomaly;			//147
	static std::string	m_sSpiFlashFail;				//148
	static std::string	m_sSpiOwnershipFail;			//148.1
	static std::string	m_sMpcVoltageFail;				//149
	static std::string	m_sPackageTypeFail;				//150
	static std::string	m_sAFETestFail;					//151
	static std::string  m_sDynOffCorrDacTestFail;		//153
	static std::string  m_sLnaVgaGainTestFail;			//154
	static std::string  m_sIromChecksumTestFail;		//155
	static std::string  m_sFlexIdTestFail;				//156
	//huangpu define,not copy from autoclient
	static std::string  m_sBootSectorFail;				//152H
	static std::string  m_sMainSectorFail;				//153H
	static std::string  m_sROMChecsumFail;				//154H
};

