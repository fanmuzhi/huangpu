#pragma once

//Local
#include "Syn_Module.h"

//DutUtls
#include "Syn_Dut.h"
#include "Syn_DutCtrl.h"
#include "Syn_Config.h"

//C++
#include <string>
#include <vector>
using namespace std;

extern "C" {
#include "SYN_TestUtils.h"
};

class Syn_TestStep
{
public:
	
	Syn_TestStep(string &strName, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
		:_strName(strName), _pSyn_DutCtrl(pDutCtrl), _pSyn_Dut(pDut)
	{
	}

	virtual ~Syn_TestStep()
	{
	}

	string GetName()
	{
		return _strName; 
	}

	//pure virtual function
	virtual int	SetUp() = 0;

	virtual int	Excute() = 0;

	virtual int ProcessData() = 0;

	virtual int	CleanUp() = 0;

protected:

	string _strName;

	Syn_DutCtrl *_pSyn_DutCtrl;
	
	Syn_Dut *_pSyn_Dut;
};

