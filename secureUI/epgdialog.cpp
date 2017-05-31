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
        int column = 0;
        do
        {
            char buf[1024];
            tableWidget->insertRow(tableWidget->rowCount()+1);
            QTableWidgetItem *pItem = new QTableWidgetItem(QString(movie.movie_name));
            tableWidget->setItem(column, 1, pItem);
            sprintf(buf, "%d", movie.movie_id);
            pItem = new QTableWidgetItem(QString(buf));
            tableWidget->setItem(column, 0, pItem);
            pItem = new QTableWidgetItem(QString(strMovieStatus(movie.movie_status)));
            tableWidget->setItem(column, 2, pItem);
            pItem = new QTableWidgetItem(QString(strMovieState(movie.movie_state)));
            tableWidget->setItem(column, 3, pItem);
            pItem = new QTableWidgetItem(QString(strExpiry(movie.movie_expiration)));
            tableWidget->setItem(column, 4, pItem);

        }while (pInstance->nextMovie(&movie));

    }
    selected_movie = (unsigned int)-1;
}

void EPGDialog::itemDoubleClicked(int row, int column)
{
   // download if not yet and start playing
    SGXware *pSGX = SGXware::getInstance();
    QString idstr = tableWidget->item(0, column)->text();
    unsigned int id = idstr.toInt();
    pSGX->prepareMovie((unsigned int)id);
    selected_movie = id;
    this->accept();
}
