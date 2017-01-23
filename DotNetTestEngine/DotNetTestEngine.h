// DotNetTestEngine.h

#pragma once

#include "syn_devicemanager.h"
#include "Syn_Site.h"
#include "Syn_TestUtils.h"

using namespace System;
using namespace System::Collections::Generic;

namespace DotNetTestEngine {

	public ref class DllVersion
	{
		public:
			inline static String^ GetDllVersion() { return SW_VERSION; }
	};

	//
	//Managed AdcBaseLineInfo
	//
	public ref struct SynapticsAdcBaseLineInfo
	{
		//array<uint32_t, 2>^ m_arAdcBaseLines = gcnew array<uint32_t,2>(4,4);
		List<UInt32>^ m_arAdcBaseLines = gcnew List<UInt32>();
		UInt32 m_nVdd;
		UInt32 m_nVio;
		UInt32 m_nVled;
		UInt32 m_nVddh;
	};


	//
	//Managed TestResult
	//
	public ref struct SynapticsTestResult
	{
		Dictionary<String^, String^>^ StepResult = gcnew Dictionary<String^, String^>();

		String^ SensorSerialNumber = gcnew String("");

		//List<Double>^ SNRValues = gcnew List<Double>();
		float SNR = 0;

		List<String^>^ BinCodes = gcnew List<String^>();

		List<uint8_t>^ ImageNoFinger = gcnew List<uint8_t>();
		uint8_t ImageNoFingerRow;
		uint8_t ImageNoFingerCol;
		//uint8_t arrImageNoFinger[200][200];
		//int imageNoFingerRows;
		//int imageNoFingerCols;

		List<uint8_t>^ ImageFinger = gcnew List<uint8_t>();
		uint8_t ImageFingerRow;
		uint8_t ImageFingerCol;
	};


	//
	//Managed Site
	//
	public ref class SynapticsSite
	{
		public:
			SynapticsSite(UInt16 siteNumber, String^ devSerNum, String^ configPath, SynapticsAdcBaseLineInfo^ adcInfo);
			~SynapticsSite();

			UInt32 Open();
			UInt32 ExecuteTestStep(String^ testName);
			UInt32 GetTestResult(SynapticsTestResult^ %testResult);
			UInt32 Close();
			List<String^>^ GetTestStepList();
			void WriteLog(String^ path, String^ fileName);

		private:
			Syn_Site * site;
	};

	//
	//Managed DeviceManager
	//
	public enum class Syn_DeviceType{ MPC04 = 0, M5 };

	public ref class SynapticsDeviceManager
	{
		public:
			SynapticsDeviceManager();
			~SynapticsDeviceManager();

			UInt32 Open(Syn_DeviceType Type);
			List<String^>^ GetSNList();
			List<UInt32>^ UpdateADCOffsets(Syn_DeviceType Type, String^ serialnumber, UInt32 vdd, UInt32 vio, UInt32 vled, UInt32 vddh);
			UInt32 Close();

		private:
			syn_devicemanager * deviceManager;
	};
}
