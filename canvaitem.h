#ifndef CANVAITEM_H
#define CANVAITEM_H

#include <QIcon>
#include <QListWidgetItem>
#include <QString>
class canvaItem : public QListWidgetItem
{
public:
    canvaItem(const QIcon &icon,const QString &text,const QString &path);
    QString getText();
    QString getPath();
private:
    QString text;
    QString pathToFile;
};

#endif // CANVAITEM_H
