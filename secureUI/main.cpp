#include "mainwindow.h"
#include <QApplication>
#include "sgxware.h"
#include <QSettings>
#include "setupdialog.h"
#include <QString>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QSettings settings("DVSE", "videogui");
	while (!settings.value("valid").toBool())
	{
		SetupDialog dlg;
		dlg.exec();
		dlg.finalize();
	}


    MainWindow w;
    w.show();

    int res =  a.exec();

	SGXware::destroyInstance();
	return res;
}
