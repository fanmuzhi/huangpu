#include <QtWidgets>
#include "fps_testexecutive.h"

//C++ std
#include <thread>

FPS_TestExecutive::FPS_TestExecutive(QWidget *parent)
: QMainWindow(parent)
, _bStopTag(true)
, _iRealDeviceCounts(0)
{
	ui.setupUi(this);

	_ListOfSitePtr.clear();
	//_ListOfSynThread.clear();
	
	//slots
	QObject::connect(ui.ConfigFileSelectPushButton, SIGNAL(clicked()), this, SLOT(SelectConfigFile()));
	QObject::connect(ui.pushButtonRun, SIGNAL(clicked()), this, SLOT(ThreadTest()));
	//QObject::connect(&_synThread, SIGNAL(send(void*)), this, SLOT(ReceiveOTPTestSlot(void*)));

	for (int i = 1; i <= DeviceCounts; i++)
	{
		QObject::connect(&(_SynThreadArray[i - 1]), SIGNAL(send(void*)), this, SLOT(ReceiveOTPTestSlot(void*)));
	}

	_logfile = std::ofstream("sys.log");
}

//uint32_t FPS_TestExecutive::Init()
bool FPS_TestExecutive::Init(QString strConfigFile)
{
	//ui.textBrowser->append("Hello World");

	//std::ofstream logfile("sys.log");
	std::cout.rdbuf(_logfile.rdbuf());
	cout << "cout start!" << endl;

	QFile ConfigFile(strConfigFile);
	if (!ConfigFile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("Config File is not exists!"));
		return false;
	}

	Syn_SysConfig SysConfig;
	bool result = ConstructSyn_SysConfig(strConfigFile.toStdString(), SysConfig);
	if (!result)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't construct the Site list,check it please!"));
		return false;
	}

	_ListOfSitePtr.clear();
	//_ListOfSynThread.clear();
	//std::vector<Syn_Site*> listOfSyn_SiteInstance;
	bool rc = Syn_Site::ConstructSiteList(SysConfig, _ListOfSitePtr);
	size_t ilistCounts = _ListOfSitePtr.size();
	if (0 == ilistCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't construct the Site list!"));
		return false;
	}
	for (size_t i = 0; i < ilistCounts; i++)
	{
		_SynThreadArray[i].SetSite(_ListOfSitePtr[i]);
		_SynThreadArray[i].SetStopTag(true);
		//Syn_Thread NewThread;
		//_ListOfSynThread.push_back(NewThread);
	}
	_iRealDeviceCounts = ilistCounts;

	//slots
	/*for (size_t i = 1; i <= _ListOfSynThread.size(); i++)
	{
		QObject::connect(_ListOfSynThread[i-1], SIGNAL(send(void*)), this, SLOT(ReceiveOTPTestSlot(void*)));
	}*/

	//_synThread.SetSite(_ListOfSitePtr[0]);

	cout << "end!" << endl;


	return true;
}



FPS_TestExecutive::~FPS_TestExecutive()
{
	_ListOfSitePtr.clear();

	//_ListOfSynThread.clear();

	_logfile.close();
}

//slot
void FPS_TestExecutive::SelectConfigFile()
{
	QString strConfigFilePath = QFileDialog::getOpenFileName(this, "Select Config File", "./", "xml file(*.xml)");
	if (QString("") != strConfigFilePath)
	{
		QFile TempFile(strConfigFilePath);
		if (!TempFile.exists())
			return;

		ui.ConfigFileLineEdit->clear();
		ui.ConfigFileLineEdit->setText(strConfigFilePath);

		bool rc = Init(strConfigFilePath);
		/*if (rc)
		{
			QMessageBox::information(this, QString("OK"), QString("Construct the Site list succeed!"));
		}*/
	}
	
}

