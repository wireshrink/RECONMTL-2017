/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel® SGX ENCLAVES.                     *
*	This application made vulnerable DELIBERATELY - its main purpose is to demonstrate, shame and blame     *
*   common mistakes usually made with SGX enclave programming.                                              *
*   ONCE AGAIN, IT CONTAINS MISTAKES.                                                                       *
*   Please DO NOT USE it's source for any healthy development/production activity.                          *
*	It is intended to use for training of hacking SGX enclaves ONLY.                                        *
*	It is written ugly(deliberately), designed badly(intentionally) and running very slow.		        	*
*	You can freely use this application for training purposes.												*
*	I'd be glad to hear about your progress.    															*
*																											*
*	This application requires QT5.8 (which uses LGPL v3 license), Intel SGX SDK and							*
*   the Intel® Software Guard Extensions SSL (Intel® SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/


#include "epgdialog.h"
#include "sgxware.h"
EPGDialog::EPGDialog(QWidget *parent) : QDialog(parent)
{

    SGXware *pInstance = SGXware::getInstance();
    movie_t movie;

    setupUi(this);


    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("id")));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("name")));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(QString("status")));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(QString("state")));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(QString("expiry date")));


    if (pInstance->firstMovie(&movie))
    {
        
        do
        {
            char buf[1024];
			int row = tableWidget->rowCount() ;
            tableWidget->insertRow(tableWidget->rowCount());
            QTableWidgetItem *pItem = new QTableWidgetItem(QString(movie.movie_name));
            tableWidget->setItem(row, 1, pItem);
            sprintf(buf, "%d", movie.movie_id);
            pItem = new QTableWidgetItem(QString(buf));
            tableWidget->setItem(row, 0, pItem);
            pItem = new QTableWidgetItem(QString(strMovieStatus(movie.movie_status)));
            tableWidget->setItem(row, 2, pItem);
            pItem = new QTableWidgetItem(QString(strMovieState(movie.movie_state)));
            tableWidget->setItem(row, 3, pItem);
            pItem = new QTableWidgetItem(QString(strExpiry(movie.movie_expiration)));
            tableWidget->setItem(row, 4, pItem);
			

        }while (pInstance->nextMovie(&movie));

    }
    selected_movie = (unsigned int)-1;
}

void EPGDialog::itemDoubleClicked(int row, int column)
{
   // download if not yet and start playing
    SGXware *pSGX = SGXware::getInstance();
    QString idstr = tableWidget->item(row, 0)->text();
    unsigned int id = idstr.toInt();
	if (pSGX->prepareMovie((unsigned int)id))
	{
		selected_movie = id;
		this->accept();
	}
}
