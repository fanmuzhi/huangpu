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

	
}

void Ts_HuaweiImageQualityTest::Execute()
{
	Syn_Exception ex(0);

	int iRowNumber = _pSyn_Dut->_RowNumber;
	int iColumnNumber = _pSyn_Dut->_ColumnNumber - HEADER;

	uint8_t *arrImageNoFinger = new uint8_t[iRowNumber*iColumnNumber * 30];
	int k = 0;
	for (int i = 0; i < 30; i++)
	{
		for (int m = 0; m < iRowNumber; m++)
		{
			for (int n = HEADER; n < iColumnNumber + HEADER; n++)
			{
				arrImageNoFinger[k] = (_pSyn_Dut->_pSyn_DutTestResult->_acqImgNoFingerResult).arr_nofinger[i].arr[m][n];
				k++;
			}
		}
	}

	uint8_t *arrImageFinger = new uint8_t[iRowNumber*iColumnNumber * 30];
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

	float snrValue(0);
	try
	{
		snrValue = testSNR(arrImageNoFinger, arrImageFinger, iColumnNumber, iRowNumber, 30);
	}
	catch (...)
	{
		ex.SetError(Syn_ExceptionCode::Syn_SystemError);
		ex.SetDescription("testSNR is failed!");
		throw ex;
		return;
	}

	delete[] arrImageNoFinger;
	arrImageNoFinger = NULL;

	delete[] arrImageFinger;
	arrImageFinger = NULL;

	//std::cout << "snrValue is " << snrValue << std::endl;
}

void Ts_HuaweiImageQualityTest::ProcessData()
{

}

void Ts_HuaweiImageQualityTest::CleanUp()
{

}