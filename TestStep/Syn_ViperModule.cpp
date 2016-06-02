//Local
#include "Syn_ViperModule.h"

Syn_ViperModule::Syn_ViperModule()
{
}

Syn_ViperModule::~Syn_ViperModule()
{
}

void Syn_ViperModule::CopyToPrintPatch(uint8_t* pSrc, uint8_t* pPrintPatch, int nNumBytes, int nPatchIdx)
{
	//For Viper sensors, we copy twice into Print Patch. Make sure 2nd copy is alligned on
	//a four byte boundary.
	if ((nNumBytes % 4) == 0)
	{
		memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
		memcpy(&pPrintPatch[nPatchIdx + nNumBytes], pSrc, nNumBytes);
	}
	else
	{
		//Make sure the space between the two copies of data are set to zero.
		memset(&pPrintPatch[nPatchIdx], 0, nNumBytes + (4 - (nNumBytes % 4)));
		memcpy(&pPrintPatch[nPatchIdx], pSrc, nNumBytes);
		memcpy(&pPrintPatch[nPatchIdx + (nNumBytes + (4 - (nNumBytes % 4)))], pSrc, nNumBytes);
	}
}

void Syn_ViperModule::ModifySweepWofCmdData(uint8_t* pSweepCmd) 
{ 
	pSweepCmd[0x0C] = 100;	//Gain start.
	pSweepCmd[0x10] = 100;	//Gain increment.
	pSweepCmd[0x14] = 200;	//Gain end.
}

void Syn_ViperModule::ModifySweepSCMWofCmdData(uint8_t* pSweepCmd) 
{
	pSweepCmd[0x0B] = 100;	//Gain start.
	pSweepCmd[0x0F] = 100;	//Gain increment.
	pSweepCmd[0x13] = 200;	//Gain end.
}

void Syn_ViperModule::TrimOsc(OscTrimInfo &iOscTrimInfo, OscTrimResults &ioOscTrimResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV)
{
	uint32_t	nSweepBegin;
	uint8_t		pDst[10] = {0};
	uint8_t		arHWRegAddr[5] = { 52, 3, 0, 128, 4 };//actual register addr is 0x80000334	
	uint32_t	arPokeAddrOsc[NUM_REGISTERS_VIPER_TRIM_OSC] = { 0x80000350, 0x80000354, 0x80001500, 0x80001508, 0x80001510, 0x80000B00, 0x80001604 };
	uint32_t	arPokeValueOsc[NUM_REGISTERS_VIPER_TRIM_OSC] = { 0xF0002, 0x0, 0x1, 0x7F, 0xAF, 0x2, 0x3FF };
	uint32_t	nFreq_Hz;
	uint32_t	nResultOscTrim;
	int			timeout;

	PowerOff();
	PowerOn(Vdd_mV, Vio_mV, Vled_mV, Vddh_mV, true);
	//Write SiOsc TC 
	//WriteSiOsc_TempCompTrim(site);
	//Cycle power again.
	//PowerOn(Vdd_mV, Vio_mV, Vled_mV, Vddh_mV, true);

	//set z as an input to read in the divided OSC Freq
	_pSyn_DutCtrl->GpioSetPinType(6, 0x80, 5);

	//Poke - note that there is an array of data values that are being poked.
	_pSyn_DutCtrl->FpPokeRegister(0x80000350, 0x02);
	_pSyn_DutCtrl->FpPokeRegister(0x80000354, 0x00);
	_pSyn_DutCtrl->FpPokeRegister(0x80001500, 0x01);
	_pSyn_DutCtrl->FpPokeRegister(0x80001508, 0x7F);
	_pSyn_DutCtrl->FpPokeRegister(0x80001510, 0x1F);
	_pSyn_DutCtrl->FpPokeRegister(0x80000B00, 0x02);
	_pSyn_DutCtrl->FpPokeRegister(0x80001604, 0xFF);

	::Sleep(25);
	_pSyn_DutCtrl->FpGpioGetFreq(6, 0x80, &nFreq_Hz);

	//MPC_GPIO_GET_FREQ
	//used to start at 480
	nSweepBegin = 0x380;
	timeout = 700;
	nResultOscTrim = 0;//(site.m_OscTrimInfo).nLowerLimit;	
	while (timeout && (nResultOscTrim < iOscTrimInfo.nLowerLimit_Hz || nResultOscTrim >= iOscTrimInfo.nUpperLimit_Hz))
	{
		//FpPokeCmd(0x80000B08, nSweepBegin);
		_pSyn_DutCtrl->FpPokeRegister(0x80000B08, nSweepBegin);
		_pSyn_DutCtrl->FpGpioGetFreq(6, 0x80, &nFreq_Hz);
		nResultOscTrim = nFreq_Hz / 1000;
		nSweepBegin++;
		timeout--;
	}
	if (timeout != 0)
	{
		ioOscTrimResults.m_nOscTrim = nSweepBegin - 1;
		ioOscTrimResults.m_bPass = 1;
	}
	else
	{
		ioOscTrimResults.m_bPass = 0;
	}
}

