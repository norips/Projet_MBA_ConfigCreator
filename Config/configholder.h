#ifndef CONFIGHOLDER_H
#define CONFIGHOLDER_H
#include <QVector>
#include "canva.h"

class ConfigHolder
{
public:
    ConfigHolder();
    void LoadFromJSONFile(QString& filepath);
    QVector<canvaItem*> getCanvas() const;
private:
    QVector<canvaItem*> canvas;
};

#endif // CONFIGHOLDER_H
