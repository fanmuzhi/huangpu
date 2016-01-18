//local
#include "Syn_Site.h"
#include "Syn_Exception.h"
#include "Syn_SysConfigOperation.h"

//MPC_API
#include "MpcApiDll.h"
//#include "MpcApiError.h"
//#include "MPCErrors.h"

//std
#include <iostream>
#include <io.h>
#include <stdio.h>

#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

Syn_Site::Syn_Site()
:_pSyn_Dut(NULL)
, _iSiteNumber(0)
{
	_DutTestInfo._siteInfo._TestState = TestReady;

	//LOG(INFO) << "easylogging++ test!";

	LOG(INFO) << "SITE START";
}

Syn_Site::~Syn_Site()
{
	if (NULL != _pSyn_Dut)
	{
		delete _pSyn_Dut;
		_pSyn_Dut = NULL;
	}
}

void Syn_Site::Init()
{
}

bool Syn_Site::ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance)
{
	bool rc(false);

	opSyn_SiteInstance = NULL;
	if (0 == iSerialNumber)
	{
		cout << "Error:Syn_Site::ConstructSiteInstance() - iSerialNumber is 0!" << endl;
		return false;
	}

	//ProejctType:Viper1,Viper2,Metallica
	std::string strProjectType(iSyn_SysConfigInfo._strDutType);
	ProjectType iProjectType;
	if (std::string("Viper") == strProjectType || std::string("Viper1") == strProjectType)
	{
		iProjectType = Viper1;
	}
	else if (std::string("Viper2") == strProjectType)
	{
		iProjectType = Viper2;
	}
	else if (std::string("Metallica") == strProjectType)
	{
		iProjectType = Metallica;
	}
	else
	{
		iProjectType = Viper1;
		cout << "Error:Syn_Site::ConstructSiteInstance() - an unknown ProjectType,construct it to Viper1!" << endl;
	}

	//DutController:SPC,MPC04
	std::string strDutController(iSyn_SysConfigInfo._strDutController);
	DutController iDutControllerType;
	if (std::string("SPC") == strDutController)
	{
		iDutControllerType = Syn_SPC;
	}
	else if (std::string("MPC04") == strDutController)
	{
		iDutControllerType = Syn_MPC04;
	}
	else
	{
		iDutControllerType = Syn_SPC;
		cout << "Error:Syn_Site::ConstructSiteInstance() - an unknown DutController,construct it to SPC!" << endl;
	}

	//Create Dut
	Syn_Dut *pSyn_Dut = NULL;
	rc = Syn_Dut::CreateDutInstance(iProjectType, pSyn_Dut);
	if (!rc || NULL == pSyn_Dut)
	{
		cout << "Error:Syn_Site::ConstructSiteInstance() - CreateDutInstance is failed!" << endl;
		return false;
	}

	//Create DutCtrl
	Syn_DutCtrl *pSyn_DutCtrl = NULL;
	rc = Syn_DutCtrl::CreateDutCtrlInstance(iDutControllerType, iSerialNumber, pSyn_DutCtrl);
	if (!rc || NULL == pSyn_DutCtrl)
	{
		delete pSyn_Dut;
		pSyn_Dut = NULL;
		cout << "Error:Syn_Site::ConstructSiteInstance() - CreateDutInstance is failed!" << endl;
		return false;
	}

	pSyn_Dut->SetDutCtrl(pSyn_DutCtrl);

	opSyn_SiteInstance = new Syn_Site();
	
	opSyn_SiteInstance->_pSyn_Dut = pSyn_Dut;
	//opSyn_SiteInstance->SetSysConfig(iSyn_SysConfigInfo);
	opSyn_SiteInstance->_SysConfig = iSyn_SysConfigInfo;
	opSyn_SiteInstance->_uiSerialNumber = iSerialNumber;

	pSyn_Dut->SetPatchInfo(opSyn_SiteInstance->_SysConfig._listPatchInfo);


	return true;
}

//bool Syn_Site::ConstructSiteList(Syn_SysConfig &iSyn_SysConfigInfo, std::vector<Syn_Site*> &olistOfSyn_SiteInstance)
bool Syn_Site::ConstructSiteList(std::string strConfigFilePath, std::vector<Syn_Site*> &olistOfSyn_SiteInstance)
{
	bool rc(false);

	olistOfSyn_SiteInstance.clear();

	//xml config file parse
	Syn_SysConfigOperation *pSyn_SysConfigOperationInstance = NULL;
	rc = Syn_SysConfigOperation::GetSysConfigInstance(strConfigFilePath, pSyn_SysConfigOperationInstance);
	if (!rc||NULL == pSyn_SysConfigOperationInstance)
	{
		cout << "Error:Syn_Site::ConstructSiteList() - pSyn_SysConfigOperationInstance is NULL!" << endl;
		return false;
	}
	Syn_SysConfig TempSyn_SysConfig;
	rc = pSyn_SysConfigOperationInstance->GetSysConfig(TempSyn_SysConfig);
	if (!rc)
	{
		cout << "Error:Syn_Site::ConstructSiteList() - ::GetSysConfig is failed!" << endl;
		return false;
	}

	uint32_t uResult(1);

	//Init Operation
	uResult = MPC_Initialize();
	if (0 != uResult)
	{
		cout << "Error:Syn_Site::ConstructSiteList() - ::MPC_Initialize is failed!" << endl;
		return false;
	}

	int iDeviceCounts(0);
	MPC_GetNumberOfDevices(&iDeviceCounts);
	if (0 == iDeviceCounts)
	{
		cout << "Error:Syn_Site::ConstructSiteList() - iDeviceCounts is 0!" << endl;
		return false;
	}

	uint32_t *pDeviceSerNumArray = new uint32_t[iDeviceCounts];
	MPC_GetDeviceSerialNumList(pDeviceSerNumArray);
	for (int i = 0; i<iDeviceCounts; i++)
	{
		uint32_t uiSerialNumber = pDeviceSerNumArray[i];
		if (NULL != uiSerialNumber)
		{
			Syn_Site *pSyn_SiteInstance = NULL;
			bool rc = Syn_Site::ConstructSiteInstance(uiSerialNumber, TempSyn_SysConfig, pSyn_SiteInstance);
			if (!rc&&NULL==pSyn_SiteInstance)
			{
				cout << "Error:Syn_Site::ConstructSiteList() - ConstructSiteInstance is failed,i is " << i << "!" << endl;
				continue;
			}
			
			pSyn_SiteInstance->_DutTestInfo._siteInfo._uiSerialNumber = uiSerialNumber;
			pSyn_SiteInstance->_iSiteNumber = i + 1;
			pSyn_SiteInstance->_DutTestInfo._siteInfo._uiSiteNumber = pSyn_SiteInstance->_iSiteNumber;

			pSyn_SiteInstance->InitDutTestInfo();

			olistOfSyn_SiteInstance.push_back(pSyn_SiteInstance);
		}
	}

	delete[] pDeviceSerNumArray;
	pDeviceSerNumArray = NULL;

	if (pSyn_SysConfigOperationInstance)
	{
		delete pSyn_SysConfigOperationInstance;
		pSyn_SysConfigOperationInstance = NULL;
	}

	return true;
}