void FPS_TestExecutive::ThreadTest()
{
	if (0 == _ListOfSitePtr.size())
	{
		//QMessageBox::information(this, QString("Error"), QString("Site list is NULL,check it please!"));
		QMessageBox::critical(this, QString("Error"), QString("Site list is NULL,check it please!"));
		return;
	}

	/*if (_synThread.isRunning())
	{
		_synThread.SetStopTag(true);

		//_synThread.terminate();

		ui.pushButtonRun->setText(QString("Run"));
	}
	else
	{
		_synThread.start();
		_synThread.SetStopTag(false);

		//ui.pushButtonRun->setText(QString("Stop"));

	}*/

	for (int i = 1; i <= _iRealDeviceCounts; i++)
	{
		if (_SynThreadArray[i - 1].isRunning())
		{
			_SynThreadArray[i - 1].SetStopTag(true);

			//_synThread.terminate();

			ui.pushButtonRun->setText(QString("Run"));
		}
		else
		{
			_SynThreadArray[i - 1].start();
			_SynThreadArray[i - 1].SetStopTag(false);

			//ui.pushButtonRun->setText(QString("Stop"));

		}
	}

}

void FPS_TestExecutive::ReceiveOTPTestSlot(void * pOTPTestInfo)
{

	/*if (!_synThread.isRunning())
		return;*/

	if (NULL == pOTPTestInfo)
		return;

	Syn_OTPTestInfo *pTestInfo = static_cast<Syn_OTPTestInfo*>(pOTPTestInfo);
	if (NULL == pTestInfo)
	{
		clog << "FPS_TestExecutive::ReceiveOTPTestSlot() - pTestInfo is NULL!" << endl;
		return;
	}


	QString strSerialNumber = QString("Site Number:") + QString::number(pTestInfo->_uiSiteNumber) + QString(" Serial Number:") + QString::number(pTestInfo->_uiSerialNumber);
	ui.textBrowser->append(strSerialNumber);
	
	std::string strErrorMsg = pTestInfo->_strErrorMessage;
	Syn_TestState TestResult = pTestInfo->_TestState;
	if (TestError == TestResult)
	{
		//QMessageBox::critical(this, QString("Error"), strSerialNumber+QString(" Test is error,reason is ") + QString::fromStdString(strErrorMsg));
		ui.textBrowser->append(QString("Test is Error,reason is ") + QString::fromStdString(strErrorMsg));
		return;
	}
	else if (TestFailed == TestResult)
	{
		//QMessageBox::critical(this, QString("Failed"), strSerialNumber + QString(" Test is failed,reason is ") + QString::fromStdString(strErrorMsg));
		ui.textBrowser->append(QString("Test is Failed,reason is ") + QString::fromStdString(strErrorMsg));
		return;
	}

	/*std::string strErrorMsg = pTestInfo->_strErrorMessage;
	if (std::string("") != strErrorMsg)
	{
		QMessageBox::critical(this, QString("Error"), QString::fromStdString(strErrorMsg));
		return;
	}*/

	//ui.textBrowser->clear();

	//QString strSerialNumber = QString("Site Number:") + QString::number(pTestInfo->_uiSiteNumber) + QString("   Serial Number:") + QString::number(pTestInfo->_uiSerialNumber);
	

	ui.textBrowser->append(QString("Boot Sector0:"));
	for (int i = 1; i < BS0_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		Display(pTestInfo->_BootSector0Array, StartPos, EndPos);
	}

	ui.textBrowser->append(QString("Boot Sector1:"));
	for (int i = 1; i <= BS1_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		Display(pTestInfo->_BootSector1Array, StartPos, EndPos);
	}

	/*ui.textBrowser->append(QString("Main Sector0:"));
	for (int i = 1; i <= MS0_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		Display(pTestInfo->_MainSector0Array, StartPos, EndPos);
	}

	ui.textBrowser->append(QString("Main Sector1:"));
	for (int i = 1; i <= MS1_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		Display(pTestInfo->_MainSector1Array, StartPos, EndPos);
	}*/
}

