#include "Syn_LocalSettingsDlg.h"
#include "ui_Syn_LocalSettingsDlg.h"
#include <QtWidgets>

Syn_LocalSettingsDlg::Syn_LocalSettingsDlg(QWidget *parent)
: QDialog(parent)
, _pSyn_DeviceManager(NULL)
//, _pSyn_SerialNumberManageDlg(NULL)
//, _pSyn_UpdateADCOffsetsDlg(NULL)
//, _pSyn_UpdateFirmwareProcessDlg(NULL)
{
	ui = new Ui::Syn_LocalSettingsDlg();
	ui->setupUi(this);

	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowTitleHint);

	ui->SiteCountsLineEdit->setValidator(new QIntValidator(0, 20, this));

	ui->SiteTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//ui->SiteTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); 
	//ui->SiteTableWidget->setSelectionMode(QAbstractItemView::SingleSelection); 
	ui->SiteTableWidget->verticalHeader()->setVisible(false);
	ui->SiteTableWidget->horizontalHeader()->setStretchLastSection(true);

	//localSettings slots
	QObject::connect(ui->CancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui->SelectSysConfigFilePushButton, SIGNAL(clicked()), this, SLOT(SelectConfigFile()));
	QObject::connect(ui->SelectLogFilePushButton, SIGNAL(clicked()), this, SLOT(SelectLogFilePath()));
	QObject::connect(ui->SiteCountsLineEdit, SIGNAL(editingFinished()), this, SLOT(ModifySiteCounts()));//returnPressed
	QObject::connect(ui->ModifySerialNumberPushButton, SIGNAL(clicked()), this, SLOT(CreateSerialNumberManageDialog()));
	QObject::connect(ui->UpdateADCOffsetsPushButton, SIGNAL(clicked()), this, SLOT(CreateUpdateADCOffsetsDlg()));
	QObject::connect(ui->SiteTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(SetLeds(int, int)));
	QObject::connect(ui->OKPushButton, SIGNAL(clicked()), this, SLOT(ConfirmSite()));


	//load localsetting
	Syn_LocalSettingConfig *pSyn_LocalSettingConfig = NULL;
	Syn_LocalSettingConfig::CreateLSConfigInstance(Syn_LocalSettingConfig::Read,pSyn_LocalSettingConfig);
	pSyn_LocalSettingConfig->GetLocalSettings(_LocalSettingsInfo);
	delete pSyn_LocalSettingConfig;
	pSyn_LocalSettingConfig = NULL;

	//operation
	ui->SysConfigFileLlineEdit->clear();
	ui->SysConfigFileLlineEdit->setText(QString::fromStdString(_LocalSettingsInfo._strSysConfigFilePath));

	ui->LogFileLineEdit->clear();
	ui->LogFileLineEdit->setText(QString::fromStdString(_LocalSettingsInfo._strLogFilePath));

	unsigned int iLocalSettingSiteCounts = _LocalSettingsInfo._listOfSiteSettings.size();
	ui->SiteCountsLineEdit->clear();
	ui->SiteCountsLineEdit->setText(QString::number(iLocalSettingSiteCounts));

	ui->SiteTableWidget->setRowCount(iLocalSettingSiteCounts);
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

		ui->SiteTableWidget->setItem(i, 0, new QTableWidgetItem(strSiteNumber));
		ui->SiteTableWidget->setItem(i, 1, new QTableWidgetItem(strSerialNumber));
		ui->SiteTableWidget->setItem(i, 2, new QTableWidgetItem(strAdcBaseLinesValue));

		_TempVoltagesValue.nVdd = CurrentAdcBaseLineInfo.m_nVdd;
		_TempVoltagesValue.nVio = CurrentAdcBaseLineInfo.m_nVio;
		_TempVoltagesValue.nVled = CurrentAdcBaseLineInfo.m_nVled;
		_TempVoltagesValue.nVddh = CurrentAdcBaseLineInfo.m_nVddh;
	}

	if (NULL == _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager = new Syn_DeviceManager();
	}
	//_pSyn_DeviceManager->Close();
	uint32_t uiResult = _pSyn_DeviceManager->Open();
}

Syn_LocalSettingsDlg::~Syn_LocalSettingsDlg()
{
	if (NULL != ui)
	{
		delete ui;
		ui = NULL;
	}
	if (NULL != _pSyn_DeviceManager)
	{
		delete _pSyn_DeviceManager;
		_pSyn_DeviceManager = NULL;
	}
}

