#pragma once

//Reference inc
extern "C" {
#include "SYN_TestUtils.h"
};

//std C
#include "stdint.h"

//std
#include <iostream>
#include <string>
#include <vector>
//#include <unordered_map>
//#include <map>
using namespace std;

struct Syn_TestStepInfo
{
	string _strNodeName;

	string _strTestStepName;
	string _strTestStepArgs;

	uint8_t _uiScriptID;
};

struct Syn_TestScript
{
	string _strScriptName;

	vector<Syn_TestStepInfo> _listOfTestStep;
};

struct Syn_PatchInfo
{
	string    _strXepatchName;
	string    _strXepatchFileName;

	uint8_t   *_pArrayBuf;
	uint16_t  _uiArraySize;
};

struct Syn_SysConfig 
{
	Syn_SysConfig(){}
	~Syn_SysConfig()
	{
		for (size_t i = _listPatchInfo.size(); i >= 1; i--)
		{
			Syn_PatchInfo CurrentXepatchInfo = _listPatchInfo[i - 1];
			if (NULL != CurrentXepatchInfo._pArrayBuf && 0 != CurrentXepatchInfo._uiArraySize)
			{
				delete[] CurrentXepatchInfo._pArrayBuf;
				CurrentXepatchInfo._pArrayBuf = NULL;
			}

		}
	}

	const Syn_SysConfig& Syn_SysConfig::operator=(Syn_SysConfig& original)
	{
		_strAutoController = original._strAutoController;
		_strDutType = original._strDutType;
		_strDutController = original._strDutController;
		_strDutCom = original._strDutCom;

		_uiDutpwrVdd_mV = original._uiDutpwrVdd_mV;
		_uiDutpwrVio_mV = original._uiDutpwrVio_mV;
		_uiDutpwrVled_mV = original._uiDutpwrVled_mV;
		_uiDutpwrVddh_mV = original._uiDutpwrVddh_mV;

		_uiNumRows = original._uiNumRows;
		_uiNumCols = original._uiNumCols;

		_bWriteBootSectors = original._bWriteBootSectors;

		for (int i = 0; i < BS0_SIZE; i++)
		{
			_arrUserSpecifiedBS0[i] = original._arrUserSpecifiedBS0[i];
		}
		for (int i = 0; i < BS1_SIZE; i++)
		{
			_arrUserSpecifiedBS1[i] = original._arrUserSpecifiedBS1[i];
		}

		_listTestSteps.clear();
		for (size_t i = 1; i <= original._listTestSteps.size(); i++)
		{
			Syn_TestStepInfo CurrentSyn_TestStepInfo;
			CurrentSyn_TestStepInfo._strNodeName = (original._listTestSteps)[i - 1]._strNodeName;
			CurrentSyn_TestStepInfo._strTestStepName = (original._listTestSteps)[i - 1]._strTestStepName;
			CurrentSyn_TestStepInfo._strTestStepArgs = (original._listTestSteps)[i - 1]._strTestStepArgs;

			_listTestSteps.push_back(CurrentSyn_TestStepInfo);
		}
		/*_listTestScript.clear();
		for (size_t i = 1; i <= original._listTestScript.size(); i++)
		{
			Syn_TestScript CurrentTestScriptInfo;
			CurrentTestScriptInfo._strScriptName = (original._listTestScript)[i - 1]._strScriptName;
			for (size_t j = 1; j <= (original._listTestScript)[i - 1]._listOfTestStep.size(); j++)
			{
				Syn_TestStepInfo CurrentSyn_TestStepInfo;
				CurrentSyn_TestStepInfo._strNodeName = ((original._listTestScript)[i - 1])._listOfTestStep[j - 1]._strNodeName;
				CurrentSyn_TestStepInfo._strTestStepName = ((original._listTestScript)[i - 1])._listOfTestStep[j - 1]._strTestStepName;
				CurrentSyn_TestStepInfo._strTestStepArgs = ((original._listTestScript)[i - 1])._listOfTestStep[j - 1]._strTestStepArgs;

				CurrentTestScriptInfo._listOfTestStep.push_back(CurrentSyn_TestStepInfo);
			}

			_listTestScript.push_back(CurrentTestScriptInfo);
		}*/

		_listPatchInfo.clear();
		for (size_t i = 1; i <= original._listPatchInfo.size(); i++)
		{
			Syn_PatchInfo CurrentSyn_PatchInfo;
			CurrentSyn_PatchInfo._strXepatchFileName = (original._listPatchInfo)[i - 1]._strXepatchFileName;
			CurrentSyn_PatchInfo._strXepatchName = (original._listPatchInfo)[i - 1]._strXepatchName;
			CurrentSyn_PatchInfo._uiArraySize = (original._listPatchInfo)[i - 1]._uiArraySize;

			CurrentSyn_PatchInfo._pArrayBuf = new uint8_t[CurrentSyn_PatchInfo._uiArraySize];
			for (int j = 0; j < CurrentSyn_PatchInfo._uiArraySize; j++)
			{
				(CurrentSyn_PatchInfo._pArrayBuf)[j] = ((original._listPatchInfo)[i - 1]._pArrayBuf)[j];
			}

			_listPatchInfo.push_back(CurrentSyn_PatchInfo);
		}

		return *this;
	}

