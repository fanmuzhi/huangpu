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
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
//#include <thread>

//#define ELPP_STL_LOGGING
//#define ELPP_THREAD_SAFE
//#include "easylogging++.h"
//INITIALIZE_EASYLOGGINGPP

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
		//LOG(ERROR) << "Error:Syn_Site::Init() - pConfigOperationInstance is NULL!" << endl;
		return rc;
	}

	rc = pConfigOperationInstance->GetSysConfig(_SysConfig);
	if (Syn_ExceptionCode::Syn_OK != rc)
	{
		_siteState = Error;
		_uiErrorFlag = rc;
		//LOG(ERROR) << "Error:Syn_Site::Init() - ::GetSysConfig is failed!" << endl;
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
		//LOG(ERROR) << "Error:Syn_Site::Init() - an unknown DutController,construct it to SPC!";
	}

	//Create DutCtrl
	rc = Syn_DutCtrl::CreateDutCtrlInstance(iDutControllerType, _uiSerialNumber, _pSyn_DutCtrl);
	if (NULL == _pSyn_DutCtrl)
	{
		_siteState = Error;
		_uiErrorFlag = rc;
		//LOG(ERROR) << "Error:Syn_Site::Init() - CreateDutInstance is failed!";
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
	if (_siteState == SiteState::Running)
	{
		return Syn_ExceptionCode::Syn_SiteStateError;
	}
	//if (_siteState != SiteState::Closed)
	//{
	//	return Syn_ExceptionCode::Syn_SiteStateError;
	//}
	_uiErrorFlag = 0;

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
		//LOG(ERROR) << "Error:Syn_Site::Init() - an unknown ProjectType,construct it to Viper1!";
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
		//LOG(ERROR) << "Error:Syn_Site::Init() - CreateDutInstance is failed!";
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
	if (_siteState == SiteState::Running)
	{
		return Syn_ExceptionCode::Syn_SiteStateError;
	}

	if (NULL != _pSyn_Dut)
	{
		delete _pSyn_Dut;
		_pSyn_Dut = NULL;
	}
	
	_siteState = Closed;

	return  Syn_ExceptionCode::Syn_OK;
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

	/*std::thread siteThread([&]()
	{
		this->RunScript(scriptID);
	});

	siteThread.detach();
	//siteThread.join();
	
	//return Syn_ExceptionCode::Syn_SiteThread;
	return Syn_ExceptionCode::Syn_OK;*/

	RunScript(scriptID);

	if (TestDataReady == _siteState)
	{
		return Syn_ExceptionCode::Syn_OK;
	}
	else
	{
		return _uiErrorFlag;
	}
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
	bool IsExists(false);

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
	if (_siteState != Idle && _siteState != TestDataReady)
	{
		return Syn_ExceptionCode::Syn_SiteStateError;
	}

	Syn_TestStep *pTestStep = NULL;
	
	std::string strArgsValue("");
	_SysConfig.GetSyn_TestStepInfo(sTestName, strArgsValue);

	bool rc = Syn_TestStepFactory::CreateTestStepInstance(sTestName, strArgsValue, _pSyn_DutCtrl, _pSyn_Dut, pTestStep);
	if (!rc || NULL == pTestStep)
	{
		_siteState = Error;
		_uiErrorFlag = Syn_ExceptionCode::Syn_TestStepConfigError;
		return Syn_ExceptionCode::Syn_TestStepConfigError;
	}

	_siteState = Running;
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

void Syn_Site::GetTestStepList(std::vector<std::string> &olistOfTestStepName)
{
	olistOfTestStepName.clear();

	for (size_t i = 1; i <= _SysConfig._listTestSteps.size(); i++)
	{
		olistOfTestStepName.push_back(_SysConfig._listTestSteps[i - 1]._strTestStepName);
	}
}

bool Syn_Site::Write_Log(std::string sFolderPath, std::string sFileName)
{
	if (-1 == _access(sFolderPath.c_str(), 2))
	{
		return false;
	}

	Syn_DutTestInfo * DutInfo = NULL;
	Syn_DutTestResult * DutResults = NULL;

	DutInfo = _pSyn_Dut->_pSyn_DutTestInfo;
	DutResults = _pSyn_Dut->_pSyn_DutTestResult;

	if (NULL == DutInfo)
		return false;
	if (NULL == DutResults)
		return false;

	int RowNumber = _pSyn_Dut->_RowNumber;
	int ColumnNumber = _pSyn_Dut->_ColumnNumber;

	std::string sSensorSerialNumber("");
	for (int i = 0; i < 12; i++)
	{
		sSensorSerialNumber.push_back(DutInfo->_getVerInfo.sSerialNumber[i]);
	}

	std::string stringFilePath;
	if (0 == sFileName.size())
	{
		std::string strSerachContent = sFolderPath + std::string("\\")+sSensorSerialNumber + std::string("*.csv");
		std::string strSensorSerialNumber;
		std::vector<std::string> lsitOfFileName;
		long handle;
		struct _finddata_t fileinfo;
		handle = _findfirst(strSerachContent.c_str(), &fileinfo);
		if (-1 == handle)
		{
			strSensorSerialNumber = sSensorSerialNumber + std::string("_1");
		}
		else
		{
			int iCounts(1);

			do
			{
				std::string strFileName = fileinfo.name;
				if (std::string::npos != strFileName.find_first_of(sSensorSerialNumber))
				{
					iCounts += 1;
				}
			} while (_findnext(handle, &fileinfo) == 0);

			strSensorSerialNumber = sSensorSerialNumber + std::string("_") + to_string(iCounts);
			_findclose(handle);
		}

		stringFilePath = sFolderPath + "\\" + strSensorSerialNumber + ".csv";
	}
	else
	{
		if (std::string::npos != sFileName.find_first_of(".csv"))
		{
			stringFilePath = sFolderPath + "\\" + sFileName;
		}
		else
		{
			stringFilePath = sFolderPath + "\\" + sFileName + ".csv";
		}
	}

	
	FILE *pFile = fopen(stringFilePath.c_str(), "a");
	if (NULL == pFile)
	{
		return false;
	}

	fprintf(pFile, "\n%%%%%%%%%%%%%%%%%%%%%%\n");
	fprintf(pFile, "MTLog\n");

	//Put in part number.
	//fprintf(pFile, "Part Number,%s\n", "");//DebugVersion

	//fprintf(pFile, "ConfigFile,%s\n", _pSyn_Dut->_pSyn_DutTestInfo-);
	fprintf(pFile, "\n%%%%%%%%%%%%%%%%%%%%%%\n");

	fprintf(pFile, "\n---------------------\n");
	const time_t t = time(NULL);
	struct tm* current_time = localtime(&t);
	//fprintf(pFile, "Run %d,%s\n", "", asctime(current_time));
	fprintf(pFile, "Run ,%s\n", asctime(current_time));

	//Sensor Serial Number
	fprintf(pFile, "Sensor Serial Number ,%s\n", sSensorSerialNumber.c_str());

	//InitlizationStep
	fprintf(pFile, "\nInitialization, %s,%d ms\n", DutResults->_initResults.m_bPass ? "Pass" : "Fail", 0);

	//RAMTest
	if (DutInfo->_RAMTestInfo.m_bExecuted)
	{
		uint32_t fp1 = (DutResults->_RAMTestResults.m_pResponseCacheDataRam[5] & 0XFF) | ((DutResults->_RAMTestResults.m_pResponseCacheDataRam[4] & 0XFF) << 8) | ((DutResults->_RAMTestResults.m_pResponseCacheDataRam[3] & 0XFF) << 16) | ((DutResults->_RAMTestResults.m_pResponseCacheDataRam[2] & 0XFF) << 24);
		uint32_t fp2 = (DutResults->_RAMTestResults.m_pResponseCacheTagRam[5] & 0XFF) | ((DutResults->_RAMTestResults.m_pResponseCacheTagRam[4] & 0XFF) << 8) | ((DutResults->_RAMTestResults.m_pResponseCacheTagRam[3] & 0XFF) << 16) | ((DutResults->_RAMTestResults.m_pResponseCacheTagRam[2] & 0XFF) << 24);
		uint32_t fp3 = (DutResults->_RAMTestResults.m_pResponseCachInstDataRam[5] & 0XFF) | ((DutResults->_RAMTestResults.m_pResponseCachInstDataRam[4] & 0XFF) << 8) | ((DutResults->_RAMTestResults.m_pResponseCachInstDataRam[3] & 0XFF) << 16) | ((DutResults->_RAMTestResults.m_pResponseCachInstDataRam[2] & 0XFF) << 24);
		uint32_t fp4 = (DutResults->_RAMTestResults.m_pResponseCachInstTagRam[5] & 0XFF) | ((DutResults->_RAMTestResults.m_pResponseCachInstTagRam[4] & 0XFF) << 8) | ((DutResults->_RAMTestResults.m_pResponseCachInstTagRam[3] & 0XFF) << 16) | ((DutResults->_RAMTestResults.m_pResponseCachInstTagRam[2] & 0XFF) << 24);
		uint32_t fp5 = (DutResults->_RAMTestResults.m_pResponseScmAndMainRam[5] & 0XFF) | ((DutResults->_RAMTestResults.m_pResponseScmAndMainRam[4] & 0XFF) << 8) | ((DutResults->_RAMTestResults.m_pResponseScmAndMainRam[3] & 0XFF) << 16) | ((DutResults->_RAMTestResults.m_pResponseScmAndMainRam[2] & 0XFF) << 24);

		fprintf(pFile, "\nRAMTest,%s,%.0f ms,Data cache data RAM,Data cache tag RAM,Instruction cache data RAM,Instruction cache tag RAM,SCM RAM and main RAM\n", (DutResults->_RAMTestResults.bPass) ? "Pass" : "Fail", DutResults->_RAMTestResults.m_elapsedtime);
		fprintf(pFile, ",,,%s,%s,%s,%s,%s", (DutResults->_RAMTestResults.bPassCacheDataRam) ? "Pass" : "Fail", (DutResults->_RAMTestResults.bPassCacheTagRam) ? "Pass" : "Fail", (DutResults->_RAMTestResults.bPassCachInstDataRam) ? "Pass" : "Fail", (DutResults->_RAMTestResults.bPassCachInstTagRam) ? "Pass" : "Fail", (DutResults->_RAMTestResults.bPassScmAndMainRam) ? "Pass" : "Fail");

		fprintf(pFile, "\nAddresses:,,,%X,%X,%X,%X,%X",
			(DutResults->_RAMTestResults.bPassCacheDataRam) ? 0 : fp1,
			(DutResults->_RAMTestResults.bPassCacheTagRam) ? 0 : fp2,
			(DutResults->_RAMTestResults.bPassCachInstDataRam) ? 0 : fp3,
			(DutResults->_RAMTestResults.bPassCachInstTagRam) ? 0 : fp4,
			(DutResults->_RAMTestResults.bPassScmAndMainRam) ? 0 : fp5);

		fprintf(pFile, "\n");
	}

	//Opens/Shorts Test
	if (DutInfo->_opensShortsInfo.m_bExecuted)
	{
		fprintf(pFile, "\nOpens/Shorts Test,%s,%.0f ms", DutResults->_opensShortsResults.m_bPass ? "Pass" : "Fail", DutResults->_opensShortsResults.m_elapsedtime);

		for (int i = 0; i<25; i++)
			fprintf(pFile, ",%d", *((uint32_t*)(&DutResults->_opensShortsResults.m_pResponse[(i + 1) * 4])));
		fprintf(pFile, "\n");
	}

	//Pixel Patch
	if (DutInfo->_pixelPatchInfo.m_bExecuted)
	{
		fprintf(pFile, "\nPixel Patch, %s,%.0f ms\n", DutResults->_pixelPatchResults.m_bPass ? "Pass" : "Fail", DutResults->_pixelPatchResults.m_elapsedtime);
		fprintf(pFile, ",,,");
		for (int i = 0; i < (DutInfo->_pixelPatchInfo.m_nNumResBytes) / 4; i++)
			fprintf(pFile, "%d,", *((uint32_t*)&DutResults->_pixelPatchResults.m_pResponse[i * 4]));
		fprintf(pFile, "\n");
	}

	//WoVarTest
	if (DutInfo->_woVarInfo.m_bExecuted)
	{
		fprintf(pFile, "\nWoVarTest,%s,%.0f ms\n", (DutResults->_woVarResults.m_bPass != 0) ? "Pass" : "Fail", DutResults->_woVarResults.m_elapsedtime);

		fprintf(pFile, ",,,");
		for (int i = 0; i<(DutInfo->_woVarInfo.m_nNumResBytes / 4); i++)
			fprintf(pFile, "%d,", *((uint32_t*)&DutResults->_woVarResults.m_pResponse[i * 4]));
		fprintf(pFile, "\n");
	}

	//AFE Test
	if (DutInfo->_AFETestInfo.m_bExecuted)
	{
		fprintf(pFile, "\nAFE Test,%s,%.0f ms,", (DutResults->_AFETestResults.m_bPass) ? "Pass" : "Fail", DutResults->_AFETestResults.m_elapsedtime);

		for (int i = 0; i<13; i++)
		{
			if (i == 0)
				fprintf(pFile, "%d,", DutResults->_AFETestResults.m_pResponse[4]);
			else
				fprintf(pFile, "0,");
		}
		fprintf(pFile, "\n,,,");

		uint32_t temp = 0x00000000;
		for (int i = 0; i<13; i++)
		{
			temp = 0x00000000;
			temp |= int(DutResults->_AFETestResults.m_pResponse[(4 * i) + 11]) << 24;
			temp |= int(DutResults->_AFETestResults.m_pResponse[(4 * i) + 10]) << 16;
			temp |= int(DutResults->_AFETestResults.m_pResponse[(4 * i) + 9]) << 8;
			temp |= int(DutResults->_AFETestResults.m_pResponse[(4 * i) + 8]);
			fprintf(pFile, "%d,", temp);
		}

		fprintf(pFile, "\n");
	}

	//WOF-Lowpower
	if (DutInfo->_wofLowPowerInfo.m_bExecuted)
	{
		if (DutResults->_wofLowPowerResults.m_bPass == 0)
		{
			fprintf(pFile, "\nWOF-Low Power,Fail,%.0f ms,Current (uA),%.3f\n", DutResults->_wofLowPowerResults.m_elapsedtime, DutResults->_wofLowPowerResults.m_nCurrent_uA);
		}
		else //Pass
		{
			fprintf(pFile, "\nWOF-Low Power,Pass,%.0f ms,Current (uA),%.3f\n", DutResults->_wofLowPowerResults.m_elapsedtime, DutResults->_wofLowPowerResults.m_nCurrent_uA);
		}
	}

	//Cablication
	if (DutInfo->_calibrationInfo.m_bExecuted)
	{
		fprintf(pFile, "\nCalibration, %s,%.0f ms\n", DutResults->_calibrationResults.m_bPass ? "Pass" : "Fail", DutResults->_calibrationResults.m_elapsedtime);
		// Stage1 LNA values from print patch
		fprintf(pFile, ",,,Stage1");
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, ",%02X", DutResults->_calibrationResults.m_pPrintPatch[i + DutInfo->_calibrationInfo.m_nLnaIdx]);
		}
		if (DutInfo->_calibrationInfo.m_nCalType == 0)
		{
			fprintf(pFile, "\n,,,Stage2");
			for (int i = 0; i < RowNumber; i++)
			{
				fprintf(pFile, ",%02X", DutResults->_calibrationResults.m_arPgaOffsets[i]);
			}
			fprintf(pFile, "\n");
		}
		else if (DutInfo->_calibrationInfo.m_nCalType == 1)
		{
			fprintf(pFile, "\n,,,Stage2 Used");
			for (int i = 0; i<(RowNumber)* (ColumnNumber - 8); i++)
			{
				fprintf(pFile, ",%02X", DutResults->_calibrationResults.m_arPgaOffsets[i]);
			}
			fprintf(pFile, "\n");
		}
		else
		{
			fprintf(pFile, ",,,Stage2 Used,N/A\n");
		}
		// Stage2 OTP values	
		if (DutInfo->_calibrationInfo.m_nCalType == 1)
		{
			if (DutResults->_calibrationResults.m_nPGA_OOPP_count != 0)
			{
				fprintf(pFile, ",,,Stage2 OTP");
				for (int i = 0; i< (NUM_PGA_OOPP_OTP_ROWS * (ColumnNumber - 8)); i++)
				{
					fprintf(pFile, ",%02X", DutResults->_calibrationResults.m_pPGAOtpArray[i]);
				}

				fprintf(pFile, "\n,,,Stage2 Variance Score,N/A\n");
			}
			else
			{
				fprintf(pFile, ",,,Stage2 OTP,N/A\n");
				fprintf(pFile, ",,,Stage2 Variance Score,N/A\n");
			}
		}
		fprintf(pFile, ",,,FlexId,%04X\n", DutInfo->_initInfo.m_nFlexId);
	}

	//WakeOnFinger Test
	if ((DutInfo->_wofInfo.m_bExecutedWithoutStimulus == 1) || (DutInfo->_wofInfo.m_bExecutedWithStimulus == 1))
	{
		fprintf(pFile, "\nWakeOnFinger Test,%s,%.0f ms,NoFinger,WithFinger,Gain,Delta,WithFinger_3.7v,Delta_3.7v,Limit_Delta_100,Limit_Delta_200,Limit_Delta_200_3p7",DutResults->_wofResults.m_bPass ? "Pass" : "Fail", DutResults->_wofResults.m_elapsedtime);

		if (DutResults->_wofResults.m_bPassAtGain100)
		{
			fprintf(pFile, "\n,,,%d,%d,%d,%d,%s,%s,%d,%d,%d\n",
				DutResults->_wofResults.m_nTriggerIdxWithoutStim_100, DutResults->_wofResults.m_nTriggerIdxWithStim_100, 100,
				DutResults->_wofResults.m_nTriggerIdxWithoutStim_100 - DutResults->_wofResults.m_nTriggerIdxWithStim_100, "None", "None",
				DutInfo->_wofInfo.m_nDelta_100, DutInfo->_wofInfo.m_nDelta_200, DutInfo->_wofInfo.m_nDelta_200_3p7);
		}
		else if (DutResults->_wofResults.m_bPassAtGain200)
		{
			fprintf(pFile, "\n,,,%d,%d,%d,%d,%s,%s,%d,%d,%d\n",
				DutResults->_wofResults.m_nTriggerIdxWithoutStim_200, DutResults->_wofResults.m_nTriggerIdxWithStim_200, 200,
				DutResults->_wofResults.m_nTriggerIdxWithoutStim_200 - DutResults->_wofResults.m_nTriggerIdxWithStim_200, "None", "None",
				DutInfo->_wofInfo.m_nDelta_100, DutInfo->_wofInfo.m_nDelta_200, DutInfo->_wofInfo.m_nDelta_200_3p7);
		}
		else
		{
			fprintf(pFile, "\n,,,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
				DutResults->_wofResults.m_nTriggerIdxWithoutStim_200, DutResults->_wofResults.m_nTriggerIdxWithStim_200, 200,
				DutResults->_wofResults.m_nTriggerIdxWithoutStim_200 - DutResults->_wofResults.m_nTriggerIdxWithStim_200, DutResults->_wofResults.m_nTriggerIdxWithStim_200_3p7,
				DutResults->_wofResults.m_nTriggerIdxWithoutStim_200 - DutResults->_wofResults.m_nTriggerIdxWithStim_200_3p7,
				DutInfo->_wofInfo.m_nDelta_100, DutInfo->_wofInfo.m_nDelta_200, DutInfo->_wofInfo.m_nDelta_200_3p7);
		}
		fprintf(pFile, "\n");
	}
	
	//SCM WakeOnFinger Test
	Syn_PatchInfo Cmd3SweepTagPatchInfo;
	if (_pSyn_Dut->FindPatch("Cmd3SweepTag", Cmd3SweepTagPatchInfo) && (DutInfo->_TopSCM_wofInfo.m_bExecutedWithoutStimulus == 1 || DutInfo->_TopSCM_wofInfo.m_bExecutedWithStimulus == 1))
	{
		//If we got the zone 0 data out of OTP rather than calculating.
		if (DutResults->_calibrationResults.m_nScmWofBot_count != 0)
		{
			fprintf(pFile, "\nSCM Bottom WakeOnFinger Test,%s,%.0f ms,NoFinger,WithFinger,Gain,Delta", "Pass (OTP)", DutResults->_BottomSCM_wofResults.m_elapsedtime);
			fprintf(pFile, "\n,,,%d,%d,%d,%d\n", DutResults->_BottomSCM_wofResults.m_nTriggerWithoutStim,
					DutResults->_BottomSCM_wofResults.m_nTriggerWithStim,
					DutResults->_BottomSCM_wofResults.m_nGain,
					DutResults->_BottomSCM_wofResults.m_nTriggerWithoutStim - DutResults->_BottomSCM_wofResults.m_nTriggerWithStim);
		}
		else if (_pSyn_Dut->_pSyn_DutTestResult->_BottomSCM_wofResults.m_bPass != 0)	//If passed.
		{
			fprintf(pFile, "\nSCM Bottom WakeOnFinger Test,%s,%d.0f ms,NoFinger,WithFinger,Gain,Delta", "Pass", DutResults->_BottomSCM_wofResults.m_elapsedtime);
			fprintf(pFile, "\n,,,%d,%d,%d,%d\n", DutResults->_BottomSCM_wofResults.m_nTriggerWithoutStim,
					DutResults->_BottomSCM_wofResults.m_nTriggerWithStim,
					DutResults->_BottomSCM_wofResults.m_nGain,
					DutResults->_BottomSCM_wofResults.m_nTriggerWithoutStim - DutResults->_BottomSCM_wofResults.m_nTriggerWithStim);
		}
		else	//Failed.
		{
			fprintf(pFile, "\nSCM Bottom WakeOnFinger Test,%s,%.0f ms,NoFinger,WithFinger,Gain,Delta", "Fail", DutResults->_BottomSCM_wofResults.m_elapsedtime);
			fprintf(pFile, "\n,,,%d,%d,%d,%d\n", DutResults->_BottomSCM_wofResults.m_nTriggerWithoutStim,
					DutResults->_BottomSCM_wofResults.m_nTriggerWithStim,
					DutResults->_BottomSCM_wofResults.m_nGain,
					DutResults->_BottomSCM_wofResults.m_nTriggerWithoutStim - DutResults->_BottomSCM_wofResults.m_nTriggerWithStim);
		}
	}

	Syn_PatchInfo Cmd4SweepTagPatchInfo;
	if (_pSyn_Dut->FindPatch("Cmd4SweepTag", Cmd4SweepTagPatchInfo) && (DutInfo->_BottomSCM_wofInfo.m_bExecutedWithoutStimulus == 1 || DutInfo->_BottomSCM_wofInfo.m_bExecutedWithStimulus == 1))
	{
		//If we got the zone 1 data out of OTP rather than calculating.
		if (DutResults->_calibrationResults.m_nScmWofTop_count != 0)
		{
			fprintf(pFile, "\nSCM Top WakeOnFinger Test,%s,%d ms,NoFinger,WithFinger,Gain,Delta", "Pass (OTP)", DutResults->_TopSCM_wofResults.m_elapsedtime);
			fprintf(pFile, "\n,,,%d,%d,%d,%d\n", DutResults->_TopSCM_wofResults.m_nTriggerWithoutStim,
				DutResults->_TopSCM_wofResults.m_nTriggerWithStim,
				DutResults->_TopSCM_wofResults.m_nGain,
				DutResults->_TopSCM_wofResults.m_nTriggerWithoutStim - DutResults->_TopSCM_wofResults.m_nTriggerWithStim);
		}
		else if (DutResults->_TopSCM_wofResults.m_bPass != 0)	//If passed.
		{
			fprintf(pFile, "\nSCM Top WakeOnFinger Test,%s,%d ms,NoFinger,WithFinger,Gain,Delta", "Pass", DutResults->_TopSCM_wofResults.m_elapsedtime);
			fprintf(pFile, "\n,,,%d,%d,%d,%d\n", DutResults->_TopSCM_wofResults.m_nTriggerWithoutStim,
					DutResults->_TopSCM_wofResults.m_nTriggerWithStim,
					DutResults->_TopSCM_wofResults.m_nGain,
					DutResults->_TopSCM_wofResults.m_nTriggerWithoutStim - DutResults->_TopSCM_wofResults.m_nTriggerWithStim);
		}
		else	//Failed.
		{
			fprintf(pFile, "\nSCM Top WakeOnFinger Test,%s,%d ms,NoFinger,WithFinger,Gain,Delta", "Fail", DutResults->_TopSCM_wofResults.m_elapsedtime);
			fprintf(pFile, "\n,,,%d,%d,%d,%d\n", DutResults->_TopSCM_wofResults.m_nTriggerWithoutStim,
					DutResults->_TopSCM_wofResults.m_nTriggerWithStim,
					DutResults->_TopSCM_wofResults.m_nGain,
					DutResults->_TopSCM_wofResults.m_nTriggerWithoutStim - DutResults->_TopSCM_wofResults.m_nTriggerWithStim);
		}
	}

	//Pegged Pixels Test
	if (DutInfo->_peggedPixelsInfo.m_bExecuted)
	{
		fprintf(pFile, "\nPegged Pixels Test,%s,%.0f ms,Rows,", DutResults->_peggedPixelsResults.m_bPass ? "Pass" : "Fail", DutResults->_peggedPixelsResults.m_elapsedtime);
		for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithoutStim + DutInfo->_initInfo.m_nTrimBotWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_peggedPixelsResults.pegged_pixel_rows[i]);

		fprintf(pFile, "\n,,,Columns,");
		for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_peggedPixelsResults.pegged_pixel_cols[i]);
		fprintf(pFile, "\n");
	}

	//Floored Pixels Test
	if (DutInfo->_flooredPixelsInfo.m_bExecuted)
	{
		fprintf(pFile, "\nFloored Pixels Test,%s,%.0f ms,Rows,", DutResults->_flooredPixelsResults.m_bPass ? "Pass" : "Fail", DutResults->_flooredPixelsResults.m_elapsedtime);
		for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithoutStim + DutInfo->_initInfo.m_nTrimBotWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_flooredPixelsResults.floored_pixel_rows[i]);

		fprintf(pFile, "\n,,,Columns,");
		for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_flooredPixelsResults.floored_pixel_cols[i]);
		fprintf(pFile, "\n");
	}

	//DRdy Test
	if (DutInfo->_DRdyInfo.m_bExecuted)
	{
		fprintf(pFile, "\nDRdy Test,%s,%.0f ms\n", DutResults->_DRdyResults.m_bPass ? "Pass" : "Fail", DutResults->_DRdyResults.m_elapsedtime);
	}

	//Consecutive Pixels Test
	if (DutInfo->_consecutivePixelsInfo.m_bExecuted)
	{
		fprintf(pFile, "\nConsecutive Pixels Test,%s,%.0f ms,Pegged Rows,", DutResults->_consecutivePixelsResults.m_bPass ? "Pass" : "Fail", DutResults->_consecutivePixelsResults.m_elapsedtime);
		for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithoutStim + DutInfo->_initInfo.m_nTrimBotWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_consecutivePixelsResults.consecutive_pegged_rows[i]);

		fprintf(pFile, "\n,,,Floored Rows,");
		for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithoutStim + DutInfo->_initInfo.m_nTrimBotWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_consecutivePixelsResults.consecutive_floored_rows[i]);

		fprintf(pFile, "\n,,,Pegged Columns,");
		for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_consecutivePixelsResults.consecutive_pegged_cols[i]);

		fprintf(pFile, "\n,,,Floored Columns,");
		for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_consecutivePixelsResults.consecutive_floored_cols[i]);
		fprintf(pFile, "\n");
	}

	//Current Test
	if (DutInfo->_currentInfo.m_bExecuted)
	{
		fprintf(pFile, "\nCurrent Test,%s,%.0f ms,", DutResults->_currentResults.bPass ? "Pass" : "Fail", DutResults->_currentResults.m_elapsedtime);
		//If the test was successful.
		if (DutResults->_currentResults.bPass != 0)
		{
			fprintf(pFile, "Digital image acq current (mA),%.3f\n", (float)(DutResults->_currentResults.m_nImageAcqDigCurrent_uA) / 1000);
			fprintf(pFile, ",,,Analog image acq current (mA),%.3f\n", (float)(DutResults->_currentResults.m_nImageAcqAnaCurrent_uA) / 1000);
		}
		else
		{
			fprintf(pFile, "Digital image acq current (mA),%.3f\n", (float)(DutResults->_currentResults.m_nImageAcqDigCurrent_uA) / 1000);
			fprintf(pFile, ",,,Analog image acq current (mA),%.3f\n", (float)(DutResults->_currentResults.m_nImageAcqAnaCurrent_uA) / 1000);
		}
	}

	//Retain Mode Test
	if (DutInfo->_retainModeInfo.m_bExecuted)
	{
		fprintf(pFile, "\nRetain Mode Test,%s,%0.f ms,ADC Value,", DutResults->_retainModeResults.m_bPass ? "Pass" : "Fail", DutResults->_retainModeResults.m_elapsedtime);
		fprintf(pFile, "\n,,,%.3f", DutResults->_retainModeResults.m_nRetainModeCurrent);
		fprintf(pFile, "\n");
	}

	//SNR Test
	if (DutInfo->_snrInfo.m_bExecuted)
	{
		fprintf(pFile, "\nSNR Test,%s,%.0f ms,", DutResults->_snrResults.bPass ? "Pass" : "Fail", DutResults->_snrResults.m_elapsedtime);
		fprintf(pFile, "Signal_Z1,Noise_Z1,SNR_Z1,Signal_Z2,Noise_Z2,SNR_Z2,Signal_Z3,Noise_Z3,SNR_Z3,Signal_Z4,Noise_Z4,SNR_Z4,Signal_Z5,Noise_Z5,SNR_Z5,Signal_Z6,Noise_Z6,SNR_Z6,Signal_OVERALL,Noise_OVERALL,SNR_OVERALL\n");
		fprintf(pFile, ",,,");
		for (int i = 0; i<REGIONS; i++)
			fprintf(pFile, "%d,%f,%f,", DutResults->_snrResults.SIGNAL[i], DutResults->_snrResults.NOISE[i], DutResults->_snrResults.SNR[i]);
		fprintf(pFile, "\n");
	}

	//Pixel Uniformity Test
	if (DutInfo->_pixelInfo.m_bExecuted)
	{
		fprintf(pFile, "\nPixel Uniformity Test,%s,%.0f ms,", DutResults->_pixelResults.bPass ? "Pass" : "Fail", DutResults->_pixelResults.m_elapsedtime);
		fprintf(pFile, "Minimum Pixel,Maximum Pixel,Failing Pixel Count\n");
		fprintf(pFile, ",,,%d,%d,%d,", DutResults->_pixelResults.nMinPixelValue, DutResults->_pixelResults.nMaxPixelValue, DutResults->_pixelResults.nCountAboveThreshold);
		fprintf(pFile, "\n");
	}

	//Sharpness Test
	if (DutInfo->_SharpnessInfo.m_bExecuted)
	{
		fprintf(pFile, "\nSharpness Test,%s,%.0f ms\n", DutResults->_SharpnessResults.bPass ? "Pass" : "Fail", DutResults->_SharpnessResults.m_elapsedtime);
		fprintf(pFile, ",,,Variation(%%),Zone1,Zone2,Zone3,Overall\n");
		fprintf(pFile, ",,,%f,%d,%d,%d,%d", DutResults->_SharpnessResults.percent, (int)DutResults->_SharpnessResults.SHARPNESS[0], (int)DutResults->_SharpnessResults.SHARPNESS[1], (int)DutResults->_SharpnessResults.SHARPNESS[2], (int)DutResults->_SharpnessResults.SHARPNESS[3]);
		fprintf(pFile, "\n");
	}

	//RxStandardDev Test
	if (DutInfo->_RxStandardDevInfo.m_bExecuted)
	{
		fprintf(pFile, "\nRxStandardDev Test,%s,%.0f ms", DutResults->_RxStandardDevResults.m_bPass ? "Pass" : "Fail", DutResults->_RxStandardDevResults.m_elapsedtime);
		fprintf(pFile, "\n,,,Percent:,");
		for (int i = 0; i<(RowNumber - DutInfo->_initInfo.m_nTrimTopWithStim - DutInfo->_initInfo.m_nTrimBotWithStim); i++)
			fprintf(pFile, "%f,", DutResults->_RxStandardDevResults.percent[i]);
		fprintf(pFile, "\n");
	}

	//Imperfections Test
	if (DutInfo->_imperfectionsTestInfo.m_bExecuted)
	{
		fprintf(pFile, "\nImperfections Test,%s,%.0f ms,Along Rows,", DutResults->_imperfectionsTestResults.m_bPass ? "Pass" : "Fail", DutResults->_imperfectionsTestResults.m_elapsedtime);
		for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithStim + DutInfo->_initInfo.m_nTrimBotWithStim); i++)
			fprintf(pFile, "%d,", DutResults->_imperfectionsTestResults.consecutive_pegged_rows[i]);
		fprintf(pFile, "\n,,,Along Columns,");
		for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
			fprintf(pFile, "%d,", DutResults->_imperfectionsTestResults.consecutive_pegged_cols[i]);
		fprintf(pFile, "\n");
	}

	//Average No Finger & Average Finger
	//int numFrames = 30;
	//float temp = 0.0;
	fprintf(pFile, "\nAverage No Finger\n");
	for (int i = 0; i< DutResults->_acqImgNoFingerResult.iRealRowNumber; i++)
	{
		for (int j = 0; j< DutResults->_acqImgNoFingerResult.iRealColumnNumber; j++)
		{
			fprintf(pFile, "%d,", (int)DutResults->_acqImgNoFingerResult.arr_ImageFPSFrame.arr[i][j]);
		}
		fprintf(pFile, "\n");
	}
	//temp = 0.0;
	fprintf(pFile, "\nNormalized Finger\n");
	for (int i = 0; i< DutResults->_acqImgFingerResult.iRealRowNumber; i++)
	{
		for (int j = 0; j< DutResults->_acqImgFingerResult.iRealColumnNumber; j++)
		{
			fprintf(pFile, "%d,", (int)DutResults->_acqImgFingerResult.arr_ImageFPSFrame.arr[i][j]);
		}
		fprintf(pFile, "\n");
	}

	fprintf(pFile, "\n,Bin Codes");
	for (size_t i = 1; i <= DutResults->_binCodes.size(); i++)
	{
		fprintf(pFile, ",%s", (DutResults->_binCodes[i - 1]).c_str());
	}

	fclose(pFile);

	return true;
}

//bool Syn_Site::RegisterLoggingConfig()
//{
//	el::Configurations defaultConf;
//	//defaultConf.setToDefault();
//	const time_t t = time(NULL);
//	struct tm* current_time = localtime(&t);
//	std::string strMonthValue = to_string(current_time->tm_mon + 1);
//	if (1 == strMonthValue.length())
//		strMonthValue = "0" + strMonthValue;
//	std::string strTimeValue = to_string(current_time->tm_year + 1900) + "_" + strMonthValue + "_" + to_string(current_time->tm_mday);
//	std::string strLogFilePath = std::string(".\\logs\\") + strTimeValue + std::string(".log");
//	el::Configuration confFilenameInfo(el::Level::Global, el::ConfigurationType::Filename, strLogFilePath);
//	defaultConf.set(&confFilenameInfo);
//	defaultConf.parseFromText("*GLOBAL:\n FORMAT = [%datetime] [%thread] [%level] [%func] %msg");
//	el::Loggers::reconfigureLogger("default", defaultConf);

//	return true;
//}
