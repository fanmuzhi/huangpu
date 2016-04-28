#ifndef SYN_BINCODESDLG_H
#define SYN_BINCODESDLG_H

#include <QWidget>
#include "ui_Syn_BinCodesDlg.h"

class Syn_BinCodesDlg : public QWidget
{
	Q_OBJECT
	
	friend class FPS_TestExecutive;

public:
	Syn_BinCodesDlg(QWidget *parent = 0);
	~Syn_BinCodesDlg();

	void closeEvent(QCloseEvent * event);

private:
	Ui::Syn_BinCodesDlg *ui;
};

#endif // SYN_BINCODESDLG_H
