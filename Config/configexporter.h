#ifndef CONFIGEXPORTER_H
#define CONFIGEXPORTER_H
#include <QString>
#include <QByteArray>

//Interface for exporter
class ConfigExporter
{
public:
    ConfigExporter();
    //Take a file in input and return a URL to this file
    virtual QString upload(const QByteArray &payload) = 0;
};

#endif // CONFIGEXPORTER_H
