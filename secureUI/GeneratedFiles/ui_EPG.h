/********************************************************************************
** Form generated from reading UI file 'EPG.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EPG_H
#define UI_EPG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_EPGDialog
{
public:
    QDialogButtonBox *buttonBox;
    QTableWidget *tableWidget;

    void setupUi(QDialog *EPGDialog)
    {
        if (EPGDialog->objectName().isEmpty())
            EPGDialog->setObjectName(QStringLiteral("EPGDialog"));
        EPGDialog->resize(1299, 425);
        buttonBox = new QDialogButtonBox(EPGDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(1030, 390, 261, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Close|QDialogButtonBox::Ok);
        tableWidget = new QTableWidget(EPGDialog);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 10, 1281, 371));
        tableWidget->setSortingEnabled(true);
        tableWidget->setColumnCount(5);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setCascadingSectionResizes(true);
        tableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableWidget->verticalHeader()->setStretchLastSection(true);

        retranslateUi(EPGDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), EPGDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EPGDialog, SLOT(reject()));
        QObject::connect(tableWidget, SIGNAL(cellDoubleClicked(int,int)), EPGDialog, SLOT(itemDoubleClicked(int,int)));

        QMetaObject::connectSlotsByName(EPGDialog);
    } // setupUi

    void retranslateUi(QDialog *EPGDialog)
    {
        EPGDialog->setWindowTitle(QApplication::translate("EPGDialog", "Electronic program guide (EPG). Doubleclick the movie to view.", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class EPGDialog: public Ui_EPGDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EPG_H
