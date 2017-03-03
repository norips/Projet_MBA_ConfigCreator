#ifndef DROPBOXEXPORTER_H
#define DROPBOXEXPORTER_H

#include "configexporter.h"
#include <QString>
#include <QByteArray>
#include <QNetworkReply>
#include <QEventLoop>
class DropboxExporter : public QObject,  public ConfigExporter
{
    Q_OBJECT
public:
    DropboxExporter();
    ~DropboxExporter();
    QString upload(const QString fileName, const QByteArray &payload);


private slots:
    void replyFinished(QNetworkReply*);

private:
    QString path;
    QEventLoop pause;
    QNetworkAccessManager *manager;
    bool first = true;
    bool secondFirst = true;
    QString apiKey = "xxYPEC7HMMAAAAAAAAAAG8o1JTnQRgXNT7-Kfq1dXaOYTVo4LVpGqe6Vf3AfFXCu";
    QString fileId;

signals:
    void urlRetrieved();



};

#endif // DROPBOXEXPORTER_H
