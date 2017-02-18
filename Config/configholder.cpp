#include "configholder.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDebug>
#include <QEventLoop>
#include <QBuffer>
#include <QProcess>
#include <QDir>

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
        QJsonObject features = obj["feature"].toObject();
        QJsonArray featureFiles = features["files"].toArray();
        if(featureFiles.size()<3) {
            //Error
        }
        Feature *feature = new Feature(name);
        foreach(const QJsonValue &jfilev, featureFiles) {
            QJsonObject jfileo = jfilev.toObject();
            File f(jfileo["name"].toString(),jfileo["path"].toString(),jfileo["MD5"].toString());
            qDebug() << f.getName();
            if(f.getName().endsWith(".iset")) {
                feature->setIset(f);
            }
            if(f.getName().endsWith(".fset")) {
                feature->setFset(f);
            }
            if(f.getName().endsWith(".fset3")) {
                feature->setFset3(f);
            }
        }
        c->setFeature(feature);

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
                   TextureIMG *tmp = new TextureIMG(pix);
                   tmp->setModified(false);
                   m->addTexture(tmp);
               } else if(m->type.compare("texte") == 0 ) {
                   TextureTXT *tmp = new TextureTXT(tobj["texte"].toString());
                   tmp->setModified(false);
                   m->addTexture(tmp);
               }

           }
           m->setModified(false);
           c->addModel(m);
        }
        c->setModified(false);
        canvas.append(c);
    }


}
void ConfigHolder::ExportToJSONFile(QString &filepath,ConfigExporter &cex) {
    QFile file(filepath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QString base = "../tmp/";
    QJsonDocument doc;
    QJsonArray canvas;
    bool pass=true;
    foreach(Canva *v, getCanvas()){
        if(pass) {
            pass = false;
            continue;

        }
        QJsonObject canva;
        canva["name"] = v->getName();
        QJsonObject feature;
        QJsonArray files;

        if(v->modified()) {
            feature["name"] = v->getName().remove(QRegExp("[ \"'_-]"));
            QString trimName = v->getName().remove(QRegExp("[ \"'_-]"));
            QString filename = base + trimName +".jpg";
            qDebug() << "Filename : " + filename;
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            v->getPix().save(&file, "JPG");
            QProcess *process = new QProcess();
            QString prog = "../bin/genTexData " + filename + " -level=2 -leveli=1 -dpi=100 -min_dpi=20 -max_dpi=100";
            process->start(prog);
            process->waitForFinished(-1); //Timeout 20min
            qDebug() << "Code : " + process->exitCode();
            QString ext[] = {"iset","fset3","fset"};
            for(int i = 0; i < 3; i++) {
                QJsonObject file;
                QFile filup(base + trimName + "." + ext[i]);
                filup.open(QFile::ReadOnly);
                QCryptographicHash hash(QCryptographicHash::Md5);
                QByteArray strHash;
                if (hash.addData(&filup)) {
                    strHash = hash.result();
                }
                file["name"] = trimName + "." + ext[i];
                file["path"] = cex.upload(filup.readAll());
                file["MD5"] = QString(strHash.toHex());
                files.append(file);
            }
        } else {
            feature["name"] = v->getFeature()->getName();
            QJsonObject iset;
            iset["name"] = v->getFeature()->getIset().getName();
            iset["path"] = v->getFeature()->getIset().getPath();
            iset["MD5"] = v->getFeature()->getIset().getMD5();
            files.append(iset);
            QJsonObject fset;
            fset["name"] = v->getFeature()->getFset().getName();
            fset["path"] = v->getFeature()->getFset().getPath();
            fset["MD5"] = v->getFeature()->getFset().getMD5();
            files.append(fset);
            QJsonObject fset3;
            fset3["name"] = v->getFeature()->getFset3().getName();
            fset3["path"] = v->getFeature()->getFset3().getPath();
            fset3["MD5"] = v->getFeature()->getFset3().getMD5();
            files.append(fset3);
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
            int indexTex = 0;
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
                    QCryptographicHash hash(QCryptographicHash::Md5);
                    QByteArray strHash;
                    hash.addData(bArray);
                    strHash = hash.result();
                    QString tmpName = m->name.remove(QRegExp("[ \"'_-]")) + "%1" + ".png";
                    tex["name"] = tmpName.arg(++indexTex);
                    tex["path"] = cex.upload(bArray);
                    tex["MD5"] = QString(strHash.toHex());
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
