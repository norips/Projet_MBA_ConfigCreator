#include "configholder.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDebug>
#include "modelitem.h"
#include <QEventLoop>
ConfigHolder::ConfigHolder()
{

}

void ConfigHolder::LoadFromJSONFile(QString &filepath){
    QFile file(filepath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject rootO = doc.object();
    QJsonArray rootA = rootO["canvas"].toArray();
    FileDownloader *fileD = NULL;
    QString base = "../img/";
    foreach(const QJsonValue &v, rootA){
        QJsonObject obj = v.toObject();
        qDebug() << obj["name"] << endl;
        QString name = obj["name"].toString();
        QString url = obj["url"].toString();
        QEventLoop pause;
        fileD = new FileDownloader(QUrl(url),base + name + ".jpg");
        QObject::connect(fileD, SIGNAL (downloaded()), &pause, SLOT (quit()));
        pause.exec();
        QImage img;
        img.loadFromData(fileD->downloadedData());
        canvaItem *c = new canvaItem(QPixmap::fromImage(img),name,base + name);
        delete fileD;

        QJsonArray models = obj["models"].toArray();
        foreach (const QJsonValue &mod, models) {
           QJsonObject mobObj = mod.toObject();
           QString modName = mobObj["name"].toString();
           modelItem *m = new modelItem(modName);
           c->addModel(m);
        }
        canvas.append(c);
    }


}

QVector<canvaItem*> ConfigHolder::getCanvas() const {
    return canvas;
}
