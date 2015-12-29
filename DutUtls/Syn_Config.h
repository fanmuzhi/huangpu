#pragma once

//Reference inc
#include "SYN_TestUtils.h"

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
	string _strTestStepName;
	string _strTestStepArgs;
};

struct Syn_XepatchInfo
{
	string    _strXepatchName;

	uint8_t   *_pArrayBuf;
	uint16_t  _uiArraySize;
};

struct Syn_SysConfig 
{	
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

struct Syn_LocalSettings 
{
	string     _strSysConfigFilePath;

	//NumSites

	/*vector<SiteSettings>		m_arSiteSettings;
	bool						m_bRunRepeatedly;
	int							m_nNumRepetitions;
	bool						m_bLGAMode;
	bool						m_bQAMode;
	bool						m_bVerboseMode;
	bool						m_bManualControl;
	CString						m_sAutoController;*/
};