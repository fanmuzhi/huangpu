/********************************************************************************
** Form generated from reading UI file 'Syn_DebugModeDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYN_DEBUGMODEDLG_H
#define UI_SYN_DEBUGMODEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Syn_DebugModeDlg
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *DebugModePasswordLabel;
    QLineEdit *DebugModeLineEdit;
    QPushButton *DebugModeOKPushButton;
    QPushButton *DebugModeCancelPushButton;

    void setupUi(QWidget *Syn_DebugModeDlg)
    {
        if (Syn_DebugModeDlg->objectName().isEmpty())
            Syn_DebugModeDlg->setObjectName(QStringLiteral("Syn_DebugModeDlg"));
        Syn_DebugModeDlg->setWindowModality(Qt::ApplicationModal);
        Syn_DebugModeDlg->resize(361, 68);
        horizontalLayout = new QHBoxLayout(Syn_DebugModeDlg);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        DebugModePasswordLabel = new QLabel(Syn_DebugModeDlg);
        DebugModePasswordLabel->setObjectName(QStringLiteral("DebugModePasswordLabel"));

        horizontalLayout->addWidget(DebugModePasswordLabel);

        DebugModeLineEdit = new QLineEdit(Syn_DebugModeDlg);
        DebugModeLineEdit->setObjectName(QStringLiteral("DebugModeLineEdit"));
        DebugModeLineEdit->setEchoMode(QLineEdit::Password);

        horizontalLayout->addWidget(DebugModeLineEdit);

        DebugModeOKPushButton = new QPushButton(Syn_DebugModeDlg);
        DebugModeOKPushButton->setObjectName(QStringLiteral("DebugModeOKPushButton"));

        horizontalLayout->addWidget(DebugModeOKPushButton);

        DebugModeCancelPushButton = new QPushButton(Syn_DebugModeDlg);
        DebugModeCancelPushButton->setObjectName(QStringLiteral("DebugModeCancelPushButton"));

        horizontalLayout->addWidget(DebugModeCancelPushButton);


        retranslateUi(Syn_DebugModeDlg);

        QMetaObject::connectSlotsByName(Syn_DebugModeDlg);
    } // setupUi

    void retranslateUi(QWidget *Syn_DebugModeDlg)
    {
        Syn_DebugModeDlg->setWindowTitle(QApplication::translate("Syn_DebugModeDlg", "Engineer Mode", 0));
        DebugModePasswordLabel->setText(QApplication::translate("Syn_DebugModeDlg", "Password:", 0));
        DebugModeOKPushButton->setText(QApplication::translate("Syn_DebugModeDlg", "OK", 0));
        DebugModeCancelPushButton->setText(QApplication::translate("Syn_DebugModeDlg", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_DebugModeDlg: public Ui_Syn_DebugModeDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_DEBUGMODEDLG_H
