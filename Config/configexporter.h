#ifndef CONFIGEXPORTER_H
#define CONFIGEXPORTER_H
#include <QString>
#include <QByteArray>

class ConfigExporter
{
public:
    ConfigExporter();
    //Take a file in input and return a URL to this file
    virtual QString upload(QByteArray &payload) = 0;
};

#endif // CONFIGEXPORTER_H
