#include "Ts_OTPWriteBootSector.h"

Ts_OTPWriteBootSector::Ts_OTPWriteBootSector(string &strName, string &strArgs, Syn_DutCtrl * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_OTPWriteBootSector::~Ts_OTPWriteBootSector()
{
}

void Ts_OTPWriteBootSector::SetUp()
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

	//Power on
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

void Ts_OTPWriteBootSector::Execute()
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

	uint8_t otpBs0[BS0_SIZE] = { 0 };
	uint8_t otpBs1[BS1_SIZE] = { 0 };
	//_pSyn_DutCtrl->FpOtpRomRead();

	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 0, otpBs0, BS0_SIZE);
	_pSyn_DutCtrl->FpOtpRomRead(BOOT_SEC, 1, otpBs1, BS1_SIZE);

	//Communication Option and flash bit
	int index = 51;
	int mask = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[index];
	if ((otpBs0[index] | mask) == mask)		//check if writable
	{
		otpBs0[index] = mask;
		otpBs0[index + 8] = mask;
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sOtpReadWriteFail);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteBootSector", "Fail"));
	}

	//FW secure
	index = 50;
	mask = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[index] & 0x40;
	if (mask)
	{
		otpBs0[index] |= 0x40;
		otpBs0[index + 8] |= 0x40;
	}

	//HW secure
	index = 1;
	mask = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[index] & 0x2;
	if (mask)
	{
		otpBs0[index] |= 0x2;
		otpBs0[index + 8] |= 0x2;
	}

	//Sleep Enable
	index = 49;
	mask = _pSyn_Dut->_pSyn_DutTestInfo->_otpCheckInfo._UserSpecifiedBS0[index] & 0x10;
	if (mask)
	{
		otpBs0[index] |= 0x10;
		otpBs0[index + 8] |= 0x10;
	}

	_pSyn_DutCtrl->FpOtpRomWrite(BOOT_SEC, 0, otpBs0, BS0_SIZE);

	//test
	uint8_t snArray[DUT_SER_NUM_SIZE] = { 0 };
	Create_SN(snArray, _pSyn_Dut->_DeviceSerialNumber, _pSyn_Dut->_iSiteNumber, 0);
	SerialNumReverseBitFields(snArray, snArray);

	return;
}

void Ts_OTPWriteBootSector::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("OTPWriteBootSector", "Pass"));
}

void Ts_OTPWriteBootSector::CleanUp()
{
	PowerOff();
}

void Ts_OTPWriteBootSector::Create_SN(uint8_t* SN, uint32_t nDutdSerNum, int nSiteNum, int nTestLocationId)
{
	int year, day, hour, min, sec, YearSec;
	unsigned long long int TimeInSecs;
	unsigned long long serial;
	time_t t = time(NULL);
	struct tm tm;

	//Get elapsed time (secs) since 1-1-2014.
	localtime_s(&tm, &t);
	year = (tm.tm_year + 1900) - 2014;
	day = tm.tm_yday + 1;
	hour = tm.tm_hour;
	min = tm.tm_min;
	sec = tm.tm_sec;
	YearSec = year * 365 * 24 * 3600;
	day = day * 24 * 3600;
	hour = hour * 3600;
	min = min * 60;
	TimeInSecs = YearSec + day + hour + min + sec;

	serial = 0x01;							//Denotes MT set the SN.
	serial = (serial << 1) | 0x00;			//Denotes ATE set the SN.
	serial = (serial << 4) | (nSiteNum & 0x0F);
	serial = (serial << 28) | (TimeInSecs & 0xFFFFFFF);
	serial = (serial << 4) | (nTestLocationId & 0x0F);
	serial = (serial << 10) | (nDutdSerNum & 0x3FF);

	SN[0] = (serial >> 40) & 0x00000000000000FF;
	SN[1] = (serial >> 32) & 0x00000000000000FF;
	SN[2] = (serial >> 24) & 0x00000000000000FF;
	SN[3] = (serial >> 16) & 0x00000000000000FF;
	SN[4] = (serial >> 8) & 0x00000000000000FF;
	SN[5] = serial & 0x00000000000000FF;
}

void Ts_OTPWriteBootSector::SerialNumReverseBitFields(uint8_t* pSrc, uint8_t* pDst)
{
	uint64_t	nTmp, nTmp2, nReversed;
	uint8_t		arRev[16] = { 0x00, 0x08, 0x04, 0x0C, 0x02, 0x0A, 0x06, 0x0E,
		0x01, 0x09, 0x05, 0x0D, 0x03, 0x0B, 0x07, 0x0F };

	//Copy the ATE (bit 46) and MT (bit 47) bits.
	nReversed = (pSrc[0] & 0x80) ? 0x800000000000L : 0;
	nReversed |= (pSrc[0] & 0x40) ? 0x400000000000L : 0;

	//Get the site number (bits 42-45) and reverse the bits.
	nReversed |= ((uint64_t)(arRev[((pSrc[0] >> 2) & 0x0F)])) << 42;

	//Get time (in seconds) since 1-1-2014 (bits 14-41) and reverse the bits.
	nTmp = ((((uint64_t)pSrc[0]) << 32) | ((uint64_t)pSrc[1]) << 24) | (((uint64_t)pSrc[2]) << 16) | (((uint64_t)pSrc[3]) << 8) | pSrc[4];
	nTmp = nTmp >> 6;
	nTmp = nTmp & 0xFFFFFFF;
	nTmp2 = (arRev[(nTmp >> 0) & 0x0F]) << 24;
	nTmp2 |= (arRev[(nTmp >> 4) & 0x0F]) << 20;
	nTmp2 |= (arRev[(nTmp >> 8) & 0x0F]) << 16;
	nTmp2 |= (arRev[(nTmp >> 12) & 0x0F]) << 12;
	nTmp2 |= (arRev[(nTmp >> 16) & 0x0F]) << 8;
	nTmp2 |= (arRev[(nTmp >> 20) & 0x0F]) << 4;
	nTmp2 |= (arRev[(nTmp >> 24) & 0x0F]) << 0;
	nReversed |= nTmp2 << 14;

	//Get the test location (bits 10-13) and reverse the bits.
	nReversed |= ((uint64_t)(arRev[(pSrc[4] >> 2) & 0x0F])) << 10;

	//Get the tester serial number (bits 0-9) and reverse the bits.
	nReversed |= ((uint64_t)(arRev[(pSrc[5] >> 4) & 0x0F]) | ((uint64_t)(arRev[pSrc[5] & 0x0F] << 4))) << 2;
	nReversed |= (pSrc[4] & 0x02) ? 0x01 : 0;
	nReversed |= (pSrc[4] & 0x01) ? 0x02 : 0;

	//Copy the result into the destination.
	pDst[0] = (uint8_t)(nReversed >> 40);
	pDst[1] = (uint8_t)(nReversed >> 32);
	pDst[2] = (uint8_t)(nReversed >> 24);
	pDst[3] = (uint8_t)(nReversed >> 16);
	pDst[4] = (uint8_t)(nReversed >> 8);
	pDst[5] = (uint8_t)(nReversed >> 0);
}