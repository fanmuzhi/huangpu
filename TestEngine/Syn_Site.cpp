//local
#include "Syn_Site.h"
#include "Syn_Exception.h"
#include "Syn_SysConfigOperation.h"

//std
#include <thread>

//MPC_API
#include "MpcApiDll.h"
//#include "MpcApiError.h"
//#include "MPCErrors.h"

//std
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <thread>

#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

Syn_Site::Syn_Site(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath)
:_pSyn_Dut(NULL)
, _pSyn_DutCtrl(NULL)
, _iSiteNumber(siteNumber)
,_uiSerialNumber(deviceSerNumber)
, _strConfigFilePath(strConfigFilePath)
,_sitState(Closed)
,_stopFlag(false)
,_uiErrorFlag(Syn_Info::Syn_OK)
,_strErrorMessage("")
{
	//this->Init();
}

Syn_Site::~Syn_Site()
{
	if (NULL != _pSyn_Dut)
	{
		delete _pSyn_Dut;
		_pSyn_Dut = NULL;
	}

	if (NULL != _pSyn_DutCtrl)
	{
		delete _pSyn_DutCtrl;
		_pSyn_DutCtrl = NULL;
	}
}

uint32_t Syn_Site::Init()
{
	bool rc(false);

	//xml config file parse
	Syn_SysConfigOperation *pConfigOperationInstance = NULL;
	rc = Syn_SysConfigOperation::GetSysConfigInstance(_strConfigFilePath, pConfigOperationInstance);
	if (!rc || NULL == pConfigOperationInstance)
	{
		LOG(ERROR) << "Error:Syn_Site::Init() - pConfigOperationInstance is NULL!" << endl;
		return Syn_Info::Syn_ConfigError;
	}
	
	rc = pConfigOperationInstance->GetSysConfig(_SysConfig);
	if (!rc)
	{
		LOG(ERROR) << "Error:Syn_Site::Init() - ::GetSysConfig is failed!" << endl;
		return Syn_Info::Syn_ConfigError;
	}

	//ProejctType:Viper1,Viper2,Metallica
	std::string strProjectType(_SysConfig._strDutType);
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
		LOG(ERROR) << "Error:Syn_Site::Init() - an unknown ProjectType,construct it to Viper1!";
	}

	//DutController:SPC,MPC04
	std::string strDutController(_SysConfig._strDutController);
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
		LOG(ERROR) << "Error:Syn_Site::Init() - an unknown DutController,construct it to SPC!";
	}

	//Create Dut
	rc = Syn_Dut::CreateDutInstance(iProjectType, _pSyn_Dut);
	if (!rc || NULL == _pSyn_Dut)
	{
		LOG(ERROR) << "Error:Syn_Site::Init() - CreateDutInstance is failed!";
		return false;
	}

	//Create DutCtrl
	rc = Syn_DutCtrl::CreateDutCtrlInstance(iDutControllerType, _uiSerialNumber, _pSyn_DutCtrl);
	if (!rc || NULL == _pSyn_DutCtrl)
	{
		LOG(ERROR) << "Error:Syn_Site::Init() - CreateDutInstance is failed!";
		return false;
	}

	_pSyn_Dut->SetPatchInfo(_SysConfig._listPatchInfo);

	//fill info
	//_pSyn_Dut->InitData(_SysConfig);

	_siteInfo._uiSerialNumber = _uiSerialNumber;

	if (pConfigOperationInstance)
	{
		delete pConfigOperationInstance;
		pConfigOperationInstance = NULL;
	}

	_sitState = Idle;

	return 1;
}

uint32_t Syn_Site::ExecuteScript(uint8_t scriptID)
{
	bool rc(false);

	if (Idle != _sitState)
	{
		return false;
	}

	_sitState = Running;

	//std::thread siteThread(RunScript, this, scriptID);

	std::thread siteThread([&]()
	{
		this->RunScript(scriptID);
	});

	siteThread.detach();
	//siteThread.join();

	return Syn_Info::Syn_OK;
}

