//local
#include "fps_testexecutive.h"
#include "ui_Syn_LocalSettingsDlg.h"
//Qt
#include <QtWidgets>

#include <time.h>
#include <stdio.h>
#include <io.h>

FPS_TestExecutive::FPS_TestExecutive(QWidget *parent)
: QMainWindow(parent)
, _bStopTag(true)
, _iRealDeviceCounts(0)
//, _logfile("sys.log")
, _pSyn_LocalSettingsDlg(NULL)
, _pSyn_SerialNumberManageDlg(NULL)
, _pSyn_UpdateADCOffsetsDlg(NULL)
, _pSyn_BinCodesDlg(NULL)
, _pSyn_DeviceManager(NULL)
, _FinishedSiteCounts(0)
, m_MenuBar(NULL)
{
	//ui setting
	ui.setupUi(this);
	ui.TestTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//ui.TestTableWidget->setRowHeight(5, 200);
	ui.TestTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.TestTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	//ui.TestTableWidget->verticalHeader()->setStretchLastSection(true);

	//Init
	Initialize();

	//slots
	//Exit
	QObject::connect(ui.actionExit, SIGNAL(triggered(bool)), this, SLOT(Exit()));

	//Create LocalSettings Dialog
	QObject::connect(ui.actionLocalSetttings, SIGNAL(triggered(bool)), this, SLOT(CreateLocalSettings()));

	//Testing Operation
	//QObject::connect(ui.pushButtonRun, SIGNAL(clicked()), this, SLOT(RunningTest()));
	QObject::connect(ui.pushButtonRun, SIGNAL(clicked()), this, SLOT(Run()));

	//Thread
	for (int i = 1; i <= DeviceCounts; i++)
	{
		QObject::connect(&(_SynThreadArray[i - 1]), SIGNAL(send(unsigned int, const QString, const QString)), this, SLOT(ReceiveTest(unsigned int, const QString, const QString)));
		QObject::connect(&(_SynThreadArray[i - 1]), SIGNAL(send(unsigned int)), this, SLOT(ReceiveSiteInfo(unsigned int)));
	}

	//BinCodes Dislpay
	QObject::connect(ui.actionBinCodes, SIGNAL(triggered(bool)), this, SLOT(DisplayBinCodes()));
	
	//Debug
	//OTP Dump
	QObject::connect(ui.pushButtonGetVer, SIGNAL(clicked()), this, SLOT(GetVersionForDutDump()));
	QObject::connect(ui.pushButtonReadOTP, SIGNAL(clicked()), this, SLOT(ReadOTPForDutDump()));

	//Calibration,Fingerprint
	QObject::connect(ui.ImageCalibrationPushButton, SIGNAL(clicked()), this, SLOT(PushCablicationImageButton()));
	QObject::connect(ui.FigerprintImagePushButton, SIGNAL(clicked()), this, SLOT(PushFigerprintImageButton())); 

	_threadForDebug._dbgType = getImageType;
	QObject::connect(&(_threadForDebug), SIGNAL(send(unsigned int)), this, SLOT(FigerprintImage(unsigned int)));
	_threadForDebugCalibrate._dbgType = calibrateType;
	QObject::connect(&(_threadForDebugCalibrate), SIGNAL(send(unsigned int)), this, SLOT(ImageCalibration(unsigned int)));
	
}

FPS_TestExecutive::~FPS_TestExecutive()
{
	if (0 != _ListOfSitePtr.size())
	{
		for (size_t i = _ListOfSitePtr.size(); i >= 1; i--)
		{
			delete _ListOfSitePtr[i - 1];
			_ListOfSitePtr[i - 1] = NULL;
		}
		_ListOfSitePtr.clear();
	}

	if (NULL != _pSyn_LocalSettingsDlg)
	{
		delete _pSyn_LocalSettingsDlg;
		_pSyn_LocalSettingsDlg = NULL;
	}

	if (NULL != _pSyn_SerialNumberManageDlg)
	{
		delete _pSyn_SerialNumberManageDlg;
		_pSyn_SerialNumberManageDlg = NULL;
	}

	if (NULL != _pSyn_UpdateADCOffsetsDlg)
	{
		delete _pSyn_UpdateADCOffsetsDlg;
		_pSyn_UpdateADCOffsetsDlg = NULL;
	}

	if (NULL != _pSyn_BinCodesDlg)
	{
		delete _pSyn_BinCodesDlg;
		_pSyn_BinCodesDlg = NULL;
	}

	if (NULL != _pSyn_DeviceManager)
	{
		delete _pSyn_DeviceManager;
		_pSyn_DeviceManager = NULL;
	}
}

void FPS_TestExecutive::Initialize()
{
	bool rc(false);

	//LocalSettings
	Syn_LocalSettingConfig *pSyn_LocalSettingConfig = NULL;
	rc = Syn_LocalSettingConfig::CreateLSConfigInstance(pSyn_LocalSettingConfig);
	if (!rc || NULL == pSyn_LocalSettingConfig)
	{
		cout << "Error:FPS_TestExecutive::Initialize() - pSyn_LocalSettingConfig is NULL!" << endl;
		return;
	}

	rc = pSyn_LocalSettingConfig->GetLocalSettings(_LocalSettingsInfo);

	unsigned int iLocalSettingSiteCounts = _LocalSettingsInfo._listOfSiteSettings.size();
	if (0 == iLocalSettingSiteCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Local Settings' Site info is NULL,check it please!"));
		return;
	}
	
	delete pSyn_LocalSettingConfig;
	pSyn_LocalSettingConfig = NULL;

	//Config file & SiteList
	QString strConfigFile = QString::fromStdString(_LocalSettingsInfo._strSysConfigFilePath);
	rc = ConstructSiteList(_LocalSettingsInfo);
	if (!rc)
	{
		cout << "Error:FPS_TestExecutive::Initialize() - ::ConstructSiteList() is failed!" << endl;
		return;
	}
}

void FPS_TestExecutive::Exit()
{
	QCoreApplication::exit();
}

bool FPS_TestExecutive::ConstructSiteList(const Syn_LocalSettings &LocalSettingsInfo)
{
	uint32_t rc = 0;

	//Config file
	std::string strConfigFilePath = LocalSettingsInfo._strSysConfigFilePath;
	QFile ConfigFile(QString::fromStdString(strConfigFilePath));
	if (!ConfigFile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't find config file,check it please!"));
		return false;
	}

	unsigned int iLocalSettingsSiteCounts = LocalSettingsInfo._listOfSiteSettings.size();
	if (0 == iLocalSettingsSiteCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't retrieve Site info in LocalSettings,check it please!"));
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
		_ListOfSitePtr.clear();
	}

	bool IsSucess(true);
	for (size_t i = 0; i < iLocalSettingsSiteCounts; i++)
	{
		uint32_t uiSerialNumber = LocalSettingsInfo._listOfSiteSettings[i]._uiDutSerNum;
		uint8_t uiSiteNumber = i + 1;
		Syn_Site *pSyn_SiteInstance = NULL; 
		rc = Syn_Site::CreateSiteInstance(uiSiteNumber, uiSerialNumber, _LocalSettingsInfo._strSysConfigFilePath, LocalSettingsInfo._listOfSiteSettings[i]._adcBaseLineInfo, pSyn_SiteInstance);
		if (NULL == pSyn_SiteInstance || Syn_ExceptionCode::Syn_OK != rc)
		{
			QMessageBox::critical(this, QString("Error"), QString("Can't cosntruct Serial Number:") + QString::number(uiSerialNumber) + QString(" device,check it please!"));
			IsSucess = false;
			break;
		}
		
		_ListOfSitePtr.push_back(pSyn_SiteInstance);
	}

	if (!IsSucess)
	{
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
			_ListOfSitePtr.clear();
		}

		return false;
	}

	size_t ilistCounts = _ListOfSitePtr.size();
	if (0 == ilistCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't construct the Site list,check it please!"));
		return false;
	}

	if (DeviceCounts < ilistCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Current Devices' counts is more than limit,the limit is ") + QString::number(DeviceCounts) + ("!"));
		ilistCounts = DeviceCounts;
	}

	for (size_t i = 0; i < ilistCounts; i++)
	{
		_SynThreadArray[i].SetSite(_ListOfSitePtr[i]);
		_SynThreadArray[i].SetStopTag(true);
	}
	_iRealDeviceCounts = ilistCounts;

	//Site TableWidget
	for (int t = ui.TestTableWidget->columnCount(); t >= 1; t--)
	{
		ui.TestTableWidget->removeColumn(t - 1);
	}

	ui.TestTableWidget->setColumnCount(_iRealDeviceCounts);
	QStringList strListOfHeader;
	for (size_t t = 1; t <= _iRealDeviceCounts; t++)
	{
		uint32_t uiSerialNumber(0);
		_ListOfSitePtr[t - 1]->GetSerialNumber(uiSerialNumber);
		unsigned int iSiteNumber(0);
		_ListOfSitePtr[t - 1]->GetSiteNumber(iSiteNumber);

		QString strSiteStatus("");
		Syn_Site::SiteState oSiteState;
		_ListOfSitePtr[t - 1]->GetState(oSiteState);
		switch (oSiteState)
		{
			case Syn_Site::Idle:
				strSiteStatus = "Idle";
				break;
			case Syn_Site::TestDataReady:
				strSiteStatus = "TestData Ready";
				break;
			case Syn_Site::Running:
				strSiteStatus = "Running";
				break;
			case Syn_Site::Error:
				strSiteStatus = "Error";
				break;
			case Syn_Site::Closed:
				strSiteStatus = "Closed";
				break;
			default:
				strSiteStatus = "Error";
				break;
		}
		
		QTableWidgetItem *itemSiteNumber = new QTableWidgetItem(QString::number(iSiteNumber) + QString(" (") + QString::number(uiSerialNumber) + QString(") "));
		QTableWidgetItem *itemSiteStatus = new QTableWidgetItem(strSiteStatus);
		itemSiteNumber->setTextAlignment(Qt::AlignCenter);
		itemSiteStatus->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(0, t - 1, itemSiteNumber);
		ui.TestTableWidget->setItem(2, t - 1, itemSiteStatus);

		/*QTableWidgetItem *cubesHeaderItem = new QTableWidgetItem(tr("Cubes"));
		cubesHeaderItem->setIcon(QIcon(QPixmap(":/Images/cubed.png")));
		cubesHeaderItem->setTextAlignment(Qt::AlignVCenter);*/

		ui.TestTableWidget->setColumnWidth(t - 1, 200);

		strListOfHeader.append(QString("#") + QString::number(iSiteNumber));
	}
	ui.TestTableWidget->setHorizontalHeaderLabels(strListOfHeader);

	//Debug,not release
	//DutDump
	ui.comboBox->clear();
	ui.textBrowser->clear();
	for (size_t i = 1; i <= _ListOfSitePtr.size(); i++)
	{
		unsigned int iSiteNumber(0);
		_ListOfSitePtr[i - 1]->GetSiteNumber(iSiteNumber);
		ui.comboBox->addItem(QString("#") + QString::number(iSiteNumber));
	}

	//Calibration,Fingerprint
	ui.ImageSiteComboBox->clear();
	for (size_t i = 1; i <= _ListOfSitePtr.size(); i++)
	{
		unsigned int iSiteNumber(0);
		_ListOfSitePtr[i - 1]->GetSiteNumber(iSiteNumber);
		ui.ImageSiteComboBox->addItem(QString("#") + QString::number(iSiteNumber));
	}

	//_pSyn_DeviceManager->UpdateFirmware(_ListOfSitePtr);

	return true;
}

