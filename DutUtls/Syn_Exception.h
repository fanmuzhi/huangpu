// MTExceptions.h
#pragma once

//std
#include <exception>
#include <cstdint>
#include <string>
using namespace std;

struct Syn_ExceptionCode
{
	static const uint32_t Syn_OK = 0x0000;

	static const uint32_t Syn_UnknownError = 0x1000;

	static const uint32_t Syn_DutInfoNull = 0x1001;
	static const uint32_t Syn_DutResultNull = 0x1002;
	static const uint32_t Syn_DutNull = 0x1003;
	static const uint32_t Syn_DutCtrlNull = 0x1004;
	static const uint32_t Syn_DutPatchError = 0x1005;

	static const uint32_t Syn_ConfigError = 0x2000;
	static const uint32_t Syn_ScriptConfigError = 0x2001;
	static const uint32_t Syn_TestStepConfigError = 0x2002;

	static const uint32_t Syn_MPCError = 0x3000;
	static const uint32_t Syn_ERR_INITIALIZATION_FAILED = 0x3001;
	static const uint32_t Syn_ERR_DEINITIALIZATION_FAILED = 0x3002;
	static const uint32_t Syn_ERR_GET_DEVICE_NUMBER_FAILED = 0x3003;
	static const uint32_t Syn_ERR_GET_DEVICE_HANDLE_FAILED = 0x3004;
	static const uint32_t Syn_ERR_COMMUNICATION_INITIALIZE_FAILED = 0x3005;
	static const uint32_t Syn_ERR_COMMUNICATION_FAILED = 0x3006;
	static const uint32_t Syn_ERR_FILE_NAME_TOO_LONG = 0x3007;
	static const uint32_t Syn_ERR_OPEN_LOCAL_FILE_FAILED = 0x3008;
	static const uint32_t Syn_ERR_INVALID_FILE_INFORMATION = 0x3009;
	static const uint32_t Syn_ERR_SCRIPT_CONVERT_FAILED = 0x300a;
	static const uint32_t Syn_ERR_LOAD_PYTHON_MODULE_FAILED = 0x300b;
	static const uint32_t Syn_ERR_PYTHON_VERSION_NOT_MATCH = 0x300c;
	static const uint32_t Syn_ERR_INVALID_DEVICE_HANDLE = 0x300d;
	static const uint32_t Syn_ERR_DEVICE_NOT_CONNECTED = 0x300e;
	static const uint32_t Syn_ERR_INSUFICIENT_SCRIPT_DATA = 0x300f;
	static const uint32_t Syn_ERR_FILE_NOT_FOUND = 0x3010;
	static const uint32_t Syn_ERR_NOT_INITIALIZED = 0x3011;
	static const uint32_t Syn_ERR_COMMAND_VALIDATION_FAILED = 0x3012;
	static const uint32_t Syn_ERR_SET_ABORT_UPDATE_FIRMWARE_FAILED = 0x3013;
	static const uint32_t Syn_ERR_UPDATE_FIRMWARE_ABORTED = 0x3014;
	static const uint32_t Syn_ERR_INSUFFICIENT_SCRIPT_OUTPUT_BUFFER = 0x3015;
	static const uint32_t Syn_ERR_ARG_LIST_TOO_LONG = 0x3016;
	static const uint32_t Syn_ERR_SCRIPT_EXE_FAILED = 0x3017;
	static const uint32_t Syn_ERR_HID_NULL_REPORT_DESC = 0x3018;
	static const uint32_t Syn_ERR_HID_LENGTH_FAILED = 0x3019;
	static const uint32_t Syn_ERR_HID_TABLE_FAILED = 0x301a;
	static const uint32_t Syn_ERR_HID_VALUE_RANGE_FAILED = 0x301b;
	static const uint32_t Syn_ERR_HID_ENUM_FAILED = 0x301c;
	static const uint32_t Syn_ERR_HID_SWITCH_MODE_FAILED = 0x301d;
	static const uint32_t Syn_ERR_HID_REPORT_UNAVAILABLE = 0x301e;
	static const uint32_t Syn_ERR_FP_COMMAND_FAILED = 0x301f;
	static const uint32_t Syn_ERR_FP_INVALID_SENSOR_TYPE = 0x3020;
	static const uint32_t Syn_ERR_FP_INVALID_PARAMS = 0x3021;
	static const uint32_t Syn_ERR_PARAM_VLIDATION_FAILED = 0x3ffe;
	static const uint32_t Syn_ERR_NOT_IMPLEMENTED = 0x3fff;
	static const uint32_t Syn_ERR_MAX = 0x4000;

	static const uint32_t Syn_SystemError = 0x4001;
	static const uint32_t Syn_RunTimeError = 0x4002;

	static const uint32_t Syn_SiteStateError = 0x4003;
	static const uint32_t Syn_SiteThread = 0x4004;
};

class Syn_Exception : public exception
{
public:

	Syn_Exception(uint32_t iErrorNum) :_uiErrorNumber(iErrorNum), _strDescription("")
	{
	}
	Syn_Exception(uint32_t iErrorNum, std::string strDescription) :_uiErrorNumber(iErrorNum), _strDescription(strDescription)
	{
	}
	
	inline void SetError(const uint32_t error)
	{
		_uiErrorNumber = error;
	}

	inline uint32_t GetError()
	{
		return _uiErrorNumber;
	}

	inline void	SetDescription(const std::string &strDescription)
	{ 
		_strDescription = strDescription;
	}

	inline std::string GetDescription() const
	{
		return _strDescription + "(" + to_string(_uiErrorNumber) + ")";
	}

	inline const char* what()
	{
		std::string descrip = GetDescription();
		return descrip.c_str();
	}

private:

	string		_strDescription;
	uint32_t	_uiErrorNumber;
};