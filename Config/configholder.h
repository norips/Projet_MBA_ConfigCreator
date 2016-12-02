#ifndef CONFIGHOLDER_H
#define CONFIGHOLDER_H
#include <QVector>
#include "canva.h"

class ConfigHolder
{
public:
    static ConfigHolder& Instance();
    void LoadFromJSONFile(QString& filepath);
    QVector<Canva*> getCanvas() const;
private:
    ConfigHolder();
    QVector<Canva*> canvas;
    static ConfigHolder m_instance;
};

#endif // CONFIGHOLDER_H