void Syn_Site::GetVersion()
{

}


void Syn_Site::ReadOTP()
{
	if (NULL == _pSyn_Dut)
	{
		cout << "Error:Syn_Site::Run() - _pSyn_Dut is NULL!" << endl;
		_DutTestInfo._siteInfo._TestState = TestError;
		return;
	}
	uint8_t arMS0[BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE] = { 0 };
	int iSize(BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE);
	try
	{
		_DutTestInfo._siteInfo._TestState = TestRunning;
		_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
		_pSyn_Dut->ReadOTP(arMS0, iSize);
		_pSyn_Dut->PowerOff();

	}
	catch (Syn_Exception ex)
	{
		_pSyn_Dut->PowerOff();
		std::cout << "Error:ReadOTP is failed!" << std::endl;
		_DutTestInfo._siteInfo._strErrorMessage = ex.GetDescription();
		_DutTestInfo._siteInfo._TestState = TestFailed;

		return;
	}

	//Fill
	for (int i = 0; i < BS0_SIZE; i++)
	{
		(_DutTestInfo._otpInfo._BootSector0Array)[i] = arMS0[i];
	}

	for (int i = 0; i < BS1_SIZE; i++)
	{
		(_DutTestInfo._otpInfo._BootSector1Array)[i] = arMS0[i + BS0_SIZE];
	}

	for (int i = 0; i < MS0_SIZE; i++)
	{
		(_DutTestInfo._otpInfo._MainSector0Array)[i] = arMS0[i + BS0_SIZE + BS1_SIZE];
	}
	
	for (int i = 0; i < MS1_SIZE; i++)
	{
		(_DutTestInfo._otpInfo._MainSector1Array)[i] = arMS0[i + BS0_SIZE + BS1_SIZE + MS0_SIZE];
	}

	_DutTestInfo._siteInfo._TestState = TestOK;
}


void Syn_Site::Run()
{
	::Sleep(5000);
	return;
}


void Syn_Site::GetTestInfo(Syn_DutTestInfo &oSyn_DutTestInfo)
{
	oSyn_DutTestInfo = _DutTestInfo;
}


