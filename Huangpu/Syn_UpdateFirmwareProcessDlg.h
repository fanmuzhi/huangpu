#ifndef SYN_UPDATEFIRMWAREPROCESSDLG_H
#define SYN_UPDATEFIRMWAREPROCESSDLG_H

#include <QWidget>
#include "ui_Syn_UpdateFirmwareProcessDlg.h"

class Syn_UpdateFirmwareProcessDlg : public QWidget
{
	Q_OBJECT

public:
	Syn_UpdateFirmwareProcessDlg(QWidget *parent = 0);
	~Syn_UpdateFirmwareProcessDlg();

private:
	Ui::Syn_UpdateFirmwareProcessDlg ui;
};

#endif // SYN_UPDATEFIRMWAREPROCESSDLG_H
