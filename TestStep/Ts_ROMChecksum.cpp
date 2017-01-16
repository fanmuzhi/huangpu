#include "Ts_ROMChecksum.h"


Ts_ROMChecksum::Ts_ROMChecksum(string &strName, string &strArgs, FpAlphaModule * &pDutCtrl, Syn_Dut * &pDut)
:Syn_FingerprintTest(strName, strArgs, pDutCtrl, pDut)
{
}

Ts_ROMChecksum::~Ts_ROMChecksum()
{
}

void Ts_ROMChecksum::SetUp()
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
	_pSyn_Dut->_pSyn_DutTestInfo->_ROMChecksumInfo.m_bExecuted = false;
	_pSyn_Dut->_pSyn_DutTestInfo->_ROMChecksumInfo.m_Type = 1;
	_pSyn_Dut->_pSyn_DutTestInfo->_ROMChecksumInfo.m_ChecksumValue = 0x0ec16784;

	ParseTestStepArgs(_strArgs, listOfArgValue);
	size_t ilistSize = listOfArgValue.size();
	if (ilistSize < 2)
	{
		for (size_t t = 1; t <= 2 - ilistSize; t++)
			listOfArgValue.push_back(std::string(""));
	}
	if (0 != listOfArgValue[0].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_ROMChecksumInfo.m_Type = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pSyn_Dut->_pSyn_DutTestInfo->_ROMChecksumInfo.m_ChecksumValue = std::stoul(listOfArgValue[1], NULL, 16);
}


void Ts_ROMChecksum::Execute()
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

	Get_Startinfo StartInfo;
	uint32_t rc = _pSyn_DutCtrl->FpGetStartInfo((uint8_t*)&StartInfo, sizeof(Get_Startinfo));
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpGetStartInfo is failed!");
		throw ex;
		return;
	}

	GetVerInfo VersionInfo;
	rc = _pSyn_DutCtrl->FpGetVersion((uint8_t*)&VersionInfo, sizeof(GetVerInfo));
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpGetVersion is failed!");
		throw ex;
		return;
	}

	rc = _pSyn_DutCtrl->FpTidleSet();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpTidleSet is failed!");
		throw ex;
		return;
	}

	//load OtpReadWritePatch
	Syn_PatchInfo OtpReadWritePatch;
	if (!_pSyn_Dut->FindPatch("OtpReadWritePatch", OtpReadWritePatch) || NULL == OtpReadWritePatch._pArrayBuf)
	{
		ex.SetError(Syn_ExceptionCode::Syn_DutPatchError);
		ex.SetDescription("OtpReadWritePatch Patch is NULL!");
		throw ex;
		return;
	}
	rc = _pSyn_DutCtrl->FpLoadPatch(OtpReadWritePatch._pArrayBuf, OtpReadWritePatch._uiArraySize);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpLoadPatch(OtpReadWritePatch) is failed!");
		throw ex;
		return;
	}

	ChecksumType Type;
	switch (_pSyn_Dut->_pSyn_DutTestInfo->_ROMChecksumInfo.m_Type)
	{
		case 1:
			Type = ChecksumType::XOR;
			break;
		case 2:
			Type = ChecksumType::CRC32;
			break;
		case 3:
			Type = ChecksumType::SHA256;
			break;
		case 4:
			Type = ChecksumType::SHA1;
			break;
		default:
			Type = ChecksumType::XOR;
			break;
	}

	uint32_t checksumResult(0);
	rc = _pSyn_DutCtrl->FpROMChecksum(Type, checksumResult);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpROMChecksum is failed!");
		throw ex;
		return;
	}
	rc = _pSyn_DutCtrl->FpUnloadPatch();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("Unload patch failed!");
		throw ex;
		return;
	}
	_pSyn_Dut->_pSyn_DutTestResult->_ROMChecksumResults.m_ChecksumResult = checksumResult;

	_pSyn_Dut->_pSyn_DutTestInfo->_ROMChecksumInfo.m_bExecuted = true;
}


void Ts_ROMChecksum::ProcessData()
{
	_pSyn_Dut->_pSyn_DutTestResult->_ROMChecksumResults.m_bPass = true;

	if (_pSyn_Dut->_pSyn_DutTestResult->_ROMChecksumResults.m_ChecksumResult != _pSyn_Dut->_pSyn_DutTestInfo->_ROMChecksumInfo.m_ChecksumValue)
	{
		_pSyn_Dut->_pSyn_DutTestResult->_ROMChecksumResults.m_bPass = false;
		_pSyn_Dut->_pSyn_DutTestResult->_binCodes.push_back(Syn_BinCodes::m_sROMChecsum);
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("ROMChecksum", "Fail"));
	}
	else
	{
		_pSyn_Dut->_pSyn_DutTestResult->_mapTestPassInfo.insert(std::map<std::string, std::string>::value_type("ROMChecksum", "Pass"));
	}

}


void Ts_ROMChecksum::CleanUp()
{
	Syn_Exception ex(0);
	uint32_t rc = _pSyn_DutCtrl->FpReset();
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpReset() Failed");
		throw ex;
	}

	rc = _pSyn_DutCtrl->FpTidleSet(0);
	if (0 != rc)
	{
		ex.SetError(rc);
		ex.SetDescription("FpTidleSet command failed!");
		throw ex;
		return;
	}
	ComputeRunningTime(_pSyn_Dut->_pSyn_DutTestResult->_ROMChecksumResults.m_elapsedtime);
}
