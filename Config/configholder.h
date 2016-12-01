#ifndef CONFIGHOLDER_H
#define CONFIGHOLDER_H
#include <QVector>
#include "canva.h"

class ConfigHolder
{
public:
    ConfigHolder();
    void LoadFromJSONFile(QString& filepath);
    QVector<Canva> getCanvas() const;
private:
    QVector<Canva> canvas;
};

#endif // CONFIGHOLDER_H
