//local
#include "fps_testexecutive.h"
//Qt
#include <QtWidgets>

#include <time.h>
#include <stdio.h>
#include <io.h>

FPS_TestExecutive::FPS_TestExecutive(QWidget *parent)
: QMainWindow(parent)
//, _bStopTag(true)
, _iRealDeviceCounts(0)
//, _logfile("sys.log")
, _FinishedSiteCounts(0)
{
	//ui setting
	ui.setupUi(this);
	ui.TestTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//ui.TestTableWidget->setRowHeight(5, 200);
	ui.TestTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.TestTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	//ui.TestTableWidget->verticalHeader()->setStretchLastSection(true);

	//remove OTPDump tab
	if (0 != ui.tabWidget->indexOf(ui.tab_2))
	{
		ui.tabWidget->removeTab(ui.tabWidget->indexOf(ui.tab_2));
	}

	//Init
	Initialize();

	//slots
	//Exit
	QObject::connect(ui.actionExit, SIGNAL(triggered(bool)), this, SLOT(Exit()));

	//Create LocalSettings Dialog
	QObject::connect(ui.actionLocalSetttings, SIGNAL(triggered(bool)), this, SLOT(CreateLocalSettings()));

	//Testing Operation
	QObject::connect(ui.pushButtonRun, SIGNAL(clicked()), this, SLOT(Run()));

	//Thread
	for (int i = 1; i <= DeviceCounts; i++)
	{
		QObject::connect(&(_SynMiniThreadArray[i - 1]), SIGNAL(sendImageInTime(unsigned int, const Syn_DutTestResult *)), this, SLOT(DisplayImageInTime(unsigned int, const Syn_DutTestResult *)));
		QObject::connect(&(_SynThreadArray[i - 1]), SIGNAL(send(unsigned int, const QString, const QString)), 
			this, SLOT(ReceiveTestStep(unsigned int, const QString, const QString)), Qt::ConnectionType(Qt::QueuedConnection));
		QObject::connect(&(_SynThreadArray[i - 1]), SIGNAL(send(unsigned int, const Syn_DutTestResult *)),
			this, SLOT(ReceiveTestResults(unsigned int, const Syn_DutTestResult *)), Qt::ConnectionType(Qt::QueuedConnection));
	}

	//BinCodes Dislpay
	QObject::connect(ui.actionBinCodes, SIGNAL(triggered(bool)), this, SLOT(CreateBinCodesDlg()));

	//About FPS
	QObject::connect(ui.actionAbout_FPS_Test_Executive, SIGNAL(triggered(bool)), this, SLOT(ShowAboutDlg()));

	//Debug Mode
	QObject::connect(ui.actionEngineer_Mode, SIGNAL(triggered(bool)), this, SLOT(CreateDebugModeDlg()));
	
	//Debug
	//OTP Dump
	QObject::connect(ui.pushButtonGetVer, SIGNAL(clicked()), this, SLOT(GetVersion()));
	QObject::connect(ui.pushButtonReadOTP, SIGNAL(clicked()), this, SLOT(ReadOTP()));

	//Invalidate sensor
	QObject::connect(ui.pushButtonInvalidate, SIGNAL(clicked()), this, SLOT(Invalidate()));
}

FPS_TestExecutive::~FPS_TestExecutive()
{
	this->ClearSites();
}

