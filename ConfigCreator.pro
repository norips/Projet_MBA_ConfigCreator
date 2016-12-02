#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T12:26:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConfigCreator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    formtableau.cpp \
    canvaitem.cpp \
    Config/configholder.cpp \
    Config/canva.cpp \
    filedownloader.cpp \
    Config/modelitem.cpp \
    Config/texture.cpp

HEADERS  += mainwindow.h \
    formtableau.h \
    canvaitem.h \
    Config/configholder.h \
    Config/canva.h \
    filedownloader.h \
    Config/modelitem.h \
    Config/texture.h

FORMS    += mainwindow.ui \
    formtableau.ui
