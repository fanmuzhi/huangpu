#include "Syn_AboutDlg.h"
#include "QDate"
#include "Syn_TestUtils.h"

Syn_AboutDlg::Syn_AboutDlg(QWidget *parent)
	: QWidget(parent)
	, ui(NULL)
{
	ui = new Ui::Syn_AboutDlg();
	ui->setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	this->setWindowFlags(flags);

	this->SetInfo();

	QObject::connect(ui->OKPushButton, SIGNAL(clicked()), this, SLOT(close()));
}

Syn_AboutDlg::~Syn_AboutDlg()
{
	if (ui)
	{
		delete ui;
		ui = NULL;
	}
}

void Syn_AboutDlg::SetInfo()
{
	ui->LogoLabel->setPixmap(QPixmap("images/Synaptics.bmp"));

	ui->NameLabel->setText("Huangpu");

	ui->VersionLabel->setText(QString("Version:") + SW_VERSION);
	
	QDate currentDate = QDate::currentDate();
	QString strCurDate = QString::number(currentDate.year()) + "-" + QString::number(currentDate.month()) + "-" + QString::number(currentDate.day());
	ui->DateLabel->setText("Release Date:" + strCurDate);

	ui->CopyrightLabel->setText("Copyright@2016 Synaptics");
}