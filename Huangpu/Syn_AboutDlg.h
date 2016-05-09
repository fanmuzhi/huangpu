#ifndef SYN_ABOUTDLG_H
#define SYN_ABOUTDLG_H

#include <QWidget>
#include "ui_Syn_AboutDlg.h"

class Syn_AboutDlg : public QWidget
{
	Q_OBJECT

public:
	Syn_AboutDlg(QWidget *parent = 0);
	~Syn_AboutDlg();

private:

	void SetInfo();

	Ui::Syn_AboutDlg *ui;
};

#endif // SYN_ABOUTDLG_H