void Syn_ViperModule::TrimSlowOsc(SlowOscInfo &iSlowOscInfo, SlowOscResults &ioSlowOscResults, uint16_t Vdd_mV, uint16_t Vio_mV, uint16_t Vled_mV, uint16_t Vddh_mV)
{
	uint8_t		pDst[4] = {0};
	uint32_t	nFreq_Hz = 0;
	uint16_t	nHvOscBias, nHvOscTrim;
	int			timeout;

	ioSlowOscResults.m_bPass = 0;
	ioSlowOscResults.m_bDefaultValueUsed = 0;

	//CycleDutPower(GetSysConfig().GetPwrVdd(), GetSysConfig().GetPwrVio(),GetSysConfig().GetPwrVled(), GetSysConfig().GetPwrVddh(), true);
	PowerOff();
	PowerOn(Vdd_mV, Vio_mV, Vled_mV, Vddh_mV, true);

	_pSyn_DutCtrl->GpioDirModSet(6, 0x80, 0);//Set pin as direct input
	_pSyn_DutCtrl->GpioSetPinType(6, 0x80, 5);

	//Poke - note that there is an array of data values that are being poked.
	_pSyn_DutCtrl->FpPokeRegister(0x80002050, 0x17);
	_pSyn_DutCtrl->FpPokeRegister(0x8000036C, 0x0A);
	_pSyn_DutCtrl->FpPokeRegister(0x80000338, 0x20);
	_pSyn_DutCtrl->FpPokeRegister(0x8000034C, 0x20);
	_pSyn_DutCtrl->FpPokeRegister(0x80000384, 0x03);
	_pSyn_DutCtrl->FpPokeRegister(0x80001604, 0x02);

	::Sleep(50);

	timeout = 10;
	nHvOscBias = 3;
	nHvOscTrim = 0;
	//Calculate optimal Trim and Bias values.
	uint32_t nData = (((0x1F & nHvOscTrim) << 8) | nHvOscBias);
	_pSyn_DutCtrl->FpPokeRegister(0x80000330, nData);

	_pSyn_DutCtrl->FpGpioGetFreq(6, 0x80, &nFreq_Hz);
	while (timeout && ((nFreq_Hz < iSlowOscInfo.nLowerLimit_Hz) || ((nFreq_Hz >= iSlowOscInfo.nUpperLimit_Hz))))
	{
		nHvOscTrim++;
		nData = (((0x1F & nHvOscTrim) << 8) | nHvOscBias);
		_pSyn_DutCtrl->FpPokeRegister(0x80000330, nData);
		
		_pSyn_DutCtrl->FpGpioGetFreq(6, 0x80, &nFreq_Hz);
		timeout--;
	}

	//If trimming was successful
	if (timeout != 0)
	{
		ioSlowOscResults.m_nTrim = nHvOscTrim;
		ioSlowOscResults.m_nBias = nHvOscBias;
		//FpWriteSlowOscFreq(nHvOscTrim, nHvOscBias);
		ioSlowOscResults.m_bPass = 1;
	}
	else //Trimming unsuccessful
	{
		ioSlowOscResults.m_bPass = 0;

		////If default value specified in config file, use it.
		//if ((iSlowOscInfo.m_nDefaultTrim != 0) && (iSlowOscInfo.m_nDefaultBias != 0))
		//{
		//	ioSlowOscResults.m_bDefaultValueUsed = 1;
		//	ioSlowOscResults.m_nTrim = iSlowOscInfo.m_nDefaultTrim;
		//	ioSlowOscResults.m_nBias = iSlowOscInfo.m_nDefaultBias;
		//	FpWriteSlowOscFreq(ioSlowOscResults.m_nTrim, ioSlowOscResults.m_nBias);
		//	ioSlowOscResults.m_bPass = 1;
		//}
	}
}