void FPS_TestExecutive::CreateLocalSettings()
{
	if (NULL != _pSyn_LocalSettingsDlg)
	{
		delete _pSyn_LocalSettingsDlg;
		_pSyn_LocalSettingsDlg = NULL;
	}
	_pSyn_LocalSettingsDlg = new Syn_LocalSettingsDlg();
	_pSyn_LocalSettingsDlg->show();

	//localSettings slots
	QObject::connect(_pSyn_LocalSettingsDlg->ui->CancelPushButton, SIGNAL(clicked()), this, SLOT(CloseLocalSettingsDialog()));
	QObject::connect(_pSyn_LocalSettingsDlg->ui->SelectSysConfigFilePushButton, SIGNAL(clicked()), this, SLOT(SelectConfigFile()));
	QObject::connect(_pSyn_LocalSettingsDlg->ui->SelectLogFilePushButton, SIGNAL(clicked()), this, SLOT(SelectLogFilePath()));
	QObject::connect(_pSyn_LocalSettingsDlg->ui->SiteCountsLineEdit, SIGNAL(editingFinished()), this, SLOT(ModifySiteCounts()));//returnPressed
	QObject::connect(_pSyn_LocalSettingsDlg->ui->ModifySerialNumberPushButton, SIGNAL(clicked()), this, SLOT(ModifySerialNumber()));
	QObject::connect(_pSyn_LocalSettingsDlg->ui->UpdateADCOffsetsPushButton, SIGNAL(clicked()), this, SLOT(CreateUpdateADCOffsetsDlg()));
	QObject::connect(_pSyn_LocalSettingsDlg->ui->SiteTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(SetLeds(int, int)));

	QObject::connect(_pSyn_LocalSettingsDlg->ui->OKPushButton, SIGNAL(clicked()), this, SLOT(ConfirmSite()));

	if (0 != _ListOfSitePtr.size())
	{
		for (size_t i = _ListOfSitePtr.size(); i >= 1; i--)
		{
			delete _ListOfSitePtr[i - 1];
			_ListOfSitePtr[i - 1] = NULL;
		}
		_ListOfSitePtr.clear();
	}

	//operation
	_pSyn_LocalSettingsDlg->ui->SysConfigFileLlineEdit->clear();
	_pSyn_LocalSettingsDlg->ui->SysConfigFileLlineEdit->setText(QString::fromStdString(_LocalSettingsInfo._strSysConfigFilePath));

	_pSyn_LocalSettingsDlg->ui->LogFileLineEdit->clear();
	_pSyn_LocalSettingsDlg->ui->LogFileLineEdit->setText(QString::fromStdString(_LocalSettingsInfo._strLogFilePath));


	unsigned int iLocalSettingSiteCounts = _LocalSettingsInfo._listOfSiteSettings.size();
	_pSyn_LocalSettingsDlg->ui->SiteCountsLineEdit->clear();
	_pSyn_LocalSettingsDlg->ui->SiteCountsLineEdit->setText(QString::number(iLocalSettingSiteCounts));
	if (0 == iLocalSettingSiteCounts)
	{
		return;
	}

	_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setRowCount(iLocalSettingSiteCounts);
	for (size_t i = 0; i < iLocalSettingSiteCounts; i++)
	{
		AdcBaseLineInfo CurrentAdcBaseLineInfo = (_LocalSettingsInfo._listOfSiteSettings)[i]._adcBaseLineInfo;
		QString strVoltagesAll = QString::number(CurrentAdcBaseLineInfo.m_nVdd) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVio) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVled) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVddh);

		QString strAdcBaseLinesValue("");
		for (int a = 0; a < NUM_CURRENT_VALUES; a++)
		{
			for (int b = 0; b < KNUMGAINS; b++)
			{
				strAdcBaseLinesValue += QString::number((CurrentAdcBaseLineInfo.m_arAdcBaseLines)[a][b]) + QString(" ");
			}
		}

		QString strSiteNumber(QString::number(i+1));
		QString strSerialNumber(QString::number((_LocalSettingsInfo._listOfSiteSettings)[i]._uiDutSerNum));

		_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(i, 0, new QTableWidgetItem(strSiteNumber));
		_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(i, 1, new QTableWidgetItem(strSerialNumber));
		_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(i, 2, new QTableWidgetItem(strAdcBaseLinesValue));

		_TempVoltagesValue.nVdd = CurrentAdcBaseLineInfo.m_nVdd;
		_TempVoltagesValue.nVio = CurrentAdcBaseLineInfo.m_nVio;
		_TempVoltagesValue.nVled = CurrentAdcBaseLineInfo.m_nVled;
		_TempVoltagesValue.nVddh = CurrentAdcBaseLineInfo.m_nVddh;
	}

	if (NULL == _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager = new Syn_DeviceManager();
		return;
	}
	uint32_t uiResult = _pSyn_DeviceManager->Open();
}

void FPS_TestExecutive::CloseLocalSettingsDialog()
{
	if (NULL != _pSyn_LocalSettingsDlg)
	{
		_pSyn_LocalSettingsDlg->hide();
		delete _pSyn_LocalSettingsDlg;
		_pSyn_LocalSettingsDlg = NULL;
	}

	if (NULL != _pSyn_SerialNumberManageDlg)
	{
		_pSyn_SerialNumberManageDlg->hide();
		delete _pSyn_SerialNumberManageDlg;
		_pSyn_SerialNumberManageDlg = NULL;
	}

	if (NULL != _pSyn_UpdateADCOffsetsDlg)
	{
		_pSyn_UpdateADCOffsetsDlg->hide();
		delete _pSyn_UpdateADCOffsetsDlg;
		_pSyn_UpdateADCOffsetsDlg = NULL;
	}

	if (NULL != _pSyn_DeviceManager)
	{
		delete _pSyn_DeviceManager;
		_pSyn_DeviceManager = NULL;
	}

	ConstructSiteList(_LocalSettingsInfo);
}

void FPS_TestExecutive::SelectConfigFile()
{
	QString strConfigFilePath = QFileDialog::getOpenFileName(_pSyn_LocalSettingsDlg, "Select Config File", "./", "xml file(*.xml)");
	if (QString("") != strConfigFilePath)
	{
		QFile TempFile(strConfigFilePath);
		if (!TempFile.exists())
			return;

		_pSyn_LocalSettingsDlg->ui->SysConfigFileLlineEdit->clear();
		_pSyn_LocalSettingsDlg->ui->SysConfigFileLlineEdit->setText(strConfigFilePath);

		_LocalSettingsInfo._strSysConfigFilePath = strConfigFilePath.toStdString();
	}
}

void FPS_TestExecutive::SelectLogFilePath()
{
	QString strLogFileFolderPath = QFileDialog::getExistingDirectory(_pSyn_LocalSettingsDlg, "Select LogFile Path");
	if (QString("") != strLogFileFolderPath)
	{
		QFile TempFile(strLogFileFolderPath);
		if (!TempFile.exists())
			return;

		_pSyn_LocalSettingsDlg->ui->LogFileLineEdit->clear();
		_pSyn_LocalSettingsDlg->ui->LogFileLineEdit->setText(strLogFileFolderPath);

		_LocalSettingsInfo._strLogFilePath = strLogFileFolderPath.toStdString();
	}
}

void FPS_TestExecutive::ModifySiteCounts()
{
	if (NULL == _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager = new Syn_DeviceManager();
	}

	uint32_t rc = _pSyn_DeviceManager->Open();
	if (Syn_ExceptionCode::Syn_OK != rc)
	{
		QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Can't Open Device Manage,check it please!"));
		return;
	}

	QString strUserSiteCounts = _pSyn_LocalSettingsDlg->ui->SiteCountsLineEdit->text();
	int iUserSiteCounts = strUserSiteCounts.toInt();

	std::vector<uint32_t> listOfSerialNumber;
	_pSyn_DeviceManager->GetSerialNumberList(listOfSerialNumber);
	if (iUserSiteCounts > listOfSerialNumber.size())
	{
		QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Current Site Numbers is more than real Device conunts(") + QString::number(listOfSerialNumber.size()) + QString(")!"));
		return;
	}

	int iCurrentRowCounts = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->rowCount();
	if (iCurrentRowCounts < iUserSiteCounts)
	{
		for (unsigned int i = iCurrentRowCounts+1; i <= iUserSiteCounts; i++)
		{
			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->insertRow(i-1);
			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(i - 1, 0, new QTableWidgetItem(QString::number(i)));
			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(i - 1, 1, new QTableWidgetItem(QString("0")));
			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(i - 1, 2, new QTableWidgetItem("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "));
		}
	}
	else if (iCurrentRowCounts > iUserSiteCounts)
	{
		for (unsigned int i = iCurrentRowCounts; i > iUserSiteCounts; i--)
		{
			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->removeRow(i - 1);
		}
	}
}

