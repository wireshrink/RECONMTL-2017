#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QWidget>
#include "ui_setup.h"

class SetupDialog : public QDialog, private Ui::setupDialog
{
    Q_OBJECT
public:

    SetupDialog();
	void finalize();
public slots:
    void applyCouponSlot();
    void testConnSlot();
    void chooseFolderSlot();

};

#endif // SETUPDIALOG_H
