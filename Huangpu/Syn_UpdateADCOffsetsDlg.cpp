#include "Syn_UpdateADCOffsetsDlg.h"

Syn_UpdateADCOffsetsDlg::Syn_UpdateADCOffsetsDlg(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::Syn_UpdateADCOffsetsDlg();
	ui->setupUi(this);

	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowTitleHint);

	ui->VddLineEdit->setValidator(new QIntValidator(0, 3300, this));
	ui->VioLineEdit->setValidator(new QIntValidator(0, 3300, this));
	ui->VledLineEdit->setValidator(new QIntValidator(0, 3300, this));
	ui->VddhLineEdit->setValidator(new QIntValidator(0, 3300, this));

	ui->VddLineEdit->setText(QString("1800"));
	ui->VioLineEdit->setText(QString("1800"));
	ui->VledLineEdit->setText(QString("3300"));
	ui->VddhLineEdit->setText(QString("3300"));

	QObject::connect(ui->OKPushButton, SIGNAL(clicked()), this, SLOT(UpdateADC()));
	QObject::connect(ui->CancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
}

Syn_UpdateADCOffsetsDlg::~Syn_UpdateADCOffsetsDlg()
{
	if (NULL != ui)
	{
		delete ui;
		ui = NULL;
	}
}

void Syn_UpdateADCOffsetsDlg::UpdateADC()
{
	QString strVdd  = ui->VddLineEdit->text();
	QString strVio  = ui->VioLineEdit->text();
	QString strVled = ui->VledLineEdit->text();
	QString strVddh = ui->VddhLineEdit->text();

	uint32_t uiVdd = strVdd.toInt();
	uint32_t uiVio = strVio.toInt();
	uint32_t uiVled = strVled.toInt();
	uint32_t uiVddh = strVddh.toInt();

	Syn_LocalSettingsDlg *pLocalSettingsDlg = (Syn_LocalSettingsDlg*)parentWidget();
	pLocalSettingsDlg->UpdateADCOffsets(uiVdd, uiVio, uiVled, uiVddh);

	this->close();
}