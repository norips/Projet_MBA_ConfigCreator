#ifndef CANVA_H
#define CANVA_H
#include <QVector>
#include "canvaitem.h"
#include <QObject>

class Canva
{
public:
    Canva(QString& name,QString& url);
    Canva();
    QString info() const;
    QString getURL() const;
    QString getName() const;
private:
    QString name;
    QString url;
};

#endif // CANVA_H