void FPS_TestExecutive::Initialize()
{
	bool rc(false);

	ui.OTPResultLabel->setText("");

	ui.TestInfoLabel->hide();

	//LocalSettings
	Syn_LocalSettingConfig *pSyn_LocalSettingConfig = NULL;
	rc = Syn_LocalSettingConfig::CreateLSConfigInstance(Syn_LocalSettingConfig::Read,pSyn_LocalSettingConfig);
	if (!rc || NULL == pSyn_LocalSettingConfig)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't retrieve Site info from LocalSettings,check it please!!"));
		cout << "Error:FPS_TestExecutive::Initialize() - pSyn_LocalSettingConfig is NULL!" << endl;
		return;
	}

	rc = pSyn_LocalSettingConfig->GetLocalSettings(_LocalSettingsInfo);

	unsigned int iLocalSettingSiteCounts = _LocalSettingsInfo._listOfSiteSettings.size();
	if (0 == iLocalSettingSiteCounts)
	{
		QMessageBox::critical(this, QString("Error"), QString("Can't retrieve Site info from LocalSettings,check it please!!"));
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

void FPS_TestExecutive::keyPressEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return)
	{
		Run();
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
		QMessageBox::critical(this, QString("Error"), QString("Can't get Site info from LocalSettings,check it please!"));
		return false;
	}

	//clear
	this->ClearSites();

	bool IsSucess(true);
	for (size_t i = 0; i < iLocalSettingsSiteCounts; i++)
	{
		string strSerialNumber = LocalSettingsInfo._listOfSiteSettings[i]._strDutSerNum;
		uint8_t uiSiteNumber = i + 1;
		Syn_Site *pSyn_SiteInstance = NULL; 
		rc = Syn_Site::CreateSiteInstance(uiSiteNumber, strSerialNumber, _LocalSettingsInfo._strSysConfigFilePath, LocalSettingsInfo._listOfSiteSettings[i]._adcBaseLineInfo, pSyn_SiteInstance);
		if (NULL == pSyn_SiteInstance || Syn_ExceptionCode::Syn_OK != rc)
		{
			string osErrorInfo("");
			GetErrorInfo(rc, osErrorInfo);
			QMessageBox::critical(this, QString("Error"), QString("Can't cosntruct Serial Number:") + QString::fromStdString(strSerialNumber) + QString(" device(") + QString::fromStdString(osErrorInfo) + QString("),check it please!"));
			IsSucess = false;
			break;
		}
		
		_ListOfSitePtr.push_back(pSyn_SiteInstance);
	}

	if (!IsSucess)
	{
		this->ClearSites();
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

		_SynMiniThreadArray[i].SetSite(_ListOfSitePtr[i]);
		_SynMiniThreadArray[i].SetStopTag(true);
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
		string strSerialNumber;
		_ListOfSitePtr[t - 1]->GetSerialNumber(strSerialNumber);
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
		
		QTableWidgetItem *itemSiteNumber = new QTableWidgetItem(QString::number(iSiteNumber) + QString(" (") + QString::fromStdString(strSerialNumber) + QString(") "));
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

	ui.pushButtonRun->setText("Run");

	//DutDump
	ui.comboBox->clear();
	ui.textBrowser->clear();
	for (size_t i = 1; i <= _ListOfSitePtr.size(); i++)
	{
		unsigned int iSiteNumber(0);
		_ListOfSitePtr[i - 1]->GetSiteNumber(iSiteNumber);
		ui.comboBox->addItem(QString("#") + QString::number(iSiteNumber));
	}

	//TestInfo:
	Syn_SysConfig TestConfig;
	_ListOfSitePtr[0]->GetSysConfig(TestConfig);
	QString qsTestInfo("");
	QString qsConfigFilePath = QString::fromStdString(LocalSettingsInfo._strSysConfigFilePath);
	int LastBackslashIndex = qsConfigFilePath.lastIndexOf(QChar('/'));
	QString qsConfigFileName = qsConfigFilePath.remove(0, LastBackslashIndex+1);
	qsTestInfo = QString("Vdd:") + QString::number(TestConfig._uiDutpwrVdd_mV) + QString(" Vio:") + QString::number(TestConfig._uiDutpwrVio_mV) +
					QString(" Vled:") + QString::number(TestConfig._uiDutpwrVled_mV) + QString(" Vddh:") + QString::number(TestConfig._uiDutpwrVddh_mV)+
					QString("\n") + qsConfigFileName;
	ui.TestInfoLabel->setText(qsTestInfo);
	QFont font;
	font.setFamily(QStringLiteral("Segoe UI"));
	font.setPointSize(9);
	font.setBold(false);
	font.setWeight(50);
	ui.TestInfoLabel->setFont(font);
	ui.TestInfoLabel->show();

	return true;
}

void FPS_TestExecutive::CreateLocalSettings()
{
	//clear
	this->ClearSites();

	Syn_LocalSettingsDlg *_pSyn_LocalSettingsDlg = new Syn_LocalSettingsDlg(this);
	_pSyn_LocalSettingsDlg->show();
	_pSyn_LocalSettingsDlg->setAttribute(Qt::WA_DeleteOnClose);
	//connect LocalSettings close signal to re-construct site list
	QObject::connect(_pSyn_LocalSettingsDlg, SIGNAL(destroyed()), this, SLOT(Initialize()));
}

void FPS_TestExecutive::CreateBinCodesDlg()
{
	Syn_BinCodesDlg *_pSyn_BinCodesDlg = new Syn_BinCodesDlg(this);
	_pSyn_BinCodesDlg->show();
	_pSyn_BinCodesDlg->setAttribute(Qt::WA_DeleteOnClose);
}

