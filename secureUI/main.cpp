#include "mainwindow.h"
#include <QApplication>
#include "sgxware.h"
#include <QSettings>
#include "setupdialog.h"
#include <QString>

#pragma comment(lib,"Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	// initializing sockets
	// Declare and initialize variables
	WSADATA wsaData = { 0 };
	int iResult = 0;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}
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
