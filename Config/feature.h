#ifndef FEATURE_H
#define FEATURE_H
#include <QString>
#include "file.h"
class Feature
{
public:
    Feature(QString name,File iset, File fset, File fset3 );
    Feature(QString name);
    File getIset();
    File getFset();
    File getFset3();
    void setIset(File f);
    void setFset(File f);
    void setFset3(File f);
    QString getName();
private:
    QString name;
    File iset,fset,fset3;
};

#endif // FEATURE_H
