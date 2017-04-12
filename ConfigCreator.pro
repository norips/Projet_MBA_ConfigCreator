#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T12:26:44
#
#-------------------------------------------------

QT += core gui network
QT += multimedia
QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
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
    Config/texture.cpp \
    dialogmodel.cpp \
    Config/model.cpp \
    Config/textureimg.cpp \
    Config/texturetxt.cpp \
    Config/configexporter.cpp \
    Config/dropboxexporter.cpp \
    Config/feature.cpp \
    Config/file.cpp \
    threadexport.cpp \
    Config/modified.cpp \
    Config/widgetSelection.cpp \
    preview.cpp \
    Config/texturemov.cpp

HEADERS  += mainwindow.h \
    formtableau.h \
    canvaitem.h \
    Config/configholder.h \
    Config/canva.h \
    filedownloader.h \
    Config/modelitem.h \
    Config/texture.h \
    dialogmodel.h \
    Config/model.h \
    Config/textureimg.h \
    Config/texturetxt.h \
    Config/configexporter.h \
    Config/dropboxexporter.h \
    Config/feature.h \
    Config/file.h \
    threadexport.h \
    Config/modified.h \
    Config/widgetSelection.h \
    textedit.h \
    preview.h \
    Config/texturemov.h

FORMS    += mainwindow.ui \
    formtableau.ui \
    dialogmodel.ui \
    preview.ui