void FPS_TestExecutive::ShowAboutDlg()
{
	Syn_AboutDlg *pSyn_AboutDlg = new Syn_AboutDlg();
	pSyn_AboutDlg->show();
	pSyn_AboutDlg->setAttribute(Qt::WA_DeleteOnClose);
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
			if (NULL != ui.TestTableWidget->cellWidget(6, i - 1))
				ui.TestTableWidget->removeCellWidget(6, i - 1);
			if (NULL != ui.TestTableWidget->cellWidget(7, i - 1))
				ui.TestTableWidget->removeCellWidget(7, i - 1);
			if (NULL != ui.TestTableWidget->item(8, i - 1))
				ui.TestTableWidget->takeItem(8, i - 1);
		}

		ui.pushButtonRun->setDisabled(true);
		ui.actionBinCodes->setDisabled(true);
		ui.actionLocalSetttings->setDisabled(true);

		bool bWaitStimulus(false);
		std::vector<std::string> listOfTestStepName;
		_ListOfSitePtr[0]->GetTestStepList(listOfTestStepName);
		for (size_t i = 0; i < listOfTestStepName.size(); i++)
		{
			if (std::string("WaitStimulus") == listOfTestStepName[i])
			{
				bWaitStimulus = true;
				break;
			}
		}

		if (!bWaitStimulus)
		{
			iRunFlag = 3;
		}
	}
	else if (QString("Continue") == qText)
	{
		iRunFlag = 2;

		ui.pushButtonRun->setDisabled(true);
		ui.actionBinCodes->setDisabled(true);
		ui.actionLocalSetttings->setDisabled(true);
	}

	for (int i = 1; i <= iSiteCounts; i++)
	{
		if (_SynMiniThreadArray[i - 1].isRunning())
		{
			_SynMiniThreadArray[i - 1].SetStopTag(true);
			//::Sleep(400);
			_SynMiniThreadArray[i - 1].wait();
		}

		if (!_SynThreadArray[i - 1].isRunning())
		{
			_SynThreadArray[i - 1].SetFlag(iRunFlag);
			_SynThreadArray[i - 1].start();
			_SynThreadArray[i - 1].SetStopTag(false);
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

void FPS_TestExecutive::ReceiveTestStep(unsigned int iSiteNumber, const QString strTestStepName, const QString strPassResults)
{
	unsigned int iPos = iSiteNumber - 1;

	//Display Results first
	QString qsStepAndResult = strTestStepName + QString(" : ") + strPassResults;
	if (NULL != ui.TestTableWidget->item(8, iPos))
	{
		QString qsTempContent = ui.TestTableWidget->item(8, iPos)->text();
		ui.TestTableWidget->item(8, iPos)->setText(qsTempContent + QString("\n") + qsStepAndResult);
		ui.TestTableWidget->resizeRowToContents(8);
	}
	else
	{
		QTableWidgetItem *item = new QTableWidgetItem(qsStepAndResult);
		item->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(8, iPos, item);
		ui.TestTableWidget->resizeRowToContents(8);
	}

	if (QString("pass") != strPassResults.toLower())
	{
		ui.TestTableWidget->item(8, iPos)->setBackgroundColor(QColor(255, 0, 0));
	}

	Syn_Site::SiteState oTempState;
	_ListOfSitePtr[iPos]->GetState(oTempState);
	if (oTempState == Syn_Site::Error)
	{
		string errMsg = "";
		_ListOfSitePtr[iPos]->GetRunTimeError(errMsg);
		//QMessageBox::information(this, QString("Error"), QString("Error:") + QString::fromStdString(errMsg));

		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Error"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(2, iPos, itemState);
		itemState->setBackgroundColor(QColor(255, 0, 0));
		return;
	}

	//State
	QTableWidgetItem *itemState = new QTableWidgetItem(QString("Running"));
	itemState->setTextAlignment(Qt::AlignCenter);
	ui.TestTableWidget->setItem(2, iPos, itemState);

	//SerialNumber
	if (QString("InitializationStep") == strTestStepName)
	{
		Syn_DutTestInfo *pTestInfo = NULL;
		_ListOfSitePtr[iPos]->GetTestInfo(pTestInfo);
		if (NULL != pTestInfo)
		{
			QString strSerialumber = TransformSerialnumber(pTestInfo->_getVerInfo.serial_number, 6);
			QTableWidgetItem *itemSerialNumber = new QTableWidgetItem(strSerialumber);
			itemSerialNumber->setTextAlignment(Qt::AlignCenter);
			ui.TestTableWidget->setItem(1, iPos, itemSerialNumber);
		}
	}
}

QString FPS_TestExecutive::TransformSerialnumber(uint8_t *arrSerialnumber, uint32_t size)
{
	if (size < 6)
		return "";

	QString strSerialnumber("");

	strSerialnumber = QString::number(arrSerialnumber[4], 16) + QString::number(arrSerialnumber[5], 16) + QString::number(arrSerialnumber[0], 16) + 
						QString::number(arrSerialnumber[1], 16) + QString::number(arrSerialnumber[2], 16) + QString::number(arrSerialnumber[3], 16);

	return strSerialnumber.toUpper();
}

void FPS_TestExecutive::ReceiveTestResults(unsigned int iSiteNumber,const Syn_DutTestResult *pDutResult)
{
	unsigned int iPos = iSiteNumber - 1;

	unsigned int iFlag(0);
	bool bWaitStimulus(false);
	std::vector<std::string> listOfTestStepName;
	_ListOfSitePtr[iPos]->GetTestStepList(listOfTestStepName);
	if (0 != listOfTestStepName.size())
	{
		for (size_t i = 0; i < listOfTestStepName.size(); i++)
		{
			if (std::string("WaitStimulus") == listOfTestStepName[i])
			{
				bWaitStimulus = true;
				break;
			}
		}
	}
	if (bWaitStimulus)
	{
		QString qText = ui.pushButtonRun->text();
		if (QString("Run") == qText)
		{
			iFlag = 1;
		}
		else if (QString("Continue") == qText)
		{
			iFlag = 2;
		}
	}
	else
	{
		iFlag = 3;
	}

	_FinishedSiteCounts += 1;

	Syn_Site::SiteState oTempState;
	_ListOfSitePtr[iPos]->GetState(oTempState);
	if (oTempState == Syn_Site::Error)
	{
		string errMsg = "";
		_ListOfSitePtr[iPos]->GetRunTimeError(errMsg);

		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Error"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(2, iPos, itemState);
		itemState->setBackgroundColor(QColor(255, 0, 0));
		this->ManageButtonStatus(iFlag);
		return;
	}

	Syn_SysConfig CurrentSysConfig;
	_ListOfSitePtr[iPos]->GetSysConfig(CurrentSysConfig);
	if (NULL == pDutResult)
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
		rowNumber = pDutResult->_acqImgNoFingerResult.iRealRowNumber;
		columnNumber = pDutResult->_acqImgNoFingerResult.iRealColumnNumber;
		data.resize((rowNumber)*(columnNumber));
		
		for (int m = 0; m < rowNumber; m++)
		{
			for (int n = 0; n < columnNumber; n++)
			{
				data[m*(columnNumber)+n] = (pDutResult->_acqImgNoFingerResult).arr_ImageFPSFrame.arr[m][n];
				//data[m*(columnNumber)+n] = (pDutResult->_acqImgNoFingerResult.arr_nofinger[0]).arr[m][n];
			}
		}

		iRowNumber = 6;
	}
	else if (2 == iFlag)
	{
		rowNumber = pDutResult->_acqImgFingerResult.iRealRowNumber;
		columnNumber = pDutResult->_acqImgFingerResult.iRealColumnNumber;
		data.resize((rowNumber)*(columnNumber));

		for (int m = 0; m < rowNumber; m++)
		{
			for (int n = 0; n < columnNumber; n++)
			{
				data[m*(columnNumber)+n] = (pDutResult->_acqImgFingerResult).arr_ImageFPSFrame.arr[m][n];
			}
		}

		iRowNumber = 7;

		//SNR
		QString strSNRValue = QString::number(pDutResult->_snrResults.SNR[6]);
		Syn_DutTestInfo *pDutTestInfo = NULL;
		_ListOfSitePtr[iPos]->GetTestInfo(pDutTestInfo);
		if (NULL != pDutTestInfo)
		{
			/*if (pDutTestInfo->_huaweiIqTestInfo._bExecuted)
			{
				strSNRValue += QString(" ") + QString::number(pDutResult->_huaweiIqTestResults.snr);
			}*/

			if (pDutTestInfo->_snrInfo.m_bExecuted)
			{
				QTableWidgetItem *itemSNR = new QTableWidgetItem(strSNRValue);
				itemSNR->setTextAlignment(Qt::AlignCenter);
				ui.TestTableWidget->setItem(4, iPos, itemSNR);
			}
		}
		
		//BinCode
		QString qsBinCodes("");
		for (size_t i = 1; i <= pDutResult->_binCodes.size(); i++)
		{
			if (1 == i)
			{
				qsBinCodes += QString::fromStdString(pDutResult->_binCodes[i - 1]);
			}
			else
			{
				qsBinCodes = qsBinCodes + QString(" , ") + QString::fromStdString(pDutResult->_binCodes[i - 1]);
			}
		}
		QTableWidgetItem *itemBinCode = new QTableWidgetItem(qsBinCodes);
		itemBinCode->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(3, iPos, itemBinCode);

		//TotalResults
		QString strTotalResults("");
		if (1 == pDutResult->_binCodes.size())
		{
			if (Syn_BinCodes::m_sPass == pDutResult->_binCodes[0])
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
		ui.TestTableWidget->setItem(5, iPos, itemTotalResults);
	}
	else
	{
		//BinCode
		QString qsBinCodes("");
		for (size_t i = 1; i <= pDutResult->_binCodes.size(); i++)
		{
			if (1 == i)
			{
				qsBinCodes += QString::fromStdString(pDutResult->_binCodes[i - 1]);
			}
			else
			{
				qsBinCodes = qsBinCodes + QString(" , ") + QString::fromStdString(pDutResult->_binCodes[i - 1]);
			}
		}
		QTableWidgetItem *itemBinCode = new QTableWidgetItem(qsBinCodes);
		itemBinCode->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(3, iPos, itemBinCode);

		//TotalResults
		QString strTotalResults("");
		if (1 == pDutResult->_binCodes.size())
		{
			if (Syn_BinCodes::m_sPass == pDutResult->_binCodes[0])
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
		ui.TestTableWidget->setItem(5, iPos, itemTotalResults);
	}
	
	if (bWaitStimulus)
	{
		QImage image((const uchar*)data.constData(), columnNumber, rowNumber, columnNumber, QImage::Format_Indexed8);
		image.setColorTable(vcolorTable);
		//image = image.copy(iStartColumn, iStartRow, iEndColumn, iEndRow);

		QLabel *pImageLabel = new QLabel();
		pImageLabel->setPixmap(QPixmap::fromImage(image));
		pImageLabel->setAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setCellWidget(iRowNumber, iPos, pImageLabel);
		//ui.TestTableWidget->cellWidget(iRowNumber, iPos)->setStyle(QStyleFactory::create("Fusion"));

		ui.TestTableWidget->resizeRowToContents(iRowNumber);
	}

	if (2 == iFlag||3 == iFlag)
	{
		//writelog
		_ListOfSitePtr[iPos]->WriteLog(_LocalSettingsInfo._strLogFilePath);
		_ListOfSitePtr[iPos]->Close();

		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Closed"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(2, iPos, itemState);
	}
	else if (1==iFlag)
	{
		//State
		QTableWidgetItem *itemState = new QTableWidgetItem(QString("Idle"));
		itemState->setTextAlignment(Qt::AlignCenter);
		ui.TestTableWidget->setItem(2, iPos, itemState);
	}

	this->ManageButtonStatus(iFlag);
}

void FPS_TestExecutive::DisplayImageInTime(unsigned int iSiteNumber, const Syn_DutTestResult *pDutResult)
{
	unsigned int iPos = iSiteNumber - 1;
	if (NULL == pDutResult)
		return;

	Syn_DutTestInfo *pInfo = NULL;
	_ListOfSitePtr[iPos]->GetTestInfo(pInfo);
	if (NULL == pInfo)
		return;
	if (!pInfo->_calibrationInfo.m_bExecuted)
		return;

	int rowNumber = pDutResult->_WaitStimulusResults.iRealRowNumber;
	int columnNumber = pDutResult->_WaitStimulusResults.iRealColumnNumber;

	QVector<QRgb> vcolorTable;
	for (int i = 0; i < 256; i++)
	{
		vcolorTable.append(qRgb(i, i, i));
	}
	QByteArray data;
	data.resize((rowNumber)*(columnNumber));
	int k(0);
	for (int m = 0; m < rowNumber; m++)
	{
		for (int n = 0; n < columnNumber; n++)
		{
			data[k] = (pDutResult->_WaitStimulusResults)._ImagepFrame.arr[m][n];
			k++;
		}
	}

	QImage image((const uchar*)data.constData(), columnNumber, rowNumber, columnNumber, QImage::Format_Indexed8);
	image.setColorTable(vcolorTable);
	//image = image.copy(iStartColumn, iStartRow, iEndColumn, iEndRow);

	QLabel *pImageLabel = NULL;
	if (NULL != ui.TestTableWidget->cellWidget(7, iPos))
	{
		pImageLabel = static_cast<QLabel*>(ui.TestTableWidget->cellWidget(7, iPos));
	}
	else
	{
		pImageLabel = new QLabel();
	}

	pImageLabel->setPixmap(QPixmap::fromImage(image));
	pImageLabel->setAlignment(Qt::AlignCenter);
	ui.TestTableWidget->setCellWidget(7, iPos, pImageLabel);
	ui.TestTableWidget->resizeRowToContents(7);
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

			//if all failed,reset it to init
			ui.pushButtonRun->setText(QString("Run"));
			ui.pushButtonRun->setDisabled(false);
			ui.actionBinCodes->setDisabled(false);
			ui.actionLocalSetttings->setDisabled(false);
		}
		else
		{
			if (1 == iFlag)
			{
				ui.pushButtonRun->setText(QString("Continue"));
				ui.pushButtonRun->setDisabled(false);

				//DisplayImage In time
				for (size_t i = 1; i <= _ListOfSitePtr.size(); i++)
				{
					if (!_SynMiniThreadArray[i - 1].isRunning())
					{
						_SynMiniThreadArray[i - 1].SetStopTag(false);
						_SynMiniThreadArray[i - 1].start();
					}
				}
			}
			else if (2 == iFlag)
			{
				ui.pushButtonRun->setText(QString("Run"));
				ui.pushButtonRun->setDisabled(false);
				ui.actionBinCodes->setDisabled(false);
				ui.actionLocalSetttings->setDisabled(false);
			}
			else
			{
				ui.pushButtonRun->setText(QString("Run"));
				ui.pushButtonRun->setDisabled(false);
				ui.actionBinCodes->setDisabled(false);
				ui.actionLocalSetttings->setDisabled(false);
			}
		}
		
	}
}

void FPS_TestExecutive::GetErrorInfo(uint32_t iErrorCode,std::string &osErrorInfo)
{
	switch (iErrorCode)
	{
		case Syn_ExceptionCode::Syn_UnknownError:
			osErrorInfo = "Unknown Error";
			break;

		case Syn_ExceptionCode::Syn_DutInfoNull:
			osErrorInfo = "DutInfo is NULL";
			break;

		case Syn_ExceptionCode::Syn_DutResultNull:
			osErrorInfo = "DutResult is NULL";
			break;

		case Syn_ExceptionCode::Syn_DutNull:
			osErrorInfo = "Dut is NULL";
			break;

		case Syn_ExceptionCode::Syn_DutCtrlNull:
			osErrorInfo = "DutCtrl is NULL";
			break;

		case Syn_ExceptionCode::Syn_DutPatchError:
			osErrorInfo = "DutPatch is NULL";
			break;

		case Syn_ExceptionCode::Syn_ConfigError:
			osErrorInfo = "Config is Error";
			break;

		case Syn_ExceptionCode::Syn_TestStepConfigError:
			osErrorInfo = "TestStep Config is Error";
			break;

		case Syn_ExceptionCode::Syn_TestStepNotExecuted:
			osErrorInfo = "TestStep NotExecuted Error";
			break;

		case Syn_ExceptionCode::Syn_SiteStateError:
			osErrorInfo = "SiteState is Error";
			break;

		case Syn_ExceptionCode::Syn_SiteThread:
			osErrorInfo = "SiteThread is Error";
			break;

		case Syn_ExceptionCode::Syn_ERR_GET_DEVICE_HANDLE_FAILED:
			osErrorInfo = "Cann't get device handle";
			break;

		default:
			osErrorInfo = "unknown error";
	}
}

void FPS_TestExecutive::ClearSites()
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
}

