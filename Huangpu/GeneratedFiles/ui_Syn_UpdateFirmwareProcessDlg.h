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
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Syn_UpdateFirmwareProcessDlg
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QProgressBar *progressBar;

    void setupUi(QDialog *Syn_UpdateFirmwareProcessDlg)
    {
        if (Syn_UpdateFirmwareProcessDlg->objectName().isEmpty())
            Syn_UpdateFirmwareProcessDlg->setObjectName(QStringLiteral("Syn_UpdateFirmwareProcessDlg"));
        Syn_UpdateFirmwareProcessDlg->setWindowModality(Qt::ApplicationModal);
        Syn_UpdateFirmwareProcessDlg->resize(623, 78);
        verticalLayout = new QVBoxLayout(Syn_UpdateFirmwareProcessDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        progressBar = new QProgressBar(Syn_UpdateFirmwareProcessDlg);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        verticalLayout_2->addWidget(progressBar);


        verticalLayout->addLayout(verticalLayout_2);


        retranslateUi(Syn_UpdateFirmwareProcessDlg);

        QMetaObject::connectSlotsByName(Syn_UpdateFirmwareProcessDlg);
    } // setupUi

    void retranslateUi(QDialog *Syn_UpdateFirmwareProcessDlg)
    {
        Syn_UpdateFirmwareProcessDlg->setWindowTitle(QApplication::translate("Syn_UpdateFirmwareProcessDlg", "Update Firmware", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_UpdateFirmwareProcessDlg: public Ui_Syn_UpdateFirmwareProcessDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_UPDATEFIRMWAREPROCESSDLG_H
