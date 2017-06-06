#include "setupdialog.h"
#include "sgxware.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>


SetupDialog::SetupDialog(void):QDialog()
{
    char buf[1024];
    char folder[1024];
    setupUi(this);
	// reading the folder from teh qsettings
	QSettings settings("DVSE", "videogui");
	QString shost, sport, sfolder;

	shost = settings.value("host").toString();
	sport = settings.value("port").toString();
	sfolder = settings.value("folder").toString();

	strcpy(folder, sfolder.toLatin1().data());
	this->serverIPEdit->setText(shost);
	this->serverPortEdit->setText(sport);
	this->folderEdit->setText(sfolder);
	sprintf(buf, "Balance: %d", 0);
	this->labelBalance->setText(buf);

}

void SetupDialog::applyCouponSlot(void)
{
    SGXware *pSGX = SGXware::getInstance();
    QString str = this->couponText->toPlainText();
    QByteArray ba = str.toLatin1();
    char buf[1024];
    pSGX->tryCoupon(ba.data());
    sprintf (buf, "Balance: %d", pSGX->getBalance() );
    this->labelBalance->setText(buf);

}
void SetupDialog::finalize()
{
	QSettings settings("DVSE", "videogui");
	QString shost, sport, sfolder;

	sport = serverPortEdit->text();
	shost = serverIPEdit->text();
	sfolder = folderEdit->text();

	if (sport.length() > 0 && shost.length() > 0 && sfolder.length() > 0)
	{
		settings.setValue("folder", sfolder);
		settings.setValue("port", sport);
		settings.setValue("host", shost);
		settings.setValue("valid", true);

	}
	else
	{
		settings.setValue("folder", sfolder);
		settings.setValue("port", sport);
		settings.setValue("host", shost);
		settings.setValue("valid", false);
	}
	SGXware::getInstance()->initUser(this->serverIPEdit->text().toLatin1().data(), serverPortEdit->text().toInt(), this->folderEdit->text().toLatin1().data()); // insert getting properties here
}
void SetupDialog::testConnSlot(void)
{
    QTcpSocket t;
    QMessageBox b;
    b.setText(QString("Connection status"));
    b.setStandardButtons(QMessageBox::Ok);
    t.connectToHost(this->serverIPEdit->text(), serverPortEdit->text().toInt());
    if (t.isOpen())
    {
        b.setInformativeText(QString("Server data is OK, connected"));
    }
    else
    {
        b.setInformativeText(QString("Can not connect to host ..."));
    }
    t.disconnectFromHost();
}
void SetupDialog::chooseFolderSlot(void)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose library folder"));
    this->folderEdit->setText(dir);
    SGXware::getInstance()->initUser(this->serverIPEdit->text().toLatin1().data(), serverPortEdit->text().toInt(), dir.toLatin1().data()); // insert getting properties here
}
