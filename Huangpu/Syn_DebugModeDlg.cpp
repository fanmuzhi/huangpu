#include "Syn_DebugModeDlg.h"
//Qt
#include <QtWidgets>

Syn_DebugModeDlg::Syn_DebugModeDlg(QWidget *parent)
	: QWidget(parent)
{

	ui = new Ui::Syn_DebugModeDlg();
	ui->setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	this->setWindowFlags(flags);

	QObject::connect(ui->DebugModeOKPushButton, SIGNAL(clicked()), this, SLOT(OKPushButtonClick()));

	QObject::connect(ui->DebugModeCancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
}

Syn_DebugModeDlg::~Syn_DebugModeDlg()
{
	if (NULL != ui)
	{
		delete ui;
		ui = NULL;
	}
}

void Syn_DebugModeDlg::OKPushButtonClick()
{
	QString qsPassword = ui->DebugModeLineEdit->text();
	if (QString("SYNABPDTest") == qsPassword)
	{
		emit sendPass();
		this->close();
	}
	else
	{
		QMessageBox::warning(this, QString("Warning"), QString("Password is wrong!"));
	}
}

void Syn_DebugModeDlg::keyPressEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return)
	{
		OKPushButtonClick();
	}
}