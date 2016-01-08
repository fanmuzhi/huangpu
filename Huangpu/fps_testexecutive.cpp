//local
#include "fps_testexecutive.h"
#include "Syn_LocalSettingConfig.h"

//Qt
#include <QtWidgets>

FPS_TestExecutive::FPS_TestExecutive(QWidget *parent)
: QMainWindow(parent)
, _bStopTag(true)
, _iRealDeviceCounts(0)
, _logfile("sys.log")
{
	ui.setupUi(this);

	_ListOfSitePtr.clear();
	
	//slots
	QObject::connect(ui.ConfigFileSelectPushButton, SIGNAL(clicked()), this, SLOT(SelectConfigFile()));
	QObject::connect(ui.pushButtonRun, SIGNAL(clicked()), this, SLOT(RunningTest()));

	for (int i = 1; i <= DeviceCounts; i++)
	{
		QObject::connect(&(_SynThreadArray[i - 1]), SIGNAL(send(void*)), this, SLOT(ReceiveOTPTestSlot(void*)));
	}

	//_logfile = std::ofstream("sys.log");
	std::cout.rdbuf(_logfile.rdbuf());

	//Test
	/*Syn_LocalSettingConfig *pInstance = NULL;
	bool rc = Syn_LocalSettingConfig::CreateLSConfigInstance(pInstance);
	if (rc&&NULL != pInstance)
	{
		Syn_LocalSettings testLoaclSettings;

		rc = pInstance->GetLocalSettings(testLoaclSettings);

		ui.textBrowser->append(QString::fromStdString(testLoaclSettings._strSysConfigFilePath));

		for (size_t a = 1; a <= testLoaclSettings._listOfSiteSettings.size(); a++)
		{
			ui.textBrowser->append(QString("Serial Number:")+QString::number((testLoaclSettings._listOfSiteSettings)[a - 1]._uiDutSerNum));

			AdcBaseLineInfo CurrentAdcBaseLineInfo = (testLoaclSettings._listOfSiteSettings)[a - 1]._adcBaseLineInfo;
			QString strVoltagesAll = QString::number(CurrentAdcBaseLineInfo.m_nVdd) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVio) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVled) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVddh);
			ui.textBrowser->append(QString("Voltages:") + strVoltagesAll);

			QString strAdcBaseLinesValue("");
			for (int a = 0; a < NUM_CURRENT_VALUES; a++)
			{
				for (int b = 0; b < KNUMGAINS; b++)
				{
					strAdcBaseLinesValue += QString::number((CurrentAdcBaseLineInfo.m_arAdcBaseLines)[a][b]) + QString(" ");
				}
			}
			ui.textBrowser->append(QString("AdcBaseLines:") + strAdcBaseLinesValue);
		}


		ui.textBrowser->append(QString::fromStdString(testLoaclSettings._strAutoController));

		delete pInstance;
		pInstance = NULL;
	}*/
}

FPS_TestExecutive::~FPS_TestExecutive()
{
	_ListOfSitePtr.clear();

	_logfile.close();
}

bool FPS_TestExecutive::Init(QString strConfigFile)
{
	bool rc(false);
	//ui.textBrowser->append("Hello World");

	cout << "Init!" << endl;

	QFile ConfigFile(strConfigFile);
	if (!ConfigFile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("Config File is not exists!"));
		return false;
	}

	//Parse SysConfig File(xml)
	SysConfig *pSysConfigOperation = NULL;
	rc = SysConfig::CreateSysConfigInstance(strConfigFile, pSysConfigOperation);
	if (!rc || NULL == pSysConfigOperation)
	{
		QMessageBox::critical(this, QString("Error"), QString("Create SysConfig Instance is failed!"));
		cout << "Error:ConstructSyn_SysConfig() - ::CreateSysConfigInstance is failed!" << endl;
		return false;
	}
	Syn_SysConfig synSysConfig;
	rc = pSysConfigOperation->GetSyn_SysConfig(synSysConfig);
	delete pSysConfigOperation;
	pSysConfigOperation = NULL;
	if (!rc)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't parse the config file,check it please!"));
		return false;
	}

	//clear
	if (0 != _ListOfSitePtr.size())
	{
		for (size_t i = _ListOfSitePtr.size(); i >= 1; i--)
		{
			if (NULL != _ListOfSitePtr[i - 1])
			{
				delete _ListOfSitePtr[i - 1];
				_ListOfSitePtr[i - 1] = NULL;
			}
		}
	}
	_ListOfSitePtr.clear();
	rc = Syn_Site::ConstructSiteList(synSysConfig, _ListOfSitePtr);
	size_t ilistCounts = _ListOfSitePtr.size();
	if (0 == ilistCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't construct the Site list,check it please!"));
		return false;
	}
	for (size_t i = 0; i < ilistCounts; i++)
	{
		_SynThreadArray[i].SetSite(_ListOfSitePtr[i]);
		_SynThreadArray[i].SetStopTag(true);
	}
	_iRealDeviceCounts = ilistCounts;

	//cout << "end!" << endl;

	return true;
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
		//ui.ConfigFileLineEdit->setText(strConfigFilePath);

		bool rc = Init(strConfigFilePath);
		if (rc)
		{
			//QMessageBox::information(this, QString("OK"), QString("Construct the Site list succeed!"));
			ui.ConfigFileLineEdit->setText(strConfigFilePath);
			ui.textBrowser->clear();
		}
	}
	
}

void FPS_TestExecutive::RunningTest()
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
			//ui.pushButtonRun->setText(QString("Run"));
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

	//ui.textBrowser->clear();

	ui.textBrowser->append(QString("Boot Sector0:"));
	for (int i = 1; i <= BS0_SIZE / 8; i++)
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

	ui.textBrowser->append(QString("Main Sector0:"));
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
	}
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



