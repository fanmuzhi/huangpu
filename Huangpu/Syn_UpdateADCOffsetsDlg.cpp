#include "Syn_UpdateADCOffsetsDlg.h"

Syn_UpdateADCOffsetsDlg::Syn_UpdateADCOffsetsDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowTitleHint);

	ui.VddLineEdit->setValidator(new QIntValidator(0, 3300, this));
	ui.VioLineEdit->setValidator(new QIntValidator(0, 3300, this));
	ui.VledLineEdit->setValidator(new QIntValidator(0, 3300, this));
	ui.VddhLineEdit->setValidator(new QIntValidator(0, 3300, this));
}

Syn_UpdateADCOffsetsDlg::~Syn_UpdateADCOffsetsDlg()
{

}
