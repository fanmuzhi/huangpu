//local
#include "Syn_Site.h"
#include "Syn_Exception.h"

//MPC_API
#include "MpcApiDll.h"
//#include "MpcApiError.h"
//#include "MPCErrors.h"

//std
#include <iostream>

Syn_Site::Syn_Site()
:_pSyn_Dut(NULL)
//, _syn_SiteThread()
, _iTestEndTag(0)
, _tm(0)
{
}

Syn_Site::~Syn_Site()
{
	if (NULL != _pSyn_Dut)
	{
		delete _pSyn_Dut;
		_pSyn_Dut = NULL;
	}
}

void Syn_Site::Init()
{
}

bool Syn_Site::ConstructSiteInstance(uint32_t iSerialNumber, Syn_SysConfig &iSyn_SysConfigInfo, Syn_Site * &opSyn_SiteInstance)
{
	bool rc(false);

	opSyn_SiteInstance = NULL;
	if (0 == iSerialNumber)
	{
		cout << "Error:Syn_Site::ConstructSiteInstance() - iSerialNumber is 0!" << endl;
		return false;
	}

	//ProejctType:Viper1,Viper2,Metallica
	std::string strProjectType(iSyn_SysConfigInfo._strDutType);
	ProjectType iProjectType;
	if (std::string("Viper") == strProjectType || std::string("Viper1") == strProjectType)
	{
		iProjectType = Viper1;
	}
	else if (std::string("Viper2") == strProjectType)
	{
		iProjectType = Viper2;
	}
	else if (std::string("Metallica") == strProjectType)
	{
		iProjectType = Metallica;
	}
	else
	{
		iProjectType = Viper1;
		cout << "Error:Syn_Site::ConstructSiteInstance() - an unknown ProjectType,construct it to Viper1!" << endl;
	}

	//DutController:SPC,MPC04
	std::string strDutController(iSyn_SysConfigInfo._strDutController);
	DutController iDutControllerType;
	if (std::string("SPC") == strDutController)
	{
		iDutControllerType = Syn_SPC;
	}
	else if (std::string("MPC04") == strDutController)
	{
		iDutControllerType = Syn_MPC04;
	}
	else
	{
		iDutControllerType = Syn_SPC;
		cout << "Error:Syn_Site::ConstructSiteInstance() - an unknown DutController,construct it to SPC!" << endl;
	}

	//Create Dut
	Syn_Dut *pSyn_Dut = NULL;
	rc = Syn_Dut::CreateDutInstance(iProjectType, pSyn_Dut);
	if (!rc || NULL == pSyn_Dut)
	{
		cout << "Error:Syn_Site::ConstructSiteInstance() - CreateDutInstance is failed!" << endl;
		return false;
	}

	//Create DutCtrl
	Syn_DutCtrl *pSyn_DutCtrl = NULL;
	rc = Syn_DutCtrl::CreateDutCtrlInstance(iDutControllerType, iSerialNumber, pSyn_DutCtrl);
	if (!rc || NULL == pSyn_DutCtrl)
	{
		delete pSyn_Dut;
		pSyn_Dut = NULL;
		cout << "Error:Syn_Site::ConstructSiteInstance() - CreateDutInstance is failed!" << endl;
		return false;
	}

	pSyn_Dut->SetDutCtrl(pSyn_DutCtrl);

	opSyn_SiteInstance = new Syn_Site();
	
	opSyn_SiteInstance->_pSyn_Dut = pSyn_Dut;
	opSyn_SiteInstance->_SysConfig = iSyn_SysConfigInfo;

	return true;
}

