/********************************************************************************
** Form generated from reading UI file 'Syn_UpdateFirmwareProcessDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYN_UPDATEFIRMWAREPROCESSDLG_H
#define UI_SYN_UPDATEFIRMWAREPROCESSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Syn_UpdateFirmwareProcessDlg
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *UpdateFirmwareLabel;

    void setupUi(QWidget *Syn_UpdateFirmwareProcessDlg)
    {
        if (Syn_UpdateFirmwareProcessDlg->objectName().isEmpty())
            Syn_UpdateFirmwareProcessDlg->setObjectName(QStringLiteral("Syn_UpdateFirmwareProcessDlg"));
        Syn_UpdateFirmwareProcessDlg->setWindowModality(Qt::ApplicationModal);
        Syn_UpdateFirmwareProcessDlg->resize(525, 78);
        verticalLayout = new QVBoxLayout(Syn_UpdateFirmwareProcessDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        UpdateFirmwareLabel = new QLabel(Syn_UpdateFirmwareProcessDlg);
        UpdateFirmwareLabel->setObjectName(QStringLiteral("UpdateFirmwareLabel"));
        QFont font;
        font.setPointSize(16);
        UpdateFirmwareLabel->setFont(font);

        verticalLayout->addWidget(UpdateFirmwareLabel);


        retranslateUi(Syn_UpdateFirmwareProcessDlg);

        QMetaObject::connectSlotsByName(Syn_UpdateFirmwareProcessDlg);
    } // setupUi

    void retranslateUi(QWidget *Syn_UpdateFirmwareProcessDlg)
    {
        Syn_UpdateFirmwareProcessDlg->setWindowTitle(QApplication::translate("Syn_UpdateFirmwareProcessDlg", "Update Firmware", 0));
        UpdateFirmwareLabel->setText(QApplication::translate("Syn_UpdateFirmwareProcessDlg", "    MPC04 Bootloader will be updated.Please wait...    ", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_UpdateFirmwareProcessDlg: public Ui_Syn_UpdateFirmwareProcessDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_UPDATEFIRMWAREPROCESSDLG_H
