/********************************************************************************
** Form generated from reading UI file 'Syn_AboutDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYN_ABOUTDLG_H
#define UI_SYN_ABOUTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Syn_AboutDlg
{
public:
    QFormLayout *formLayout;
    QLabel *LogoLabel;
    QFrame *frame;
    QFormLayout *formLayout_2;
    QLabel *NameLabel;
    QLabel *VersionLabel;
    QLabel *DateLabel;
    QLabel *CopyrightLabel;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QPushButton *OKPushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Syn_AboutDlg)
    {
        if (Syn_AboutDlg->objectName().isEmpty())
            Syn_AboutDlg->setObjectName(QStringLiteral("Syn_AboutDlg"));
        Syn_AboutDlg->setWindowModality(Qt::ApplicationModal);
        Syn_AboutDlg->resize(260, 160);
        formLayout = new QFormLayout(Syn_AboutDlg);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        LogoLabel = new QLabel(Syn_AboutDlg);
        LogoLabel->setObjectName(QStringLiteral("LogoLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, LogoLabel);

        frame = new QFrame(Syn_AboutDlg);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        formLayout_2 = new QFormLayout(frame);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        NameLabel = new QLabel(frame);
        NameLabel->setObjectName(QStringLiteral("NameLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, NameLabel);

        VersionLabel = new QLabel(frame);
        VersionLabel->setObjectName(QStringLiteral("VersionLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, VersionLabel);

        DateLabel = new QLabel(frame);
        DateLabel->setObjectName(QStringLiteral("DateLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, DateLabel);

        CopyrightLabel = new QLabel(frame);
        CopyrightLabel->setObjectName(QStringLiteral("CopyrightLabel"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, CopyrightLabel);


        formLayout->setWidget(0, QFormLayout::FieldRole, frame);

        frame_2 = new QFrame(Syn_AboutDlg);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        OKPushButton = new QPushButton(frame_2);
        OKPushButton->setObjectName(QStringLiteral("OKPushButton"));

        gridLayout_2->addWidget(OKPushButton, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);


        formLayout->setWidget(1, QFormLayout::SpanningRole, frame_2);


        retranslateUi(Syn_AboutDlg);

        QMetaObject::connectSlotsByName(Syn_AboutDlg);
    } // setupUi

    void retranslateUi(QWidget *Syn_AboutDlg)
    {
        Syn_AboutDlg->setWindowTitle(QApplication::translate("Syn_AboutDlg", "About Huangpu", 0));
        LogoLabel->setText(QApplication::translate("Syn_AboutDlg", "SYNALogo", 0));
        NameLabel->setText(QApplication::translate("Syn_AboutDlg", "Name", 0));
        VersionLabel->setText(QApplication::translate("Syn_AboutDlg", "Version", 0));
        DateLabel->setText(QApplication::translate("Syn_AboutDlg", "Date", 0));
        CopyrightLabel->setText(QApplication::translate("Syn_AboutDlg", "Copyright", 0));
        OKPushButton->setText(QApplication::translate("Syn_AboutDlg", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class Syn_AboutDlg: public Ui_Syn_AboutDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYN_ABOUTDLG_H
