#include "configholder.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDebug>
#include <QEventLoop>

#include "canva.h"
#include "model.h"

ConfigHolder ConfigHolder::m_instance=ConfigHolder();
ConfigHolder::ConfigHolder()
{

}

ConfigHolder& ConfigHolder::Instance(){
    return m_instance;
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
        Canva *c = new Canva(QPixmap::fromImage(img),name,base + name);
        delete fileD;

        QJsonArray models = obj["models"].toArray();
        int modInd = 0;
        foreach (const QJsonValue &mod, models) {
           QJsonObject modObj = mod.toObject();
           QString modName = modObj["name"].toString();
           Model *m = new Model(modName,modInd++);
           m->tlc = modObj["tlc"].toString();
           m->trc = modObj["trc"].toString();
           m->blc = modObj["blc"].toString();
           m->brc = modObj["brc"].toString();
           m->type = modObj["type"].toString();
           c->addModel(m);
        }
        canvas.append(c);
    }


}

QVector<Canva*> ConfigHolder::getCanvas() const {
    return canvas;
}
/**
 * @brief ConfigHolder::init
 * Init an empty config
 */
void ConfigHolder::init() {
    canvas.clear();
    canvas.append(new Canva(QPixmap("../img/plus.png"),"Nouveau",""));
}
