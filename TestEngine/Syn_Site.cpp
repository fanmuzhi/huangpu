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
,_siteState(Closed)
,_stopFlag(false)
, _uiErrorFlag(Syn_ExceptionCode::Syn_OK)
,_strErrorMessage("")
{	
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

uint32_t Syn_Site::CreateSiteInstance(uint8_t siteNumber, uint32_t deviceSerNumber, std::string strConfigFilePath, const AdcBaseLineInfo &iADCInfo, Syn_Site * &opSiteInstance)
{
	opSiteInstance = NULL;

	opSiteInstance = new Syn_Site(siteNumber, deviceSerNumber, strConfigFilePath);
	uint32_t rc = opSiteInstance->Init();
	if (Syn_ExceptionCode::Syn_OK != rc)
	{
		delete opSiteInstance;
		opSiteInstance = NULL;
	}
	else
	{
		opSiteInstance->_ADCInfo.m_bExecuted = true;
		opSiteInstance->_ADCInfo.m_nVdd = iADCInfo.m_nVdd;
		opSiteInstance->_ADCInfo.m_nVio = iADCInfo.m_nVio;
		opSiteInstance->_ADCInfo.m_nVled = iADCInfo.m_nVled;
		opSiteInstance->_ADCInfo.m_nVddh = iADCInfo.m_nVddh;

		for (int a = 0; a < NUM_CURRENT_VALUES; a++)
		{
			for (int b = 0; b < KNUMGAINS; b++)
			{
				(opSiteInstance->_ADCInfo.m_arAdcBaseLines)[a][b] = (iADCInfo.m_arAdcBaseLines)[a][b];
			}
		}
	}
	

	return rc;
}

uint32_t Syn_Site::Init()
{
	//xml config file parse
	Syn_SysConfigOperation *pConfigOperationInstance = NULL;
	uint32_t rc = Syn_SysConfigOperation::GetSysConfigInstance(_strConfigFilePath, pConfigOperationInstance);
	if (NULL == pConfigOperationInstance)
	{
		_siteState = Error;
		_uiErrorFlag = rc;
		LOG(ERROR) << "Error:Syn_Site::Init() - pConfigOperationInstance is NULL!" << endl;
		return rc;
	}

	rc = pConfigOperationInstance->GetSysConfig(_SysConfig);
	if (Syn_ExceptionCode::Syn_OK != rc)
	{
		_siteState = Error;
		_uiErrorFlag = rc;
		LOG(ERROR) << "Error:Syn_Site::Init() - ::GetSysConfig is failed!" << endl;
		return rc;
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

	//Create DutCtrl
	rc = Syn_DutCtrl::CreateDutCtrlInstance(iDutControllerType, _uiSerialNumber, _pSyn_DutCtrl);
	if (NULL == _pSyn_DutCtrl)
	{
		_siteState = Error;
		_uiErrorFlag = rc;
		LOG(ERROR) << "Error:Syn_Site::Init() - CreateDutInstance is failed!";
		return rc;
	}

	if (pConfigOperationInstance)
	{
		delete pConfigOperationInstance;
		pConfigOperationInstance = NULL;
	}

	_siteState = Closed;

	return Syn_ExceptionCode::Syn_OK;
}

uint32_t Syn_Site::Open()
{
	if (_siteState == SiteState::Error)
	{
		return _uiErrorFlag;
	}
	if (_siteState != SiteState::Closed)
	{
		return Syn_ExceptionCode::Syn_SiteStateError;
	}

	bool rc(false);

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

	if (NULL != _pSyn_Dut)
	{
		delete _pSyn_Dut;
		_pSyn_Dut = NULL;
	}

	//Create Dut
	rc = Syn_Dut::CreateDutInstance(iProjectType, _pSyn_Dut);
	if (NULL == _pSyn_Dut)
	{
		_siteState = Error;
		_uiErrorFlag = Syn_ExceptionCode::Syn_DutNull;
		LOG(ERROR) << "Error:Syn_Site::Init() - CreateDutInstance is failed!";
		return _uiErrorFlag;
	}

	_pSyn_Dut->SetPatchInfo(_SysConfig._listPatchInfo);

	//fill info
	_pSyn_Dut->InitData(_SysConfig,_ADCInfo);

	_siteState = Idle;

	return Syn_ExceptionCode::Syn_OK;
}

uint32_t Syn_Site::Close()
{
	if (_siteState == SiteState::Error)
	{
		return _uiErrorFlag;
	}

	if (_siteState == TestDataReady || _siteState == Idle)
	{
		delete _pSyn_Dut;
		_pSyn_Dut = NULL;
		_siteState = Closed;

		return  Syn_ExceptionCode::Syn_OK;
	}

	return Syn_ExceptionCode::Syn_SiteStateError;
}

uint32_t Syn_Site::ExecuteScript(uint8_t scriptID)
{
	if (_siteState == SiteState::Error)
	{
		return _uiErrorFlag;
	}
	if (_siteState != Idle)
	{
		return Syn_ExceptionCode::Syn_SiteStateError;
	}

	bool rc(false);
	_siteState = Running;

	//std::thread siteThread(RunScript, this, scriptID);

	std::thread siteThread([&]()
	{
		this->RunScript(scriptID);
	});

	siteThread.detach();
	//siteThread.join();
	
	//return Syn_ExceptionCode::Syn_SiteThread;
	return Syn_ExceptionCode::Syn_OK;
}

void Syn_Site::RunScript(uint8_t scriptID)
{

	//_pSyn_Dut->InitData(_SysConfig);

	Syn_TestScript ExceteScriptInfo;
	bool rc = GetTestScriptInfo(scriptID, ExceteScriptInfo);
	if (!rc)
	{
		_uiErrorFlag = Syn_ExceptionCode::Syn_ScriptConfigError;
		return;
	}

	unsigned int listSize = ExceteScriptInfo._listOfTestStep.size();
	if (0 == listSize)
	{
		_uiErrorFlag = Syn_ExceptionCode::Syn_ScriptConfigError;
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

		std::string strArgsValue("");
		_SysConfig.GetSyn_TestStepInfo(CurrentTestStepInfo._strTestStepName, strArgsValue);

		rc = Syn_TestStepFactory::CreateTestStepInstance(CurrentTestStepInfo._strTestStepName, strArgsValue, _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
		if (rc && NULL != pTestStep)
		{
			try
			{
				if (1 == i)
				{
					pTestStep->SetUp();
				}

				pTestStep->Execute();

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
				//_siteInfo._strErrorMessage = ex.GetDescription();
				errorFlag = true;

				//_uiErrorFlag = Syn_Info::
				_strErrorMessage = ex.GetDescription();
				_uiErrorFlag = ex.GetError();

				delete pTestStep;
				pTestStep = NULL;
				
				break;
			}
			catch (...)
			{
				errorFlag = true;
				_uiErrorFlag = Syn_ExceptionCode::Syn_UnknownError;
				delete pTestStep;
				pTestStep = NULL;
				break;
			}
		}
	}

	_stopFlag = false;

	if (errorFlag)
	{
		_siteState = Error;
	}
	else
	{
		_siteState = TestDataReady;
	}
}

bool Syn_Site::GetTestScriptInfo(uint8_t scriptID, Syn_TestScript &oTestScriptInfo)
{
	/*std::string strScriptName;

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
	}*/

	bool IsExists(false);

	/*for (size_t i = 1; i <= _SysConfig._listTestScript.size(); i++)
	{
		if (strScriptName == _SysConfig._listTestScript[i - 1]._strScriptName)
		{
			oTestScriptInfo = _SysConfig._listTestScript[i - 1];
			IsExists = true;
			break;
		}
	}*/

	oTestScriptInfo._listOfTestStep.clear();
	for(size_t i = 1; i <= _SysConfig._listTestSteps.size(); i++)
	{
		if (scriptID == _SysConfig._listTestSteps[i - 1]._uiScriptID)
		{
			oTestScriptInfo._listOfTestStep.push_back(_SysConfig._listTestSteps[i - 1]);
			IsExists = true;
		}
	}
		
	return IsExists;
}

uint32_t Syn_Site::GetTestInfo(Syn_DutTestInfo * &opTestInfo)
{
	opTestInfo = NULL;
	if (NULL != _pSyn_Dut)
	{
		if (NULL != _pSyn_Dut->_pSyn_DutTestInfo)
		{
			opTestInfo = _pSyn_Dut->_pSyn_DutTestInfo;

			return Syn_ExceptionCode::Syn_OK;
		}
	}

	return Syn_ExceptionCode::Syn_DutInfoNull;
}

uint32_t Syn_Site::GetTestResult(Syn_DutTestResult * &opTestResult)
{
	opTestResult = NULL;

	if (_siteState == SiteState::Error)
	{
		return _uiErrorFlag;
	}
	if (_siteState != TestDataReady)
	{
		return Syn_ExceptionCode::Syn_SiteStateError;
	}

	if (NULL != _pSyn_Dut)
	{
		if (NULL != _pSyn_Dut->_pSyn_DutTestResult)
		{
			opTestResult = _pSyn_Dut->_pSyn_DutTestResult;
			_siteState = Idle;
			return Syn_ExceptionCode::Syn_OK;
		}
	}

	_siteState = Error;

	return Syn_ExceptionCode::Syn_DutResultNull;
}

uint32_t Syn_Site::ExecuteTestStep(std::string sTestName)
{
	if (_siteState == SiteState::Error)
	{
		return _uiErrorFlag;
	}
	if (_siteState != Idle)
	{
		return Syn_ExceptionCode::Syn_SiteStateError;
	}
	_siteState = Running;

	Syn_TestStep *pTestStep = NULL;
	
	std::string strArgsValue("");
	_SysConfig.GetSyn_TestStepInfo(sTestName, strArgsValue);

	bool rc = Syn_TestStepFactory::CreateTestStepInstance(sTestName, strArgsValue, _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
	if (!rc || NULL == pTestStep)
	{
		return Syn_ExceptionCode::Syn_TestStepConfigError;
	}

	try
	{
		pTestStep->SetUp();
		pTestStep->Execute();
		pTestStep->ProcessData();
		pTestStep->CleanUp();

		delete pTestStep;
		pTestStep = NULL;

		_siteState = TestDataReady;
	}
	catch (Syn_Exception ex)
	{
		try
		{
			pTestStep->CleanUp();
		}
		catch (...){}
		delete pTestStep;
		pTestStep = NULL;

		_siteState = Error;
		_strErrorMessage = ex.GetDescription();
		_uiErrorFlag = ex.GetError();
		return _uiErrorFlag;
	}
	return Syn_ExceptionCode::Syn_OK;
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
//		
//		return;
//	}

//	//test get image
//	try
//	{
//		/*_siteInfo._TestState = TestRunning;
//		_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
//		_pSyn_Dut->CheckDUTexists();
//		_pSyn_Dut->Calibration(_SysConfig._uiNumCols, _SysConfig._uiNumRows, _DutTestInfo._calibrationInfo, _pDutTestResult->_calibrationResults);
//		_pSyn_Dut->PowerOff();*/

//	}
//	catch (Syn_Exception ex)
//	{
//		//_pSyn_Dut->PowerOff();
//		LOG(ERROR) << "Error:ReadOTP is failed!" << std::endl;
//		_siteInfo._strErrorMessage = ex.GetDescription();

//		return;
//	}

//	return;
//}


//void Syn_Site::Calibration()
//{
//	/*if (NULL != _pTempTestStep)
//	{
//		delete _pTempTestStep;
//		_pTempTestStep = NULL;
//	}*/

//	Syn_TestStep *pTestStep = NULL;

//	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
//	if (!rc || NULL == pTestStep)
//	{
//		return;
//	}

//	try
//	{
//		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
//		_pSyn_Dut->CheckDUTexists();
//		_pSyn_Dut->Calibration(_SysConfig._uiNumCols, _SysConfig._uiNumRows, _DutTestInfo._calibrationInfo, _pDutTestResult->_calibrationResults);
//		_pSyn_Dut->GetFingerprintImage(_pDutTestResult->_calibrationResults, &(_pDutTestResult->_acquireFpsResults.arr_ImageFPSFrame), _SysConfig._uiNumRows, _SysConfig._uiNumCols);*/
//		//_pSyn_Dut->PowerOff();

//		pTestStep->SetUp();
//		pTestStep->Excute();

//		delete pTestStep;
//		pTestStep = NULL;

//	}
//	catch (Syn_Exception ex)
//	{
//		delete pTestStep;
//		pTestStep = NULL;
//		LOG(ERROR) << "Error:Calibration is failed!";
//		_siteInfo._strErrorMessage = ex.GetDescription();
//		return;
//	}
//}

//void Syn_Site::GetFingerprintImage()
//{
//	/*if (NULL == _pTempTestStep)
//	{
//		return;
//	}*/

//	Syn_TestStep *pTestStep = NULL;

//	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
//	if (!rc || NULL == pTestStep)
//	{
//		return;
//	}


//	try
//	{
//		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
//		_pSyn_Dut->GetFingerprintImage(_pDutTestResult->_calibrationResults, &(_pDutTestResult->_acquireFpsResults.arr_ImageFPSFrame), _SysConfig._uiNumRows, _SysConfig._uiNumCols);*/
//		//_pSyn_Dut->PowerOff();

//		pTestStep->ProcessData();

//		delete pTestStep;
//		pTestStep = NULL;

//	}
//	catch (Syn_Exception ex)
//	{
//		delete pTestStep;
//		pTestStep = NULL;
//		//_pSyn_Dut->PowerOff();
//		LOG(ERROR) << "Error:GetFingerprintImage is failed!";
//		_siteInfo._strErrorMessage = ex.GetDescription();
//		return;
//	}
//}

//void Syn_Site::PowerOff()
//{
//	//_pSyn_Dut->PowerOff();

//	/*if (NULL != _pTempTestStep)
//	{
//		_pTempTestStep->CleanUp();
//	}*/

//	Syn_TestStep *pTestStep = NULL;
//	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
//	if (NULL != pTestStep)
//	{
//		pTestStep->CleanUp();

//		delete pTestStep;
//		pTestStep = NULL;
//	}
//}
