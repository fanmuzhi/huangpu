#ifndef SYN_DEBUGMODEDLG_H
#define SYN_DEBUGMODEDLG_H

#include <QWidget>
#include "ui_Syn_DebugModeDlg.h"

class Syn_DebugModeDlg : public QWidget
{
	Q_OBJECT

public:
	Syn_DebugModeDlg(QWidget *parent = 0);
	~Syn_DebugModeDlg();

	void keyPressEvent(QKeyEvent * ev);

public Q_SLOTS:
	
	void OKPushButtonClick();

signals:

	void sendPass();

private:
	Ui::Syn_DebugModeDlg *ui;
};

#endif // SYN_DEBUGMODEDLG_H
