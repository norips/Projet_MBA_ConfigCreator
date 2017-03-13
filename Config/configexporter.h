#ifndef CONFIGEXPORTER_H
#define CONFIGEXPORTER_H
#include <QString>
#include <QByteArray>

//Interface for exporter
class ConfigExporter
{
public:
    ConfigExporter();
    virtual ~ConfigExporter(){}
    //Take a file in input and return a URL to this file
    virtual QString upload(const QString fileName, const QByteArray &payload) = 0;
};

#endif // CONFIGEXPORTER_H