void FPS_TestExecutive::GetVersion()
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
		QMessageBox::information(this, QString("Getverison Error : 0x"), QString::number(rc, 16).toUpper());
		return;
	}

	Syn_DutTestResult *oDutTestResult = NULL;
	rc = pSelectedSite->ExecuteTestStep("InitializationStep");
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Getverison Error : 0x"), QString::number(rc, 16).toUpper());
		return;
	}
	rc = pSelectedSite->GetTestResult(oDutTestResult);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Getverison Error : 0x"), QString::number(rc, 16).toUpper());
		return;
	}
	Syn_DutTestInfo *pInfo = NULL;
	rc = pSelectedSite->GetTestInfo(pInfo);
	if (0 != rc)
	{
		QMessageBox::information(this, QString("Getverison Error : 0x"), QString::number(rc, 16).toUpper());
		return;
	}
	rc = pSelectedSite->ExecuteTestStep("FinalizationStep");

	time_t buildtime;
	struct tm mytm;
	wchar_t timestr[80];
	buildtime = (time_t)pInfo->_getVerInfo.buildtime;
	mytm = *localtime(&buildtime);
	(void)wcsftime(&(timestr[0]), (sizeof((timestr)) / sizeof((timestr)[0])), L"%d-%b-%Y %I:%M:%S %p %Z", &mytm);
	
	QString strProductType("");
	switch (pInfo->_getVerInfo.product)
	{
		case 48:
			strProductType = " (Metallica) ";
			break;
		case 51:
			strProductType = " (Viper2) ";
			break;
		default:
			break; 
	}

	QString strTarget("");
	switch (pInfo->_getVerInfo.target)
	{
		case 1:
			strTarget = " (ROM) ";
			break;
		case 2:
			strTarget = " (FPGA) ";
			break;
		case 3:
			strTarget = " (RTLSIM) ";
			break;
		case 4:
			strTarget = " (ISS) ";
			break;
		case 5:
			strTarget = " (FPGADBG) ";
			break;
		default:
			strTarget = " (???) ";
			break;
	}

	ui.textBrowser->clear();
	ui.textBrowser->append(QString("Version:"));
	//QDateTime timeValue = QDateTime::fromTime_t(pInfo->_getVerInfo.buildtime + 43200);
	//ui.textBrowser->append(QString("buildtime:") + timeValue.toString());
	ui.textBrowser->append(QString("buildtime:") + QString::fromWCharArray(timestr));
	ui.textBrowser->append(QString("buildnum:") + QString::number(pInfo->_getVerInfo.buildnum));
	ui.textBrowser->append(QString("vmajor:") + QString::number(pInfo->_getVerInfo.vmajor));
	ui.textBrowser->append(QString("vminor:") + QString::number(pInfo->_getVerInfo.vminor));
	ui.textBrowser->append(QString("target:") + QString::number(pInfo->_getVerInfo.target) + strTarget);
	ui.textBrowser->append(QString("product:") + QString::number(pInfo->_getVerInfo.product) + strProductType);
	ui.textBrowser->append(QString("siliconrev:") + QString::number(pInfo->_getVerInfo.siliconrev));
	ui.textBrowser->append(QString("formalrel:") + QString::number(pInfo->_getVerInfo.formalrel));
	ui.textBrowser->append(QString("platform:") + QString::number(pInfo->_getVerInfo.platform));
	ui.textBrowser->append(QString("patch:") + QString::number(pInfo->_getVerInfo.patch));
	ui.textBrowser->append(QString("serial_number:") + QString::fromLatin1(oDutTestResult->_versionResult.sSerialNumber, 12));
	ui.textBrowser->append(QString("security:") + QString::number(pInfo->_getVerInfo.security[0]) + QString("|") + QString::number(pInfo->_getVerInfo.security[1]));
	ui.textBrowser->append(QString("patchsig:") + QString::number(pInfo->_getVerInfo.patchsig));
	ui.textBrowser->append(QString("iface:") + QString::number(pInfo->_getVerInfo.iface));
	ui.textBrowser->append(QString("otpsig:") + QString::number(pInfo->_getVerInfo.otpsig[0]) + QString("|") + QString::number(pInfo->_getVerInfo.otpsig[1]) + QString("|") + QString::number(pInfo->_getVerInfo.otpsig[2]));
	ui.textBrowser->append(QString("otpspare1:") + QString::number(pInfo->_getVerInfo.otpspare1));
	ui.textBrowser->append(QString("reserved:") + QString::number(pInfo->_getVerInfo.reserved));
	ui.textBrowser->append(QString("device_type:") + QString::number(pInfo->_getVerInfo.device_type));

	pSelectedSite->Close();
}