	//function
	bool GetSyn_XepatchInfo(const std::string &strXepatchName, Syn_PatchInfo &oSyn_XepatchInfo)
	{
		bool IsExists(false);
		for (size_t i = 1; i <= _listPatchInfo.size(); i++)
		{
			if (strXepatchName == _listPatchInfo[i - 1]._strXepatchName)
			{
				oSyn_XepatchInfo = _listPatchInfo[i - 1];
				IsExists = true;
				break;
			}
		}

		return IsExists;
	}

	bool GetSyn_TestStepInfo(const std::string &strTestStepName, std::string &ostrArgsValue)
	{
		bool IsExists(false);
		for (size_t i = 1; i <= _listTestSteps.size(); i++)
		{
			if (strTestStepName == _listTestSteps[i - 1]._strTestStepName)
			{
				ostrArgsValue = _listTestSteps[i - 1]._strTestStepArgs;
				IsExists = true;
				break;
			}
		}

		/*for (size_t i = 1; i <= _listTestScript.size(); i++)
		{
			for (size_t j = 1; j <= (_listTestScript[i - 1]._listOfTestStep).size(); j++)
			{
				if (strTestStepName == (_listTestScript[i - 1])._listOfTestStep[j - 1]._strTestStepName)
				{
					ostrArgsValue = (_listTestScript[i - 1])._listOfTestStep[j - 1]._strTestStepArgs;
					IsExists = true;
					return IsExists;
				}
			}
		}*/

		return IsExists;
	}

	//variables
	string			_strAutoController;//AutoController
	string			_strDutType;//DutType
	string			_strDutController;//DutController
	string			_strDutCom;//DutCom

	uint16_t		_uiDutpwrVdd_mV;//DutpwrVdd_mV
	uint16_t		_uiDutpwrVio_mV;//DutpwrVio_mV
	uint16_t		_uiDutpwrVled_mV;//DutpwrVled_mV
	uint16_t		_uiDutpwrVddh_mV;//DutpwrVddh_mV

	uint16_t		_uiNumRows;//NumRows
	uint16_t		_uiNumCols;//NumCols

	bool			_bWriteBootSectors;//WriteBootSectors
	uint8_t			_arrUserSpecifiedBS0[BS0_SIZE];//BootSector0
	uint8_t			_arrUserSpecifiedBS1[BS1_SIZE];//BootSector1

	//vector<Syn_TestScript>		_listTestScript;//TestSeq
	vector<Syn_TestStepInfo>	_listTestSteps;
	
	vector<Syn_PatchInfo>		_listPatchInfo;//All patch
	
};

//struct SiteSettings
//{
//	uint32_t		_uiDutSerNum;
//	AdcBaseLineInfo	_adcBaseLineInfo;//SYN_TestUtils
//};
//
//struct Syn_LocalSettings 
//{
//	string					_strSysConfigFilePath;
//
//	vector<SiteSettings>	_listOfSiteSettings;
//
//	//?
//	bool					m_bRunRepeatedly;
//	int						m_nNumRepetitions;
//	bool					m_bLGAMode;
//	bool					m_bQAMode;
//	bool					m_bVerboseMode;
//	bool					m_bManualControl;
//
//	string					_strAutoController;
//};