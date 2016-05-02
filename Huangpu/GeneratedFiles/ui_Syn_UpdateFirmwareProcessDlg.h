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
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Syn_UpdateFirmwareProcessDlg
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout1;
    QLabel *labelInfo;

    void setupUi(QDialog *Syn_UpdateFirmwareProcessDlg)
    {
        if (Syn_UpdateFirmwareProcessDlg->objectName().isEmpty())
            Syn_UpdateFirmwareProcessDlg->setObjectName(QStringLiteral("Syn_UpdateFirmwareProcessDlg"));
        Syn_UpdateFirmwareProcessDlg->setWindowModality(Qt::ApplicationModal);
        Syn_UpdateFirmwareProcessDlg->resize(408, 78);
        verticalLayout = new QVBoxLayout(Syn_UpdateFirmwareProcessDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout1 = new QVBoxLayout();
        verticalLayout1->setSpacing(6);
        verticalLayout1->setObjectName(QStringLiteral("verticalLayout1"));
        labelInfo = new QLabel(Syn_UpdateFirmwareProcessDlg);
        labelInfo->setObjectName(QStringLiteral("labelInfo"));
        QFont font;
        font.setFamily(QStringLiteral("Segoe UI"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        labelInfo->setFont(font);

        verticalLayout1->addWidget(labelInfo);


        verticalLayout->addLayout(verticalLayout1);


        retranslateUi(Syn_UpdateFirmwareProcessDlg);

        QMetaObject::connectSlotsByName(Syn_UpdateFirmwareProcessDlg);
    } // setupUi

    void retranslateUi(QDialog *Syn_UpdateFirmwareProcessDlg)
    {
        Syn_UpdateFirmwareProcessDlg->setWindowTitle(QApplication::translate("Syn_UpdateFirmwareProcessDlg", "Update Firmware", 0));
        labelInfo->setText(QApplication::translate("Syn_UpdateFirmwareProcessDlg", "MPC04 Firmware updating....", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_UpdateFirmwareProcessDlg: public Ui_Syn_UpdateFirmwareProcessDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_UPDATEFIRMWAREPROCESSDLG_H