void FPS_TestExecutive::ConfirmSite()
{
	bool rc(false);

	if (NULL == _pSyn_LocalSettingsDlg)
	{
		return;
	}

	//check Site Info first
	int iUserDefineSiteCounts = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->rowCount();
	if (0 == iUserDefineSiteCounts)
	{
		QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Current Site counts is 0!"));
		return;
	}

	std::vector<uint32_t> listOfCurrentSerialNumber;
	for (int i = 0; i < iUserDefineSiteCounts; i++)
	{
		QString strSerialNumber("");
		QString strAdcValue("");

		QTableWidgetItem *pSerialNumberItem = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->item(i, 1);
		if (NULL != pSerialNumberItem)
			strSerialNumber = pSerialNumberItem->text();
		QTableWidgetItem *pAdcValueItem = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->item(i, 2);
		if (NULL != pAdcValueItem)
			strAdcValue = pAdcValueItem->text();

		uint32_t uiSerialNumber = strSerialNumber.toInt();
		if (0 == uiSerialNumber || QString("") == strSerialNumber)
		{
			QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Site ")+QString::number(i+1)+QString("'s SerialNumber is 0!"));
			return;
		}

		bool IsExists(false);
		for (int j = 1; j <= listOfCurrentSerialNumber.size(); j++)
		{
			if (uiSerialNumber == listOfCurrentSerialNumber[j - 1])
			{
				IsExists = true;
				break;
			}
		}
		if (IsExists)
		{
			QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Site ") + QString::number(i + 1) + QString("'s SerialNumber has been used!"));
			return;
		}

		listOfCurrentSerialNumber.push_back(uiSerialNumber);
	}

	//update LocalSettings info
	QString strConfigFilePath = _pSyn_LocalSettingsDlg->ui->SysConfigFileLlineEdit->text();
	QFile configfile(strConfigFilePath);
	if (!configfile.exists())
	{
		QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Config file is not exists,check it please!"));
		return;
	}
	_LocalSettingsInfo._strSysConfigFilePath = strConfigFilePath.toStdString();

	QString strLogFilePath = _pSyn_LocalSettingsDlg->ui->LogFileLineEdit->text();
	QFile Logfile(strLogFilePath);
	if (!Logfile.exists())
	{
		QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("LogFile Path is not exists,check it please!"));
		return;
	}
	_LocalSettingsInfo._strLogFilePath = strLogFilePath.toStdString();

	/*_LocalSettingsInfo.m_bVerboseMode = _pSyn_LocalSettingsDlg->ui->VerboseLogCheckBox->isChecked();
	_LocalSettingsInfo.m_bQAMode = _pSyn_LocalSettingsDlg->ui->QAModeCheckBox->isChecked();
	_LocalSettingsInfo.m_bLGAMode = _pSyn_LocalSettingsDlg->ui->LGAModecheckBox->isChecked();
	_LocalSettingsInfo.m_bRunRepeatedly = _pSyn_LocalSettingsDlg->ui->AutoRepeatEnabledCheckBox->isChecked();
	_LocalSettingsInfo._strAutoController = _pSyn_LocalSettingsDlg->ui->AutoControllerComboBox->currentText().toStdString();*/

	//Set SiteInfo
	//fill current info to LocalSettings Config
	_LocalSettingsInfo._listOfSiteSettings.clear();

	for (int i = 0; i < iUserDefineSiteCounts; i++)
	{
		QString strSerialNumber("");
		QString strAdcValue("");
		QTableWidgetItem *pSerialNumberItem = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->item(i, 1);
		if (NULL != pSerialNumberItem)
			strSerialNumber = pSerialNumberItem->text();
		QTableWidgetItem *pAdcValueItem = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->item(i, 2);
		if (NULL != pAdcValueItem)
			strAdcValue = pAdcValueItem->text();

		uint32_t uiSerialNumber = strSerialNumber.toInt();

		SiteSettings CurrentSiteSettings;
		CurrentSiteSettings._uiDutSerNum = uiSerialNumber;

		AdcBaseLineInfo CurrentAdcBaseLineInfo;
		QStringList strListOfAdcBaseLinesValue = strAdcValue.split(QString(" "));
		if ((NUM_CURRENT_VALUES*KNUMGAINS) > strListOfAdcBaseLinesValue.size())
		{
			continue;
		}

		for (int a = 0; a < NUM_CURRENT_VALUES; a++)
		{
			for (int b = 0; b < KNUMGAINS; b++)
			{
				(CurrentAdcBaseLineInfo.m_arAdcBaseLines)[a][b] = strListOfAdcBaseLinesValue[a + (NUM_CURRENT_VALUES)*b].toInt();
			}
		}

		//need modify...................................................................................
		CurrentAdcBaseLineInfo.m_nVdd = _TempVoltagesValue.nVdd;
		CurrentAdcBaseLineInfo.m_nVio = _TempVoltagesValue.nVio;
		CurrentAdcBaseLineInfo.m_nVled = _TempVoltagesValue.nVled;
		CurrentAdcBaseLineInfo.m_nVddh = _TempVoltagesValue.nVddh;

		CurrentSiteSettings._adcBaseLineInfo = CurrentAdcBaseLineInfo;
		_LocalSettingsInfo._listOfSiteSettings.push_back(CurrentSiteSettings);
	}

	//save
	Syn_LocalSettingConfig *pLocalSettingsInstance = NULL;
	rc = Syn_LocalSettingConfig::CreateLSConfigInstance(pLocalSettingsInstance);
	if (rc&&NULL != pLocalSettingsInstance)
	{
		pLocalSettingsInstance->SetLocalSettings(_LocalSettingsInfo);
		delete pLocalSettingsInstance;
		pLocalSettingsInstance = NULL;
	}

	_pSyn_DeviceManager->UpdateFirmware();

	//ReConstruct SiteList
	if (ConstructSiteList(_LocalSettingsInfo))
	{
		_pSyn_LocalSettingsDlg->hide();
		delete _pSyn_LocalSettingsDlg;
		_pSyn_LocalSettingsDlg = NULL;

		if (NULL != _pSyn_SerialNumberManageDlg)
		{
			delete _pSyn_SerialNumberManageDlg;
			_pSyn_SerialNumberManageDlg = NULL;
		}

		if (NULL != _pSyn_UpdateADCOffsetsDlg)
		{
			_pSyn_UpdateADCOffsetsDlg->hide();
			delete _pSyn_UpdateADCOffsetsDlg;
			_pSyn_UpdateADCOffsetsDlg = NULL;
		}

		if (NULL != _pSyn_DeviceManager)
		{
			delete _pSyn_DeviceManager;
			_pSyn_DeviceManager = NULL;
		}
	}

}

void FPS_TestExecutive::SetLeds(int rowNumber, int columnNumber)
{
	//get SerialNumber
	int iSerialNumberColumn(1);
	QTableWidgetItem *item = NULL;
	item = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->item(rowNumber, iSerialNumberColumn);
	if (NULL == item)
		return;

	QString strSerialNumber("");
	strSerialNumber = item->text();
	uint32_t iSerialNumber = strSerialNumber.toInt();
	if (0 == iSerialNumber)
		return;

	if (NULL != _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager->SetLeds(iSerialNumber);
	}

}

void FPS_TestExecutive::ModifySerialNumber()
{
	if (NULL == _pSyn_LocalSettingsDlg)
	{
		return;
	}

	int iSelectRowIndex = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->currentRow();
	if (iSelectRowIndex < 0)
		iSelectRowIndex = 0;

	if (NULL != _pSyn_SerialNumberManageDlg)
	{
		delete _pSyn_SerialNumberManageDlg;
		_pSyn_SerialNumberManageDlg = NULL;
	}

	if (NULL == _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager = new Syn_DeviceManager();
	}

	uint32_t rc = _pSyn_DeviceManager->Open();
	std::vector<uint32_t> listOfSerialNumber;
	_pSyn_DeviceManager->GetSerialNumberList(listOfSerialNumber);
	if (0 == listOfSerialNumber.size())
	{
		QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Can't find device,check it please!"));
		return;
	}

	_pSyn_SerialNumberManageDlg = new Syn_SerialNumberManageDlg();
	_pSyn_SerialNumberManageDlg->show();

	//slots
	QObject::connect(_pSyn_SerialNumberManageDlg->ui.OKPushButton, SIGNAL(clicked()), this, SLOT(ConfirmSerialNumberForSite()));
	QObject::connect(_pSyn_SerialNumberManageDlg->ui.CancelPushButton, SIGNAL(clicked()), this, SLOT(CloseSiteManageDialog()));

	_pSyn_SerialNumberManageDlg->ui.SiteManagePromptLabel->setText(QString("Select Serial Number for Site ") + QString::number(iSelectRowIndex+1)+QString(":"));
	_pSyn_SerialNumberManageDlg->ui.SerialNumberTableWidget->setRowCount(listOfSerialNumber.size());
	for (size_t i = 1; i <= listOfSerialNumber.size(); i++)
	{
		_pSyn_SerialNumberManageDlg->ui.SerialNumberTableWidget->setItem(i - 1, 0, new QTableWidgetItem(QString::number(listOfSerialNumber[i-1])));
	}
}

void FPS_TestExecutive::ConfirmSerialNumberForSite()
{
	bool focus = _pSyn_SerialNumberManageDlg->ui.SerialNumberTableWidget->isItemSelected(_pSyn_SerialNumberManageDlg->ui.SerialNumberTableWidget->currentItem());
	if (!focus)
	{
		QMessageBox::critical(_pSyn_SerialNumberManageDlg, QString("Error"), QString("Select a SerialNumber for Site,please!"));
		return;
	}

	int iSerialNumberRowIndex = _pSyn_SerialNumberManageDlg->ui.SerialNumberTableWidget->currentRow();
	QString strSerialNumber = _pSyn_SerialNumberManageDlg->ui.SerialNumberTableWidget->item(iSerialNumberRowIndex, 0)->text();

	int iSiteRowIndex = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->currentRow();
	if (iSiteRowIndex < 0)
		iSiteRowIndex = 0;
	_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(iSiteRowIndex, 1, new QTableWidgetItem(strSerialNumber));
	//_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(iSiteRowIndex, 2, new QTableWidgetItem(QString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ")));

	_pSyn_SerialNumberManageDlg->hide();
	delete _pSyn_SerialNumberManageDlg;
	_pSyn_SerialNumberManageDlg = NULL;
}

void FPS_TestExecutive::CloseSiteManageDialog()
{
	if (NULL == _pSyn_SerialNumberManageDlg)
		return;

	_pSyn_SerialNumberManageDlg->hide();
	delete _pSyn_SerialNumberManageDlg;
	_pSyn_SerialNumberManageDlg = NULL;
}

void FPS_TestExecutive::CreateUpdateADCOffsetsDlg()
{
	if (NULL != _pSyn_UpdateADCOffsetsDlg)
	{
		_pSyn_UpdateADCOffsetsDlg->hide();
		delete _pSyn_UpdateADCOffsetsDlg;
		_pSyn_UpdateADCOffsetsDlg = NULL;
	}

	_pSyn_UpdateADCOffsetsDlg = new Syn_UpdateADCOffsetsDlg();
	_pSyn_UpdateADCOffsetsDlg->show();

	_pSyn_UpdateADCOffsetsDlg->ui.VddLineEdit->setText(QString("1800"));
	_pSyn_UpdateADCOffsetsDlg->ui.VioLineEdit->setText(QString("1800"));
	_pSyn_UpdateADCOffsetsDlg->ui.VledLineEdit->setText(QString("3300"));
	_pSyn_UpdateADCOffsetsDlg->ui.VddhLineEdit->setText(QString("3300"));

	QObject::connect(_pSyn_UpdateADCOffsetsDlg->ui.OKPushButton, SIGNAL(clicked()), this, SLOT(UpdateADCOffsets()));
	QObject::connect(_pSyn_UpdateADCOffsetsDlg->ui.CancelPushButton, SIGNAL(clicked()), this, SLOT(CloseUpdateADCOffsetsDialog()));
}

