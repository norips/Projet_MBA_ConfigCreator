#include "configholder.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDebug>
#include <QEventLoop>
#include <QBuffer>

#include "canva.h"
#include "model.h"
#include "texture.h"
#include "texturetxt.h"
#include "textureimg.h"
#include "configexporter.h"

ConfigHolder ConfigHolder::m_instance=ConfigHolder();
ConfigHolder::ConfigHolder()
{
    first = true;
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
        qDebug() << "URL : " << url << endl;
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
           QJsonArray textures = modObj["textures"].toArray();
           foreach (const QJsonValue &t, textures) {
               QJsonObject tobj = t.toObject();
               if(m->type.compare("image") == 0) {
                   url = tobj["path"].toString();
                   name = tobj["name"].toString();
                   fileD = new FileDownloader(QUrl(url),base + name);
                   qDebug() << "URL : " << url << endl;
                   QObject::connect(fileD, SIGNAL (downloaded()), &pause, SLOT (quit()));
                   pause.exec();
                   QImage img;
                   img.loadFromData(fileD->downloadedData());
                   QPixmap pix = QPixmap::fromImage(img);
                   m->addTexture(new TextureIMG(pix));
               } else if(m->type.compare("texte") == 0 ) {
                   m->addTexture(new TextureTXT(tobj["texte"].toString()));
               }

           }
           c->addModel(m);
        }
        canvas.append(c);
    }


}

void ConfigHolder::ExportToJSONFile(QString &filepath,ConfigExporter &cex) {
    QFile file(filepath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QJsonDocument doc;
    QJsonArray canvas;
    foreach(Canva *v, getCanvas()){
        QJsonObject canva;
        canva["name"] = v->getName();
        QJsonObject feature;
        feature["name"] = v->getName().trimmed();
        QJsonArray files;
        for(int i = 0; i < 3; i++) {
            QJsonObject file;
            file["name"] = v->getName().trimmed() + i + ".file";
            file["path"] = "http://google.fr/";
            file["MD5"] = QString("0000") + i + QString("0000");
            files.append(file);
        }
        feature["files"] = files;
        canva["feature"] = feature;
        QJsonArray models;
        foreach(Model *m, v->getItems()){
            QJsonObject model;
            model["name"] = m->name;
            model["type"] = "texte";
            model["tlc"] = m->tlc;
            model["trc"] = m->trc;
            model["blc"] = m->blc;
            model["brc"] = m->brc;
            QJsonArray textures;
            foreach(Texture *t, m->getTextures()){
                if(t->getType() == Texture::TEXT) {
                    TextureTXT *ttxt =  dynamic_cast<TextureTXT *>(t);
                    QJsonObject tex;
                    tex["text"] = ttxt->getData();
                    textures.append(tex);
                }
                if(t->getType() == Texture::IMG) {
                    TextureIMG *timg =  dynamic_cast<TextureIMG *>(t);
                    QJsonObject tex;
                    tex["name"] = "test";
                    QByteArray bArray;
                    QBuffer buffer(&bArray);
                    buffer.open(QIODevice::WriteOnly);
                    timg->getData().save(&buffer, "PNG");
                    tex["path"] = cex.upload(bArray);
                    tex["MD5"] = "8de9d56c59ce3695f5416f2b0c4c7537";
                    textures.append(tex);
                }
            }
            model["textures"] = textures;
            models.append(model);
        }
        canva["models"] = models;
        canvas.append(canva);
    }
    QJsonObject canvasRoot;
    canvasRoot["canvas"] = canvas;
    doc.setObject(canvasRoot);
    file.write(doc.toJson());
}

QVector<Canva*> ConfigHolder::getCanvas() const {
    return canvas;
}
/**
 * @brief ConfigHolder::init
 * Add an empty canva
 */
void ConfigHolder::addEmpty() {
    if(first){
        canvas.append(new Canva(QPixmap("../img/plus.png"),"Nouveau",""));
        first = false;
    } else {
        canvas.append(new Canva(QPixmap("../img/mba.png"),"Nouveau+",""));
    }
}
