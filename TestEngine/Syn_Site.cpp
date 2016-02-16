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
#include <time.h>

#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

Syn_Site::Syn_Site()
:_pSyn_Dut(NULL)
,_pSyn_DutCtrl(NULL)
, _iSiteNumber(0)
, _pDutTestResult(NULL)

//, _pTempTestStep(NULL)
{
	_siteInfo._TestState = TestReady;

	_pDutTestResult = new Syn_DutTestResult();
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

	if (NULL != _pDutTestResult)
	{
		delete _pDutTestResult;
		_pDutTestResult = NULL;
	}


	/*if (NULL != _pTempTestStep)
	{
		delete _pTempTestStep;
		_pTempTestStep = NULL;
	}*/
}

bool Syn_Site::ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance)
{
	bool rc(false);

	opSyn_SiteInstance = NULL;
	if (0 == iSerialNumber)
	{
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteInstance() - iSerialNumber is 0!" ;
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
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteInstance() - an unknown ProjectType,construct it to Viper1!" ;
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
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteInstance() - an unknown DutController,construct it to SPC!" ;
	}

	//Create Dut
	Syn_Dut *pSyn_Dut = NULL;
	rc = Syn_Dut::CreateDutInstance(iProjectType, pSyn_Dut);
	if (!rc || NULL == pSyn_Dut)
	{
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteInstance() - CreateDutInstance is failed!" ;
		return false;
	}

	//Create DutCtrl
	Syn_DutCtrl *pSyn_DutCtrl = NULL;
	rc = Syn_DutCtrl::CreateDutCtrlInstance(iDutControllerType, iSerialNumber, pSyn_DutCtrl);
	if (!rc || NULL == pSyn_DutCtrl)
	{
		delete pSyn_Dut;
		pSyn_Dut = NULL;
		LOG(ERROR) << "Error:Syn_Site::ConstructSiteInstance() - CreateDutInstance is failed!" ;
		return false;
	}

	opSyn_SiteInstance = new Syn_Site();
	
	opSyn_SiteInstance->_pSyn_Dut = pSyn_Dut;
	opSyn_SiteInstance->_pSyn_DutCtrl = pSyn_DutCtrl;
	opSyn_SiteInstance->_SysConfig = iSyn_SysConfigInfo;
	opSyn_SiteInstance->_uiSerialNumber = iSerialNumber;

	pSyn_Dut->SetPatchInfo(opSyn_SiteInstance->_SysConfig._listPatchInfo);

	//fill info
	opSyn_SiteInstance->_pSyn_Dut->InitData(opSyn_SiteInstance->_SysConfig);

	return true;
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
			bool rc = Syn_Site::ConstructSiteInstance(uiSerialNumber, TempSyn_SysConfig, pSyn_SiteInstance);
			if (!rc&&NULL==pSyn_SiteInstance)
			{
				LOG(ERROR) << "Error:Syn_Site::ConstructSiteList() - ConstructSiteInstance is failed,i is " << i << "!" << endl;
				continue;
			}
			
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


void Syn_Site::Run()
{
	if (NULL == _pSyn_Dut)
	{
		LOG(ERROR) << "Error:Syn_Site::Run() - _pSyn_Dut is NULL!" << endl;
		_siteInfo._TestState = TestError;
		return;
	}

	//test get image
	try
	{
		/*_siteInfo._TestState = TestRunning;
		_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
		_pSyn_Dut->CheckDUTexists();
		_pSyn_Dut->Calibration(_SysConfig._uiNumCols, _SysConfig._uiNumRows, _DutTestInfo._calibrationInfo, _pDutTestResult->_calibrationResults);
		_pSyn_Dut->PowerOff();*/

	}
	catch (Syn_Exception ex)
	{
		//_pSyn_Dut->PowerOff();
		LOG(ERROR) << "Error:ReadOTP is failed!" << std::endl;
		_siteInfo._strErrorMessage = ex.GetDescription();
		_siteInfo._TestState = TestFailed;

		return;
	}

	return;
}


void Syn_Site::GetVersion()
{
	try
	{
		_siteInfo._TestState = TestRunning;
		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
		_pSyn_Dut->GetDutCtrl()->FpGetVersion(_DutTestInfo._getVerInfo._GerVerArray, VERSION_SIZE);
		_pSyn_Dut->PowerOff();*/

	}
	catch (Syn_Exception ex)
	{
		//_pSyn_Dut->PowerOff();
		LOG(ERROR) << "Error:GetVersion is failed!" ;
		_siteInfo._strErrorMessage = ex.GetDescription();
		_siteInfo._TestState = TestFailed;
		return;
	}

	_siteInfo._TestState = TestOK;
}


void Syn_Site::ReadOTP()
{
	if (NULL == _pSyn_Dut)
	{
		LOG(ERROR) << "Error:Syn_Site::Run() - _pSyn_Dut is NULL!" << endl;
		_siteInfo._TestState = TestError;
		return;
	}
	uint8_t arMS0[BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE] = { 0 };
	int iSize(BS0_SIZE + BS1_SIZE + MS0_SIZE + MS1_SIZE);
	try
	{
		_siteInfo._TestState = TestRunning;
		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
		_pSyn_Dut->ReadOTP(arMS0, iSize);
		_pSyn_Dut->PowerOff();*/

	}
	catch (Syn_Exception ex)
	{
		//_pSyn_Dut->PowerOff();
		LOG(ERROR) << "Error:ReadOTP is failed!" << std::endl;
		_siteInfo._strErrorMessage = ex.GetDescription();
		_siteInfo._TestState = TestFailed;

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

	_siteInfo._TestState = TestOK;
}



void Syn_Site::Calibration()
{
	/*if (NULL != _pTempTestStep)
	{
		delete _pTempTestStep;
		_pTempTestStep = NULL;
	}*/

	Syn_TestStep *pTestStep = NULL;

	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
	if (!rc || NULL == pTestStep)
	{
		return;
	}

	try
	{
		_siteInfo._TestState = TestRunning;
		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
		_pSyn_Dut->CheckDUTexists();
		_pSyn_Dut->Calibration(_SysConfig._uiNumCols, _SysConfig._uiNumRows, _DutTestInfo._calibrationInfo, _pDutTestResult->_calibrationResults);
		_pSyn_Dut->GetFingerprintImage(_pDutTestResult->_calibrationResults, &(_pDutTestResult->_acquireFpsResults.arr_ImageFPSFrame), _SysConfig._uiNumRows, _SysConfig._uiNumCols);*/
		//_pSyn_Dut->PowerOff();

		pTestStep->SetUp();
		pTestStep->Excute();

		delete pTestStep;
		pTestStep = NULL;

	}
	catch (Syn_Exception ex)
	{
		//_pSyn_Dut->PowerOff();
		LOG(ERROR) << "Error:Calibration is failed!";
		_siteInfo._strErrorMessage = ex.GetDescription();
		_siteInfo._TestState = TestFailed;
		return;
	}

	_siteInfo._TestState = TestOK;
}

void Syn_Site::GetFingerprintImage()
{
	/*if (NULL == _pTempTestStep)
	{
		return;
	}*/

	Syn_TestStep *pTestStep = NULL;

	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
	if (!rc || NULL == pTestStep)
	{
		return;
	}


	try
	{
		_siteInfo._TestState = TestRunning;
		/*_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
		_pSyn_Dut->GetFingerprintImage(_pDutTestResult->_calibrationResults, &(_pDutTestResult->_acquireFpsResults.arr_ImageFPSFrame), _SysConfig._uiNumRows, _SysConfig._uiNumCols);*/
		//_pSyn_Dut->PowerOff();

		pTestStep->ProcessData();

		delete pTestStep;
		pTestStep = NULL;

	}
	catch (Syn_Exception ex)
	{
		//_pSyn_Dut->PowerOff();
		LOG(ERROR) << "Error:GetFingerprintImage is failed!";
		_siteInfo._strErrorMessage = ex.GetDescription();
		_siteInfo._TestState = TestFailed;
		return;
	}

	_siteInfo._TestState = TestOK;
}

void Syn_Site::PowerOff()
{
	//_pSyn_Dut->PowerOff();

	/*if (NULL != _pTempTestStep)
	{
		_pTempTestStep->CleanUp();
	}*/

	Syn_TestStep *pTestStep = NULL;
	bool rc = Syn_TestStepFactory::CreateTestStepInstance("Calibrate", _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
	if (NULL != pTestStep)
	{
		pTestStep->CleanUp();

		delete pTestStep;
		pTestStep = NULL;
	}
}

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
}