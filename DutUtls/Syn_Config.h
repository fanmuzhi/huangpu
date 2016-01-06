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

	/*Syn_SysConfig& Syn_SysConfig::operator=(Syn_SysConfig& original)
	{
		return *this;
	}*/

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
	AdcBaseLineInfo	_adcBaseLineInfo;
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