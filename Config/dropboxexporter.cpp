#include "dropboxexporter.h"
#include "configexporter.h"

#include <QFileDialog>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QTextStream>

DropboxExporter::DropboxExporter() : QObject()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    /* connect the QHttp.requestFinished() Signal to the QEventLoop.quit() Slot */
    connect(this, SIGNAL(urlRetrieved()), &pause, SLOT(quit()));
    QFile file("../bin/dropboxapi.key");
    if(!file.open(QIODevice::ReadOnly)) {
        QTextStream(stdout) << "Error while opening dropbox api key (in bin/dropboxapi.key)" << endl;
    } else {
        QTextStream in(&file);
        apiKey = in.readLine();
        file.close();
    }

}
//TODO




DropboxExporter::~DropboxExporter()
{
    delete(manager);

}
QString DropboxExporter::upload(const QString fileName, const QByteArray &payload) {

    QNetworkRequest nr(QUrl("https://content.dropboxapi.com/2/files/upload"));
    nr.setRawHeader("Authorization",(tr("Bearer ") + apiKey).toUtf8());
    QString dropboxarg;
    dropboxarg.append("{\"path\": \"/");
    dropboxarg.append(fileName);
    dropboxarg.append("\",\"mode\": \"overwrite\",\"mute\": false}");
    nr.setRawHeader("Dropbox-API-Arg",dropboxarg.toUtf8());
    nr.setRawHeader("Content-Type","application/octet-stream");
    manager->post(nr, payload);

    /* Execute the QEventLoop - it will quit when the above finished due to the connect() */
    pause.exec();
    first = true;

    return path;
}



void DropboxExporter::replyFinished(QNetworkReply *nr) {
    if(first){
        QByteArray output = nr->readAll();
        //qInfo(output);
        QJsonDocument doc = QJsonDocument::fromJson(output);
        QString formattedJsonString = doc.toJson(QJsonDocument::Indented);

        QNetworkRequest nr(QUrl("https://api.dropboxapi.com/2/sharing/create_shared_link_with_settings"));
        nr.setRawHeader("Authorization",(tr("Bearer ") + apiKey).toUtf8());
        QString dropboxarg;
        dropboxarg.append("{\"path\": \"");
        QJsonObject jsonObject = doc.object();
        fileId = jsonObject["id"].toString();
        QString pathDrop = jsonObject["path_display"].toString();
        dropboxarg.append(pathDrop);
        dropboxarg.append("\",\"settings\": {\"requested_visibility\": \"public\"}}");
        nr.setRawHeader("Content-Type","application/json");
        manager->post(nr, dropboxarg.toUtf8());
        first = false;
        secondFirst = true;
    } else {
        QByteArray output = nr->readAll();
        //qInfo(output);
        if(nr->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
            if(secondFirst == false) {
                path = "Error while exporting : " + QString(output);
                emit urlRetrieved();
            }
            qDebug() << "Error while retrieving url, assuming already existing link" << endl;
            QNetworkRequest nr(QUrl("https://api.dropboxapi.com/2/sharing/list_shared_links"));
            nr.setRawHeader("Authorization",(tr("Bearer ") + apiKey).toUtf8());
            QString dropboxarg;
            dropboxarg.append("{\"path\": \"");
            dropboxarg.append(fileId);
            dropboxarg.append("\",\"direct_only\": true}");
            nr.setRawHeader("Content-Type","application/json");
            manager->post(nr, dropboxarg.toUtf8());
            secondFirst = false;
        } else if(secondFirst) {
            QJsonDocument doc = QJsonDocument::fromJson(output);
            QString formattedJsonString = doc.toJson(QJsonDocument::Indented);
            QString tex;
            tex.append(formattedJsonString);
            tex.append("URL:\n");
            QString url = doc.object()["url"].toString();
            url[url.size()-1] = '1';
            tex.append(url);
            path = url; // url contient url du fichier uploadé

            emit urlRetrieved();
        } else {
            //Read url from links array
            QJsonDocument doc = QJsonDocument::fromJson(output);
            QString formattedJsonString = doc.toJson(QJsonDocument::Indented);
            QString tex;
            tex.append(formattedJsonString);
            QJsonArray links = doc.object()["links"].toArray();
            QString url = links.at(0).toObject()["url"].toString();
            url[url.size()-1] = '1';
            tex.append(url);
            path = url; // url contient url du fichier uploadé

            emit urlRetrieved();
        }
    }
}
