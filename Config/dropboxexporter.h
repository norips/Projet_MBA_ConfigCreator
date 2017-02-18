#ifndef DROPBOXEXPORTER_H
#define DROPBOXEXPORTER_H

#include "configexporter.h"
#include <QString>
#include <QByteArray>
class DropboxExporter : public ConfigExporter
{
public:
    DropboxExporter();
    QString upload(const QByteArray &payload);
};

#endif // DROPBOXEXPORTER_H
