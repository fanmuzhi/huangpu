#include "Syn_AboutDlg.h"

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
	
	ui->DateLabel->setText("Release Date:2016-08-04");

	ui->CopyrightLabel->setText("Copyright@2016 Synaptics");
}