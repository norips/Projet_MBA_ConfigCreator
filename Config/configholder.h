#ifndef CONFIGHOLDER_H
#define CONFIGHOLDER_H
#include <QVector>
#include <QFile>
#include "canva.h"
#include "configexporter.h"

class ConfigHolder : public QObject
{
        Q_OBJECT
public:
    static ConfigHolder* Instance( QObject* parent=0 );
    void LoadFromJSONFile(QString& filepath);
    void ExportToJSONFile(QString& filepath,ConfigExporter* cex);
    QVector<Canva*> getCanvas() const;
    void delCanva(int i) ;
    void addEmpty();
private:
    ConfigHolder( QObject* parent=0);
    QVector<Canva*> canvas;
    static ConfigHolder* m_instance;
    bool first;
signals:
    void configExported(int code);
    void configImported(int code);
};

#endif // CONFIGHOLDER_H