void FPS_TestExecutive::UpdateADCOffsets()
{
	if (NULL == _pSyn_UpdateADCOffsetsDlg)
	{
		return;
	}

	if (NULL == _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager = new Syn_DeviceManager();
	}
	_pSyn_DeviceManager->Open();

	QString strVdd = _pSyn_UpdateADCOffsetsDlg->ui.VddLineEdit->text();
	QString strVio = _pSyn_UpdateADCOffsetsDlg->ui.VioLineEdit->text();
	QString strVled = _pSyn_UpdateADCOffsetsDlg->ui.VledLineEdit->text();
	QString strVddh = _pSyn_UpdateADCOffsetsDlg->ui.VddhLineEdit->text();

	uint32_t uiVdd = strVdd.toInt();
	uint32_t uiVio = strVio.toInt();
	uint32_t uiVled = strVled.toInt();
	uint32_t uiVddh = strVddh.toInt();

	//if (uiVdd<0 || uiVdd>3300)

	int iSiteCounts = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->rowCount();
	if (0 == iSiteCounts)
	{
		return;
	}

	_TempVoltagesValue.nVdd = uiVdd;
	_TempVoltagesValue.nVio = uiVio;
	_TempVoltagesValue.nVled = uiVled;
	_TempVoltagesValue.nVddh = uiVddh;

	uint32_t rc = 0;
	for (int i = 0; i < iSiteCounts; i++)
	{
		QString strSerialNumber("");
		QTableWidgetItem *item = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->item(i, 1);
		if (NULL != item)
		{
			strSerialNumber = item->text();
			uint32_t uiSerialNumber = strSerialNumber.toInt();
			uint32_t arAdcBaseLines[NUM_CURRENT_VALUES][KNUMGAINS] = { 0 };

			rc = _pSyn_DeviceManager->UpdateADCOffsets(uiSerialNumber, uiVdd, uiVio, uiVled, uiVddh, arAdcBaseLines);
			if (Syn_ExceptionCode::Syn_OK == rc)
			{
				QString strAdcBaseLinesValue("");
				for (int a = 0; a < NUM_CURRENT_VALUES; a++)
				{
					for (int b = 0; b < KNUMGAINS; b++)
					{
						strAdcBaseLinesValue += QString::number(arAdcBaseLines[a][b]) + QString(" ");
					}
				}

				_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(i, 2, new QTableWidgetItem(strAdcBaseLinesValue));
			}
		}
	}

	_pSyn_UpdateADCOffsetsDlg->hide();
	delete _pSyn_UpdateADCOffsetsDlg;
	_pSyn_UpdateADCOffsetsDlg = NULL;
}

void FPS_TestExecutive::CloseUpdateADCOffsetsDialog()
{
	if (NULL != _pSyn_UpdateADCOffsetsDlg)
	{
		_pSyn_UpdateADCOffsetsDlg->hide();
		delete _pSyn_UpdateADCOffsetsDlg;
		_pSyn_UpdateADCOffsetsDlg = NULL;
	}
}


void FPS_TestExecutive::Run()
{
	unsigned int iSiteCounts = _ListOfSitePtr.size();
	if (0 == iSiteCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Site list is NULL,check it please!"));
		return;
	}

	_FinishedSiteCounts = 0;

	unsigned int iRunFlag(0);
	QString qText = ui.pushButtonRun->text();
	if (QString("Run") == qText)
	{
		//ui.LocalSettingsPushButton->setDisabled(true);

		iRunFlag = 1;

		for (int i = 1; i <= iSiteCounts; i++)
		{
			if (NULL != ui.TestTableWidget->item(1, i - 1))
				ui.TestTableWidget->takeItem(1, i - 1);
			if (NULL != ui.TestTableWidget->item(2, i - 1))
				ui.TestTableWidget->takeItem(2, i - 1);
			if (NULL != ui.TestTableWidget->item(3, i - 1))
				ui.TestTableWidget->takeItem(3, i - 1);
			if (NULL != ui.TestTableWidget->item(4, i - 1))
				ui.TestTableWidget->takeItem(4, i - 1);
			if (NULL != ui.TestTableWidget->item(5, i - 1))
				ui.TestTableWidget->takeItem(5, i - 1);
			if (NULL != ui.TestTableWidget->item(6, i - 1))
				ui.TestTableWidget->takeItem(6, i - 1);
			if (NULL != ui.TestTableWidget->cellWidget(7, i - 1))
				ui.TestTableWidget->removeCellWidget(7, i - 1);
			if (NULL != ui.TestTableWidget->cellWidget(8, i - 1))
				ui.TestTableWidget->removeCellWidget(8, i - 1);
		}

		ui.pushButtonRun->setDisabled(true);
	}
	else if (QString("Continue") == qText)
	{
		iRunFlag = 2;

		ui.pushButtonRun->setDisabled(true);
	}

	for (int i = 1; i <= iSiteCounts; i++)
	{
		if (!_SynThreadArray[i - 1].isRunning())
		{
			_SynThreadArray[i - 1].SetFlag(iRunFlag);
			_SynThreadArray[i - 1].start();
			_SynThreadArray[i - 1].SetStopTag(false);
		}
		else
		{
			//_SynThreadArray[i - 1].SetStopTag(true);

			//_synThread.terminate();
			//ui.pushButtonRun->setText(QString("Run"));
		}

		Syn_Site::SiteState oState;
		_ListOfSitePtr[i - 1]->GetState(oState);
		if (iRunFlag == 2)
		{
			if (Syn_Site::Error == oState)
			{
				_FinishedSiteCounts += 1;
			}
		}
	}
}

void FPS_TestExecutive::ReceiveSiteInfo(unsigned int iSiteNumber)
{
	bool synFind(false);

	unsigned int iFlag(0);
	QString qText = ui.pushButtonRun->text();
	if (QString("Run") == qText)
	{
		iFlag = 1;
	}
	else if (QString("Continue") == qText)
	{
		iFlag = 2;
	}

	_FinishedSiteCounts += 1;

	Syn_DutTestResult *pCurrentDutTestResult = NULL;
	//Syn_DutTestInfo *pCurrentDutTestInfo = NULL;
	Syn_SysConfig CurrentSysConfig;
	unsigned int iPos(0);
	for (size_t i = 0; i < _ListOfSitePtr.size(); i++)
	{
		unsigned int iTempSiteNumber(0);
		_ListOfSitePtr[i]->GetSiteNumber(iTempSiteNumber);
		if (iSiteNumber == iTempSiteNumber)
		{
			Syn_Site::SiteState oTempState;
			_ListOfSitePtr[i]->GetState(oTempState);
			if (oTempState == Syn_Site::Error)
			{
				//_FinishedSiteCounts += 1;
				string errMsg = "";
				_ListOfSitePtr[i]->GetRunTimeError(errMsg);
				//QMessageBox::information(this, QString("Error"), QString("Error:") + QString::fromStdString(errMsg));
				//ui.LocalSettingsPushButton->setEnabled(true);

				//BinCode
				_ListOfSitePtr[i]->GetTestResult(pCurrentDutTestResult);
				if (NULL != pCurrentDutTestResult)
				{
					if (0 != pCurrentDutTestResult->_binCodes.size())
					{
						QTableWidgetItem *itemBinCodes = new QTableWidgetItem(QString::fromStdString(pCurrentDutTestResult->_binCodes[0]));
						itemBinCodes->setTextAlignment(Qt::AlignCenter);
						ui.TestTableWidget->setItem(4, iSiteNumber - 1, itemBinCodes);
					}
				}
				
				//State
				QTableWidgetItem *itemState = new QTableWidgetItem(QString("Error"));
				itemState->setTextAlignment(Qt::AlignCenter);
				ui.TestTableWidget->setItem(2, iSiteNumber - 1, itemState);

				this->ManageButtonStatus(iFlag);

				return;
			}
			
			_ListOfSitePtr[i]->GetTestResult(pCurrentDutTestResult);
			//_ListOfSitePtr[i]->GetTestInfo(pCurrentDutTestInfo);
			_ListOfSitePtr[i]->GetSysConfig(CurrentSysConfig);
			iPos = i;
			synFind = true;
			break;
		}
	}

	if (!synFind || NULL == pCurrentDutTestResult)
		return;

	int rowNumber = CurrentSysConfig._uiNumRows;
	int columnNumber = CurrentSysConfig._uiNumCols;

	QVector<QRgb> vcolorTable;
	for (int i = 0; i < 256; i++)
	{
		vcolorTable.append(qRgb(i, i, i));
	}
	QByteArray data;
	//data.resize((rowNumber)*(columnNumber));
	unsigned int iRowNumber(0);
	if (1 == iFlag)
	{
		rowNumber = pCurrentDutTestResult->_acqImgNoFingerResult.iRealRowNumber;
		columnNumber = pCurrentDutTestResult->_acqImgNoFingerResult.iRealColumnNumber;
		data.resize((rowNumber)*(columnNumber));
		
		for (int m = 0; m < rowNumber; m++)
		{
			for (int n = 0; n < columnNumber; n++)
			{
				data[m*(columnNumber)+n] = (pCurrentDutTestResult->_acqImgNoFingerResult).arr_ImageFPSFrame.arr[m][n];
			}
		}

		iRowNumber = 7;
	}
	else if (2 == iFlag)
	{
		rowNumber = pCurrentDutTestResult->_acqImgFingerResult.iRealRowNumber;
		columnNumber = pCurrentDutTestResult->_acqImgFingerResult.iRealColumnNumber;
		data.resize((rowNumber)*(columnNumber));

		for (int m = 0; m < rowNumber; m++)
		{
			for (int n = 0; n < columnNumber; n++)
			{
				data[m*(columnNumber)+n] = (pCurrentDutTestResult->_acqImgFingerResult).arr_ImageFPSFrame.arr[m][n];
			}
		}

		iRowNumber = 8;

		QString strSNRValue = QString::number(pCurrentDutTestResult->_snrResults.SNR[6]);
		
		QTableWidgetItem *itemSNR = new QTableWidgetItem(strSNRValue);
		itemSNR->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(5, iSiteNumber - 1, itemSNR);

		//BinCode
		QString qsBinCodes("");
		for (size_t i = 1; i <= pCurrentDutTestResult->_binCodes.size(); i++)
		{
			if (1 == i)
			{
				qsBinCodes += QString::fromStdString(pCurrentDutTestResult->_binCodes[i - 1]);
			}
			else
			{
				qsBinCodes = qsBinCodes + QString(" , ") + QString::fromStdString(pCurrentDutTestResult->_binCodes[i - 1]);
			}
		}
		QTableWidgetItem *itemBinCode = new QTableWidgetItem(qsBinCodes);
		itemBinCode->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(4, iSiteNumber - 1, itemBinCode);

		//TotalResults
		QString strTotalResults("");
		if (1 == pCurrentDutTestResult->_binCodes.size())
		{
			if (Syn_BinCodes::m_sPass == pCurrentDutTestResult->_binCodes[0])
			{
				strTotalResults = "Pass";
			}
			else
			{
				strTotalResults = "Fail";
			}
		}
		else
		{
			strTotalResults = "Fail";
		}
		QTableWidgetItem *itemTotalResults = new QTableWidgetItem(strTotalResults);
		itemTotalResults->setTextAlignment(Qt::AlignCenter);
		if (QString("Pass") == strTotalResults)
		{
			itemTotalResults->setBackgroundColor(QColor(0, 255, 0));
		}
		else
		{
			itemTotalResults->setBackgroundColor(QColor(255, 0, 0));
		}
		ui.TestTableWidget->setItem(6, iSiteNumber - 1, itemTotalResults);
	}
	
	QImage image((uchar*)data.constData(), columnNumber, rowNumber, QImage::Format_Indexed8);
	image.setColorTable(vcolorTable);
	//image = image.copy(iStartColumn, iStartRow, iEndColumn, iEndRow);

	QLabel *pImageLabel = new QLabel();
	pImageLabel->setPixmap(QPixmap::fromImage(image));
	pImageLabel->setAlignment(Qt::AlignCenter);
	ui.TestTableWidget->setCellWidget(iRowNumber, iSiteNumber - 1, pImageLabel); 
	//ui.TestTableWidget->cellWidget(iRowNumber, iSiteNumber - 1)->setStyle(QStyleFactory::create("Fusion"));

	ui.TestTableWidget->resizeRowToContents(iRowNumber);

	//_FinishedSiteCounts += 1;
	if (2 == iFlag)
	{
		//writelog
		Syn_DutTestInfo *pDutInfo = NULL;
		_ListOfSitePtr[iSiteNumber - 1]->GetTestInfo(pDutInfo);

		//WriteLog(_LocalSettingsInfo._strLogFilePath, pDutInfo, pCurrentDutTestResult, rowNumber, columnNumber);

		_ListOfSitePtr[iSiteNumber - 1]->Write_Log(_LocalSettingsInfo._strLogFilePath);
		_ListOfSitePtr[iSiteNumber - 1]->Close();

		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Closed"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(2, iSiteNumber - 1, itemState);
	}
	else
	{
		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Idle"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(2, iSiteNumber - 1, itemState);
	}

	this->ManageButtonStatus(iFlag);
}