void FPS_TestExecutive::ReadOTP()
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
		QMessageBox::information(this, QString("Error"), QString("OTPDump Error : 0x") + QString::number(rc, 16).toUpper());
		return;
	}

	rc = pSelectedSite->ExecuteTestStep("InitializationStep");
	rc = pSelectedSite->ExecuteTestStep("OTPCheck");
	rc = pSelectedSite->ExecuteTestStep("FinalizationStep");
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("OTPDump Error : 0x") + QString::number(rc, 16).toUpper());
		return;
	}
	Syn_DutTestInfo *oDutTestInfo = NULL;
	rc = pSelectedSite->GetTestInfo(oDutTestInfo);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("OTPDump Error : 0x") + QString::number(rc, 16).toUpper());
		return;
	}
	Syn_DutTestResult *oDutTestResult = NULL;
	rc = pSelectedSite->GetTestResult(oDutTestResult);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("OTPDump Error : 0x") + QString::number(rc, 16).toUpper());
		return;
	}

	//Syn_SiteInfo oSiteInfo;
	unsigned int oSiteNumber;
	pSelectedSite->GetSiteNumber(oSiteNumber);

	string strSerialNumber;
	pSelectedSite->GetSerialNumber(strSerialNumber);

	ui.textBrowser->clear();
	ui.textBrowser->append(QString("SiteNumber:") + QString::number(oSiteNumber));
	ui.textBrowser->append(QString("Device SerialNumber:") + QString::fromStdString(strSerialNumber));
	ui.textBrowser->append(QString("Sensor SerialNumber:") + QString::fromLatin1(oDutTestResult->_versionResult.sSerialNumber, 12));

	QPalette pa;
	if (oDutTestResult->_binCodes[0] == "1")
	{
		ui.textBrowser->append(QString("Result: PASS"));
		ui.OTPResultLabel->setText("Result: PASS");
		pa.setColor(QPalette::WindowText, Qt::green);
	}
	else
	{
		QString sBinCodes = "";
		if (1 == oDutTestResult->_binCodes.size())
		{
			sBinCodes += (oDutTestResult->_binCodes[0]).c_str();
		}
		else
		{
			for (size_t i = 0; i < oDutTestResult->_binCodes.size(); i++)
			{
				sBinCodes += (oDutTestResult->_binCodes[i] + ", ").c_str();
			}
		}
		
		ui.textBrowser->append(QString("Result: FAIL ") + sBinCodes);
		ui.OTPResultLabel->setText(QString("Result: FAIL - Bincode: ") + sBinCodes);
		pa.setColor(QPalette::WindowText, Qt::red);
	}
	QFont ft;
	ft.setPointSize(10);
	ui.OTPResultLabel->setFont(ft);
	ui.OTPResultLabel->setPalette(pa);


	ui.textBrowser->append(QString("\nBoot Sector 0:"));
	ui.textBrowser->append(QString("=============="));
	for (int i = 1; i <= BS0_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;

		Display(oDutTestInfo->_otpCheckInfo._BootSector0Array, StartPos, EndPos);
	}

	ui.textBrowser->append(QString("\nBoot Sector 1:"));
	ui.textBrowser->append(QString("=============="));
	for (int i = 1; i <= BS1_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;

		Display(oDutTestInfo->_otpCheckInfo._BootSector1Array, StartPos, EndPos);
	}

	ui.textBrowser->append(QString("\nMain Sector 0:"));
	ui.textBrowser->append(QString("=============="));
	for (int i = 1; i <= MS0_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		Display(oDutTestInfo->_otpCheckInfo._MainSector0Array, StartPos, EndPos);
	}

	ui.textBrowser->append(QString("\nMain Sector 1:"));
	ui.textBrowser->append(QString("=============="));
	for (int i = 1; i <= MS1_SIZE / 8; i++)
	{
		int StartPos = (i - 1) * 8;
		int EndPos = i * 8 - 1;
		Display(oDutTestInfo->_otpCheckInfo._MainSector1Array, StartPos, EndPos);
	}

	//tag value
	ui.textBrowser->append(QString("\nTAG Value:"));
	ui.textBrowser->append(QString("=============="));
	for (std::map<std::string, std::string>::iterator iter = oDutTestResult->_mapMainSectorInfo.begin(); iter != oDutTestResult->_mapMainSectorInfo.end(); iter++)
	{
		QString qsTempValue = QString::fromStdString(iter->first) + ":" + QString::fromStdString(iter->second) + "\n";
		ui.textBrowser->append(qsTempValue);
	}

	pSelectedSite->Close();
}

