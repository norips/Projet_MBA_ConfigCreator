#ifndef THREADEXPORT_H
#define THREADEXPORT_H

#include <QThread>
#include "Config/configexporter.h"
#include "Config/configholder.h"
class ThreadExport : public QThread
{
    Q_OBJECT

public:
    ThreadExport(QString filename, ConfigHolder* holder);
protected:
    void run();
private:
    ConfigHolder *holder;
    ConfigExporter *exporter;
    QString filename;
};

#endif // THREADEXPORT_H
