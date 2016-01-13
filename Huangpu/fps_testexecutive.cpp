//local
#include "fps_testexecutive.h"
#include "Syn_LocalSettingConfig.h"
#include "ui_Syn_LocalSettingsDlg.h"
//Qt
#include <QtWidgets>

FPS_TestExecutive::FPS_TestExecutive(QWidget *parent)
: QMainWindow(parent)
, _bStopTag(true)
, _iRealDeviceCounts(0)
, _logfile("sys.log")
, _pSyn_LocalSettingsDlg(NULL)
{
	ui.setupUi(this);
	ui.TestTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.TestTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.TestTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.TestTableWidget->setRowHeight(5, 200);
	ui.TestTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.TestTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.TestTableWidget->verticalHeader()->setStretchLastSection(true);
	Initialize();

	//LocalSettings
	QObject::connect(ui.LocalSettingsPushButton, SIGNAL(clicked()), this, SLOT(CreateLocalSettings()));
	QObject::connect(_pSyn_LocalSettingsDlg->ui->CancelPushButton, SIGNAL(clicked()), this, SLOT(CloseLocalSettingsDialog()));

	QObject::connect(_pSyn_LocalSettingsDlg->ui->SelectSysConfigFilePushButton, SIGNAL(clicked()), this, SLOT(SelectConfigFile()));

	QObject::connect(_pSyn_LocalSettingsDlg->ui->UpdateSitePushButton, SIGNAL(clicked()), this, SLOT(UpdateSiteLocalSettings()));
	
	QObject::connect(_pSyn_LocalSettingsDlg->ui->OKPushButton, SIGNAL(clicked()), this, SLOT(LocalSettingsOKAction()));


	//Testing Operation
	QObject::connect(ui.pushButtonRun, SIGNAL(clicked()), this, SLOT(RunningTest()));

	//Thread
	for (int i = 1; i <= DeviceCounts; i++)
	{
		QObject::connect(&(_SynThreadArray[i - 1]), SIGNAL(send(void*)), this, SLOT(ReceiveOTPTestSlot(void*)));
	}
}

FPS_TestExecutive::~FPS_TestExecutive()
{
	_ListOfSitePtr.clear();

	_logfile.close();

	if (NULL != _pSyn_LocalSettingsDlg)
	{
		delete _pSyn_LocalSettingsDlg;
		_pSyn_LocalSettingsDlg = NULL;
	}
}