void FPS_TestExecutive::Invalidate()
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

	QMessageBox::StandardButton ret = QMessageBox::warning(this, QString("Warning"), QString("The Selected Sensor will be invalidate,click \"Yes\" to continue,\"No\" to exit!"), QMessageBox::Yes | QMessageBox::No);
	if (QMessageBox::Yes != ret)
	{
		return;
	}

	uint32_t rc = pSelectedSite->Open();
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error : 0x"), QString::number(rc, 16).toUpper());
		return;
	}

	Syn_DutTestResult *oDutTestResult = NULL;
	rc = pSelectedSite->ExecuteTestStep("InitializationStep");
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("Invalidate is failed(1), error code is 0x") + QString::number(rc, 16).toUpper());
		return;
	}
	rc = pSelectedSite->ExecuteTestStep("Invalidate");
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("Invalidate is failed(2), error code is 0x") + QString::number(rc, 16).toUpper());
		return;
	}
	rc = pSelectedSite->ExecuteTestStep("FinalizationStep");
	pSelectedSite->GetTestResult(oDutTestResult);
	if (rc != 0)
	{
		QMessageBox::information(this, QString("Error"), QString("Invalidate is failed(3), error code is 0x") + QString::number(rc, 16).toUpper());
		return;
	}

	pSelectedSite->Close();

	//OTP Check
	this->ReadOTP();
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
		QString strValue = (QString::number(pDst[i], 16)).toUpper();
		if (1 == strValue.size())
			strValue = "0" + strValue;
		s += strValue + ",";
	}
	ui.textBrowser->append(s);
}

void FPS_TestExecutive::CreateDebugModeDlg()
{
	Syn_DebugModeDlg *pSyn_DebugModeDlg = new Syn_DebugModeDlg();
	pSyn_DebugModeDlg->show();
	pSyn_DebugModeDlg->setAttribute(Qt::WA_DeleteOnClose);

	QObject::connect(pSyn_DebugModeDlg, SIGNAL(sendPass()), this, SLOT(StartDebugMode()));
}

void FPS_TestExecutive::StartDebugMode()
{
	if (-1 == ui.tabWidget->indexOf(ui.tab_2))
	{
		ui.tabWidget->addTab(ui.tab_2, "Engineer Mode");
	}	
}
