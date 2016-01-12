//local
#include "Syn_Site.h"
#include "Syn_Exception.h"

//MPC_API
#include "MpcApiDll.h"
//#include "MpcApiError.h"
//#include "MPCErrors.h"

//std
#include <iostream>
#include <io.h>
#include <stdio.h>

//rapidxml
#include "rapidxml.hpp"  
#include "rapidxml_utils.hpp"  
#include "rapidxml_print.hpp"

//third-party
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

Syn_Site::Syn_Site()
:_pSyn_Dut(NULL)
, _iSiteNumber(0)
{
	_OTPTestInfo._TestState = TestReady;

	LOG(INFO) << "easylogging++ test!";

	LOG(INFO) << "SITE START";
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
	//opSyn_SiteInstance->SetSysConfig(iSyn_SysConfigInfo);
	opSyn_SiteInstance->_SysConfig = iSyn_SysConfigInfo;
	opSyn_SiteInstance->_uiSerialNumber = iSerialNumber;

	pSyn_Dut->SetPatchInfo(opSyn_SiteInstance->_SysConfig._listPatchInfo);


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
			
			pSyn_SiteInstance->_OTPTestInfo._uiSerialNumber = uiSerialNumber;
			pSyn_SiteInstance->_iSiteNumber = i + 1;
			pSyn_SiteInstance->_OTPTestInfo._uiSiteNumber = pSyn_SiteInstance->_iSiteNumber;

			olistOfSyn_SiteInstance.push_back(pSyn_SiteInstance);
		}
	}

	delete[] pDeviceSerNumArray;
	pDeviceSerNumArray = NULL;


	//tinyxml test
	Syn_SysConfig TempSyn_SysConfig;
	ParseSysConfigXML(std::string("D:\\Project\\test.xml"), TempSyn_SysConfig);

	return true;
}

