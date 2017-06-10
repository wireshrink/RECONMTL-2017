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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupdialog.h"
#include "epgdialog.h"
#include "sgxware.h"
#include "encryptedfile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    vw = new QVideoWidget(this);
    player->setVideoOutput(vw);
    this->setCentralWidget(vw);

    slider = new QSlider(this);
    bar = new QProgressBar(this);

    slider->setOrientation(Qt::Horizontal);

    ui->statusBar->addPermanentWidget(slider);
    ui->statusBar->addPermanentWidget(bar);

    connect(player,&QMediaPlayer::durationChanged,slider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,slider,&QSlider::setValue);
    connect(slider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);

    connect(player,&QMediaPlayer::durationChanged,bar,&QProgressBar::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,bar,&QProgressBar::setValue);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
	on_actionManageEPG_triggered();

}

void MainWindow::on_actionPlay_triggered()
{
    player->play();
    ui->statusBar->showMessage("Playing");
}

void MainWindow::on_actionPause_triggered()
{
    player->pause();
    ui->statusBar->showMessage("Paused...");
}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
    ui->statusBar->showMessage("Stopped");
}

void MainWindow::on_actionManageEPG_triggered()
{
    EPGDialog *pdlg = new EPGDialog;
	char movie_file_name[1024];
    unsigned int movie_to_play = (unsigned int) -1;
	static EncryptedFile * s_encFile = nullptr;

    pdlg->exec();
    movie_to_play = pdlg->selected_movie;
    delete pdlg;
    SGXware *pSGX = SGXware::getInstance();
    if (movie_to_play != (unsigned int)(-1) && pSGX->getFileName(movie_to_play, 1024, movie_file_name) )
    {
        on_actionStop_triggered();
		if (s_encFile) delete s_encFile;
		s_encFile = new EncryptedFile(movie_to_play);
		s_encFile->open(QFile::ReadOnly);
        player->setMedia(QUrl::fromLocalFile(movie_file_name), s_encFile); 

		on_actionPlay_triggered();

    }
}

void MainWindow::on_actionClean_the_library_triggered()
{

}

void MainWindow::on_actionSetup_triggered()
{
    SetupDialog *pdlg = new SetupDialog;
    pdlg->exec();
	pdlg->finalize();
    delete pdlg;

}
