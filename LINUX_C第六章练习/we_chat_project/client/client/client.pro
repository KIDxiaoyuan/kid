#-------------------------------------------------
#
# Project created by QtCreator 2016-08-15T21:12:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp
LIBS +=-lpthread
HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
