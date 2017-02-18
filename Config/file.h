#ifndef FILE_H
#define FILE_H
#include <QString>

class File
{
public:
    File(QString name,QString path,QString MD5);
    QString getName();

    QString getPath();

    QString getMD5();
private:
    QString name,path,MD5;
};

#endif // FILE_H