//TEST
void Syn_Site::InitDutTestInfo()
{
	bool rc(true);

	std::string strArgsValue("");
	std::vector<std::string> listOfArgValue;

	//InitializationStep
	_DutTestInfo._initInfo.m_bExecuted = false;
	_DutTestInfo._initInfo.m_bBga = 0;
	_DutTestInfo._initInfo.m_nFlexId = 0x08F3;
	_DutTestInfo._initInfo.m_nTrimTopWithoutStim = 1;
	_DutTestInfo._initInfo.m_nTrimBotWithoutStim = 1;
	_DutTestInfo._initInfo.m_nTrimLeftWithoutStim = 1;
	_DutTestInfo._initInfo.m_nTrimRightWithoutStim = 1;
	_DutTestInfo._initInfo.m_nBuildNum = 154;
	_DutTestInfo._initInfo.m_nTrimTopWithStim = 0;
	_DutTestInfo._initInfo.m_nTrimBotWithStim = 0;
	_DutTestInfo._initInfo.m_nTrimLeftWithStim = 0;
	_DutTestInfo._initInfo.m_nTrimRightWithStim = 0;
	_DutTestInfo._initInfo.m_nMaxFileSize_MB = 100;
	_DutTestInfo._initInfo.m_bVerboseMode = 0;
	_DutTestInfo._initInfo.m_nProductId = 0;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("InitializationStep"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 14)
		{
			for (size_t t = 1; t <= 14 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}
		
		if (0 != listOfArgValue[0].length())
			_DutTestInfo._initInfo.m_bBga = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._initInfo.m_nFlexId = strtoul(listOfArgValue[1].c_str(), NULL, 0);
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._initInfo.m_nTrimTopWithStim = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._initInfo.m_nTrimBotWithStim = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._initInfo.m_nTrimLeftWithStim = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_DutTestInfo._initInfo.m_nTrimRightWithStim = atoi(listOfArgValue[5].c_str());
		if (0 != listOfArgValue[6].length())
			_DutTestInfo._initInfo.m_nBuildNum = atoi(listOfArgValue[6].c_str());
		if (0 != listOfArgValue[7].length())
			_DutTestInfo._initInfo.m_nTrimTopWithoutStim = atoi(listOfArgValue[7].c_str());
		if (0 != listOfArgValue[8].length())
			_DutTestInfo._initInfo.m_nTrimBotWithoutStim = atoi(listOfArgValue[8].c_str());
		if (0 != listOfArgValue[9].length())
			_DutTestInfo._initInfo.m_nTrimLeftWithoutStim = atoi(listOfArgValue[9].c_str());
		if (0 != listOfArgValue[10].length())
			_DutTestInfo._initInfo.m_nTrimRightWithoutStim = atoi(listOfArgValue[10].c_str());
		if (0 != listOfArgValue[11].length())
			_DutTestInfo._initInfo.m_nMaxFileSize_MB = atoi(listOfArgValue[11].c_str());
		if (0 != listOfArgValue[12].length())
			_DutTestInfo._initInfo.m_bVerboseMode = atoi(listOfArgValue[12].c_str());
		if (0 != listOfArgValue[13].length())
			_DutTestInfo._initInfo.m_nProductId = strtoul(listOfArgValue[13].c_str(), NULL, 0);
	}

	//Calibrate
	_DutTestInfo._calibrationInfo.m_bExecuted = false;
	_DutTestInfo._calibrationInfo.m_nLnaIdx = 1028;
	_DutTestInfo._calibrationInfo.m_nPgaIdx = 1140;
	_DutTestInfo._calibrationInfo.m_bForceCal = 0;
	_DutTestInfo._calibrationInfo.m_nNumImagesToDiscard = 20;
	_DutTestInfo._calibrationInfo.m_nLnaOffsetLow = 108;
	_DutTestInfo._calibrationInfo.m_nLnaOffsetHigh = 148;
	_DutTestInfo._calibrationInfo.m_nPgaLimitLow = 108;
	_DutTestInfo._calibrationInfo.m_nPgaLimitHigh = 148;
	_DutTestInfo._calibrationInfo.m_nCalType = 0;
	_DutTestInfo._calibrationInfo.m_nPgaOffsetRatio = (float)0.3;
	_DutTestInfo._calibrationInfo.m_nNumPgaSamples = 4;
	_DutTestInfo._calibrationInfo.m_nPgaVarianceLimit = 90;
	_DutTestInfo._calibrationInfo.m_nHpfOffset = 0;
	_DutTestInfo._calibrationInfo.m_bPgaFineTuning = 0;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("Calibrate"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 14)
		{
			for (size_t t = 1; t <= 14 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._calibrationInfo.m_nLnaIdx = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._calibrationInfo.m_nPgaIdx = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._calibrationInfo.m_bForceCal = atoi(listOfArgValue[2].c_str()) ? 1 : 0;
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._calibrationInfo.m_nNumImagesToDiscard = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._calibrationInfo.m_nLnaOffsetLow = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_DutTestInfo._calibrationInfo.m_nLnaOffsetHigh = atoi(listOfArgValue[5].c_str());
		if (0 != listOfArgValue[6].length())
			_DutTestInfo._calibrationInfo.m_nPgaLimitLow = atoi(listOfArgValue[6].c_str());
		if (0 != listOfArgValue[7].length())
			_DutTestInfo._calibrationInfo.m_nPgaLimitHigh = atoi(listOfArgValue[7].c_str());
		if (0 != listOfArgValue[8].length())
			_DutTestInfo._calibrationInfo.m_nCalType = atoi(listOfArgValue[8].c_str());
		if (0 != listOfArgValue[9].length())
			_DutTestInfo._calibrationInfo.m_nPgaOffsetRatio = stof(listOfArgValue[9]);//(float)_tstof((LPCTSTR)listOfArgValue[9].c_str());
		if (0 != listOfArgValue[10].length())
			_DutTestInfo._calibrationInfo.m_nNumPgaSamples = atoi(listOfArgValue[10].c_str());
		if (0 != listOfArgValue[11].length())
			_DutTestInfo._calibrationInfo.m_nPgaVarianceLimit = atoi(listOfArgValue[11].c_str());
		if (0 != listOfArgValue[12].length())
			_DutTestInfo._calibrationInfo.m_nHpfOffset = atoi(listOfArgValue[12].c_str());
		if (0 != listOfArgValue[13].length())
			_DutTestInfo._calibrationInfo.m_bPgaFineTuning = atoi(listOfArgValue[13].c_str());
	}

	//AcquireFPSSamples
	_DutTestInfo._acquireFpsInfo.m_bExecuted = false;
	_DutTestInfo._acquireFpsInfo.m_nNumImagesToDiscard = 20;
	_DutTestInfo._acquireFpsInfo.m_nNumImagesWithoutStimulus = 30;
	_DutTestInfo._acquireFpsInfo.m_nNumImagesWithStimulus = 30;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("AcquireFPSSamples"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 3)
		{
			for (size_t t = 1; t <= 3 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._acquireFpsInfo.m_nNumImagesToDiscard = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._acquireFpsInfo.m_nNumImagesWithoutStimulus = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._acquireFpsInfo.m_nNumImagesWithStimulus = atoi(listOfArgValue[2].c_str());

		if (_DutTestInfo._acquireFpsInfo.m_nNumImagesWithoutStimulus > MAXFRAMES)
		{
			Syn_Exception ex(0);
			//str.Format("The maximum of %d images without the stimulus has been exceeded.", MAXFRAMES);
			ex.SetDescription("Timeout waiting for stimulus.");
			throw(ex);
		}
		if (_DutTestInfo._acquireFpsInfo.m_nNumImagesWithStimulus > MAXFRAMES)
		{
			Syn_Exception ex(0);
			//CString str;
			//str.Format("The maximum of %d images with the stimulus has been exceeded.", MAXFRAMES);
			ex.SetDescription("Timeout waiting for stimulus.");
			throw(ex);
		}
	}

	//PeggedPixelsTest
	_DutTestInfo._peggedPixelsInfo.m_bExecuted = false;
	_DutTestInfo._peggedPixelsInfo.numFrames = 30;
	_DutTestInfo._peggedPixelsInfo.fail_row = 9;
	_DutTestInfo._peggedPixelsInfo.fail_col = 28;
	_DutTestInfo._peggedPixelsInfo.fail_limits = 255;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("PeggedPixelsTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		_DutTestInfo._peggedPixelsInfo.numFrames = _DutTestInfo._acquireFpsInfo.m_nNumImagesWithoutStimulus;//atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._peggedPixelsInfo.fail_row = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._peggedPixelsInfo.fail_col = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._peggedPixelsInfo.fail_limits = atoi(listOfArgValue[3].c_str());
	}

	//CurrentTest
	_DutTestInfo._currentInfo.m_bExecuted = false;
	_DutTestInfo._currentInfo.m_nLowGain = 2;
	_DutTestInfo._currentInfo.m_nHighGain = 3;
	_DutTestInfo._currentInfo.m_nImageAcqAnaMax_uA = (int)(50 * 1000);
	_DutTestInfo._currentInfo.m_nImageAcqDigMax_uA = (int)(5 * 1000);
	_DutTestInfo._currentInfo.m_nImageAcqAnaMin_uA = (int)(1 * 1000);
	_DutTestInfo._currentInfo.m_nImageAcqDigMin_uA = (int)(0.01 * 1000);
	if (_SysConfig.GetSyn_TestStepInfo(std::string("CurrentTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 6)
		{
			for (size_t t = 1; t <= 6 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._currentInfo.m_nLowGain = atoi(listOfArgValue[0].c_str()) % 4;			//Gain range is 0-3.
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._currentInfo.m_nHighGain = atoi(listOfArgValue[1].c_str()) % 4;		//Gain range is 0-3.
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._currentInfo.m_nImageAcqAnaMax_uA = stoi(listOfArgValue[2]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[2].c_str()) * 1000);
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._currentInfo.m_nImageAcqDigMax_uA = stoi(listOfArgValue[3]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[3].c_str()) * 1000);
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._currentInfo.m_nImageAcqAnaMin_uA = stoi(listOfArgValue[4]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[4].c_str()) * 1000);
		if (0 != listOfArgValue[5].length())
			_DutTestInfo._currentInfo.m_nImageAcqDigMin_uA = stoi(listOfArgValue[5]) * 1000;//(int)(_tstof((LPCTSTR)listOfArgValue[5].c_str()) * 1000);
	}


	//OpensShortsTest
	_DutTestInfo._opensShortsInfo.m_bExecuted = false;
	_DutTestInfo._opensShortsInfo.m_nNumResBytes = 2056;
	_DutTestInfo._opensShortsInfo.m_nDelay_ms = 500;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("OpensShortsTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._opensShortsInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._opensShortsInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());
	}

	//FlooredPixelsTest
	_DutTestInfo._flooredPixelsInfo.m_bExecuted = false;
	_DutTestInfo._flooredPixelsInfo.numFrames = 30;
	_DutTestInfo._flooredPixelsInfo.fail_row = 9;
	_DutTestInfo._flooredPixelsInfo.fail_col = 28;
	_DutTestInfo._flooredPixelsInfo.fail_limits = 25;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("FlooredPixelsTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		_DutTestInfo._flooredPixelsInfo.numFrames = _DutTestInfo._acquireFpsInfo.m_nNumImagesWithoutStimulus;
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._flooredPixelsInfo.fail_row = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._flooredPixelsInfo.fail_col = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._flooredPixelsInfo.fail_limits = atoi(listOfArgValue[3].c_str());
	}

	//DeltaPixelTest
	_DutTestInfo._deltaPixelsInfo.m_bExecuted = false;
	_DutTestInfo._deltaPixelsInfo.numFrames = 30;
	_DutTestInfo._deltaPixelsInfo.fail_row = 0;
	_DutTestInfo._deltaPixelsInfo.fail_col = 0;
	_DutTestInfo._deltaPixelsInfo.fail_limits = 10;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("DeltaPixelTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._deltaPixelsInfo.numFrames = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._deltaPixelsInfo.fail_row = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._deltaPixelsInfo.fail_col = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._deltaPixelsInfo.fail_limits = atoi(listOfArgValue[3].c_str());
	}

	//ConsecutivePixels
	_DutTestInfo._consecutivePixelsInfo.m_bExecuted = false;
	_DutTestInfo._consecutivePixelsInfo.m_peggedThreshold = 225;
	_DutTestInfo._consecutivePixelsInfo.m_flooredThreshold = 25;
	_DutTestInfo._consecutivePixelsInfo.m_maxAdjacentPixelsAllowed = 4;
	_DutTestInfo._consecutivePixelsInfo.m_cutEdge = 7;
	_DutTestInfo._consecutivePixelsInfo.numFrames = 30;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("ConsecutivePixels"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();//ilistSize is 5??????????????
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._consecutivePixelsInfo.m_peggedThreshold = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._consecutivePixelsInfo.m_flooredThreshold = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._consecutivePixelsInfo.m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._consecutivePixelsInfo.m_cutEdge = atoi(listOfArgValue[3].c_str());

		_DutTestInfo._consecutivePixelsInfo.numFrames = _DutTestInfo._acquireFpsInfo.m_nNumImagesWithoutStimulus;
	}

	//PixelTest
	_DutTestInfo._pixelInfo.m_bExecuted = false;
	_DutTestInfo._pixelInfo.numFrames = 30;
	_DutTestInfo._pixelInfo.fail_row = 50;
	_DutTestInfo._pixelInfo.fail_col = 50;
	_DutTestInfo._pixelInfo.nTrim = 0;
	_DutTestInfo._pixelInfo.nCountFailedPixels = 0;
	_DutTestInfo._pixelInfo.nThresholdRange = 45;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("PixelTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 6)
		{
			for (size_t t = 1; t <= 6 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		_DutTestInfo._pixelInfo.numFrames = _DutTestInfo._acquireFpsInfo.m_nNumImagesWithoutStimulus;
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._pixelInfo.fail_row = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._pixelInfo.fail_col = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._pixelInfo.nTrim = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._pixelInfo.nThresholdRange = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_DutTestInfo._pixelInfo.nCountFailedPixels = atoi(listOfArgValue[5].c_str());
	}

	//PixelPatchTest
	_DutTestInfo._pixelPatchInfo.m_bExecuted = false;
	_DutTestInfo._pixelPatchInfo.m_nNumResBytes = 3000;
	_DutTestInfo._pixelPatchInfo.m_nDelay_ms = 500;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("PixelPatchTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._pixelPatchInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._pixelPatchInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());
	}
	
	//SNRTest
	_DutTestInfo._snrInfo.m_bExecuted = false;

	int numMaxColsWithStim(0), numMinColsWithStim(0), numMaxRowsWithStim(0), numMinRowsWithStim(0);
	int numMaxColsWOStim(0), numMinColsWOStim(0), numMaxRowsWOStim(0), numMinRowsWOStim(0);

	numMaxColsWithStim = _SysConfig._uiNumCols - _DutTestInfo._initInfo.m_nTrimRightWithStim;
	numMinColsWithStim = _DutTestInfo._initInfo.m_nTrimLeftWithStim;
	numMaxRowsWithStim = _SysConfig._uiNumRows - _DutTestInfo._initInfo.m_nTrimBotWithStim;;
	numMinRowsWithStim = _DutTestInfo._initInfo.m_nTrimTopWithStim;
	numMaxColsWOStim = _SysConfig._uiNumCols - _DutTestInfo._initInfo.m_nTrimRightWithoutStim;
	numMinColsWOStim = _DutTestInfo._initInfo.m_nTrimLeftWithoutStim;
	numMaxRowsWOStim = _SysConfig._uiNumRows - _DutTestInfo._initInfo.m_nTrimBotWithoutStim;
	numMinRowsWOStim = _DutTestInfo._initInfo.m_nTrimTopWithoutStim;

	_DutTestInfo._snrInfo.numcols = _SysConfig._uiNumCols;
	_DutTestInfo._snrInfo.numrows = _SysConfig._uiNumRows;
	//Set defaults.
	_DutTestInfo._snrInfo.numFrames = 30;
	_DutTestInfo._snrInfo.numMinCols = 2;
	_DutTestInfo._snrInfo.numMaxCols = 147;
	_DutTestInfo._snrInfo.numMinRows = 1;
	_DutTestInfo._snrInfo.numMaxRows = 55;
	_DutTestInfo._snrInfo.percent = 10;
	_DutTestInfo._snrInfo.limit_overall_snr_high = 500;
	_DutTestInfo._snrInfo.limit_overall_snr_low = 25;
	_DutTestInfo._snrInfo.limit_overall_signal_high = 500;
	_DutTestInfo._snrInfo.limit_overall_signal_low = 80;
	_DutTestInfo._snrInfo.limit_overall_noise_high = 5;
	_DutTestInfo._snrInfo.limit_overall_noise_low = 0;
	_DutTestInfo._snrInfo.limit_zone_snr_high = 500;
	_DutTestInfo._snrInfo.limit_zone_snr_low = 14;
	_DutTestInfo._snrInfo.limit_zone_signal_high = 500;
	_DutTestInfo._snrInfo.limit_zone_signal_low = 50;
	_DutTestInfo._snrInfo.limit_zone_noise_high = 6;
	_DutTestInfo._snrInfo.limit_zone_noise_low = 0;

	_DutTestInfo._snrInfo.numMaxCols = (numMaxColsWOStim < numMaxColsWithStim) ? numMaxColsWOStim : numMaxColsWithStim;
	_DutTestInfo._snrInfo.numMinCols = (numMinColsWOStim < numMinColsWithStim) ? numMinColsWithStim : numMinColsWOStim;
	_DutTestInfo._snrInfo.numMaxRows = (numMaxRowsWOStim < numMaxRowsWithStim) ? numMaxRowsWOStim : numMaxRowsWithStim;
	_DutTestInfo._snrInfo.numMinRows = (numMinRowsWOStim < numMinRowsWithStim) ? numMinRowsWithStim : numMinRowsWOStim;

	_DutTestInfo._snrInfo.numFrames = _DutTestInfo._acquireFpsInfo.m_nNumImagesWithStimulus;//Image Count
	if (_SysConfig.GetSyn_TestStepInfo(std::string("SNRTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 18)
		{
			for (size_t t = 1; t <= 18 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[5].length())
			_DutTestInfo._snrInfo.percent = atoi(listOfArgValue[5].c_str());
		////////////////////////////////////////////////
		//SNR LIMITS
		////////////////////////////////////////////////
		if (0 != listOfArgValue[6].length())
			_DutTestInfo._snrInfo.limit_overall_snr_high = atoi(listOfArgValue[6].c_str());
		if (0 != listOfArgValue[7].length())
			_DutTestInfo._snrInfo.limit_overall_snr_low = atoi(listOfArgValue[7].c_str());
		if (0 != listOfArgValue[8].length())
			_DutTestInfo._snrInfo.limit_overall_signal_high = atoi(listOfArgValue[8].c_str());
		if (0 != listOfArgValue[9].length())
			_DutTestInfo._snrInfo.limit_overall_signal_low = atoi(listOfArgValue[9].c_str());
		if (0 != listOfArgValue[10].length())
			_DutTestInfo._snrInfo.limit_overall_noise_high = atoi(listOfArgValue[10].c_str());
		if (0 != listOfArgValue[11].length())
			_DutTestInfo._snrInfo.limit_overall_noise_low = atoi(listOfArgValue[11].c_str());
		if (0 != listOfArgValue[12].length())
			_DutTestInfo._snrInfo.limit_zone_snr_high = atoi(listOfArgValue[12].c_str());
		if (0 != listOfArgValue[13].length())
			_DutTestInfo._snrInfo.limit_zone_snr_low = atoi(listOfArgValue[13].c_str());
		if (0 != listOfArgValue[14].length())
			_DutTestInfo._snrInfo.limit_zone_signal_high = atoi(listOfArgValue[14].c_str());
		if (0 != listOfArgValue[15].length())
			_DutTestInfo._snrInfo.limit_zone_signal_low = atoi(listOfArgValue[15].c_str());
		if (0 != listOfArgValue[16].length())
			_DutTestInfo._snrInfo.limit_zone_noise_high = atoi(listOfArgValue[16].c_str());
		if (0 != listOfArgValue[17].length())
			_DutTestInfo._snrInfo.limit_zone_noise_low = atoi(listOfArgValue[17].c_str());
	}

	//WakeOnFinger
	_DutTestInfo._wofInfo.m_bExecutedWithoutStimulus = false;
	_DutTestInfo._wofInfo.m_bExecutedWithStimulus = false;
	_DutTestInfo._wofInfo.m_bWithStimulus = 0;
	_DutTestInfo._wofInfo.m_nDelta_100 = 7;
	_DutTestInfo._wofInfo.m_nMinTriggerThreshold = 25;
	_DutTestInfo._wofInfo.m_nMaxTriggerThreshold = 60;
	_DutTestInfo._wofInfo.m_nNumResponseBytes = 2000;
	_DutTestInfo._wofInfo.m_nNumNoiseSamples = 10;
	_DutTestInfo._wofInfo.m_nThresholdOffsetHigh = 3;
	_DutTestInfo._wofInfo.m_nThresholdOffsetLow = 3;
	_DutTestInfo._wofInfo.m_nStdDevThreshold = (float)0.9;
	_DutTestInfo._wofInfo.m_nDelta_200 = 20;
	_DutTestInfo._wofInfo.m_nDelta_200_3p7 = 13;
	_DutTestInfo._wofInfo.m_nVCC = (float)3.7;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("WakeOnFinger"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 18)
		{
			for (size_t t = 1; t <= 18 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._wofInfo.m_bWithStimulus = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._wofInfo.m_nDelta_100 = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._wofInfo.m_nMinTriggerThreshold = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._wofInfo.m_nMaxTriggerThreshold = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._wofInfo.m_nNumResponseBytes = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_DutTestInfo._wofInfo.m_nNumNoiseSamples = atoi(listOfArgValue[5].c_str());
		if (0 != listOfArgValue[6].length())
			_DutTestInfo._wofInfo.m_nThresholdOffsetHigh = atoi(listOfArgValue[6].c_str());
		if (0 != listOfArgValue[7].length())
			_DutTestInfo._wofInfo.m_nThresholdOffsetLow = atoi(listOfArgValue[7].c_str());
		if (0 != listOfArgValue[8].length())
			_DutTestInfo._wofInfo.m_nStdDevThreshold = stof(listOfArgValue[8]);
		if (0 != listOfArgValue[9].length())
			_DutTestInfo._wofInfo.m_nDelta_200 = atoi(listOfArgValue[9].c_str());
		if (0 != listOfArgValue[10].length())
			_DutTestInfo._wofInfo.m_nDelta_200_3p7 = atoi(listOfArgValue[10].c_str());
		if (0 != listOfArgValue[11].length())
			_DutTestInfo._wofInfo.m_nVCC = stof(listOfArgValue[11]);
	}


	//SCM_WOFWithoutStimulus

	//SCM_WOFWithStimulus


	//WoVarTest
	_DutTestInfo._woVarInfo.m_bExecuted = false;
	_DutTestInfo._woVarInfo.m_nNumResBytes = 1000;
	_DutTestInfo._woVarInfo.m_nDelay_ms = 500;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("WoVarTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._woVarInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._woVarInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());
	}

	//DRdyTest
	_DutTestInfo._DRdyInfo.m_bExecuted = false;
	_DutTestInfo._DRdyInfo.m_portId = 1;
	_DutTestInfo._DRdyInfo.m_pinMsk = 0x10;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("DRdyTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._DRdyInfo.m_portId = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._DRdyInfo.m_pinMsk = strtoul(listOfArgValue[1].c_str(), NULL, 16);
	}

	//UsbCommTest
	_DutTestInfo._usbCommInfo.m_bExecuted = false;
	_DutTestInfo._usbCommInfo.m_nExpectedVid = 0x0000;
	_DutTestInfo._usbCommInfo.m_nExpectedPid = 0x0000;
	_DutTestInfo._usbCommInfo.m_nExpectedMaximRev = 0x13;
	_DutTestInfo._usbCommInfo.m_nSlaveSelPin = 17;
	_DutTestInfo._usbCommInfo.m_nMisoPin = 19;
	_DutTestInfo._usbCommInfo.m_nMosiPin = 21;
	_DutTestInfo._usbCommInfo.m_nSclkPin = 23;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("UsbCommTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 7)
		{
			for (size_t t = 1; t <= 7 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._usbCommInfo.m_nExpectedVid = strtol(listOfArgValue[0].c_str(), NULL, 0);
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._usbCommInfo.m_nExpectedPid = strtol(listOfArgValue[1].c_str(), NULL, 0);
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._usbCommInfo.m_nExpectedMaximRev = strtol(listOfArgValue[2].c_str(), NULL, 0);
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._usbCommInfo.m_nSlaveSelPin = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._usbCommInfo.m_nMisoPin = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_DutTestInfo._usbCommInfo.m_nMosiPin = atoi(listOfArgValue[5].c_str());
		if (0 != listOfArgValue[6].length())
			_DutTestInfo._usbCommInfo.m_nSclkPin = atoi(listOfArgValue[6].c_str());
	}

	//SpiFlashRam
	_DutTestInfo._spiFlashInfo.m_bExecuted = false;
	_DutTestInfo._spiFlashInfo.m_nNumResBytes = 1024;
	/*if (_SysConfig.GetSyn_TestStepInfo(std::string("SpiFlashRam"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize >= 1)
		{
			_DutTestInfo.m_spiFlashInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		}
	}*/
	
	//ButtonTestWithoutStimulus
	_DutTestInfo._btnTestWithoutStimInfo.m_bExecuted = false;
	_DutTestInfo._btnTestWithoutStimInfo.m_portId = 8;
	_DutTestInfo._btnTestWithoutStimInfo.m_pinMsk = 0x10;
	_DutTestInfo._btnTestWithoutStimInfo.m_expectedState = 1;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("ButtonTestWithoutStimulus"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 3)
		{
			for (size_t t = 1; t <= 3 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._btnTestWithoutStimInfo.m_portId = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._btnTestWithoutStimInfo.m_pinMsk = strtoul(listOfArgValue[1].c_str(), NULL, 16);
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._btnTestWithoutStimInfo.m_expectedState = atoi(listOfArgValue[2].c_str());
	}

	//ButtonTestWithStimulus
	_DutTestInfo._btnTestWithStimInfo.m_bExecuted = false;
	_DutTestInfo._btnTestWithStimInfo.m_portId = 8;
	_DutTestInfo._btnTestWithStimInfo.m_pinMsk = 0x10;
	_DutTestInfo._btnTestWithStimInfo.m_expectedState = 0;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("ButtonTestWithStimulus"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 3)
		{
			for (size_t t = 1; t <= 3 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._btnTestWithStimInfo.m_portId = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._btnTestWithStimInfo.m_pinMsk = strtoul(listOfArgValue[1].c_str(), NULL, 16);
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._btnTestWithStimInfo.m_expectedState = atoi(listOfArgValue[2].c_str());
	}

	//SharpnessTest
	_DutTestInfo._SharpnessInfo.m_bExecuted = false;
	_DutTestInfo._SharpnessInfo.limit = 30;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("SharpnessTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize >= 1)
		{
			_DutTestInfo._SharpnessInfo.limit = atoi(listOfArgValue[0].c_str());
		}
	}

	//ReadDutAdc
	_DutTestInfo._ReadDutAdcInfo.m_bExecuted = false;

	//Imperfections
	_DutTestInfo._imperfectionsTestInfo.m_bExecuted = false;
	_DutTestInfo._imperfectionsTestInfo.m_peggedThreshold = 138;
	_DutTestInfo._imperfectionsTestInfo.m_flooredThreshold = 118;
	_DutTestInfo._imperfectionsTestInfo.m_maxAdjacentPixelsAllowed = 10;
	_DutTestInfo._imperfectionsTestInfo.numFrames = _DutTestInfo._acquireFpsInfo.m_nNumImagesWithStimulus;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("Imperfections"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 3)
		{
			for (size_t t = 1; t <= 3 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._imperfectionsTestInfo.m_peggedThreshold = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._imperfectionsTestInfo.m_flooredThreshold = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._imperfectionsTestInfo.m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
	}

	//RxStandardDev
	_DutTestInfo._RxStandardDevInfo.m_bExecuted = false;
	//_DutTestInfo.m_RxStandardDevInfo.numFrames = 30;
	_DutTestInfo._RxStandardDevInfo.limit = 80;
	_DutTestInfo._RxStandardDevInfo.numFrames = _DutTestInfo._acquireFpsInfo.m_nNumImagesWithStimulus;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("RxStandardDev"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize >=1)
		{
			_DutTestInfo._RxStandardDevInfo.limit = atoi(listOfArgValue[0].c_str());
		}
	}

	//OscTrim
	_DutTestInfo._OscTrimInfo.m_bExecuted = false;
	_DutTestInfo._OscTrimInfo.nLowerLimit_Hz = 122;
	_DutTestInfo._OscTrimInfo.nUpperLimit_Hz = 123;
	_DutTestInfo._OscTrimInfo.nInitialTrim = 0x380;
	_DutTestInfo._OscTrimInfo.m_OscTrimDefault = 0x0;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("OscTrim"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._OscTrimInfo.nLowerLimit_Hz = strtoul(listOfArgValue[0].c_str(), NULL, 0);
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._OscTrimInfo.nUpperLimit_Hz = strtoul(listOfArgValue[1].c_str(), NULL, 0);
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._OscTrimInfo.nInitialTrim = strtoul(listOfArgValue[2].c_str(), NULL, 0);
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._OscTrimInfo.m_OscTrimDefault = strtoul(listOfArgValue[3].c_str(), NULL, 0);
	}

	//SlowOsc
	_DutTestInfo._SlowOscInfo.m_bExecuted = false;
	_DutTestInfo._SlowOscInfo.nLowerLimit_Hz = 475000;
	_DutTestInfo._SlowOscInfo.nUpperLimit_Hz = 501000;
	_DutTestInfo._SlowOscInfo.m_nDefaultTrim = 0x0;
	_DutTestInfo._SlowOscInfo.m_nDefaultBias = 0x0;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("SlowOsc"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._SlowOscInfo.nLowerLimit_Hz = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._SlowOscInfo.nUpperLimit_Hz = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._SlowOscInfo.m_nDefaultTrim = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._SlowOscInfo.m_nDefaultBias = atoi(listOfArgValue[3].c_str());
	}

	//RAMTest
	_DutTestInfo._RAMTestInfo.m_bExecuted = false;
	//Set defaults.

	//RetainModeCurrentTest
	_DutTestInfo._retainModeInfo.m_bExecuted = false;
	_DutTestInfo._retainModeInfo.m_nGain = 2;
	_DutTestInfo._retainModeInfo.m_nMaxCurrent = 1;
	_DutTestInfo._retainModeInfo.m_nMinCurrent = (float)0.1;
	_DutTestInfo._retainModeInfo.m_nDelay = 0;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("RetainModeCurrentTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._retainModeInfo.m_nGain = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._retainModeInfo.m_nMaxCurrent = stof(listOfArgValue[1]);
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._retainModeInfo.m_nMinCurrent = stof(listOfArgValue[2]);
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._retainModeInfo.m_nDelay = atoi(listOfArgValue[3].c_str());
	}

	//WOF-LowPower
	_DutTestInfo._wofLowPowerInfo.m_bExecuted = false;
	_DutTestInfo._wofLowPowerInfo.m_nGain = 2;
	_DutTestInfo._wofLowPowerInfo.m_nMaxCurrent_uA = 50;
	_DutTestInfo._wofLowPowerInfo.m_nMinCurrent_uA = 0;
	_DutTestInfo._wofLowPowerInfo.m_nDelay_ms = 200;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("WOF-LowPower"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 4)
		{
			for (size_t t = 1; t <= 4 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._wofLowPowerInfo.m_nGain = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._wofLowPowerInfo.m_nMaxCurrent_uA = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._wofLowPowerInfo.m_nMinCurrent_uA = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._wofLowPowerInfo.m_nDelay_ms = atoi(listOfArgValue[3].c_str());
	}

	//AdcBaseLineInfo????????????????????????????????????????

	//Security
	_DutTestInfo._securityStepInfo.m_bExecuted = false;
	_DutTestInfo._securityStepInfo.m_bEngineering = 0;
	_DutTestInfo._securityStepInfo.m_bProduction = 0;
	_DutTestInfo._securityStepInfo.m_bUsbMode = 0;
	_DutTestInfo._securityStepInfo.m_bSpiOwnership = 0;
	_DutTestInfo._securityStepInfo.m_nNumSpiOwnershipRuns = 5;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("Security"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 5)
		{
			for (size_t t = 1; t <= 5 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._securityStepInfo.m_bEngineering = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._securityStepInfo.m_bProduction = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._securityStepInfo.m_bUsbMode = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._securityStepInfo.m_bSpiOwnership = atoi(listOfArgValue[3].c_str());
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._securityStepInfo.m_nNumSpiOwnershipRuns = atoi(listOfArgValue[4].c_str());
	}

	//ProductIdTest
	_DutTestInfo._productIdTestInfo.m_bExecuted = false;

	//AFETest
	_DutTestInfo._AFETestInfo.m_bExecuted = false;
	_DutTestInfo._AFETestInfo.m_nNumResBytes = 2056;
	_DutTestInfo._AFETestInfo.m_nDelay_ms = 500;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("AFETest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 2)
		{
			for (size_t t = 1; t <= 2 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._AFETestInfo.m_nNumResBytes = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._AFETestInfo.m_nDelay_ms = atoi(listOfArgValue[1].c_str());
	}
	
	//LEDTest
	_DutTestInfo._LEDTestInfo.m_bExecuted = false;
	_DutTestInfo._LEDTestInfo.m_nGPIO1 = 0x02;
	_DutTestInfo._LEDTestInfo.m_nGPIO2 = 0x04;
	_DutTestInfo._LEDTestInfo.m_nGPIO3 = 0x08;
	memset(_DutTestInfo._LEDTestInfo.m_sUserMsg, NULL, MAX_LED_TEST_MSG_LEN);
	_DutTestInfo._LEDTestInfo.m_nMpcGpioPin = 22;
	_DutTestInfo._LEDTestInfo.m_nRegisterAddress = 0x80001600;
	_DutTestInfo._LEDTestInfo.m_nPokeValue = 0x04;
	_DutTestInfo._LEDTestInfo.m_nTimeout = 5000;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("LEDTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 8)
		{
			for (size_t t = 1; t <= 8 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._LEDTestInfo.m_nGPIO1 = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._LEDTestInfo.m_nGPIO2 = atoi(listOfArgValue[1].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._LEDTestInfo.m_nGPIO3 = atoi(listOfArgValue[2].c_str());
		if (0 != listOfArgValue[3].length())
			memcpy(_DutTestInfo._LEDTestInfo.m_sUserMsg, listOfArgValue[3].c_str(), listOfArgValue[3].length());
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._LEDTestInfo.m_nMpcGpioPin = atoi(listOfArgValue[4].c_str());
		if (0 != listOfArgValue[5].length())
			_DutTestInfo._LEDTestInfo.m_nRegisterAddress = strtoul(listOfArgValue[5].c_str(), NULL, 0);
		if (0 != listOfArgValue[6].length())
			_DutTestInfo._LEDTestInfo.m_nPokeValue = strtoul(listOfArgValue[6].c_str(), NULL, 0);
		if (0 != listOfArgValue[7].length())
			_DutTestInfo._LEDTestInfo.m_nTimeout = atoi(listOfArgValue[7].c_str());
	}

	//SDKBaselineTest
	_DutTestInfo._SdkBaselineInfo.m_bExecuted = false;
	_DutTestInfo._SdkBaselineInfo.m_nTrimTop = 0;
	_DutTestInfo._SdkBaselineInfo.m_nTrimBottom = 0;
	_DutTestInfo._SdkBaselineInfo.m_nTrimLeft = 0;
	_DutTestInfo._SdkBaselineInfo.m_nTrimRight = 0;
	_DutTestInfo._SdkBaselineInfo.m_nRightShiftBits = 3;
	_DutTestInfo._SdkBaselineInfo.m_nStrideWidth = 3;
	_DutTestInfo._SdkBaselineInfo.m_nMaxSumAllowed = 250;
	_DutTestInfo._SdkBaselineInfo.m_nPercentFail = 25;
	if (_SysConfig.GetSyn_TestStepInfo(std::string("SDKBaselineTest"), strArgsValue))
	{
		ParseTestStepArgs(strArgsValue, listOfArgValue);
		size_t ilistSize = listOfArgValue.size();
		if (ilistSize < 8)
		{
			for (size_t t = 1; t <= 8 - ilistSize; t++)
				listOfArgValue.push_back(std::string(""));
		}

		if (0 != listOfArgValue[0].length())
			_DutTestInfo._SdkBaselineInfo.m_nTrimTop = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[1].length())
			_DutTestInfo._SdkBaselineInfo.m_nTrimBottom = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[2].length())
			_DutTestInfo._SdkBaselineInfo.m_nTrimLeft = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[3].length())
			_DutTestInfo._SdkBaselineInfo.m_nTrimRight = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[4].length())
			_DutTestInfo._SdkBaselineInfo.m_nRightShiftBits = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[5].length())
			_DutTestInfo._SdkBaselineInfo.m_nStrideWidth = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[6].length())
			_DutTestInfo._SdkBaselineInfo.m_nMaxSumAllowed = atoi(listOfArgValue[0].c_str());
		if (0 != listOfArgValue[7].length())
			_DutTestInfo._SdkBaselineInfo.m_nPercentFail = atoi(listOfArgValue[0].c_str());
	}
}


bool Syn_Site::ParseTestStepArgs(const std::string &strArgsValue, std::vector<std::string> &olistOfArgValue, std::string strSymbol)
{
	olistOfArgValue.clear();

	unsigned int iLength = strArgsValue.length();
	if (0 == iLength)
	{
		return false;
	}

	std::string strTempValue = strArgsValue;
	while (true)
	{
		size_t SymbolPosition = strTempValue.find(strSymbol);
		if (SymbolPosition == std::string::npos)
		{
			if (0 != strTempValue.length())
			{
				olistOfArgValue.push_back(strTempValue);
			}
			break;
		}

		std::string strArgValue = strTempValue.substr(0, SymbolPosition);
		olistOfArgValue.push_back(strArgValue);
		strTempValue = strTempValue.substr(SymbolPosition + 1, strTempValue.length() - SymbolPosition);
		//strTempValue = strTempValue.substr(SymbolPosition + strSymbol.length(), strTempValue.length() - SymbolPosition-strSymbol.length());
	}

	return true;
}