void FPS_TestExecutive::ReceiveTest(unsigned int iSiteNumber, const QString strTestStepName, const QString strPassResults)
{
	bool synFind(false);

	unsigned int iFlag(0);
	QString qText = ui.pushButtonRun->text();
	if (QString("Run") == qText)
	{
		iFlag = 1;
	}
	else if (QString("Continue") == qText)
	{
		iFlag = 2;
	}

	Syn_SysConfig CurrentSysConfig;
	unsigned int iPos(0);
	for (size_t i = 0; i < _ListOfSitePtr.size(); i++)
	{
		unsigned int iTempSiteNumber(0);
		_ListOfSitePtr[i]->GetSiteNumber(iTempSiteNumber);
		if (iSiteNumber == iTempSiteNumber)
		{
			//_ListOfSitePtr[i]->GetTestInfo(*CurrentDutTestInfo);
			Syn_Site::SiteState oTempState;
			_ListOfSitePtr[i]->GetState(oTempState);
			if (oTempState == Syn_Site::Error)
			{
				string errMsg = "";
				_ListOfSitePtr[i]->GetRunTimeError(errMsg);
				//QMessageBox::information(this, QString("Error"), QString("Error:") + QString::fromStdString(errMsg));

				//State
				QTableWidgetItem *itemState = new QTableWidgetItem(QString("Error"));
				itemState->setTextAlignment(Qt::AlignCenter);
				ui.TestTableWidget->setItem(2, iSiteNumber - 1, itemState);

				return;
			}

			//_ListOfSitePtr[i]->GetTestResult(pCurrentDutTestResult);
			_ListOfSitePtr[i]->GetSysConfig(CurrentSysConfig);
			iPos = i;
			synFind = true;
			break;
		}
	}

	if (!synFind)
		return;

	int rowNumber = CurrentSysConfig._uiNumRows;
	int columnNumber = CurrentSysConfig._uiNumCols;

	QString qsStepAndResult = strTestStepName + QString(" : ") + strPassResults;
	if (NULL != ui.TestTableWidget->item(3, iSiteNumber - 1))
	{
		QString qsTempContent = ui.TestTableWidget->item(3, iSiteNumber - 1)->text();
		ui.TestTableWidget->item(3, iSiteNumber - 1)->setText(qsTempContent + QString("\n") + qsStepAndResult);
		ui.TestTableWidget->resizeRowToContents(3);
	}
	else
	{
		QTableWidgetItem *item = new QTableWidgetItem(qsStepAndResult);
		item->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(3, iSiteNumber - 1, item);
		ui.TestTableWidget->resizeRowToContents(3);
	}

	//State
	QTableWidgetItem *itemState = new QTableWidgetItem(QString("Running"));
	itemState->setTextAlignment(Qt::AlignCenter);
	ui.TestTableWidget->setItem(2, iSiteNumber - 1, itemState);

	//SerialNumber
	if (QString("InitializationStep") == strTestStepName)
	{
		//Syn_DutTestResult *pCurrentDutTestResult = NULL;
		Syn_DutTestInfo *DutInfo = NULL;
		_ListOfSitePtr[iSiteNumber - 1]->GetTestInfo(DutInfo);
		if (NULL != DutInfo)
		{
			QString qsSensorSerialNumber("");
			qsSensorSerialNumber = QString::fromLatin1(DutInfo->_getVerInfo.sSerialNumber, sizeof(DutInfo->_getVerInfo.sSerialNumber));

			QTableWidgetItem *itemSerialNumber = new QTableWidgetItem(qsSensorSerialNumber);
			itemSerialNumber->setTextAlignment(Qt::AlignCenter);
			ui.TestTableWidget->setItem(1, iSiteNumber - 1, itemSerialNumber);
		}
	}
}

void FPS_TestExecutive::GetVersionForDutDump()
{
	int iSiteCurrentIndex = ui.comboBox->currentIndex();
	if (iSiteCurrentIndex<0)
	{
		cout << "Error:FPS_TestExecutive - iSiteCurrentIndex is less than 0!" << endl;
		return;
	}

	size_t iSiteCounts = _ListOfSitePtr.size();
	if (0 == iSiteCounts)
	{
		cout << "Error:FPS_TestExecutive - iSiteCounts is 0!" << endl;
		return;
	}

	if (iSiteCurrentIndex > iSiteCounts)
	{
		cout << "Error:FPS_TestExecutive - iSiteCounts is less than iSiteCurrentIndex!" << endl;
		return;
	}

	Syn_Site *pSelectedSite = _ListOfSitePtr[iSiteCurrentIndex];
	if (NULL == pSelectedSite)
	{
		cout << "Error:FPS_TestExecutive - pSelectedSite is NULL!" << endl;
		return;
	}

	uint32_t rc = pSelectedSite->Open();
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}

	Syn_DutTestResult *oDutTestResult = NULL;
	rc = pSelectedSite->ExecuteTestStep("InitializationStep");
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}
	rc = pSelectedSite->GetTestResult(oDutTestResult);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}

	Syn_DutTestInfo *pInfo = NULL;
	rc = pSelectedSite->GetTestInfo(pInfo);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}

	ui.textBrowser->clear();
	ui.textBrowser->append(QString("Version:"));
	/*for (int i = 1; i <= VERSION_SIZE / 4; i++)
	{
		int StartPos = (i - 1) * 4;
		int EndPos = i * 4 - 1;

		Display(pInfo->_getVerInfo._GerVerArray, StartPos, EndPos);
	}*/


	QDateTime timeValue = QDateTime::fromTime_t(pInfo->_getVerInfo.buildtime);
	ui.textBrowser->append(QString("buildtime:") + timeValue.toString());
	ui.textBrowser->append(QString("buildnum:") + QString::number(pInfo->_getVerInfo.buildnum,16).toUpper());
	ui.textBrowser->append(QString("vmajor:") + QString::number(pInfo->_getVerInfo.vmajor));
	ui.textBrowser->append(QString("vminor:") + QString::number(pInfo->_getVerInfo.vminor));
	ui.textBrowser->append(QString("target:") + QString::number(pInfo->_getVerInfo.target));
	ui.textBrowser->append(QString("product:") + QString::number(pInfo->_getVerInfo.product));
	ui.textBrowser->append(QString("siliconrev:") + QString::number(pInfo->_getVerInfo.siliconrev));
	ui.textBrowser->append(QString("formalrel:") + QString::number(pInfo->_getVerInfo.formalrel));
	ui.textBrowser->append(QString("platform:") + QString::number(pInfo->_getVerInfo.platform));
	ui.textBrowser->append(QString("patch:") + QString::number(pInfo->_getVerInfo.patch));
	//ui.textBrowser->append(QString("serial_number:") + QString::number(pInfo->_getVerInfo.serial_number[0], 16) + QString("|") + QString::number(pInfo->_getVerInfo.serial_number[1], 16) + QString("|") + QString::number(pInfo->_getVerInfo.serial_number[2], 16) + QString("|") + QString::number(pInfo->_getVerInfo.serial_number[3], 16) + QString("|") + QString::number(pInfo->_getVerInfo.serial_number[4], 16) + QString("|") + QString::number(pInfo->_getVerInfo.serial_number[5], 16));
	ui.textBrowser->append(QString("serial_number:") + QString::fromLatin1(pInfo->_getVerInfo.sSerialNumber,12));
	ui.textBrowser->append(QString("security:") + QString::number(pInfo->_getVerInfo.security[0]) + QString("|") + QString::number(pInfo->_getVerInfo.security[1]));
	ui.textBrowser->append(QString("patchsig:") + QString::number(pInfo->_getVerInfo.patchsig));
	ui.textBrowser->append(QString("iface:") + QString::number(pInfo->_getVerInfo.iface));
	ui.textBrowser->append(QString("otpsig:") + QString::number(pInfo->_getVerInfo.otpsig[0]) + QString("|") + QString::number(pInfo->_getVerInfo.otpsig[1]) + QString("|") + QString::number(pInfo->_getVerInfo.otpsig[2]));
	ui.textBrowser->append(QString("otpspare1:") + QString::number(pInfo->_getVerInfo.otpspare1));
	ui.textBrowser->append(QString("reserved:") + QString::number(pInfo->_getVerInfo.reserved));
	ui.textBrowser->append(QString("device_type:") + QString::number(pInfo->_getVerInfo.device_type));

	pSelectedSite->Close();

	/*Syn_DutTestResult *oDutTestResult = NULL;
	rc = pSelectedSite->ExecuteTestStep("Calibrate");
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}
	rc = pSelectedSite->GetTestResult(oDutTestResult);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}
	rc = pSelectedSite->ExecuteTestStep("CurrentTest");
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}
	Syn_DutTestInfo *oDutTestInfo = NULL;
	rc = pSelectedSite->GetTestInfo(oDutTestInfo);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}
	rc = pSelectedSite->GetTestResult(oDutTestResult);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString::number(rc));
		return;
	}*/
