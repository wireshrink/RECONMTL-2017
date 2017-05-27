#-------------------------------------------------
#
# Project created by QtCreator 2015-06-12T19:03:55
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = videogui
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           epgdialog.cpp \
           setupdialog.cpp \
    sgxware.cpp \
    encryptedfile.cpp

HEADERS  += mainwindow.h \
            epgdialog.h \
            setupdialog.h \
    sgxware.h \
    encryptedfile.h

FORMS    += mainwindow.ui \
    EPG.ui \
    setup.ui

RESOURCES += \
    resources.qrc
