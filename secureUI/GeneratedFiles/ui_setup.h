/********************************************************************************
** Form generated from reading UI file 'setup.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_setupDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *serverBox;
    QLineEdit *serverIPEdit;
    QLineEdit *serverPortEdit;
    QLabel *ipLabel;
    QLabel *portLabel;
    QPushButton *testConn;
    QGroupBox *couponBox;
    QTextEdit *couponText;
    QPushButton *applyBtn;
    QLabel *labelBalance;
    QGroupBox *folderBox;
    QLineEdit *folderEdit;
    QPushButton *folderChooseButton;

    void setupUi(QDialog *setupDialog)
    {
        if (setupDialog->objectName().isEmpty())
            setupDialog->setObjectName(QStringLiteral("setupDialog"));
        setupDialog->resize(902, 254);
        buttonBox = new QDialogButtonBox(setupDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(540, 210, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        serverBox = new QGroupBox(setupDialog);
        serverBox->setObjectName(QStringLiteral("serverBox"));
        serverBox->setGeometry(QRect(10, 20, 331, 121));
        serverBox->setTitle(QStringLiteral("Server connection"));
        serverIPEdit = new QLineEdit(serverBox);
        serverIPEdit->setObjectName(QStringLiteral("serverIPEdit"));
        serverIPEdit->setGeometry(QRect(110, 20, 191, 22));
        serverPortEdit = new QLineEdit(serverBox);
        serverPortEdit->setObjectName(QStringLiteral("serverPortEdit"));
        serverPortEdit->setGeometry(QRect(110, 50, 191, 22));
        ipLabel = new QLabel(serverBox);
        ipLabel->setObjectName(QStringLiteral("ipLabel"));
        ipLabel->setGeometry(QRect(10, 20, 71, 20));
        ipLabel->setText(QLatin1String("<html><head/><body><p>Server IP</p></body></html>Most interesting things will be hidden in the enclave\n"
"Which makes the enclave the first priority target\n"
""));
        portLabel = new QLabel(serverBox);
        portLabel->setObjectName(QStringLiteral("portLabel"));
        portLabel->setGeometry(QRect(10, 50, 91, 16));
        portLabel->setText(QStringLiteral("Server port"));
        testConn = new QPushButton(serverBox);
        testConn->setObjectName(QStringLiteral("testConn"));
        testConn->setGeometry(QRect(100, 80, 121, 28));
        couponBox = new QGroupBox(setupDialog);
        couponBox->setObjectName(QStringLiteral("couponBox"));
        couponBox->setGeometry(QRect(370, 20, 511, 121));
        couponBox->setTitle(QStringLiteral("Enter coupon"));
        couponText = new QTextEdit(couponBox);
        couponText->setObjectName(QStringLiteral("couponText"));
        couponText->setGeometry(QRect(10, 20, 431, 91));
        applyBtn = new QPushButton(couponBox);
        applyBtn->setObjectName(QStringLiteral("applyBtn"));
        applyBtn->setGeometry(QRect(450, 20, 51, 91));
        labelBalance = new QLabel(setupDialog);
        labelBalance->setObjectName(QStringLiteral("labelBalance"));
        labelBalance->setGeometry(QRect(20, 160, 251, 41));
        folderBox = new QGroupBox(setupDialog);
        folderBox->setObjectName(QStringLiteral("folderBox"));
        folderBox->setGeometry(QRect(370, 140, 511, 61));
        folderEdit = new QLineEdit(folderBox);
        folderEdit->setObjectName(QStringLiteral("folderEdit"));
        folderEdit->setGeometry(QRect(10, 20, 311, 22));
        folderChooseButton = new QPushButton(folderBox);
        folderChooseButton->setObjectName(QStringLiteral("folderChooseButton"));
        folderChooseButton->setGeometry(QRect(330, 20, 171, 28));

        retranslateUi(setupDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), setupDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), setupDialog, SLOT(reject()));
        QObject::connect(testConn, SIGNAL(clicked()), setupDialog, SLOT(testConnSlot()));
        QObject::connect(folderChooseButton, SIGNAL(clicked()), setupDialog, SLOT(chooseFolderSlot()));
        QObject::connect(applyBtn, SIGNAL(clicked()), setupDialog, SLOT(applyCouponSlot()));

        QMetaObject::connectSlotsByName(setupDialog);
    } // setupUi

    void retranslateUi(QDialog *setupDialog)
    {
        setupDialog->setWindowTitle(QApplication::translate("setupDialog", "Dialog", Q_NULLPTR));
        testConn->setText(QApplication::translate("setupDialog", "Test connection", Q_NULLPTR));
        applyBtn->setText(QApplication::translate("setupDialog", "Apply", Q_NULLPTR));
        labelBalance->setText(QApplication::translate("setupDialog", "Balance is ...", Q_NULLPTR));
        folderBox->setTitle(QApplication::translate("setupDialog", "Library location", Q_NULLPTR));
        folderChooseButton->setText(QApplication::translate("setupDialog", "Choose folder and init user", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class setupDialog: public Ui_setupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP_H