void FPS_TestExecutive::Initialize()
{
	bool rc(false);

	_ListOfSitePtr.clear();
	std::cout.rdbuf(_logfile.rdbuf());

	_pSyn_LocalSettingsDlg = new Syn_LocalSettingsDlg();
	_pSyn_LocalSettingsDlg->setHidden(true);

	Syn_LocalSettingConfig *pSyn_LocalSettingConfig = NULL;
	rc = Syn_LocalSettingConfig::CreateLSConfigInstance(pSyn_LocalSettingConfig);
	if (!rc || NULL == pSyn_LocalSettingConfig)
	{
		cout << "Error:FPS_TestExecutive::Initialize() - pSyn_LocalSettingConfig is NULL!" << endl;
		return;
	}

	rc = pSyn_LocalSettingConfig->GetLocalSettings(_LocalSettingsInfo);

	QString strConfigFilePath = QString::fromStdString(_LocalSettingsInfo._strSysConfigFilePath);
	if (!strConfigFilePath.isNull())
	{
		_pSyn_LocalSettingsDlg->ui->SysConfigFileLlineEdit->clear();
		_pSyn_LocalSettingsDlg->ui->SysConfigFileLlineEdit->setText(strConfigFilePath);
	}

	int iSiteCounts = _LocalSettingsInfo._listOfSiteSettings.size();
	if (0 != iSiteCounts)
	{
		//clear
		for (int t = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->rowCount(); t >= 1; t--)
		{
			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->removeRow(t-1);
		}

		_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setRowCount(iSiteCounts);
		
		for (size_t t = 1; t <= iSiteCounts; t++)
		{
			AdcBaseLineInfo CurrentAdcBaseLineInfo = (_LocalSettingsInfo._listOfSiteSettings)[t - 1]._adcBaseLineInfo;
			QString strVoltagesAll = QString::number(CurrentAdcBaseLineInfo.m_nVdd) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVio) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVled) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVddh);

			QString strAdcBaseLinesValue("");
			for (int a = 0; a < NUM_CURRENT_VALUES; a++)
			{
				for (int b = 0; b < KNUMGAINS; b++)
				{
					strAdcBaseLinesValue += QString::number((CurrentAdcBaseLineInfo.m_arAdcBaseLines)[a][b]) + QString(" ");
				}
			}

			QString strSiteNumber(QString::number(t));
			QString strSerialNumber(QString::number((_LocalSettingsInfo._listOfSiteSettings)[t - 1]._uiDutSerNum));

			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(t-1, 0, new QTableWidgetItem(strSiteNumber));
			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(t - 1, 1, new QTableWidgetItem(strSerialNumber));
			_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(t - 1, 2, new QTableWidgetItem(strAdcBaseLinesValue));
			
		}
	}
	
	delete pSyn_LocalSettingConfig;
	pSyn_LocalSettingConfig = NULL;

	//Config file
	QString strConfigFile = QString::fromStdString(_LocalSettingsInfo._strSysConfigFilePath);
	rc = ConstructSiteList(strConfigFilePath,true);
	if (!rc)
	{
		cout << "Error:FPS_TestExecutive::Initialize() - ::ConstructSiteList() is failed!" << endl;
		return;
	}

	for (size_t m = 1; m <= iSiteCounts; m++)
	{
		uint32_t uiSerialNumber = (_LocalSettingsInfo._listOfSiteSettings)[m - 1]._uiDutSerNum;
		bool IsExists(false);
		for (size_t n = 1; n <= _iRealDeviceCounts; n++)
		{
			uint32_t CSerialNumber(0);
			_ListOfSitePtr[n - 1]->GetSerialNumber(CSerialNumber);
			if (uiSerialNumber == CSerialNumber)
			{
				_ListOfSitePtr[n - 1]->SetSiteNumber(m);
				IsExists = true;
				break;
			}
		}

		if (!IsExists)
		{
			QMessageBox::information(this, QString("Error"), QString("Can't retrieve the Serial Number:") + QString::number(uiSerialNumber) + QString(" device,check it,please!"));
			continue;
		}
	}

	//Site TableWidget
	for (int t = ui.TestTableWidget->columnCount(); t >= 1; t--)
	{
		ui.TestTableWidget->removeColumn(t - 1);
	}

	ui.TestTableWidget->setColumnCount(_iRealDeviceCounts);
	QStringList strListOfHeader;
	for (size_t t = 1; t <= _iRealDeviceCounts; t++)
	{
		strListOfHeader.append(QString("Site") + QString::number(t));
		//ui.TestTableWidget->setItem(t-1,)

		ui.TestTableWidget->setColumnWidth(t - 1, 200);
	}
	ui.TestTableWidget->setHorizontalHeaderLabels(strListOfHeader);
}

bool FPS_TestExecutive::ConstructSiteList(QString strConfigFilePath,bool SendMsg)
{
	bool rc(false);

	//Config file
	QFile ConfigFile(strConfigFilePath);
	if (!ConfigFile.exists())
	{
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

	rc = Syn_Site::ConstructSiteList(strConfigFilePath.toStdString(), _ListOfSitePtr);
	size_t ilistCounts = _ListOfSitePtr.size();
	if (0 == ilistCounts)
	{
		if (SendMsg)
			QMessageBox::critical(this, QString("Error"), QString("Can't construct the Site list,check it please!"));
		return false;
	}

	if (DeviceCounts < ilistCounts)
	{
		if (SendMsg)
			QMessageBox::critical(this, QString("Error"), QString("Current Devices' counts is more than limit,the limit is ") + QString::number(DeviceCounts) + ("!"));

		ilistCounts = DeviceCounts;
	}

	for (size_t i = 0; i < ilistCounts; i++)
	{
		_SynThreadArray[i].SetSite(_ListOfSitePtr[i]);
		_SynThreadArray[i].SetStopTag(true);
	}
	_iRealDeviceCounts = ilistCounts;

	return true;
}

void FPS_TestExecutive::CreateLocalSettings()
{
	if (NULL != _pSyn_LocalSettingsDlg)
	{
		_pSyn_LocalSettingsDlg->show();
		return;
	}
}

void FPS_TestExecutive::CloseLocalSettingsDialog()
{
	if (NULL != _pSyn_LocalSettingsDlg)
	{
		_pSyn_LocalSettingsDlg->hide();
	}
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
	}
}