bool Syn_Site::ConstructSiteList(Syn_SysConfig &iSyn_SysConfigInfo, std::vector<Syn_Site*> &olistOfSyn_SiteInstance)
{
	olistOfSyn_SiteInstance.clear();

	uint32_t uResult(1);

	//Init Operation
	uResult = MPC_Initialize();
	if (0 != uResult)
	{
		cout << "Error:Syn_Site::ConstructSiteList() - ::MPC_Initialize is failed!" << endl;
		return false;
	}

	int iDeviceCounts(0);
	MPC_GetNumberOfDevices(&iDeviceCounts);
	if (0 == iDeviceCounts)
	{
		cout << "Error:Syn_Site::ConstructSiteList() - iDeviceCounts is 0!" << endl;
		return false;
	}

	uint32_t *pDeviceSerNumArray = new uint32_t[iDeviceCounts];
	//uint32_t pDeviceSerNumArray[8] = {0};
	MPC_GetDeviceSerialNumList(pDeviceSerNumArray);
	for (int i = 0; i<iDeviceCounts; i++)
	{
		uint32_t uiSerialNumber = pDeviceSerNumArray[i];
		if (NULL != uiSerialNumber)
		{
			Syn_Site *pSyn_SiteInstance = NULL;
			bool rc = Syn_Site::ConstructSiteInstance(uiSerialNumber, iSyn_SysConfigInfo, pSyn_SiteInstance);
			if (!rc&&NULL==pSyn_SiteInstance)
			{
				cout << "Error:Syn_Site::ConstructSiteList() - ConstructSiteInstance is failed,i is " << i << "!" << endl;
				continue;
			}
			
			olistOfSyn_SiteInstance.push_back(pSyn_SiteInstance);
		}
	}

	delete[] pDeviceSerNumArray;
	pDeviceSerNumArray = NULL;

	return true;
}

//void Syn_Site::Run(uint8_t * arMS0, int iSize)
void Syn_Site::Run()
{
	if (NULL == _pSyn_Dut)
	{
		cout << "Error:Syn_Site::Run() - _pSyn_Dut is NULL!" << endl;
		return;
	}

	//ofstream logFile("D:\\error.log");

	uint8_t *pOTPReadWritePatchArray = NULL;
	int iOTPReadWritePatchSize(0);
	Syn_XepatchInfo NeedSyn_XepatchInfo;
	bool rc = _SysConfig.GetSyn_XepatchInfo(std::string("OtpReadWritePatch"), NeedSyn_XepatchInfo);
	if (rc)
	{
		pOTPReadWritePatchArray = NeedSyn_XepatchInfo._pArrayBuf;
		iOTPReadWritePatchSize = NeedSyn_XepatchInfo._uiArraySize;
	}

	if (NULL == pOTPReadWritePatchArray || 0 == iOTPReadWritePatchSize)
	{
		cout << "Error:Syn_Site::Run() - Array is NULL!" << endl;
		return;
	}

	uint8_t arMS0[4224] = { 0 };
	int iSize(4224);
	try
	{
		//_pSyn_Dut->ReadOTP(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true, pOTPReadWritePatchArray, iOTPReadWritePatchSize, arMS0, iSize);
		_pSyn_Dut->ReadOTP(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true, 
						   pOTPReadWritePatchArray, iOTPReadWritePatchSize, arMS0, iSize);
	}
	catch (...)
	{
		std::clog << "Error:ReadOTP is failed!" << std::endl;
		return;
	}

	//Fill
	for (int i = 0; i < BS0_SIZE; i++)
	{
		(_OTPTestInfo._BootSector0Array)[i] = arMS0[i];
	}

	for (int i = 0; i < BS1_SIZE; i++)
	{
		(_OTPTestInfo._BootSector1Array)[i] = arMS0[i + BS0_SIZE];
	}

	for (int i = 0; i < MS1_SIZE; i++)//MS0_SIZE
	{
		(_OTPTestInfo._MainSector0Array)[i] = arMS0[i + BS0_SIZE + BS1_SIZE];
	}
	
	for (int i = 0; i < MS1_SIZE; i++)
	{
		(_OTPTestInfo._MainSector1Array)[i] = arMS0[i + BS0_SIZE + BS1_SIZE + MS1_SIZE];//MS0_SIZE
	}

	return;
}

void Syn_Site::GetOTPTestInfo(Syn_OTPTestInfo &oSyn_OTPTestInfo)
{
	//::Sleep(500);
	oSyn_OTPTestInfo = _OTPTestInfo;
}

bool Syn_Site::TestGetValue(std::string &strTime)
{
	struct tm * timeinfo;
	timeinfo = localtime(&_tm);

	strTime = asctime(timeinfo);
	//::Sleep(100);

	return true;
}


void Syn_Site::TestSet()
{
	/*int iTag(0);
	while (iTag <= 10000)
	{
		time(&_tm);
		iTag += 1;
	}*/
	::Sleep(1500);
	time(&_tm);
}
