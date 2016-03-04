#include "Ts_SharpnessTest.h"

Ts_SharpnessTest::Ts_SharpnessTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}


Ts_SharpnessTest::~Ts_SharpnessTest()
{
}

void Ts_SharpnessTest::SetUp()
{
	Syn_Exception ex(0);
	if (NULL == _pSyn_DutCtrl)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutCtrlNull);
		ex.SetDescription("_pSyn_DutCtrl is NULL!");
		throw ex;
		return;
	}
	if (NULL == _pSyn_Dut)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutNull);
		ex.SetDescription("_pSyn_Dut is NULL!");
		throw ex;
		return;
	}

	//parse args
	std::vector<std::string> listOfArgValue;
	_pSyn_Dut->_pSyn_DutTestInfo->_SharpnessInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_SharpnessInfo.limit = 30;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize >= 1)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_SharpnessInfo.limit = atoi(listOfArgValue[0].c_str());
	}
}


void Ts_SharpnessTest::Execute()
{

}

void Ts_SharpnessTest::ProcessData()
{
	//int nTrimLeft = 0;
	//int nTrimRight = 0;
	//int nTrimTop = 0;
	//int nTrimBottom = 0;
	//int numRows = _pSyn_Dut->_RowNumber;
	//int numCols = _pSyn_Dut->_ColumnNumber;

	//int i, j;
	//int M = height;
	//int N = width;
	//int C0 = N / 3;
	//int C1 = C0 + C0;
	//int C2 = N;
	//int min, max;
	//float measure;

	//float zones[3] = { 0 };
	//float tempImg[MAXROW][MAXCOL];

	//pResults->bPass = 1;

	////overall
	//get_sharpness(M, N, pImg, &measure, pResults);
	//pResults->SHARPNESS[3] = measure;
	////zone 1
	//for (i = 0; i<M; i++)
	//{
	//	for (j = 0; j<C0; j++)
	//	{
	//		tempImg[i][j] = pImg[i][j];
	//	}
	//}
	//get_sharpness(M, (C0 + 1), tempImg, &measure, pResults);
	//pResults->SHARPNESS[0] = measure;
	////zone 2
	//for (i = 0; i<M; i++)
	//{
	//	for (j = C0; j<C1; j++)
	//	{
	//		tempImg[i][j - C0] = pImg[i][j];
	//	}
	//}
	//get_sharpness(M, (C1 - C0 + 1), tempImg, &measure, pResults);
	//pResults->SHARPNESS[1] = measure;
	////zone 3
	//for (i = 0; i<M; i++)
	//{
	//	for (j = C1; j<C2; j++)
	//	{
	//		tempImg[i][j - C1] = pImg[i][j];
	//	}
	//}
	//get_sharpness(M, (C2 - C1 + 1), tempImg, &measure, pResults);
	//pResults->SHARPNESS[2] = measure;

	//zones[0] = pResults->SHARPNESS[0];
	//zones[1] = pResults->SHARPNESS[1];
	//zones[2] = pResults->SHARPNESS[2];
	//min = find_min(zones, 3);
	//max = find_max(zones, 3);
	//pResults->percent = (float)abs((int)(((zones[max] - zones[min]) / pResults->SHARPNESS[3]) * 100));

	//if (pResults->percent > pInfo->limit)
	//	pResults->bPass = 0;

}

void Ts_SharpnessTest::CleanUp()
{
	PowerOff();
}