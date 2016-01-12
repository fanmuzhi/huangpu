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
#include <unordered_map>
#include <map>
using namespace std;

struct Syn_TestStepInfo
{
	string _strNodeName;

	string _strTestStepName;
	string _strTestStepArgs;
};

struct Syn_XepatchInfo
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
		for (size_t i = _listXepatchInfo.size(); i >= 1; i--)
		{
			Syn_XepatchInfo CurrentXepatchInfo = _listXepatchInfo[i-1];
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
		_listXepatchInfo.clear();
		for (size_t i = 1; i <= original._listXepatchInfo.size(); i++)
		{
			Syn_XepatchInfo CurrentSyn_XepatchInfo;
			CurrentSyn_XepatchInfo._strXepatchFileName = (original._listXepatchInfo)[i - 1]._strXepatchFileName;
			CurrentSyn_XepatchInfo._strXepatchName = (original._listXepatchInfo)[i - 1]._strXepatchName;
			CurrentSyn_XepatchInfo._uiArraySize = (original._listXepatchInfo)[i - 1]._uiArraySize;

			CurrentSyn_XepatchInfo._pArrayBuf = new uint8_t[CurrentSyn_XepatchInfo._uiArraySize];
			for (int j = 0; j < CurrentSyn_XepatchInfo._uiArraySize; j++)
			{
				(CurrentSyn_XepatchInfo._pArrayBuf)[j] = ((original._listXepatchInfo)[i - 1]._pArrayBuf)[j];
			}

			_listXepatchInfo.push_back(CurrentSyn_XepatchInfo);
		}

		return *this;
	}

	//function
	bool GetSyn_XepatchInfo(const std::string &strXepatchName, Syn_XepatchInfo &oSyn_XepatchInfo)
	{
		bool IsExists(false);
		for (size_t i = 1; i <= _listXepatchInfo.size(); i++)
		{
			if (strXepatchName == _listXepatchInfo[i - 1]._strXepatchName)
			{
				oSyn_XepatchInfo = _listXepatchInfo[i-1];
				IsExists = true;
				break;
			}
		}

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

	vector<Syn_TestStepInfo> _listTestSteps;//TestSeq
	
	vector<Syn_XepatchInfo>	 _listXepatchInfo;//All patch
	
};

struct SiteSettings
{
	uint32_t		_uiDutSerNum;
	AdcBaseLineInfo	_adcBaseLineInfo;//SYN_TestUtils
};

struct Syn_LocalSettings 
{
	string					_strSysConfigFilePath;

	vector<SiteSettings>	_listOfSiteSettings;

	//?
	bool					m_bRunRepeatedly;
	int						m_nNumRepetitions;
	bool					m_bLGAMode;
	bool					m_bQAMode;
	bool					m_bVerboseMode;
	bool					m_bManualControl;

	string					_strAutoController;
};