void FPS_TestExecutive::Display(uint8_t* pDst, int DstSize)
{
	QString s = "";
	for (int i = 0; i<DstSize; i++){
		s += QString::number(pDst[i], 16) + ",";
	}
	ui.textBrowser->append(s);
}

void FPS_TestExecutive::Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos)
{
	if ((EndPos - StartPos) <= 0)
	{
		ui.textBrowser->append("StartPos>=EndPos");
		return;
	}

	QString s = "";
	for (int i = StartPos; i <= EndPos; i++)
	{
		s += (QString::number(pDst[i], 16)).toUpper() + ",";
	}
	ui.textBrowser->append(s);
}







bool FPS_TestExecutive::ConstructSyn_SysConfig(const std::string &strConfigFilePath,Syn_SysConfig &oSyn_SysConfig)
{
	bool rc(false);//return code

	QString qstrConfigFilePath(QString::fromStdString(strConfigFilePath));
	SysConfig *pSysConfigOperation = NULL;
	rc = SysConfig::CreateSysConfigInstance(qstrConfigFilePath, pSysConfigOperation);
	if (!rc || NULL == pSysConfigOperation)
	{
		cout << "Error:ConstructSyn_SysConfig() - ::CreateSysConfigInstance is failed!" << endl;
		return false;
	}

	char *p = NULL;

	//1st
	QString qstrAutoControllerName("AutoController"), qstrAutoControllerValue("");
	QString qstrDutTypeName("DutType"), qstrDutTypeValue("");
	QString qstrDutControllerName("DutController"), qstrDutControllerValue("");
	QString qstrDutComName("DutCom"), qstrDutComValue("");
	rc = pSysConfigOperation->GetElementNodeText(qstrAutoControllerName, qstrAutoControllerValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutTypeName, qstrDutTypeValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutControllerName, qstrDutControllerValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutComName, qstrDutComValue);
	if (QString("") == qstrAutoControllerValue || QString("") == qstrDutTypeValue || QString("") == qstrDutControllerValue || QString("") == qstrDutComValue)
	{
		cout << "Error:ConstructSyn_SysConfig() - 1st operation is failed!" << endl; 
		return false;
	}

	//2nd
	QString qstrDutPwrVdd_mVName("DutPwrVdd_mV"), qstrDutPwrVdd_mVValue("");
	QString qstrDutPwrVio_mVName("DutPwrVio_mV"), qstrDutPwrVio_mVValue("");
	QString qstrDutPwrVled_mVName("DutPwrVled_mV"), qstrDutPwrVled_mVValue("");
	QString qstrDutPwrVddh_mVName("DutPwrVddh_mV"), qstrDutPwrVddh_mVValue("");
	rc = pSysConfigOperation->GetElementNodeText(qstrDutPwrVdd_mVName, qstrDutPwrVdd_mVValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutPwrVio_mVName, qstrDutPwrVio_mVValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutPwrVled_mVName, qstrDutPwrVled_mVValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutPwrVddh_mVName, qstrDutPwrVddh_mVValue);
	if (QString("") == qstrDutPwrVdd_mVValue || QString("") == qstrDutPwrVio_mVValue || QString("") == qstrDutPwrVled_mVValue || QString("") == qstrDutPwrVddh_mVValue)
	{
		cout << "Error:ConstructSyn_SysConfig() - 2nd operation is failed!" << endl;
		return false;
	}
	
	//3rd
	QString qstrNumRowsName("NumRows"), qstrNumRowsValue("");
	QString qstrNumColsName("NumCols"), qstrNumColsValue("");
	rc = pSysConfigOperation->GetElementNodeText(qstrNumRowsName, qstrNumRowsValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrNumColsName, qstrNumColsValue);
	if (QString("") == qstrNumRowsValue || QString("") == qstrNumColsValue)
	{
		cout << "Error:ConstructSyn_SysConfig() - 3rd operation is failed!" << endl;
		return false;
	}

	//4th
	QString qstrWriteBootSectorsName("WriteBootSectors"), qstrWriteBootSectorsValue("");
	QString qstrBootSector0Name("BootSector0"), qstrBootSector0Value("");
	QString qstrBootSector1Name("BootSector1"), qstrBootSector1Value("");
	rc = pSysConfigOperation->GetElementNodeText(qstrWriteBootSectorsName, qstrWriteBootSectorsValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrBootSector0Name, qstrBootSector0Value);
	rc = pSysConfigOperation->GetElementNodeText(qstrBootSector1Name, qstrBootSector1Value);
	if (QString("") == qstrWriteBootSectorsValue || QString("") == qstrBootSector0Value || QString("") == qstrBootSector1Value)
	{
		cout << "Error:ConstructSyn_SysConfig() - 4th operation is failed!" << endl;
		return false;
	}

	//5th
	std::vector<TestSeqInfo> listOfTestSeqInfo;
	rc = pSysConfigOperation->GetTestSeqList(listOfTestSeqInfo);
	std::vector<Syn_TestStepInfo> listOfTestSteps;
	if (0 == listOfTestSeqInfo.size())
	{
		cout << "Error:ConstructSyn_SysConfig() - 5th operation is failed!" << endl;
		return false;
	}
	for (int i = 1; i <= listOfTestSeqInfo.size(); i++)
	{
		Syn_TestStepInfo CurrentTestStepInfo;
		CurrentTestStepInfo._strNodeName = listOfTestSeqInfo[i-1].strNodeName.toStdString();
		CurrentTestStepInfo._strTestStepName = listOfTestSeqInfo[i-1].strSeqText.toStdString();
		CurrentTestStepInfo._strTestStepArgs = listOfTestSeqInfo[i-1].strSeqAttribute.toStdString();
	}

	//6th
	std::vector<std::string> listOfXepathName;
	
	listOfXepathName.push_back(std::string("ImageAcqPatch"));
	listOfXepathName.push_back(std::string("PrintFile"));
	listOfXepathName.push_back(std::string("OtpReadWritePatch"));
	listOfXepathName.push_back(std::string("OpensShortsPatch"));
	listOfXepathName.push_back(std::string("WofPatch"));
	listOfXepathName.push_back(std::string("WofLowPowerPatch"));
	listOfXepathName.push_back(std::string("ScmWofPatch"));
	listOfXepathName.push_back(std::string("AfePatch"));

	//RAM Patch Test
	listOfXepathName.push_back(std::string("CacheDataRam"));
	listOfXepathName.push_back(std::string("CacheTagRam"));
	listOfXepathName.push_back(std::string("CachInstDataRam"));
	listOfXepathName.push_back(std::string("CachInstTagRam"));
	listOfXepathName.push_back(std::string("ScmAndMainRam"));

	listOfXepathName.push_back(std::string("Cmd1ScmWofPlot"));
	listOfXepathName.push_back(std::string("Cmd2ScmWofBin"));
	listOfXepathName.push_back(std::string("Cmd3SweepTag"));

	listOfXepathName.push_back(std::string("WofCmd1"));
	listOfXepathName.push_back(std::string("WofCmd2"));
	listOfXepathName.push_back(std::string("WofCmd3"));
	listOfXepathName.push_back(std::string("WofCmd4"));
	listOfXepathName.push_back(std::string("PixelPatch"));
	listOfXepathName.push_back(std::string("SpiFlashRamPatch"));
	listOfXepathName.push_back(std::string("BulkEraseCmd"));
	listOfXepathName.push_back(std::string("ProgramCmd"));
	listOfXepathName.push_back(std::string("ReadCmd"));
	listOfXepathName.push_back(std::string("WovarPatch"));
	listOfXepathName.push_back(std::string("VCEK_IV"));
	listOfXepathName.push_back(std::string("WofLowPowerBin"));
	listOfXepathName.push_back(std::string("ProdOtpReadWritePatch"));
	listOfXepathName.push_back(std::string("SecurityMgtEngrPatch"));
	listOfXepathName.push_back(std::string("SecurityMgtProdPatch"));
	listOfXepathName.push_back(std::string("TakeOwnershipBin"));

	std::vector<Syn_XepatchInfo> listofXepatchInfo;
	for (auto i = 0; i < listOfXepathName.size(); i++)
	{
		Syn_XepatchInfo CurrentSyn_XepatchInfo;

		std::string strXepatchName(listOfXepathName[i]);

		QString qstrXepatchName(QString::fromStdString(strXepatchName)), qstrXepatchTextValue("");
		QString qstrXepatchArgsName("Args"), qstrXepatchDisplayName("");

		rc = pSysConfigOperation->GetElementNodeTextAndAttribute(qstrXepatchName, qstrXepatchTextValue, qstrXepatchArgsName, qstrXepatchDisplayName);

		std::string strXepatchTextValue(qstrXepatchTextValue.toStdString());
		int iXepatchTextLenth = strXepatchTextValue.length();
		if (0 == iXepatchTextLenth)
		{
			CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
			CurrentSyn_XepatchInfo._strXepatchFileName = qstrXepatchDisplayName.toStdString();
			CurrentSyn_XepatchInfo._pArrayBuf = NULL;
			CurrentSyn_XepatchInfo._uiArraySize = 0;
		}
		else
		{
			uint8_t *pArray = new uint8_t[iXepatchTextLenth / 2];
			pSysConfigOperation->ConvertAsciiToBinary(strXepatchTextValue, pArray, iXepatchTextLenth/2);

			CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
			CurrentSyn_XepatchInfo._strXepatchFileName = qstrXepatchDisplayName.toStdString();
			CurrentSyn_XepatchInfo._pArrayBuf = pArray;
			CurrentSyn_XepatchInfo._uiArraySize = iXepatchTextLenth/2;
		}

		listofXepatchInfo.push_back(CurrentSyn_XepatchInfo);
	}
	if (0 == listofXepatchInfo.size())
	{
		cout << "Error:ConstructSyn_SysConfig() - 6th operation is failed!" << endl;
		return false;
	}

	//Fill Syn_SysConfig
	oSyn_SysConfig._strAutoController = qstrAutoControllerValue.toStdString();
	oSyn_SysConfig._strDutType = qstrDutTypeValue.toStdString();
	oSyn_SysConfig._strDutController = qstrDutControllerValue.toStdString();
	oSyn_SysConfig._strDutCom = qstrDutComValue.toStdString();

	oSyn_SysConfig._uiDutpwrVdd_mV = (uint16_t)strtol(qstrDutPwrVdd_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVio_mV = (uint16_t)strtol(qstrDutPwrVio_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVled_mV = (uint16_t)strtol(qstrDutPwrVled_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVddh_mV = (uint16_t)strtol(qstrDutPwrVddh_mVValue.toStdString().c_str(), &p, 10);

	oSyn_SysConfig._uiNumRows = (uint16_t)strtol(qstrNumRowsValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiNumCols = (uint16_t)strtol(qstrNumColsValue.toStdString().c_str(), &p, 10);

	oSyn_SysConfig._bWriteBootSectors = (uint16_t)strtol(qstrWriteBootSectorsValue.toStdString().c_str(), &p, 10);
	pSysConfigOperation->ConvertAsciiToBinary(qstrBootSector0Value.toStdString(), oSyn_SysConfig._arrUserSpecifiedBS0, BS0_SIZE);
	pSysConfigOperation->ConvertAsciiToBinary(qstrBootSector1Value.toStdString(), oSyn_SysConfig._arrUserSpecifiedBS1, BS1_SIZE);
	
	oSyn_SysConfig._listTestSteps = listOfTestSteps;

	oSyn_SysConfig._listXepatchInfo = listofXepatchInfo;

	delete pSysConfigOperation;
	pSysConfigOperation = NULL;

	return true;
}