//	int iSiteCurrentIndex = ui.comboBox->currentIndex();
//	if (iSiteCurrentIndex<0)
//	{
//		cout << "Error:FPS_TestExecutive::GetVersionForDutDump() - iSiteCurrentIndex is less than 0!" << endl;
//		return;
//	}

//	size_t iSiteCounts = _ListOfSitePtr.size();
//	if (0 == iSiteCounts)
//	{
//		cout << "Error:FPS_TestExecutive::GetVersionForDutDump() - iSiteCounts is 0!" << endl;
//		return;
//	}

//	if (iSiteCurrentIndex > iSiteCounts)
//	{
//		cout << "Error:FPS_TestExecutive::GetVersionForDutDump() - iSiteCounts is less than iSiteCurrentIndex!" << endl;
//		return;
//	}

//	Syn_Site *pSelectedSite = _ListOfSitePtr[iSiteCurrentIndex];
//	if (NULL == pSelectedSite)
//	{
//		cout << "Error:FPS_TestExecutive::GetVersionForDutDump() - pSelectedSite is NULL!" << endl;
//		return;
//	}

//	pSelectedSite->GetVersion();

//	Syn_SiteInfo oSiteInfo;
//	pSelectedSite->GetSiteInfo(oSiteInfo);
//	Syn_DutTestInfo *pDutTestInfo = NULL;
//	pSelectedSite->GetTestInfo(pDutTestInfo);

//	ui.textBrowser->clear();
//	ui.textBrowser->append(QString("SiteNumber:") + QString::number(oSiteInfo._uiSiteNumber));
//	ui.textBrowser->append(QString("SerialNumber:") + QString::number(oSiteInfo._uiSerialNumber));
//	ui.textBrowser->append(QString("Version:"));
//	for (int i = 1; i <= VERSION_SIZE / 4; i++)
//	{
//		int StartPos = (i - 1) * 4;
//		int EndPos = i * 4 - 1;

//		Display(pDutTestInfo->_getVerInfo._GerVerArray, StartPos, EndPos);
//	}


}

void FPS_TestExecutive::ReadOTPForDutDump()
{
	int iSiteCurrentIndex = ui.comboBox->currentIndex();
	if (iSiteCurrentIndex<0)
	{
		cout << "Error:FPS_TestExecutive::GetVersionForDutDump() - iSiteCurrentIndex is less than 0!" << endl;
		return;
	}

	size_t iSiteCounts = _ListOfSitePtr.size();
	if (0 == iSiteCounts)
	{
		cout << "Error:FPS_TestExecutive::ReadOTPForDutDump() - iSiteCounts is 0!" << endl;
		return;
	}

	if (iSiteCurrentIndex > iSiteCounts)
	{
		cout << "Error:FPS_TestExecutive::ReadOTPForDutDump() - iSiteCounts is less than iSiteCurrentIndex!" << endl;
		return;
	}

	Syn_Site *pSelectedSite = _ListOfSitePtr[iSiteCurrentIndex];
	if (NULL == pSelectedSite)
	{
		cout << "Error:FPS_TestExecutive::ReadOTPForDutDump() - pSelectedSite is NULL!" << endl;
		return;
	}

	uint32_t rc = pSelectedSite->Open();
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("OTPDump Error:") + QString::number(rc));
		return;
	}

	rc = pSelectedSite->ExecuteTestStep("OTPCheck");
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("OTPDump Error:") + QString::number(rc));
		return;
	}
	Syn_DutTestInfo *oDutTestInfo = NULL;
	rc = pSelectedSite->GetTestInfo(oDutTestInfo);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("OTPDump Error:") + QString::number(rc));
		return;
	}
	Syn_DutTestResult *oDutTestResult = NULL;
	rc = pSelectedSite->GetTestResult(oDutTestResult);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("OTPDump Error:") + QString::number(rc));
		return;
	}

	//Syn_SiteInfo oSiteInfo;
	//pSelectedSite->GetSiteInfo(oSiteInfo);
	unsigned int oSiteNumber;
	pSelectedSite->GetSiteNumber(oSiteNumber);

	uint32_t oSerialNumber;
	pSelectedSite->GetSerialNumber(oSerialNumber);

	ui.textBrowser->clear();
	ui.textBrowser->append(QString("SiteNumber:") + QString::number(oSiteNumber));
	ui.textBrowser->append(QString("SerialNumber:") + QString::number(oSerialNumber));

	if (oDutTestResult->_binCodes.size() == 0)
	{
		ui.textBrowser->append(QString("Result: PASS"));
	}
	else
	{
		QString sBinCodes = "";
		for (auto i = 0; i < oDutTestResult->_binCodes.size(); i++)
		{
			sBinCodes += (oDutTestResult->_binCodes[i] + ", ").c_str();
		}

		ui.textBrowser->append(QString("Result: FAIL ") + sBinCodes);
	}

	//ui.textBrowser->append(QString("Sysconfig Boot Sector 0:"));
	//for (int i = 1; i <= BS0_SIZE / 8; i++)
	//{
	//	int StartPos = (i - 1) * 8;
	//	int EndPos = i * 8 - 1;

	//	Display(oDutTestInfo->_otpCheckInfo._UserSpecifiedBS0, StartPos, EndPos);
	//}

	ui.textBrowser->append(QString("Boot Sector 0:"));
	for (int i = 1; i <= BS0_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;

		Display(oDutTestInfo->_otpCheckInfo._BootSector0Array, StartPos, EndPos);
	}

	//ui.textBrowser->append(QString("Sysconfig Boot Sector 1:"));
	//for (int i = 1; i <= BS1_SIZE / 8; i++)
	//{
	//	int StartPos = (i - 1) * 8;
	//	int EndPos = i * 8 - 1;

	//	Display(oDutTestInfo->_otpCheckInfo._UserSpecifiedBS1, StartPos, EndPos);
	//}

	ui.textBrowser->append(QString("Boot Sector 1:"));
	for (int i = 1; i <= BS1_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;

		Display(oDutTestInfo->_otpCheckInfo._BootSector1Array, StartPos, EndPos);
	}

	ui.textBrowser->append(QString("Main Sector 0:"));
	for (int i = 1; i <= MS0_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		Display(oDutTestInfo->_otpCheckInfo._MainSector0Array, StartPos, EndPos);
	}

	ui.textBrowser->append(QString("Main Sector 1:"));
	for (int i = 1; i <= MS1_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		Display(oDutTestInfo->_otpCheckInfo._MainSector1Array, StartPos, EndPos);
	}

	pSelectedSite->Close();
}

void FPS_TestExecutive::PushCablicationImageButton()
{
	int iSiteCurrentIndex = ui.ImageSiteComboBox->currentIndex();
	if (iSiteCurrentIndex < 0)
		return;

	size_t iSiteCounts = _ListOfSitePtr.size();
	if (0 == iSiteCounts)
		return;

	if (iSiteCounts < iSiteCurrentIndex)
		return;

	ui.CalibrationImageLabel->hide();

	Syn_Site::SiteState oTempState;
	_ListOfSitePtr[iSiteCurrentIndex]->GetState(oTempState);
	if (Syn_Site::Error == oTempState)
	{
		QMessageBox::information(this, QString("Error"), QString("Cablication Error:Selected Site's state is Error!"));
		return;
	}

	if (_threadForDebugCalibrate.isRunning())
	{
		//_threadForDebugCalibrate.SetRunTag(false);
		//_ListOfSitePtr[iSiteCurrentIndex]->Close();
		//ui.FigerprintImagePushButton->setText(QString("Get Figerprint Image"));

	}
	else
	{
		_ListOfSitePtr[iSiteCurrentIndex]->Open();
		_threadForDebugCalibrate.SetSite(_ListOfSitePtr[iSiteCurrentIndex]);
		//_threadForDebugCalibrate.SetRunTag(true);
		_threadForDebugCalibrate.start();
	}
}

void FPS_TestExecutive::ImageCalibration(unsigned int iSiteNumber)
{
	//retrieve current site with sitenumber from sitelist
	bool synFind(false);
	Syn_DutTestResult *pCurrentDutTestResult = NULL;
	Syn_SysConfig CurrentSysConfig;
	for (size_t i = 0; i < _ListOfSitePtr.size(); i++)
	{
		unsigned int iTempSiteNumber(0);
		_ListOfSitePtr[i]->GetSiteNumber(iTempSiteNumber);
		if (iSiteNumber == iTempSiteNumber)
		{
			//_ListOfSitePtr[i]->GetTestInfo(*CurrentDutTestInfo);
			Syn_Site::SiteState oTempState;
			_ListOfSitePtr[i]->GetState(oTempState);
			if (oTempState == Syn_Site::Error)
			{
				string errMsg = "";
				_ListOfSitePtr[i]->GetRunTimeError(errMsg);
				QMessageBox::information(this, QString("Error"), QString("Calibrate Error:") + QString::fromStdString(errMsg));
				return;
			}
			_ListOfSitePtr[i]->GetTestResult(pCurrentDutTestResult);
			_ListOfSitePtr[i]->GetSysConfig(CurrentSysConfig);
			synFind = true;
			break;
		}
	}

	if (!synFind || NULL == pCurrentDutTestResult)
		return;

	ui.CalibrationImageLabel->clear();

	ui.CalibrationImageLabel->show();

	int rowNumber = CurrentSysConfig._uiNumRows;
	int columnNumber = CurrentSysConfig._uiNumCols;
	
	QVector<QRgb> vcolorTable;
	for (int i = 0; i < 256; i++)
	{
		vcolorTable.append(qRgb(i, i, i));
	}
	QByteArray data;
	data.resize((rowNumber)*(columnNumber));
	for (int m = 0; m < rowNumber; m++)
	{
		for (int n = 0; n < columnNumber; n++)
		{
			data[m*(columnNumber)+n] = (pCurrentDutTestResult->_calibrationResults).arr_ImageFPSFrame.arr[m][n];
		}
	}
	QImage image((uchar*)data.constData(), columnNumber, rowNumber,QImage::Format_Indexed8);
	image.setColorTable(vcolorTable);
	image = image.copy(HEADER,0, columnNumber - HEADER, rowNumber);

	//scale
	image = image.scaled((columnNumber - HEADER) * 2, rowNumber * 2, Qt::KeepAspectRatio);

	ui.CalibrationImageLabel->setPixmap(QPixmap::fromImage(image));
	ui.CalibrationImageLabel->adjustSize();
	ui.CalibtrationGroupBox->adjustSize();

	for (size_t i = 0; i < _ListOfSitePtr.size(); i++)
	{
		unsigned int iTempSiteNumber(0);
		_ListOfSitePtr[i]->GetSiteNumber(iTempSiteNumber);
		if (iSiteNumber == iTempSiteNumber)
		{
			_ListOfSitePtr[i]->Close();
		}
	}
}