//void Syn_Site::Run(uint8_t * arMS0, int iSize)
void Syn_Site::Run()
{
	if (NULL == _pSyn_Dut)
	{
		cout << "Error:Syn_Site::Run() - _pSyn_Dut is NULL!" << endl;
		_OTPTestInfo._TestState = TestError;
		return;
	}

	//ofstream logFile("D:\\error.log");

	uint8_t *pOTPReadWritePatchArray = NULL;
	int iOTPReadWritePatchSize(0);
	Syn_PatchInfo NeedSyn_XepatchInfo;
	bool rc = _SysConfig.GetSyn_XepatchInfo(std::string("OtpReadWritePatch"), NeedSyn_XepatchInfo);
	if (rc)
	{
		pOTPReadWritePatchArray = NeedSyn_XepatchInfo._pArrayBuf;
		iOTPReadWritePatchSize = NeedSyn_XepatchInfo._uiArraySize;
	}

	if (NULL == pOTPReadWritePatchArray || 0 == iOTPReadWritePatchSize)
	{
		_OTPTestInfo._TestState = TestError;
		cout << "Error:Syn_Site::Run() - Array is NULL!" << endl;
		return;
	}

	//uint8_t arMS0[4224] = { 0 };
	uint8_t arMS0[BS0_SIZE + BS1_SIZE + MS1_SIZE + MS1_SIZE] = { 0 };
	int iSize(BS0_SIZE + BS1_SIZE + MS1_SIZE + MS1_SIZE);
	try
	{
		_OTPTestInfo._TestState = TestRunning;
		//_pSyn_Dut->ReadOTP(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true, pOTPReadWritePatchArray, iOTPReadWritePatchSize, arMS0, iSize);
		_pSyn_Dut->PowerOn(_SysConfig._uiDutpwrVdd_mV, _SysConfig._uiDutpwrVio_mV, _SysConfig._uiDutpwrVled_mV, _SysConfig._uiDutpwrVddh_mV, true);
		_pSyn_Dut->ReadOTP(pOTPReadWritePatchArray, iOTPReadWritePatchSize, arMS0, iSize);
		_pSyn_Dut->PowerOff();

	}
	catch (Syn_Exception ex)
	{
		_pSyn_Dut->PowerOff();
		std::cout << "Error:ReadOTP is failed!" << std::endl;
		_OTPTestInfo._strErrorMessage = ex.GetDescription();
		_OTPTestInfo._TestState = TestFailed;

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

	_OTPTestInfo._TestState = TestOK;

	return;
}

void Syn_Site::GetOTPTestInfo(Syn_OTPTestInfo &oSyn_OTPTestInfo)
{
	//::Sleep(1000);
	oSyn_OTPTestInfo = _OTPTestInfo;
}






bool Syn_Site::ParseSysConfigXML(std::string strConfigFilePath, Syn_SysConfig &oSyn_SysConfig)
{
	if (-1 == access(strConfigFilePath.c_str(), 4))
	{
		cout << "Error:Syn_Site::ParseSysConfigXML() - strConfigFilePath is not exists!" << endl;
		return false;
	}

	rapidxml::file<> fdoc(strConfigFilePath.c_str());
	//std::cout << fdoc.data() << std::endl;

	rapidxml::xml_document<>  doc;
	doc.parse<0>(fdoc.data());
	//std::cout << doc.name() << std::endl;

	//root
	rapidxml::xml_node<> *rootNode = doc.first_node();
	//std::cout << rootNode->name() << std::endl;
	if (NULL == rootNode)
	{
		cout << "Error:Syn_Site::ParseSysConfigXML() - rootNode is NULL!" << endl;
		return false;
	}

	for (rapidxml::xml_node<char> *node = rootNode->first_node();node != NULL;node = node->next_sibling())
	{
		std::string strNodeName = node->name();
		if (std::string("AutoController") == strNodeName)
		{
			oSyn_SysConfig._strAutoController = node->value();
			cout << "" << oSyn_SysConfig._strAutoController << endl;
		}
		else if (std::string("DutType") == strNodeName)
		{
			oSyn_SysConfig._strDutType = node->value();
			cout << "" << oSyn_SysConfig._strDutType << endl;
		}
		else if (std::string("DutController") == strNodeName)
		{
			oSyn_SysConfig._strDutController = node->value();
			cout << "" << oSyn_SysConfig._strDutController << endl;
		}
		else if (std::string("DutCom") == strNodeName)
		{
			oSyn_SysConfig._strDutCom = node->value();
			cout << "" << oSyn_SysConfig._strDutCom << endl;
		}
		else if (std::string("DutPwrVdd_mV") == strNodeName)
		{
			oSyn_SysConfig._uiDutpwrVdd_mV = atoi(node->value());
			cout << "" << oSyn_SysConfig._uiDutpwrVdd_mV << endl;
		}
		else if (std::string("DutPwrVio_mV") == strNodeName)
		{
			oSyn_SysConfig._uiDutpwrVio_mV = atoi(node->value());
			cout << "" << oSyn_SysConfig._uiDutpwrVio_mV << endl;
		}
		else if (std::string("DutPwrVled_mV") == strNodeName)
		{
			oSyn_SysConfig._uiDutpwrVled_mV = atoi(node->value());
			cout << "" << oSyn_SysConfig._uiDutpwrVled_mV << endl;
		}
		else if (std::string("DutPwrVddh_mV") == strNodeName)
		{
			oSyn_SysConfig._uiDutpwrVddh_mV = atoi(node->value());
			cout << "" << oSyn_SysConfig._uiDutpwrVddh_mV << endl;
		}
		else if (std::string("NumRows") == strNodeName)
		{
			oSyn_SysConfig._uiNumRows = atoi(node->value());
			cout << "" << oSyn_SysConfig._uiNumRows << endl;
		}
		else if (std::string("NumCols") == strNodeName)
		{
			oSyn_SysConfig._uiNumRows = atoi(node->value());
			cout << "" << oSyn_SysConfig._uiNumRows << endl;
		}
		else if (std::string("WriteBootSectors") == strNodeName)
		{
			oSyn_SysConfig._bWriteBootSectors = atoi(node->value());
			cout << "" << oSyn_SysConfig._bWriteBootSectors << endl;
		}
		else if (std::string("BootSector0") == strNodeName)
		{
			Syn_Site::ConvertAsciiToBinary(node->value(), oSyn_SysConfig._arrUserSpecifiedBS0, BS0_SIZE);
			cout << "" << node->value() << endl;
		}
		else if (std::string("BootSector1") == strNodeName)
		{
			Syn_Site::ConvertAsciiToBinary(node->value(), oSyn_SysConfig._arrUserSpecifiedBS1, BS1_SIZE);
			cout << "" << node->value() << endl;
		}
		else if (std::string("TestSeq") == strNodeName)
		{
			for (rapidxml::xml_node<char> *TestNode = node->first_node(); TestNode != NULL; TestNode = TestNode->next_sibling())
			{
				Syn_TestStepInfo CurrentSyn_TestStepInfo;
				CurrentSyn_TestStepInfo._strNodeName = TestNode->name();
				CurrentSyn_TestStepInfo._strTestStepName = TestNode->value();

				rapidxml::xml_attribute<> *pAttribute = TestNode->first_attribute("Args");
				if (NULL != pAttribute)
				{
					CurrentSyn_TestStepInfo._strTestStepArgs = pAttribute->value();
					cout << CurrentSyn_TestStepInfo._strTestStepName<<" " << CurrentSyn_TestStepInfo._strTestStepArgs << endl;
				}

				oSyn_SysConfig._listTestSteps.push_back(CurrentSyn_TestStepInfo);
			}
		}
		else if (std::string("ImageAcqPatch") == strNodeName || std::string("PrintFile") == strNodeName || std::string("OtpReadWritePatch") == strNodeName || std::string("OpensShortsPatch") == strNodeName || std::string("WofPatch") == strNodeName || std::string("WofLowPowerPatch") == strNodeName || std::string("ScmWofPatch") == strNodeName || std::string("AfePatch") == strNodeName
				|| std::string("CacheDataRam") == strNodeName || std::string("CacheTagRam") == strNodeName || std::string("CachInstDataRam") == strNodeName || std::string("CachInstTagRam") == strNodeName || std::string("ScmAndMainRam") == strNodeName
				|| std::string("Cmd1ScmWofPlot") == strNodeName || std::string("Cmd2ScmWofBin") == strNodeName || std::string("Cmd3SweepTag") == strNodeName
				|| std::string("WofCmd1") == strNodeName || std::string("WofCmd2") == strNodeName || std::string("WofCmd3") == strNodeName || std::string("WofCmd4") == strNodeName || std::string("PixelPatch") == strNodeName || std::string("SpiFlashRamPatch") == strNodeName || std::string("BulkEraseCmd") == strNodeName || std::string("ProgramCmd") == strNodeName
				|| std::string("ReadCmd") == strNodeName || std::string("WovarPatch") == strNodeName || std::string("VCEK_IV") == strNodeName || std::string("WofLowPowerBin") == strNodeName || std::string("ProdOtpReadWritePatch") == strNodeName || std::string("SecurityMgtEngrPatch") == strNodeName || std::string("SecurityMgtProdPatch") == strNodeName || std::string("TakeOwnershipBin") == strNodeName)
		{
			Syn_PatchInfo CurrentSyn_XepatchInfo;

			std::string strXepatchName(strNodeName);

			rapidxml::xml_attribute<> *attribute = node->first_attribute("Args");
			if (NULL != attribute)
			{
				CurrentSyn_XepatchInfo._strXepatchFileName = attribute->value();
				cout << strNodeName << " " << CurrentSyn_XepatchInfo._strXepatchFileName << endl;
			}
			else
			{
				cout << strNodeName << " has no args " << endl;
			}

			std::string strPatchTextValue(node->value());
			int iTextLenth = strPatchTextValue.length();
			if (0 == iTextLenth)
			{
				CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
				CurrentSyn_XepatchInfo._pArrayBuf = NULL;
				CurrentSyn_XepatchInfo._uiArraySize = 0;
			}
			else
			{
				uint8_t *pArray = new uint8_t[iTextLenth / 2];
				Syn_Site::ConvertAsciiToBinary(strPatchTextValue, pArray, iTextLenth / 2);

				CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
				CurrentSyn_XepatchInfo._pArrayBuf = pArray;
				CurrentSyn_XepatchInfo._uiArraySize = iTextLenth / 2;
			}

			oSyn_SysConfig._listPatchInfo.push_back(CurrentSyn_XepatchInfo);
		}
	}


	return true;
}

void Syn_Site::ConvertAsciiToBinary(const std::string strAsciiValue, uint8_t *pDst, int nDstSize)
{
	char *p;

	int	nNumBytes = strAsciiValue.length() / 2;
	//Translate the ASCII into binary values.
	for (int i = 0; (i<nNumBytes) && (i<nDstSize); i++)
	{
		std::string strTemp(strAsciiValue, i * 2, 2);

		uint8_t nVal = (uint8_t)strtol(strTemp.c_str(), &p, 16);
		//uint8_t nVal = (uint8_t)stoul(strTemp, 0, 2);
		pDst[i] = nVal;
	}
}