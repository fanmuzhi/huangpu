#include "Ts_ReadDutAdc.h"

/*static uint8_t	m_arSetupCmds[][kSetupCmdLength] = {
	0x20, 0x20, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x08,
	0x24, 0x20, 0x00, 0x80, 0x08, 0x00, 0x00, 0x00, 0x08,
	0x38, 0x20, 0x00, 0x80, 0x00, 0x0C, 0x00, 0x00, 0x08,
	0x28, 0x20, 0x00, 0x80, 0x0F, 0x08, 0x00, 0x00, 0x08,
	0x2C, 0x20, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x00, 0x28, 0x00, 0x80, 0x06, 0x00, 0x00, 0x00, 0x08,
	0x14, 0x28, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x08, 0x28, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x14, 0x28, 0x00, 0x80, 0x31, 0x00, 0x00, 0x00, 0x08,
	0x24, 0x20, 0x00, 0x80, 0x09, 0x00, 0x00, 0x00, 0x08 };*/

Ts_ReadDutAdc::Ts_ReadDutAdc(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_ReadDutAdc::~Ts_ReadDutAdc()
{
}

void Ts_ReadDutAdc::SetUp()
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

	_pSyn_Dut->_pSyn_DutTestInfo->_ReadDutAdcInfo.m_bExecuted = false;

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

void Ts_ReadDutAdc::Execute()
{
	_pSyn_Dut->_pSyn_DutTestInfo->_ReadDutAdcInfo.m_bExecuted = true;

	uint8_t pDst[MS0_SIZE] = { 0 };
	int count = _pSyn_DutCtrl->FpOtpRomTagRead(EXT_TAG_DutTempAdc, pDst, MS0_SIZE);
	if (0 == count)
	//if (count)
	{
		int nTotal = 0;
		int nNumAdcReadings = 3;
		ReadDutAdcSetup();
		for (int i = 0; i<nNumAdcReadings; i++)
			nTotal += ReadDutAdc();
		_pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_nDutAdc = nTotal / nNumAdcReadings;

		//SYN_ReadDutAdcStepExecute(&(GetSite().m_ReadDutAdcInfo), (&GetSite().m_ReadDutAdcResults));
		_pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_bPass = 1;
	}
	else
	{
		//Use Dut temperature ADC stored in OTP.
		//if (GetMS0RecordData(TAG_CAL, EXT_TAG_DutTempAdc, pOtpData, MS0_SIZE, site) != 0)
		if (0 != count)
			_pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_nDutAdc = *((uint16_t*)pDst);
		_pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_bPass = 1;
	}
}

void Ts_ReadDutAdc::ProcessData()
{
	if (0 == _pSyn_Dut->_pSyn_DutTestResult->_ReadDutAdcResults.m_bPass)
	{
		//_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_adc);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("ReadDutAdc", "Fail"));
	}
	else
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("ReadDutAdc", "Pass"));
}

void Ts_ReadDutAdc::CleanUp()
{
	PowerOff();
}

void Ts_ReadDutAdc::ReadDutAdcSetup()
{
	/*uint8_t	pDst[10];

	//Setup for ADC read.
	for (int i = 0; i<(sizeof(m_arSetupCmds) / kSetupCmdLength); i++)
	{
		_pSyn_DutCtrl->FpGetStatus(pDst, 4);
		_pSyn_DutCtrl->FpWrite(1, 0x0008, &m_arSetupCmds[i][0], kSetupCmdLength);
		_pSyn_DutCtrl->FpRead(1, 0xFF, pDst, 2);
		_pSyn_DutCtrl->FpGetStatus(pDst, 4);
	}*/

	_pSyn_DutCtrl->FpPokeRegister(0x80002020, 0x01);
	_pSyn_DutCtrl->FpPokeRegister(0x80002024, 0x08);
	_pSyn_DutCtrl->FpPokeRegister(0x80002038, 0x0C00);
	_pSyn_DutCtrl->FpPokeRegister(0x80002028, 0x080F);
	_pSyn_DutCtrl->FpPokeRegister(0x8000202C, 0x00);
	_pSyn_DutCtrl->FpPokeRegister(0x80002800, 0x06);
	_pSyn_DutCtrl->FpPokeRegister(0x80002814, 0x00);
	_pSyn_DutCtrl->FpPokeRegister(0x80002808, 0x00);
	_pSyn_DutCtrl->FpPokeRegister(0x80002814, 0x31);
	_pSyn_DutCtrl->FpPokeRegister(0x80002024, 0x09);
}

int Ts_ReadDutAdc::ReadDutAdc()
{
	/*uint8_t arPoke[] = { 0x24, 0x20, 0x00, 0x80, 0x09, 0x00, 0x00, 0x00, 0x08 };
	uint8_t arPeek[] = { 0x24, 0x20, 0x00, 0x80, 0x04 };
	uint8_t	pDst[10];
	int nAdcReading = 0;

	_pSyn_DutCtrl->FpGetStatus(pDst, 4);
	//::Sleep(1);
	_pSyn_DutCtrl->FpWrite(1, 0x0007, arPeek, sizeof(arPeek));
	_pSyn_DutCtrl->FpRead(1, 0xFF, pDst, 6);
	nAdcReading = ((uint16_t)pDst[5] << 8) + pDst[4];	//Swap.
	_pSyn_DutCtrl->FpGetStatus(pDst, 4);
	_pSyn_DutCtrl->FpGetStatus(pDst, 4);
	_pSyn_DutCtrl->FpWrite(1, 0x0008, arPoke, sizeof(arPoke));
	_pSyn_DutCtrl->FpRead(1, 0xFF, pDst, 2);
	_pSyn_DutCtrl->FpGetStatus(pDst, 4);*/


	uint8_t	pDst[6];
	int nAdcReading = 0;
	_pSyn_DutCtrl->FpPeekRegister(0x80002024, pDst);
	nAdcReading = ((uint16_t)pDst[5] << 8) + pDst[4];	//Swap.

	_pSyn_DutCtrl->FpPokeRegister(0x80002024, 0x09);

	return nAdcReading;
}