bool FPS_TestExecutive::UpdateSiteLocalSettings()
{
	bool rc(false);

	//update LocalSettings info
	QString strConfigFilePath = _pSyn_LocalSettingsDlg->ui->SysConfigFileLlineEdit->text();
	if (QString("")==strConfigFilePath)
	{
		QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Config file value is NULL,check it please!"));
		cout << "Error:FPS_TestExecutive::UpdateSiteLocalSettings() - strConfigFilePath is NULL!" << endl;
		return false;
	}
	_LocalSettingsInfo._strSysConfigFilePath = strConfigFilePath.toStdString();


	
	_LocalSettingsInfo.m_bVerboseMode = _pSyn_LocalSettingsDlg->ui->VerboseLogCheckBox->isChecked();
	_LocalSettingsInfo.m_bQAMode = _pSyn_LocalSettingsDlg->ui->QAModeCheckBox->isChecked();
	_LocalSettingsInfo.m_bLGAMode = _pSyn_LocalSettingsDlg->ui->LGAModecheckBox->isChecked();

	_LocalSettingsInfo.m_bRunRepeatedly = _pSyn_LocalSettingsDlg->ui->AutoRepeatEnabledCheckBox->isChecked();

	_LocalSettingsInfo._strAutoController = _pSyn_LocalSettingsDlg->ui->AutoControllerComboBox->currentText().toStdString();

	//udpate SiteList
	rc = ConstructSiteList(strConfigFilePath);
	if (!rc || 0 == _iRealDeviceCounts)
	{
		QMessageBox::critical(_pSyn_LocalSettingsDlg, QString("Error"), QString("Can't retrieve the site,check it please!"));
		cout << "Error:FPS_TestExecutive::UpdateSiteLocalSettings() - ::ConstructSiteList is failed!" << endl;
		return false;
	}

	//clear
	for (int t = _pSyn_LocalSettingsDlg->ui->SiteTableWidget->rowCount(); t >= 1; t--)
	{
		_pSyn_LocalSettingsDlg->ui->SiteTableWidget->removeRow(t - 1);
	}
	_LocalSettingsInfo._listOfSiteSettings.clear();

	_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setRowCount(_iRealDeviceCounts);
	for (size_t t = 1; t <= _iRealDeviceCounts; t++)
	{
		SiteSettings CurrentSiteSettings;
		_ListOfSitePtr[t - 1]->GetSerialNumber(CurrentSiteSettings._uiDutSerNum);

		Syn_SysConfig synSysConfig;
		_ListOfSitePtr[t - 1]->GetSysConfig(synSysConfig);

		AdcBaseLineInfo CurrentAdcBaseLineInfo;
		CurrentAdcBaseLineInfo.m_nVdd = synSysConfig._uiDutpwrVdd_mV;
		CurrentAdcBaseLineInfo.m_nVio = synSysConfig._uiDutpwrVio_mV;
		CurrentAdcBaseLineInfo.m_nVled = synSysConfig._uiDutpwrVled_mV;
		CurrentAdcBaseLineInfo.m_nVddh = synSysConfig._uiDutpwrVddh_mV;

		for (int a = 0; a < NUM_CURRENT_VALUES; a++)
		{
			for (int b = 0; b < KNUMGAINS; b++)
			{
				(CurrentAdcBaseLineInfo.m_arAdcBaseLines)[a][b] = 0;
			}
		}

		//AdcBaseLineInfo CurrentAdcBaseLineInfo = (_LocalSettingsInfo._listOfSiteSettings)[t - 1]._adcBaseLineInfo;
		QString strVoltagesAll = QString::number(CurrentAdcBaseLineInfo.m_nVdd) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVio) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVled) + QString(" ") + QString::number(CurrentAdcBaseLineInfo.m_nVddh);

		QString strAdcBaseLinesValue("");
		for (int a = 0; a < NUM_CURRENT_VALUES; a++)
		{
			for (int b = 0; b < KNUMGAINS; b++)
			{
				strAdcBaseLinesValue += QString::number((CurrentAdcBaseLineInfo.m_arAdcBaseLines)[a][b]) + QString(" ");
			}
		}

		QString strSiteNumber(QString::number(t));
		//QString strSerialNumber(QString::number((_LocalSettingsInfo._listOfSiteSettings)[t - 1]._uiDutSerNum));

		_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(t - 1, 0, new QTableWidgetItem(strSiteNumber));
		_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(t - 1, 1, new QTableWidgetItem(QString::number(CurrentSiteSettings._uiDutSerNum)));
		_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(t - 1, 2, new QTableWidgetItem(strAdcBaseLinesValue));

		CurrentSiteSettings._adcBaseLineInfo = CurrentAdcBaseLineInfo;
		_LocalSettingsInfo._listOfSiteSettings.push_back(CurrentSiteSettings);
		//_ListOfSitePtr[t - 1]->SetSiteNumber(t);
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

	_pSyn_LocalSettingsDlg->SetUpdateTag(true);

	//Site TableWidget
	for (int t = ui.TestTableWidget->columnCount(); t >= 1; t--)
	{
		ui.TestTableWidget->removeColumn(t - 1);
	}

	ui.TestTableWidget->setColumnCount(_iRealDeviceCounts);
	QStringList strListOfHeader;
	for (size_t t = 1; t <= _iRealDeviceCounts; t++)
	{
		strListOfHeader.append(QString("Site")+QString::number(t));
		//ui.TestTableWidget->setItem(t-1,)

		ui.TestTableWidget->setColumnWidth(t-1, 200);
	}
	ui.TestTableWidget->setHorizontalHeaderLabels(strListOfHeader);

	return true;
}

