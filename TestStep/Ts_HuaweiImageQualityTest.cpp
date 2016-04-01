#include "Ts_HuaweiImageQualityTest.h"

Ts_HuaweiImageQualityTest::Ts_HuaweiImageQualityTest(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_HuaweiImageQualityTest::~Ts_HuaweiImageQualityTest()
{
}

void Ts_HuaweiImageQualityTest::SetUp()
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

	//int iRowNumber = _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.iRealRowNumber;
	int iRowNumber = _pSyn_Dut->_RowNumber;
	//int iColumnNumber = _pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult.iRealColumnNumber;
	int iColumnNumber = _pSyn_Dut->_ColumnNumber-HEADER;

	uint8_t *arrImageNoFinger = new uint8_t[iRowNumber*iColumnNumber * 30];
	int k = 0;
	//uint8_t arrImageNoFinger[112*112 * 30];
	for (int i = 0; i < 30; i++)
	{
		for (int m = 0; m < iRowNumber; m++)
		{
			for (int n = HEADER; n < iColumnNumber+HEADER; n++)
			{
				arrImageNoFinger[k] = (_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult).arr_nofinger[i].arr[m][n];
				k++;
			}
		}
	}

	uint8_t *arrImageFinger = new uint8_t[iRowNumber*iColumnNumber * 30];
	//uint8_t arrImageFinger[112*112 * 30];
	k = 0;
	for (int i = 0; i < 30; i++)
	{
		for (int m = 0; m < iRowNumber; m++)
		{
			for (int n = HEADER; n < iColumnNumber + HEADER; n++)
			{
				arrImageFinger[k] = (_pSyn_Dut->_pSyn_DutTestResult->_acqImgFingerResult).arr_finger[i].arr[m][n];
				k++;
			}
		}
	}

	/*typedef float(*lpHuaweiSNRTestFunction)(unsigned char* , unsigned char* , int , int , int );
	HINSTANCE hDll; 
	lpHuaweiSNRTestFunction snrTestFun;
	hDll = LoadLibrary(TEXT("snrtest.dll"));
	if (0 == hDll)
	{
		ex.SetError(Syn_ExceptionCode::Syn_ConfigError);
		ex.SetDescription("snrtest.dll is NULL!");
		throw ex;
		return;
	}

	snrTestFun = (lpHuaweiSNRTestFunction)GetProcAddress(hDll, "testSNR");
	if (!snrTestFun)
	{
		FreeLibrary(hDll);
		ex.SetError(Syn_ExceptionCode::Syn_ConfigError);
		ex.SetDescription("snrtest.dll is NULL!");
		throw ex;
		return;
	}*/

	float snrValue(0);
	try
	{
		snrValue = testSNR(arrImageNoFinger, arrImageFinger, iColumnNumber, iRowNumber, 30);
		//snrValue = snrTestFun(&arrImageNoFinger[0], &arrImageFinger[0], iColumnNumber, iRowNumber, 1);
	}
	catch (...)
	{
		ex.SetError(Syn_ExceptionCode::Syn_ConfigError);
		ex.SetDescription("testSNR is failed!");
		throw ex;
		return;
	}
	delete[] arrImageNoFinger;
	arrImageNoFinger = NULL;

	delete[] arrImageFinger;
	arrImageFinger = NULL;

	std::cout << "snrValue is " << snrValue << std::endl;

	//FreeLibrary(hDll);
}

void Ts_HuaweiImageQualityTest::Execute()
{

}

void Ts_HuaweiImageQualityTest::ProcessData()
{

}

void Ts_HuaweiImageQualityTest::CleanUp()
{

}