void Syn_Site::RunScript(uint8_t scriptID)
{

	_pSyn_Dut->InitData(_SysConfig);

	Syn_TestScript ExceteScriptInfo;
	bool rc = GetTestScriptInfo(scriptID, ExceteScriptInfo);
	if (!rc)
	{
		_uiErrorFlag = Syn_Info::Syn_ScriptConfigError;
		return;
	}

	unsigned int listSize = ExceteScriptInfo._listOfTestStep.size();
	if (0 == listSize)
	{
		_uiErrorFlag = Syn_Info::Syn_ScriptConfigError;
		return;
	}

	bool errorFlag(false);
	for (unsigned int i = 1; i <= listSize; i++)
	{
		if (errorFlag || _stopFlag)
		{
			break;
		}

		Syn_TestStepInfo CurrentTestStepInfo = ExceteScriptInfo._listOfTestStep[i - 1];
		Syn_TestStep *pTestStep = NULL;
		rc = Syn_TestStepFactory::CreateTestStepInstance(CurrentTestStepInfo._strTestStepName, _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
		if (rc && NULL != pTestStep)
		{
			try
			{
				if (1 == i)
				{
					pTestStep->SetUp();
				}

				pTestStep->Excute();

				if (listSize == i)
				{
					pTestStep->CleanUp();
				}

				delete pTestStep;
				pTestStep = NULL;
			}
			catch (Syn_Exception ex)
			{
				//pTestStep->CleanUp();
				//LOG(ERROR) << "Error:Calibration is failed!";
				_siteInfo._strErrorMessage = ex.GetDescription();
				errorFlag = true;

				//_uiErrorFlag = Syn_Info::
				_strErrorMessage = ex.GetDescription();
				
				break;
			}
			catch (...)
			{
				errorFlag = true;
				_uiErrorFlag = Syn_Info::Syn_UnknownError;
				break;
			}
		}
	}

	_stopFlag = false;

	if (errorFlag)
	{
		_sitState = Error;
	}
	else
	{
		_sitState = TestDataReady;
	}
}

bool Syn_Site::GetTestScriptInfo(uint8_t scriptID, Syn_TestScript &oTestScriptInfo)
{
	std::string strScriptName;

	switch (scriptID)
	{
		case 1:
			strScriptName = "PreCheck";
			break;
		case 2:
			strScriptName = "PatchTest";
			break;
		case 3:
			strScriptName = "WithoutStimulus";
			break;
		case 4:
			strScriptName = "WithStimulus";
			break;
		case 5:
			strScriptName = "ImageQuality";
			break;
		case 6:
			strScriptName = "PostCheck";
			break;
		default:
			strScriptName = "PreCheck";
			break;
	}

	bool IsExists(false);

	for (size_t i = 1; i <= _SysConfig._listTestScript.size(); i++)
	{
		if (strScriptName == _SysConfig._listTestScript[i - 1]._strScriptName)
		{
			oTestScriptInfo = _SysConfig._listTestScript[i - 1];
			IsExists = true;
			break;
		}
	}
		
	return IsExists;
}

uint32_t Syn_Site::GetTestResult(uint16_t stepNo, Syn_DutTestResult &oTestResult)
{
	if (NULL != _pSyn_Dut)
	{
		if (NULL != _pSyn_Dut->_pSyn_DutTestResult)
		{
			oTestResult = *(_pSyn_Dut->_pSyn_DutTestResult);

			return Syn_Info::Syn_OK;
		}
	}

	return Syn_Info::Syn_DutResultNull;
}


















bool Syn_Site::ConstructSiteList(std::string strConfigFilePath, std::vector<Syn_Site*> &olistOfSyn_SiteInstance)
{
	bool rc(false);

	olistOfSyn_SiteInstance.clear();

	rc = Syn_Site::RegisterLoggingConfig();

	//xml config file parse
	Syn_SysConfigOperation *pSyn_SysConfigOperationInstance = NULL;
	rc = Syn_SysConfigOperation::GetSysConfigInstance(strConfigFilePath, pSyn_SysConfigOperationInstance);
	if (!rc||NULL == pSyn_SysConfigOperationInstance)
	{
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteList() - pSyn_SysConfigOperationInstance is NULL!" << endl;
		return false;
	}
	Syn_SysConfig TempSyn_SysConfig;
	rc = pSyn_SysConfigOperationInstance->GetSysConfig(TempSyn_SysConfig);
	if (!rc)
	{
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteList() - ::GetSysConfig is failed!" << endl;
		return false;
	}

	uint32_t uResult(1);

	//Init Operation
	uResult = MPC_Initialize();
	if (0 != uResult)
	{
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteList() - ::MPC_Initialize is failed!" << endl;
		return false;
	}

	int iDeviceCounts(0);
	MPC_GetNumberOfDevices(&iDeviceCounts);
	if (0 == iDeviceCounts)
	{
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteList() - iDeviceCounts is 0!" << endl;
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
			/*bool rc = Syn_Site::ConstructSiteInstance(uiSerialNumber, TempSyn_SysConfig, pSyn_SiteInstance);
			if (!rc&&NULL==pSyn_SiteInstance)
			{
				LOG(ERROR) << "Error:Syn_Site::ConstructSiteList() - ConstructSiteInstance is failed,i is " << i << "!" << endl;
				continue;
			}*/

			pSyn_SiteInstance = new Syn_Site(i + 1, uiSerialNumber, strConfigFilePath);
			pSyn_SiteInstance->Init();
			
			pSyn_SiteInstance->_siteInfo._uiSerialNumber = uiSerialNumber;
			pSyn_SiteInstance->_iSiteNumber = i + 1;
			pSyn_SiteInstance->_siteInfo._uiSiteNumber = pSyn_SiteInstance->_iSiteNumber;

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

bool Syn_Site::RegisterLoggingConfig()
{
	el::Configurations defaultConf;
	//defaultConf.setToDefault();
	const time_t t = time(NULL);
	struct tm* current_time = localtime(&t);
	std::string strMonthValue = to_string(current_time->tm_mon + 1);
	if (1 == strMonthValue.length())
		strMonthValue = "0" + strMonthValue;
	std::string strTimeValue = to_string(current_time->tm_year + 1900) + "_" + strMonthValue + "_" + to_string(current_time->tm_mday);
	std::string strLogFilePath = std::string(".\\logs\\") + strTimeValue + std::string(".log");
	el::Configuration confFilenameInfo(el::Level::Global, el::ConfigurationType::Filename, strLogFilePath);
	defaultConf.set(&confFilenameInfo);
	defaultConf.parseFromText("*GLOBAL:\n FORMAT = [%datetime] [%thread] [%level] [%func] %msg");
	el::Loggers::reconfigureLogger("default", defaultConf);

	return true;
}


//void Syn_Site::Run()
//{
//	if (NULL == _pSyn_Dut)
//	{
//		LOG(ERROR) << "Error:Syn_Site::Run() - _pSyn_Dut is NULL!" << endl;
//		_siteInfo._TestState = TestError;
//		return;
//	}
//
//	//test get image
//	try
//	{
//		/*_siteInfo._TestState = TestRunning;
//		_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
//		_pSyn_Dut->CheckDUTexists();
//		_pSyn_Dut->Calibration(_SysConfig._uiNumCols, _SysConfig._uiNumRows, _DutTestInfo._calibrationInfo, _pDutTestResult->_calibrationResults);
//		_pSyn_Dut->PowerOff();*/
//
//	}
//	catch (Syn_Exception ex)
//	{
//		//_pSyn_Dut->PowerOff();
//		LOG(ERROR) << "Error:ReadOTP is failed!" << std::endl;
//		_siteInfo._strErrorMessage = ex.GetDescription();
//		_siteInfo._TestState = TestFailed;
//
//		return;
//	}
//
//	return;
//}
//
//
//void Syn_Site::GetVersion()
//{
//	try
//	{
//		_siteInfo._TestState = TestRunning;
//		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
//		_pSyn_Dut->GetDutCtrl()->FpGetVersion(_DutTestInfo._getVerInfo._GerVerArray, VERSION_SIZE);
//		_pSyn_Dut->PowerOff();*/
//
//	}
//	catch (Syn_Exception ex)
//	{
//		//_pSyn_Dut->PowerOff();
//		LOG(ERROR) << "Error:GetVersion is failed!" ;
//		_siteInfo._strErrorMessage = ex.GetDescription();
//		_siteInfo._TestState = TestFailed;
//		return;
//	}
//
//	_siteInfo._TestState = TestOK;
//}
//
//
//void Syn_Site::ReadOTP()
//{
//	if (NULL == _pSyn_Dut)
//	{
//		LOG(ERROR) << "Error:Syn_Site::Run() - _pSyn_Dut is NULL!" << endl;
//		_siteInfo._TestState = TestError;
//		return;
//	}
//	uint8_t arMS0[BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE] = { 0 };
//	int iSize(BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE);
//	try
//	{
//		_siteInfo._TestState = TestRunning;
//		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
//		_pSyn_Dut->ReadOTP(arMS0, iSize);
//		_pSyn_Dut->PowerOff();*/
//
//	}
//	catch (Syn_Exception ex)
//	{
//		//_pSyn_Dut->PowerOff();
//		LOG(ERROR) << "Error:ReadOTP is failed!" << std::endl;
//		_siteInfo._strErrorMessage = ex.GetDescription();
//		_siteInfo._TestState = TestFailed;
//
//		return;
//	}
//
//	//Fill
//	for (int i = 0; i < BS0_SIZE; i++)
//	{
//		//(_DutTestInfo._otpInfo._BootSector0Array)[i] = arMS0[i];
//		(_pSyn_Dut->_pSyn_DutTestInfo->_otpInfo._BootSector0Array)[i] = arMS0[i];
//	}
//
//	for (int i = 0; i < BS1_SIZE; i++)
//	{
//		//(_DutTestInfo._otpInfo._BootSector1Array)[i] = arMS0[i + BS0_SIZE];
//		(_pSyn_Dut->_pSyn_DutTestInfo->_otpInfo._BootSector1Array)[i] = arMS0[i + BS0_SIZE];
//	}
//
//	for (int i = 0; i < MS0_SIZE; i++)
//	{
//		//(_DutTestInfo._otpInfo._MainSector0Array)[i] = arMS0[i + BS0_SIZE + BS1_SIZE];
//		(_pSyn_Dut->_pSyn_DutTestInfo->_otpInfo._MainSector0Array)[i] = arMS0[i + BS0_SIZE + BS1_SIZE];
//	}
//	
//	for (int i = 0; i < MS1_SIZE; i++)
//	{
//		//(_DutTestInfo._otpInfo._MainSector1Array)[i] = arMS0[i + BS0_SIZE + BS1_SIZE + MS0_SIZE];
//		(_pSyn_Dut->_pSyn_DutTestInfo->_otpInfo._MainSector1Array)[i] = arMS0[i + BS0_SIZE + BS1_SIZE + MS0_SIZE];
//	}
//
//	_siteInfo._TestState = TestOK;
//}
//
//
//
//void Syn_Site::Calibration()
//{
//	/*if (NULL != _pTempTestStep)
//	{
//		delete _pTempTestStep;
//		_pTempTestStep = NULL;
//	}*/
//
//	Syn_TestStep *pTestStep = NULL;
//
//	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
//	if (!rc || NULL == pTestStep)
//	{
//		return;
//	}
//
//	try
//	{
//		_siteInfo._TestState = TestRunning;
//		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
//		_pSyn_Dut->CheckDUTexists();
//		_pSyn_Dut->Calibration(_SysConfig._uiNumCols, _SysConfig._uiNumRows, _DutTestInfo._calibrationInfo, _pDutTestResult->_calibrationResults);
//		_pSyn_Dut->GetFingerprintImage(_pDutTestResult->_calibrationResults, &(_pDutTestResult->_acquireFpsResults.arr_ImageFPSFrame), _SysConfig._uiNumRows, _SysConfig._uiNumCols);*/
//		//_pSyn_Dut->PowerOff();
//
//		pTestStep->SetUp();
//		pTestStep->Excute();
//
//		delete pTestStep;
//		pTestStep = NULL;
//
//	}
//	catch (Syn_Exception ex)
//	{
//		//_pSyn_Dut->PowerOff();
//		LOG(ERROR) << "Error:Calibration is failed!";
//		_siteInfo._strErrorMessage = ex.GetDescription();
//		_siteInfo._TestState = TestFailed;
//		return;
//	}
//
//	_siteInfo._TestState = TestOK;
//}
//
//void Syn_Site::GetFingerprintImage()
//{
//	/*if (NULL == _pTempTestStep)
//	{
//		return;
//	}*/
//
//	Syn_TestStep *pTestStep = NULL;
//
//	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
//	if (!rc || NULL == pTestStep)
//	{
//		return;
//	}
//
//
//	try
//	{
//		_siteInfo._TestState = TestRunning;
//		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
//		_pSyn_Dut->GetFingerprintImage(_pDutTestResult->_calibrationResults, &(_pDutTestResult->_acquireFpsResults.arr_ImageFPSFrame), _SysConfig._uiNumRows, _SysConfig._uiNumCols);*/
//		//_pSyn_Dut->PowerOff();
//
//		pTestStep->ProcessData();
//
//		delete pTestStep;
//		pTestStep = NULL;
//
//	}
//	catch (Syn_Exception ex)
//	{
//		//_pSyn_Dut->PowerOff();
//		LOG(ERROR) << "Error:GetFingerprintImage is failed!";
//		_siteInfo._strErrorMessage = ex.GetDescription();
//		_siteInfo._TestState = TestFailed;
//		return;
//	}
//
//	_siteInfo._TestState = TestOK;
//}
//
//void Syn_Site::PowerOff()
//{
//	//_pSyn_Dut->PowerOff();
//
//	/*if (NULL != _pTempTestStep)
//	{
//		_pTempTestStep->CleanUp();
//	}*/
//
//	Syn_TestStep *pTestStep = NULL;
//	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
//	if (NULL != pTestStep)
//	{
//		pTestStep->CleanUp();
//
//		delete pTestStep;
//		pTestStep = NULL;
//	}
//}

/*
void Syn_Site::GetSiteInfo(Syn_SiteInfo &oSyn_SiteInfo)
{
	oSyn_SiteInfo = _siteInfo;
}

void Syn_Site::GetTestInfo(Syn_DutTestInfo &oSyn_DutTestInfo)
{
	//oSyn_DutTestInfo = _DutTestInfo;

	oSyn_DutTestInfo = *(_pSyn_Dut->_pSyn_DutTestInfo);
}

void Syn_Site::GetTestResult(Syn_DutTestResult * &opSyn_DutTestResult)
{
	//opSyn_DutTestResult = _pDutTestResult;

	opSyn_DutTestResult = _pSyn_Dut->_pSyn_DutTestResult;

	_sitState = Idle;
}
*/