void FPS_TestExecutive::PushFigerprintImageButton()
{
	int iSiteCurrentIndex = ui.ImageSiteComboBox->currentIndex();
	if (iSiteCurrentIndex < 0)
		return;

	size_t iSiteCounts = _ListOfSitePtr.size();
	if (0 == iSiteCounts)
		return;

	if (iSiteCounts < iSiteCurrentIndex)
		return;

	if (_threadForDebug.isRunning())
	{
		_threadForDebug.SetRunTag(false);

		//_ListOfSitePtr[iSiteCurrentIndex]->Close();

		//ui.FigerprintImagePushButton->setText(QString("Get Figerprint Image"));

	}
	else
	{
		_threadForDebug.SetSite(_ListOfSitePtr[iSiteCurrentIndex]);
		_threadForDebug.SetRunTag(true);
		_threadForDebug.start();

		//ui.FigerprintImagePushButton->setText(QString("Stop"));

	}
}

void FPS_TestExecutive::FigerprintImage(unsigned int iSiteNumber)
{
	//debug  must delete at end

	//retrieve current site with sitenumber from sitelist
	bool synFind(false);
	Syn_DutTestResult *pCurrentDutTestResult = NULL;
	Syn_SysConfig CurrentSysConfig;
	unsigned int iPos(0);
	for (size_t i = 0; i < _ListOfSitePtr.size(); i++)
	{
		unsigned int iTempSiteNumber(0);
		_ListOfSitePtr[i]->GetSiteNumber(iTempSiteNumber);
		if (iSiteNumber == iTempSiteNumber)
		{
			//_ListOfSitePtr[i]->GetTestInfo(*CurrentDutTestInfo);
			Syn_Site::SiteState oTempState;
			_ListOfSitePtr[i]->GetState(oTempState);
			if (oTempState == Syn_Site::Error)
			{
				string errMsg = "";
				_ListOfSitePtr[i]->GetRunTimeError(errMsg);
				QMessageBox::information(this, QString("Error"), QString("Calibrate Error:") + QString::fromStdString(errMsg));
				return;
			}
			_ListOfSitePtr[i]->GetTestResult(pCurrentDutTestResult);
			_ListOfSitePtr[i]->GetSysConfig(CurrentSysConfig);
			iPos = i;
			synFind = true;
			break;
		}
	}

	if (!synFind || NULL == pCurrentDutTestResult)
		return;



	ui.FingerprintImageLabel->clear();

	ui.FingerprintImageLabel->show();

	int rowNumber = CurrentSysConfig._uiNumRows;
	int columnNumber = CurrentSysConfig._uiNumCols;

	QVector<QRgb> vcolorTable;
	for (int i = 0; i < 256; i++)
	{
		vcolorTable.append(qRgb(i, i, i));
	}
	QByteArray data;
	data.resize((rowNumber)*(columnNumber));
	for (int m = 0; m < rowNumber; m++)
	{
		for (int n = 0; n < columnNumber; n++)
		{
			data[m*(columnNumber)+n] = (pCurrentDutTestResult->_acqImgNoFingerResult).arr_ImageFPSFrame.arr[m][n];
		}
	}
	QImage image((uchar*)data.constData(), columnNumber, rowNumber, QImage::Format_Indexed8);
	image.setColorTable(vcolorTable);
	image = image.copy(HEADER, 0, columnNumber - HEADER, rowNumber);

	//scale
	image = image.scaled((columnNumber - HEADER) * 2, rowNumber * 2, Qt::KeepAspectRatio);

	ui.FingerprintImageLabel->setPixmap(QPixmap::fromImage(image));
	ui.FingerprintImageLabel->adjustSize();
	ui.FingerprintImageGroupBox->adjustSize();

	_ListOfSitePtr[iPos]->Close();
}

void FPS_TestExecutive::Display(uint8_t* pDst, int DstSize)
{
	QString s = "";
	for (int i = 0; i<DstSize; i++){
		s += QString::number(pDst[i], 16) + ",";
	}
	//ui.textBrowser->append(s);
}

void FPS_TestExecutive::Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos)
{
	if ((EndPos - StartPos) <= 0)
	{
		//ui.textBrowser->append("StartPos>=EndPos");
		return;
	}

	QString s = "";
	for (int i = StartPos; i <= EndPos; i++)
	{
		s += (QString::number(pDst[i], 16)).toUpper() + ",";
	}
	ui.textBrowser->append(s);
}

