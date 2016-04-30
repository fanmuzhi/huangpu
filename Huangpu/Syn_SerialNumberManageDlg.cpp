#include "Syn_SerialNumberManageDlg.h"
#include <QtWidgets>

Syn_SerialNumberManageDlg::Syn_SerialNumberManageDlg(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::Syn_SerialNumberManageDlg();
	ui->setupUi(this);

	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowTitleHint);

	ui->SerialNumberTableWidget->setColumnWidth(1,320);
	ui->SerialNumberTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QObject::connect(ui->CancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui->OKPushButton, SIGNAL(clicked()), this, SLOT(SelectSerialNumber()));

}

Syn_SerialNumberManageDlg::~Syn_SerialNumberManageDlg()
{
	if (NULL != ui)
	{
		delete ui;
		ui = NULL;
	}
}

void Syn_SerialNumberManageDlg::showEvent(QShowEvent * event)
{
	Syn_LocalSettingsDlg *pLocalSettingsDlg = (Syn_LocalSettingsDlg*)parentWidget();

	//get row index
	int iSelectRowIndex = pLocalSettingsDlg->GetSiteRowIndex();
	if (iSelectRowIndex < 0)
		iSelectRowIndex = 0;
	ui->SiteManagePromptLabel->setText(QString("Select Serial Number for Site ") + QString::number(iSelectRowIndex+1)+QString(":"));

	//get serial number list
	std::vector<uint32_t> listOfSerialNumber;
	pLocalSettingsDlg->GetSerialNumberList(listOfSerialNumber);

	ui->SerialNumberTableWidget->setRowCount(listOfSerialNumber.size());
	for (size_t i = 1; i <= listOfSerialNumber.size(); i++)
	{
		ui->SerialNumberTableWidget->setItem(i - 1, 0, new QTableWidgetItem(QString::number(listOfSerialNumber[i-1])));
	}
}
//void Syn_SerialNumberManageDlg::closeEvent(QCloseEvent * event)
//{
//	ui.CancelPushButton->click();
//}

void Syn_SerialNumberManageDlg::SelectSerialNumber()
{
	
	bool focus = ui->SerialNumberTableWidget->isItemSelected(ui->SerialNumberTableWidget->currentItem());
	if (!focus)
	{
		QMessageBox::critical(this, QString("Error"), QString("Select a SerialNumber for Site,please!"));
		return;
	}

	int iSerialNumberRowIndex = ui->SerialNumberTableWidget->currentRow();
	QString strSerialNumber = ui->SerialNumberTableWidget->item(iSerialNumberRowIndex, 0)->text();

	Syn_LocalSettingsDlg *pLocalSettingsDlg = (Syn_LocalSettingsDlg*)parentWidget();

	pLocalSettingsDlg->SetSerialNumberForSite(strSerialNumber);
}