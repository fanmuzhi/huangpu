#include "Ts_WOFCheck.h"

Ts_WOFCheck::Ts_WOFCheck(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
, bPass(true)
{
}

Ts_WOFCheck::~Ts_WOFCheck()
{
}

void Ts_WOFCheck::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_wofCheckInfo.m_bExecuted = false;
	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize >= 1)
	{
		_pSyn_Dut->_pSyn_DutTestInfo->_wofCheckInfo.checkLimit = atoi(listOfArgValue[0].c_str());
	}

	//Power On
	PowerOff();
	PowerOn(_pSyn_Dut->_uiDutpwrVdd_mV, _pSyn_Dut->_uiDutpwrVio_mV, _pSyn_Dut->_uiDutpwrVled_mV, _pSyn_Dut->_uiDutpwrVddh_mV, true);
	_pSyn_DutCtrl->FpUnloadPatch();

	//load OTPReadWritePatch
	Syn_PatchInfo OTPRWPatchInfo;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OTPRWPatchInfo) || NULL == OTPRWPatchInfo._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OtpReadWritePatch Patch is NULL!");
		throw ex;
		return;
	}
	_pSyn_DutCtrl->FpLoadPatch(OTPRWPatchInfo._pArrayBuf, OTPRWPatchInfo._uiArraySize);
}

void Ts_WOFCheck::Execute()
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

	uint8_t pDst[MS0_SIZE];

	int count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_WOF_FD_ZONE0, pDst, MS0_SIZE);
	_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_nWofZONE0_FD_count = count;
	_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdOtpValue = pDst[4];
	_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdOtpGain = pDst[6];

	_pSyn_Dut->_pSyn_DutTestInfo->_wofCheckInfo.m_bExecuted = true;
}

void Ts_WOFCheck::ProcessData()
{
	if (!_pSyn_Dut->_pSyn_DutTestInfo->_wofCheckInfo.m_bExecuted)
	{
		Syn_Exception ex(Syn_ExceptionCode::Syn_TestStepNotExecuted);
		ex.SetDescription("TestStep WOF_CHECK is not executed!");
		throw ex;
	}


	//calc wof no-stimulus data

	bPass = false;
	uint8_t numGains = _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_nNumGains;
	uint8_t nGainStart = _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_nGainStart;
	uint8_t nGainInc = _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_nGainInc;
	uint8_t numThresholds = _pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_nNumThresholds;

	int nTgrIndex;
	for (uint8_t gainIndex = 0; gainIndex < numGains; gainIndex++)
	{

		_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdCurrentGain = nGainStart + (nGainInc * gainIndex);
		//bool rc1 = CalcWofTriggerIdx(_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_nNumThresholds, 
		//	&_pSyn_Dut->_pSyn_DutTestResult->_z0FDWofResults.m_arDataWithoutStim[6 + (gainIndex * numThresholds)], nTgrIndex);
		//_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdCurrentValue = nTgrIndex;
		if (_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdCurrentGain  == _pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdOtpGain)
		{
			_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_m_nWofFdValue = _pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdCurrentValue[gainIndex];
			_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nDelta = abs(_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdCurrentValue[gainIndex]- 
				_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nWofFdOtpValue);

			if (_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_nDelta >= _pSyn_Dut->_pSyn_DutTestInfo->_wofCheckInfo.checkLimit)
			{
				bPass = false;
			}
			else
			{
				bPass = true;
				break;
			}
		}
	}
	if (!bPass)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sWofTestFail);
		_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_bPass = false;
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_Check", "Fail"));
		return;
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_wofCheckResults.m_bPass = true;
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("WOF_Check", "Pass"));
	}

	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_calibrationResults.m_elapsedtime);
}

void Ts_WOFCheck::CleanUp()
{
}

bool Ts_WOFCheck::CalcWofTriggerIdx(int nNumThresholds, uint8_t* pTriggerBuf, int &oTgrIdx)
{
	bool bFound(false);

	//Find the first occurence of 1.
	oTgrIdx = nNumThresholds;
	int iExpected = 0;
	for (int i = 0; i<nNumThresholds; i++)
	{
		if (((pTriggerBuf[i] & 0x01) != iExpected) && (oTgrIdx == nNumThresholds))
		{
			oTgrIdx = i;
			bFound = true;
			break;
		}
	}

	return bFound;
}