void FPS_TestExecutive::WriteLog(std::string strFolderPath, Syn_DutTestInfo * DutInfo, Syn_DutTestResult * DutResults, int RowNumber, int ColumnNumber)
{
	if (NULL == DutInfo)
		return;

	if (NULL == DutResults)
		return;

	QString qsFolderPath = QString::fromStdString(strFolderPath);
	QFile qfile(qsFolderPath);
	if (!qfile.exists())
		return;

	QString qsSensorSerialNumber("");
	qsSensorSerialNumber = QString::fromLatin1(DutInfo->_getVerInfo.sSerialNumber,12);

	QString qsSerachContent = qsFolderPath + QString("\\*.csv");

	std::string strSensorSerialNumber;
	std::vector<std::string> lsitOfFileName;
	long handle;
	struct _finddata_t fileinfo;
	handle = _findfirst(qsSerachContent.toStdString().c_str(), &fileinfo);
	if (-1 == handle)
	{
		strSensorSerialNumber = (qsSensorSerialNumber + QString("_1")).toStdString();
	}
	else
	{
		int iCounts(0);

		do
		{
			QString qsCSVFileName = fileinfo.name;
			int iResults = qsCSVFileName.indexOf(qsSensorSerialNumber);
			if (0 == iResults)
			{
				iCounts += 1;
			}
		} while (_findnext(handle, &fileinfo) == 0);

		strSensorSerialNumber = (qsSensorSerialNumber + QString("_") + QString::number(iCounts+1)).toStdString();
		_findclose(handle);
	}

	std::string stringFilePath(qsFolderPath.toStdString()+"\\"+strSensorSerialNumber + ".csv");
	FILE *pFile = fopen(stringFilePath.c_str(), "a");
	if (NULL == pFile)
	{
		return;
	}

	fprintf(pFile, "\n%%%%%%%%%%%%%%%%%%%%%%\n");
	fprintf(pFile, "MTLog\n");

	//Put in part number.
	//fprintf(pFile, "Part Number,%s\n", "");//DebugVersion

	//fprintf(pFile, "ConfigFile,%s\n", _pSyn_Dut->_pSyn_DutTestInfo-);
	fprintf(pFile, "\n%%%%%%%%%%%%%%%%%%%%%%\n");

	fprintf(pFile, "\n---------------------\n");
	const time_t t = time(NULL);
	struct tm* current_time = localtime(&t);
	fprintf(pFile, "Run %d,%s\n", "", asctime(current_time));

	//Sensor Serial Number
	fprintf(pFile, "Sensor Serial Number ,%s\n", qsSensorSerialNumber.toStdString().c_str());

	//InitlizationStep
	fprintf(pFile, "\nInitialization, %s,%d ms\n", DutResults->_initResults.m_bPass ? "Pass" : "Fail", 0);

	//Pixel Patch
	fprintf(pFile, "\nPixel Patch, %s,%lf ms\n", DutResults->_pixelPatchResults.m_bPass ? "Pass" : "Fail", DutResults->_pixelPatchResults.m_elapsedtime);
	fprintf(pFile, ",,,");
	for (int i = 0; i < (DutInfo->_pixelPatchInfo.m_nNumResBytes) / 4; i++)
		fprintf(pFile, "%d,", *((uint32_t*)&DutResults->_pixelPatchResults.m_pResponse[i * 4]));
	fprintf(pFile, "\n");

	//Cablication
	fprintf(pFile, "\nCalibration, %s,%lf ms\n", DutResults->_calibrationResults.m_bPass ? "Pass" : "Fail", DutResults->_calibrationResults.m_elapsedtime);
	// Stage1 LNA values from print patch
	fprintf(pFile, ",,,Stage1");
	for (int i = 0; i < RowNumber; i++)
	{
		fprintf(pFile, ",%02X", DutResults->_calibrationResults.m_pPrintPatch[i + DutInfo->_calibrationInfo.m_nLnaIdx]);
	}
	if (DutInfo->_calibrationInfo.m_nCalType == 0)
	{
		fprintf(pFile, "\n,,,Stage2");
		for (int i = 0; i < RowNumber; i++)
		{
			fprintf(pFile, ",%02X", DutResults->_calibrationResults.m_arPgaOffsets[i]);
		}
		fprintf(pFile, "\n");
	}
	else if (DutInfo->_calibrationInfo.m_nCalType == 1)
	{
		fprintf(pFile, "\n,,,Stage2 Used");
		for (int i = 0; i<(RowNumber) * (ColumnNumber - 8); i++)
		{
			fprintf(pFile, ",%02X", DutResults->_calibrationResults.m_arPgaOffsets[i]);
		}
		fprintf(pFile, "\n");
	}
	else
	{
		fprintf(pFile, ",,,Stage2 Used,N/A\n");
	}
	// Stage2 OTP values	
	if (DutInfo->_calibrationInfo.m_nCalType == 1)
	{
		if (DutResults->_calibrationResults.m_nPGA_OOPP_count != 0)
		{
			fprintf(pFile, ",,,Stage2 OTP");
			for (int i = 0; i< (NUM_PGA_OOPP_OTP_ROWS * (ColumnNumber - 8)); i++)
			{
				fprintf(pFile, ",%02X", DutResults->_calibrationResults.m_pPGAOtpArray[i]);
			}

			fprintf(pFile, "\n,,,Stage2 Variance Score,N/A\n");
		}
		else
		{
			fprintf(pFile, ",,,Stage2 OTP,N/A\n");
			fprintf(pFile, ",,,Stage2 Variance Score,N/A\n");
		}
	}
	fprintf(pFile, ",,,FlexId,%04X\n", DutInfo->_initInfo.m_nFlexId);

	//Pegged Pixels Test
	fprintf(pFile, "\nPegged Pixels Test,%s,%lf ms,Rows,", DutResults->_peggedPixelsResults.m_bPass ? "Pass" : "Fail", DutResults->_peggedPixelsResults.m_elapsedtime);
	for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithoutStim + DutInfo->_initInfo.m_nTrimBotWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_peggedPixelsResults.pegged_pixel_rows[i]);

	fprintf(pFile, "\n,,,Columns,");
	for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_peggedPixelsResults.pegged_pixel_cols[i]);
	fprintf(pFile, "\n");

	//Floored Pixels Test
	fprintf(pFile, "\nFloored Pixels Test,%s,%lf ms,Rows,", DutResults->_flooredPixelsResults.m_bPass ? "Pass" : "Fail", DutResults->_flooredPixelsResults.m_elapsedtime);
	for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithoutStim + DutInfo->_initInfo.m_nTrimBotWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_flooredPixelsResults.floored_pixel_rows[i]);

	fprintf(pFile, "\n,,,Columns,");
	for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_flooredPixelsResults.floored_pixel_cols[i]);
	fprintf(pFile, "\n");

	//DRdy Test
	fprintf(pFile, "\nDRdy Test,%s,%lf ms\n", DutResults->_DRdyResults.m_bPass ? "Pass" : "Fail", DutResults->_DRdyResults.m_elapsedtime);

	//Consecutive Pixels Test
	fprintf(pFile, "\nConsecutive Pixels Test,%s,%lf ms,Pegged Rows,", DutResults->_consecutivePixelsResults.m_bPass ? "Pass" : "Fail", DutResults->_consecutivePixelsResults.m_elapsedtime);
	for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithoutStim + DutInfo->_initInfo.m_nTrimBotWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_consecutivePixelsResults.consecutive_pegged_rows[i]);

	fprintf(pFile, "\n,,,Floored Rows,");
	for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithoutStim + DutInfo->_initInfo.m_nTrimBotWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_consecutivePixelsResults.consecutive_floored_rows[i]);

	fprintf(pFile, "\n,,,Pegged Columns,");
	for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_consecutivePixelsResults.consecutive_pegged_cols[i]);

	fprintf(pFile, "\n,,,Floored Columns,");
	for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_consecutivePixelsResults.consecutive_floored_cols[i]);
	fprintf(pFile, "\n");

	//Current Test
	fprintf(pFile, "\nCurrent Test,%s,%lf ms,", DutResults->_currentResults.bPass ? "Pass" : "Fail", DutResults->_currentResults.m_elapsedtime);
	//If the test was successful.
	if (DutResults->_currentResults.bPass != 0)
	{
		fprintf(pFile, "Digital image acq current (mA),%.3f\n", (float)(DutResults->_currentResults.m_nImageAcqDigCurrent_uA) / 1000);
		fprintf(pFile, ",,,Analog image acq current (mA),%.3f\n", (float)(DutResults->_currentResults.m_nImageAcqAnaCurrent_uA) / 1000);
	}
	else
	{
		fprintf(pFile, "Digital image acq current (mA),%.3f\n", (float)(DutResults->_currentResults.m_nImageAcqDigCurrent_uA) / 1000);
		fprintf(pFile, ",,,Analog image acq current (mA),%.3f\n", (float)(DutResults->_currentResults.m_nImageAcqAnaCurrent_uA) / 1000);
	}

	//SNR Test
	fprintf(pFile, "\nSNR Test,%s,%lf ms,", DutResults->_snrResults.bPass ? "Pass" : "Fail", DutResults->_snrResults.m_elapsedtime);
	fprintf(pFile, "Signal_Z1,Noise_Z1,SNR_Z1,Signal_Z2,Noise_Z2,SNR_Z2,Signal_Z3,Noise_Z3,SNR_Z3,Signal_Z4,Noise_Z4,SNR_Z4,Signal_Z5,Noise_Z5,SNR_Z5,Signal_Z6,Noise_Z6,SNR_Z6,Signal_OVERALL,Noise_OVERALL,SNR_OVERALL\n");
	fprintf(pFile, ",,,");
	for (int i = 0; i<REGIONS; i++)
		fprintf(pFile, "%d,%f,%f,", DutResults->_snrResults.SIGNAL[i], DutResults->_snrResults.NOISE[i], DutResults->_snrResults.SNR[i]);
	fprintf(pFile, "\n");

	//Pixel Uniformity Test
	fprintf(pFile, "\nPixel Uniformity Test,%s,%lf ms,", DutResults->_pixelResults.bPass ? "Pass" : "Fail", DutResults->_pixelResults.m_elapsedtime);
	fprintf(pFile, "Minimum Pixel,Maximum Pixel,Failing Pixel Count\n");
	fprintf(pFile, ",,,%d,%d,%d,", DutResults->_pixelResults.nMinPixelValue, DutResults->_pixelResults.nMaxPixelValue, DutResults->_pixelResults.nCountAboveThreshold);
	fprintf(pFile, "\n");

	//Sharpness Test
	fprintf(pFile, "\nSharpness Test,%s,%lf ms\n", DutResults->_SharpnessResults.bPass ? "Pass" : "Fail", DutResults->_SharpnessResults.m_elapsedtime);
	fprintf(pFile, ",,,Variation(%%),Zone1,Zone2,Zone3,Overall\n");
	fprintf(pFile, ",,,%f,%d,%d,%d,%d", DutResults->_SharpnessResults.percent, (int)DutResults->_SharpnessResults.SHARPNESS[0], (int)DutResults->_SharpnessResults.SHARPNESS[1], (int)DutResults->_SharpnessResults.SHARPNESS[2], (int)DutResults->_SharpnessResults.SHARPNESS[3]);
	fprintf(pFile, "\n");

	//RxStandardDev Test
	fprintf(pFile, "\nRxStandardDev Test,%s,%lf ms", DutResults->_RxStandardDevResults.m_bPass ? "Pass" : "Fail", DutResults->_RxStandardDevResults.m_elapsedtime);
	fprintf(pFile, "\n,,,Percent:,");
	for (int i = 0; i<(RowNumber - DutInfo->_initInfo.m_nTrimTopWithStim - DutInfo->_initInfo.m_nTrimBotWithStim); i++)
		fprintf(pFile, "%f,", DutResults->_RxStandardDevResults.percent[i]);
	fprintf(pFile, "\n");

	//Imperfections Test
	fprintf(pFile, "\nImperfections Test,%s,%lf ms,Along Rows,", DutResults->_imperfectionsTestResults.m_bPass ? "Pass" : "Fail", DutResults->_imperfectionsTestResults.m_elapsedtime);
	for (int i = 0; i<RowNumber - (DutInfo->_initInfo.m_nTrimTopWithStim + DutInfo->_initInfo.m_nTrimBotWithStim); i++)
		fprintf(pFile, "%d,", DutResults->_imperfectionsTestResults.consecutive_pegged_rows[i]);
	fprintf(pFile, "\n,,,Along Columns,");
	for (int i = 0; i<ColumnNumber - HEADER - (DutInfo->_initInfo.m_nTrimLeftWithoutStim + DutInfo->_initInfo.m_nTrimRightWithoutStim); i++)
		fprintf(pFile, "%d,", DutResults->_imperfectionsTestResults.consecutive_pegged_cols[i]);
	fprintf(pFile, "\n");

	//Average No Finger & Average Finger
	//int numFrames = 30;
	//float temp = 0.0;
	fprintf(pFile, "\nAverage No Finger\n");
	for (int i = 0; i< DutResults->_acqImgNoFingerResult.iRealRowNumber; i++)
	{
		for (int j = 0; j< DutResults->_acqImgNoFingerResult.iRealColumnNumber; j++)
		{
			fprintf(pFile, "%d,", (int)DutResults->_acqImgNoFingerResult.arr_ImageFPSFrame.arr[i][j]);
		}
		fprintf(pFile, "\n");
	}
	//temp = 0.0;
	fprintf(pFile, "\nNormalized Finger\n");
	for (int i = 0; i< DutResults->_acqImgFingerResult.iRealRowNumber; i++)
	{
		for (int j = 0; j< DutResults->_acqImgFingerResult.iRealColumnNumber; j++)
		{
			fprintf(pFile, "%d,", (int)DutResults->_acqImgFingerResult.arr_ImageFPSFrame.arr[i][j]);
		}
		fprintf(pFile, "\n");
	}

	fprintf(pFile, "\n,Bin Codes");
	for (size_t i = 1; i <= DutResults->_binCodes.size(); i++)
	{
		fprintf(pFile, ",%s", (DutResults->_binCodes[i - 1]).c_str());
	}

	fclose(pFile);
}

void FPS_TestExecutive::DisplayBinCodes()
{
	if (NULL == _pSyn_BinCodesDlg)
	{
		_pSyn_BinCodesDlg = new Syn_BinCodesDlg();
	}

	if (_pSyn_BinCodesDlg->isHidden())
	{
		_pSyn_BinCodesDlg->show();
	}

	QObject::connect(_pSyn_BinCodesDlg->ui.ClosePushButton, SIGNAL(clicked()), this, SLOT(CloseBinCodesDlg()));
}

void FPS_TestExecutive::CloseBinCodesDlg()
{
	if (NULL == _pSyn_BinCodesDlg)
		return;

	_pSyn_BinCodesDlg->hide();
}

void FPS_TestExecutive::ManageButtonStatus(int iFlag)
{
	if (_FinishedSiteCounts == _ListOfSitePtr.size())
	{
		bool bAllFailed(true);
		for (size_t i = 1; i <= _ListOfSitePtr.size(); i++)
		{
			Syn_Site::SiteState oTmpState;
			_ListOfSitePtr[i - 1]->GetState(oTmpState);
			if (Syn_Site::Error != oTmpState)
			{
				bAllFailed = false;
				break;
			}
		}

		if (bAllFailed)
		{
			//ui.LocalSettingsPushButton->setDisabled(false);
			ui.pushButtonRun->setDisabled(false);
		}
		else
		{
			if (1 == iFlag)
			{
				ui.pushButtonRun->setText(QString("Continue"));
				ui.pushButtonRun->setDisabled(false);
			}
			else if (2 == iFlag)
			{
				ui.pushButtonRun->setText(QString("Run"));
				//ui.LocalSettingsPushButton->setDisabled(false);
				ui.pushButtonRun->setDisabled(false);
			}
		}
		
	}
}