void Syn_LocalSettingsDlg::ConfirmSite()
{
	bool rc(false);

	//check Site Info first
	int iUserDefineSiteCounts = ui->SiteTableWidget->rowCount();
	if (0 == iUserDefineSiteCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Current Site counts is 0!"));
		return;
	}

	std::vector<uint32_t> listOfCurrentSerialNumber;
	for (int i = 0; i < iUserDefineSiteCounts; i++)
	{
		QString strSerialNumber("");
		QString strAdcValue("");

		QTableWidgetItem *pSerialNumberItem = ui->SiteTableWidget->item(i, 1);
		if (NULL != pSerialNumberItem)
			strSerialNumber = pSerialNumberItem->text();
		QTableWidgetItem *pAdcValueItem = ui->SiteTableWidget->item(i, 2);
		if (NULL != pAdcValueItem)
			strAdcValue = pAdcValueItem->text();

		uint32_t uiSerialNumber = strSerialNumber.toInt();
		if (0 == uiSerialNumber || QString("") == strSerialNumber)
		{
			QMessageBox::critical(this, QString("Error"), QString("Site ")+QString::number(i+1)+QString("'s SerialNumber is 0!"));
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
			QMessageBox::critical(this, QString("Error"), QString("Site ") + QString::number(i + 1) + QString("'s SerialNumber has been used!"));
			return;
		}

		listOfCurrentSerialNumber.push_back(uiSerialNumber);
	}

	//update LocalSettings info
	QString strConfigFilePath = ui->SysConfigFileLlineEdit->text();
	QFile configfile(strConfigFilePath);
	if (!configfile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("Config file is not exists,check it please!"));
		return;
	}
	_LocalSettingsInfo._strSysConfigFilePath = strConfigFilePath.toStdString();

	QString strLogFilePath = ui->LogFileLineEdit->text();
	QFile Logfile(strLogFilePath);
	if (!Logfile.exists())
	{
		QMessageBox::critical(this, QString("Error"), QString("LogFile Path is not exists,check it please!"));
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
		QTableWidgetItem *pSerialNumberItem = ui->SiteTableWidget->item(i, 1);
		if (NULL != pSerialNumberItem)
			strSerialNumber = pSerialNumberItem->text();
		QTableWidgetItem *pAdcValueItem = ui->SiteTableWidget->item(i, 2);
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
				(CurrentAdcBaseLineInfo.m_arAdcBaseLines)[a][b] = strListOfAdcBaseLinesValue[(NUM_CURRENT_VALUES)*b + a].toInt();
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
	rc = Syn_LocalSettingConfig::CreateLSConfigInstance(Syn_LocalSettingConfig::Write,pLocalSettingsInstance);
	if (rc&&NULL != pLocalSettingsInstance)
	{
		pLocalSettingsInstance->SetLocalSettings(_LocalSettingsInfo);
		delete pLocalSettingsInstance;
		pLocalSettingsInstance = NULL;
	}

	Syn_UpdateFirmwareProcessDlg *_pSyn_UpdateFirmwareProcessDlg = new Syn_UpdateFirmwareProcessDlg(this);
	_pSyn_UpdateFirmwareProcessDlg->setAttribute(Qt::WA_DeleteOnClose);
	_pSyn_UpdateFirmwareProcessDlg->show();
	//_pSyn_UpdateFirmwareProcessDlg->close();
	
	//uint32_t uiResults = _pSyn_DeviceManager->UpdateFirmware();
	this->close();
}

void Syn_LocalSettingsDlg::SelectConfigFile()
{
	QString strConfigFilePath = QFileDialog::getOpenFileName(this, "Select Config File", "./", "xml file(*.xml)");
	if (QString("") != strConfigFilePath)
	{
		QFile TempFile(strConfigFilePath);
		if (!TempFile.exists())
			return;

		ui->SysConfigFileLlineEdit->clear();
		ui->SysConfigFileLlineEdit->setText(strConfigFilePath);

		_LocalSettingsInfo._strSysConfigFilePath = strConfigFilePath.toStdString();
	}
}

void Syn_LocalSettingsDlg::SelectLogFilePath()
{
	QString strLogFileFolderPath = QFileDialog::getExistingDirectory(this, "Select LogFile Path");
	if (QString("") != strLogFileFolderPath)
	{
		QFile TempFile(strLogFileFolderPath);
		if (!TempFile.exists())
			return;

		ui->LogFileLineEdit->clear();
		ui->LogFileLineEdit->setText(strLogFileFolderPath);

		_LocalSettingsInfo._strLogFilePath = strLogFileFolderPath.toStdString();
	}
}

