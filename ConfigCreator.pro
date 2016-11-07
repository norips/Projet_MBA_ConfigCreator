#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T12:26:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConfigCreator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    formtableau.cpp \
    canvaitem.cpp

HEADERS  += mainwindow.h \
    formtableau.h \
    canvaitem.h

FORMS    += mainwindow.ui \
    formtableau.ui
