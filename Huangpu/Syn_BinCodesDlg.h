#ifndef SYN_BINCODESDLG_H
#define SYN_BINCODESDLG_H

#include <QWidget>
#include <QtWidgets/QDialog>
#include "ui_Syn_BinCodesDlg.h"

class Syn_BinCodesDlg : public QDialog 
{
	Q_OBJECT

public:
	Syn_BinCodesDlg(QWidget *parent = 0);
	~Syn_BinCodesDlg();

	//void closeEvent(QCloseEvent * event);

private:
	Ui::Syn_BinCodesDlg *ui;
};

#endif // SYN_BINCODESDLG_H