void FPS_TestExecutive::LocalSettingsOKAction()
{
	bool bUpdateTag(false);
	_pSyn_LocalSettingsDlg->GetUpdateTag(bUpdateTag);
	if (!bUpdateTag)
	{
		bool rc = UpdateSiteLocalSettings();
		if (!rc)
		{
			return;
		}
	}

	_pSyn_LocalSettingsDlg->SetUpdateTag(false);

	_pSyn_LocalSettingsDlg->hide();
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

	//ui.TestTableWidget->clearContents();
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

	int iSiteNumber = pTestInfo->_uiSiteNumber;
	int iColumnIndex(iSiteNumber - 1);

	if (NULL != ui.TestTableWidget->item(6, iColumnIndex))
	{
		ui.TestTableWidget->item(6, iColumnIndex)->setText(QString(" "));
		//return;
	}
	
	QString strTestInfo("");
	//ui.textBrowser->append(strSerialNumber);
	strTestInfo += QString("Serial Number:") + QString::number(pTestInfo->_uiSerialNumber)+QString("\n");

	std::string strErrorMsg = pTestInfo->_strErrorMessage;
	Syn_TestState TestResult = pTestInfo->_TestState;
	if (TestError == TestResult)
	{
		//QMessageBox::critical(this, QString("Error"), strSerialNumber+QString(" Test is error,reason is ") + QString::fromStdString(strErrorMsg));
		//ui.TestTableWidget->setItem->append(QString("Test is Error,reason is ") + QString::fromStdString(strErrorMsg));
		strTestInfo += QString(" Serial Number:") + QString::number(pTestInfo->_uiSerialNumber) + QString("\n");
		ui.TestTableWidget->setItem(6, iColumnIndex, new QTableWidgetItem(strTestInfo));
		return;
	}
	else if (TestFailed == TestResult)
	{
		//QMessageBox::critical(this, QString("Failed"), strSerialNumber + QString(" Test is failed,reason is ") + QString::fromStdString(strErrorMsg));
		//ui.textBrowser->append(QString("Test is Failed,reason is ") + QString::fromStdString(strErrorMsg));
		strTestInfo += QString("Test is Failed,reason is ") + QString::fromStdString(strErrorMsg) + QString("\n");
		ui.TestTableWidget->setItem(6, iColumnIndex, new QTableWidgetItem(strTestInfo));
		return;
	}

	strTestInfo += QString("Boot Sector0:") + QString("\n");
	for (int i = 1; i <= BS0_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		
		QString s = "";
		for (int i = StartPos; i <= EndPos; i++)
		{
			s += (QString::number(pTestInfo->_BootSector0Array[i], 16)).toUpper() + ",";
		}

		strTestInfo += s + QString("\n");
	}

	strTestInfo += QString("Boot Sector1:") + QString("\n");
	for (int i = 1; i <= BS1_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;

		QString s = "";
		for (int i = StartPos; i <= EndPos; i++)
		{
			s += (QString::number(pTestInfo->_BootSector1Array[i], 16)).toUpper() + ",";
		}

		strTestInfo += s + QString("\n");
	}

	strTestInfo += QString("Main Sector0:") + QString("\n");

	for (int i = 1; i <= MS0_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;

		QString s = "";
		for (int i = StartPos; i <= EndPos; i++)
		{
			s += (QString::number(pTestInfo->_MainSector0Array[i], 16)).toUpper() + ",";
		}

		strTestInfo += s + QString("\n");
	}

	strTestInfo += QString("Main Sector1:") + QString("\n");
	for (int i = 1; i <= MS1_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;

		QString s = "";
		for (int i = StartPos; i <= EndPos; i++)
		{
			s += (QString::number(pTestInfo->_MainSector1Array[i], 16)).toUpper() + ",";
		}

		strTestInfo += s + QString("\n");
	}

	ui.TestTableWidget->setItem(6, iColumnIndex, new QTableWidgetItem(strTestInfo));
	ui.TestTableWidget->resizeRowToContents(6);
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
	//ui.textBrowser->append(s);
}