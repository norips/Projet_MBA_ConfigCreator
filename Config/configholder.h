#ifndef CONFIGHOLDER_H
#define CONFIGHOLDER_H
#include <QVector>
#include <QFile>
#include "canva.h"
#include "configexporter.h"

class ConfigHolder
{
public:
    static ConfigHolder& Instance();
    void LoadFromJSONFile(QString& filepath);
    void ExportToJSONFile(QString& filepath,ConfigExporter &cex);
    QVector<Canva*> getCanvas() const;
    void addEmpty();
private:
    ConfigHolder();
    QVector<Canva*> canvas;
    static ConfigHolder m_instance;
    bool first;
};

#endif // CONFIGHOLDER_H
