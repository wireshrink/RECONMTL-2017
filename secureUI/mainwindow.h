#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QProgressBar>
#include <QSlider>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionPlay_triggered();

    void on_actionPause_triggered();

    void on_actionStop_triggered();

    void on_actionManageEPG_triggered();

    void on_actionClean_the_library_triggered();

    void on_actionSetup_triggered();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QVideoWidget* vw;
    QProgressBar* bar;
    QSlider* slider;


};

#endif // MAINWINDOW_H
