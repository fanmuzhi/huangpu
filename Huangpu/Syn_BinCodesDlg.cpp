#include "Syn_BinCodesDlg.h"

Syn_BinCodesDlg::Syn_BinCodesDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.BinCodesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Syn_BinCodesDlg::~Syn_BinCodesDlg()
{

}
