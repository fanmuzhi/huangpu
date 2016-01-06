/********************************************************************************
** Form generated from reading UI file 'fps_testexecutive.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FPS_TESTEXECUTIVE_H
#define UI_FPS_TESTEXECUTIVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FPS_TestExecutiveClass
{
public:
    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QGroupBox *ImageGroupBox;
    QGridLayout *gridLayout_2;
    QLabel *ImageLabel;
    QTextBrowser *textBrowser;
    QPushButton *pushButtonRun;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLineEdit *RawdataLineEdit;
    QPushButton *SelectPushButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FPS_TestExecutiveClass)
    {
        if (FPS_TestExecutiveClass->objectName().isEmpty())
            FPS_TestExecutiveClass->setObjectName(QStringLiteral("FPS_TestExecutiveClass"));
        FPS_TestExecutiveClass->resize(475, 455);
        openAct = new QAction(FPS_TestExecutiveClass);
        openAct->setObjectName(QStringLiteral("openAct"));
        saveAsAct = new QAction(FPS_TestExecutiveClass);
        saveAsAct->setObjectName(QStringLiteral("saveAsAct"));
        exitAct = new QAction(FPS_TestExecutiveClass);
        exitAct->setObjectName(QStringLiteral("exitAct"));
        centralWidget = new QWidget(FPS_TestExecutiveClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        ImageGroupBox = new QGroupBox(centralWidget);
        ImageGroupBox->setObjectName(QStringLiteral("ImageGroupBox"));
        gridLayout_2 = new QGridLayout(ImageGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        ImageLabel = new QLabel(ImageGroupBox);
        ImageLabel->setObjectName(QStringLiteral("ImageLabel"));

        gridLayout_2->addWidget(ImageLabel, 0, 0, 1, 1);


        gridLayout_3->addWidget(ImageGroupBox, 4, 0, 1, 1);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout_3->addWidget(textBrowser, 2, 0, 1, 1);

        pushButtonRun = new QPushButton(centralWidget);
        pushButtonRun->setObjectName(QStringLiteral("pushButtonRun"));

        gridLayout_3->addWidget(pushButtonRun, 3, 0, 1, 1);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        RawdataLineEdit = new QLineEdit(frame);
        RawdataLineEdit->setObjectName(QStringLiteral("RawdataLineEdit"));
        RawdataLineEdit->setReadOnly(true);

        gridLayout->addWidget(RawdataLineEdit, 0, 0, 1, 1);

        SelectPushButton = new QPushButton(frame);
        SelectPushButton->setObjectName(QStringLiteral("SelectPushButton"));

        gridLayout->addWidget(SelectPushButton, 0, 1, 1, 1);


        gridLayout_3->addWidget(frame, 1, 0, 1, 1);

        FPS_TestExecutiveClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FPS_TestExecutiveClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FPS_TestExecutiveClass->setStatusBar(statusBar);

        retranslateUi(FPS_TestExecutiveClass);

        QMetaObject::connectSlotsByName(FPS_TestExecutiveClass);
    } // setupUi

    void retranslateUi(QMainWindow *FPS_TestExecutiveClass)
    {
        FPS_TestExecutiveClass->setWindowTitle(QApplication::translate("FPS_TestExecutiveClass", "FPS_TestExecutive", 0));
        openAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Open...", 0));
        saveAsAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Save As...", 0));
        exitAct->setText(QApplication::translate("FPS_TestExecutiveClass", "Exit", 0));
        ImageGroupBox->setTitle(QApplication::translate("FPS_TestExecutiveClass", "Image", 0));
        ImageLabel->setText(QApplication::translate("FPS_TestExecutiveClass", "TextLabel", 0));
        pushButtonRun->setText(QApplication::translate("FPS_TestExecutiveClass", "Run", 0));
        SelectPushButton->setText(QApplication::translate("FPS_TestExecutiveClass", "Select", 0));
    } // retranslateUi

};

namespace Ui {
    class FPS_TestExecutiveClass: public Ui_FPS_TestExecutiveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPS_TESTEXECUTIVE_H
