#ifndef SYN_UPDATEADCOFFSETSDLG_H
#define SYN_UPDATEADCOFFSETSDLG_H

#include <QWidget>
#include "ui_Syn_UpdateADCOffsetsDlg.h"

class Syn_UpdateADCOffsetsDlg : public QWidget
{
	Q_OBJECT

	friend class FPS_TestExecutive;

public:
	Syn_UpdateADCOffsetsDlg(QWidget *parent = 0);
	~Syn_UpdateADCOffsetsDlg();

private:
	Ui::Syn_UpdateADCOffsetsDlg ui;
};

#endif // SYN_UPDATEADCOFFSETSDLG_H
