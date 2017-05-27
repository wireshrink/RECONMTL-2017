#ifndef EPGDIALOG_H
#define EPGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "ui_EPG.h"

class EPGDialog : public QDialog, private Ui::EPGDialog
{
    Q_OBJECT
public:
    explicit EPGDialog(QWidget *parent = 0);
    unsigned int selected_movie;
public slots:
    void itemDoubleClicked(int, int);

signals:

public slots:
};

#endif // EPGDIALOG_H