void Syn_LocalSettingsDlg::ModifySiteCounts()
{
	if (NULL == _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager = new Syn_DeviceManager();
	}

	uint32_t rc = _pSyn_DeviceManager->Open();
	if (Syn_ExceptionCode::Syn_OK != rc)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't Open Device Manage,check it please!"));
		return;
	}

	QString strUserSiteCounts = ui->SiteCountsLineEdit->text();
	int iUserSiteCounts = strUserSiteCounts.toInt();

	std::vector<uint32_t> listOfSerialNumber;
	_pSyn_DeviceManager->GetSerialNumberList(listOfSerialNumber);
	if (iUserSiteCounts > listOfSerialNumber.size())
	{
		QMessageBox::critical(this, QString("Error"), QString("Current Site Numbers is more than real Device conunts(") + QString::number(listOfSerialNumber.size()) + QString(")!"));
		return;
	}

	int iCurrentRowCounts = ui->SiteTableWidget->rowCount();
	if (iCurrentRowCounts < iUserSiteCounts)
	{
		for (unsigned int i = iCurrentRowCounts+1; i <= iUserSiteCounts; i++)
		{
			ui->SiteTableWidget->insertRow(i-1);
			ui->SiteTableWidget->setItem(i - 1, 0, new QTableWidgetItem(QString::number(i)));
			ui->SiteTableWidget->setItem(i - 1, 1, new QTableWidgetItem(QString("0")));
			ui->SiteTableWidget->setItem(i - 1, 2, new QTableWidgetItem("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "));
		}
	}
	else if (iCurrentRowCounts > iUserSiteCounts)
	{
		for (unsigned int i = iCurrentRowCounts; i > iUserSiteCounts; i--)
		{
			ui->SiteTableWidget->removeRow(i - 1);
		}
	}
}

void Syn_LocalSettingsDlg::SetLeds(int rowNumber, int columnNumber)
{
	//get SerialNumber
	int iSerialNumberColumn(1);
	QTableWidgetItem *item = NULL;
	item = ui->SiteTableWidget->item(rowNumber, iSerialNumberColumn);
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

void Syn_LocalSettingsDlg::CreateSerialNumberManageDialog()
{
	Syn_SerialNumberManageDlg *_pSyn_SerialNumberManageDlg = new Syn_SerialNumberManageDlg(this);
	_pSyn_SerialNumberManageDlg->setAttribute(Qt::WA_DeleteOnClose);
	_pSyn_SerialNumberManageDlg->show();
}

void Syn_LocalSettingsDlg::GetSerialNumberList(std::vector<uint32_t> &listOfSerialNumber)
{

	if (NULL == _pSyn_DeviceManager)
	{
		_pSyn_DeviceManager = new Syn_DeviceManager();
	}

	uint32_t rc = _pSyn_DeviceManager->Open();
	_pSyn_DeviceManager->GetSerialNumberList(listOfSerialNumber);
	if (0 == listOfSerialNumber.size())
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't find device,check it please!"));
		return;
	}
}

void Syn_LocalSettingsDlg::SetSerialNumberForSite(const QString strSerialNumber)
{

	int iSiteRowIndex = ui->SiteTableWidget->currentRow();
	if (iSiteRowIndex < 0)
		iSiteRowIndex = 0;
	ui->SiteTableWidget->setItem(iSiteRowIndex, 1, new QTableWidgetItem(strSerialNumber));
	//_pSyn_LocalSettingsDlg->ui->SiteTableWidget->setItem(iSiteRowIndex, 2, new QTableWidgetItem(QString("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ")));
}

void Syn_LocalSettingsDlg::CreateUpdateADCOffsetsDlg()
{

	Syn_UpdateADCOffsetsDlg *_pSyn_UpdateADCOffsetsDlg = new Syn_UpdateADCOffsetsDlg(this);
	_pSyn_UpdateADCOffsetsDlg->setAttribute(Qt::WA_DeleteOnClose);
	_pSyn_UpdateADCOffsetsDlg->show();
}

void Syn_LocalSettingsDlg::UpdateADCOffsets(int Vdd, int Vio, int Vled, int Vddh)
{
	int iSiteCounts = ui->SiteTableWidget->rowCount();
	if (0 == iSiteCounts)
	{
		return;
	}
	_TempVoltagesValue.nVdd  = Vdd;
	_TempVoltagesValue.nVio  = Vio;
	_TempVoltagesValue.nVled = Vled;
	_TempVoltagesValue.nVddh = Vddh;

	uint32_t rc = 0;
	for (int i = 0; i < iSiteCounts; i++)
	{
		QString strSerialNumber("");
		QTableWidgetItem *item = ui->SiteTableWidget->item(i, 1);
		if (NULL != item)
		{
			strSerialNumber = item->text();
			uint32_t uiSerialNumber = strSerialNumber.toInt();
			uint32_t arAdcBaseLines[NUM_CURRENT_VALUES][KNUMGAINS] = { 0 };

			rc = _pSyn_DeviceManager->UpdateADCOffsets(uiSerialNumber, Vdd, Vio, Vled, Vddh, arAdcBaseLines);
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
				ui->SiteTableWidget->setItem(i, 2, new QTableWidgetItem(strAdcBaseLinesValue));
			}
		}
	}
}

int Syn_LocalSettingsDlg::GetSiteRowIndex()
{ 
	if (NULL != ui)
	{
		return ui->SiteTableWidget->currentRow(); 
	}
	else
	{
		return 0;
	}
}