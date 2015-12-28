#pragma once

//std C
#include "stdint.h"

//std
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

typedef struct Syn_SysConfig 
{
	uint16_t		m_pwrVdd_mV;
	uint16_t		m_pwrVio_mV;
	uint16_t		m_pwrVled_mV;
	uint16_t		m_pwrVddh_mV;
	string			m_sSysConfigFile;
	string			m_sAutoController;
	string			m_sAutoCtrlCom;
	string			m_sDutType;
	string			m_sDutController;
	string			m_sDutCom;
	unordered_map<string, string> Syn_UMap_TestStep;
	vector<string>	m_arTestSteps;
	vector<string>	m_arTestArgs;
	bool			m_bWriteBootSectors;
	uint8_t			m_arUserSpecifiedBS0[BS0_SIZE];
	uint8_t			m_arUserSpecifiedBS1[BS1_SIZE];

	uint16_t		m_numRows;
	uint16_t		m_numCols;
};

struct Syn_LocalSettings 
{

};