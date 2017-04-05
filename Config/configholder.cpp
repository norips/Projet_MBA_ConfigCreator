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

ConfigHolder* ConfigHolder::m_instance=0;
ConfigHolder::ConfigHolder( QObject* parent ) : QObject( parent )
{
    first = true;
}

ConfigHolder* ConfigHolder::Instance( QObject* parent ){
    if( !m_instance )
        m_instance = new ConfigHolder( parent );
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
    QString baseB = "../tmp/";
    foreach(const QJsonValue &v, rootA){
        QJsonObject obj = v.toObject();
        qDebug() << obj["name"] << endl;
        QString name = obj["name"].toString();
        QString base = baseB + name;
        Canva *c = new Canva(QPixmap(),name,base + name);

        QJsonObject features = obj["feature"].toObject();
        QJsonArray featureFiles = features["files"].toArray();
        if(featureFiles.size()<3) {
            int ERROR_ISET = 1;
            emit configImported(ERROR_ISET);
        }
        Feature *feature = new Feature(name);
        QEventLoop pause;
        foreach(const QJsonValue &jfilev, featureFiles) {
            QJsonObject jfileo = jfilev.toObject();
            File f(jfileo["name"].toString(),jfileo["path"].toString(),jfileo["MD5"].toString());
            qDebug() << f.getName();
            if(f.getName().endsWith(".iset")) {
                feature->setIset(f);
                QString url = f.getPath();
                fileD = new FileDownloader(QUrl(url),base + name + ".jpg");
                qDebug() << "URL : " << url << endl;
                QObject::connect(fileD, SIGNAL (downloaded()), &pause, SLOT (quit()));
                pause.exec();
                QImage img;
                img.loadFromData(fileD->downloadedData().remove(0,4));
                c->setPix(QPixmap::fromImage(img));
            }
            if(f.getName().endsWith(".fset")) {
                feature->setFset(f);
            }
            if(f.getName().endsWith(".fset3")) {
                feature->setFset3(f);
            }
        }
        c->setFeature(feature);
        c->setModified(false);
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
           QJsonArray textures = modObj["textures"].toArray();
           foreach (const QJsonValue &t, textures) {
               qDebug() << "Texture found" << endl;
               QJsonObject tobj = t.toObject();
               if(tobj["type"].toString().compare("image") == 0) {
                   QString url = tobj["path"].toString();
                   name = tobj["name"].toString();
                   fileD = new FileDownloader(QUrl(url),base + name);
                   qDebug() << "URL : " << url << endl;
                   QObject::connect(fileD, SIGNAL (downloaded()), &pause, SLOT (quit()));
                   pause.exec();
                   QImage img;
                   img.loadFromData(fileD->downloadedData());
                   QPixmap pix = QPixmap::fromImage(img);
                   QString md5 = tobj["MD5"].toString();
                   TextureIMG *tmp = new TextureIMG(pix,url,md5);
                   tmp->setLocalPath(base + name);
                   tmp->setModified(false);
                   m->addTexture(tmp);
                   qDebug() << "Texture added" << endl;
               } else if(tobj["type"].toString().compare("texte") == 0 ) {
                   TextureTXT *tmp = new TextureTXT(tobj["text"].toString());
                   tmp->setModified(false);
                   m->addTexture(tmp);
                   qDebug() << "Texture added :" << tobj["text"].toString() << endl;
               }

           }
           m->setModified(false);
           c->addModel(m);
        }
        c->setModified(false);
        canvas.append(c);
    }
    int NO_ERROR = 0;
    emit configImported(NO_ERROR);

}
static QString setZ(QString pos,int z) {
    QString tmp;
    tmp = pos;
    int cut = tmp.lastIndexOf(",");
    tmp = tmp.left(cut+1);
    tmp.append(QString::number(z));
    return tmp;
}

void ConfigHolder::ExportToJSONFile(QString &filepath,ConfigExporter *cex) {
    QFile file(filepath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QString base = "../tmp/";
    QJsonDocument doc;
    QJsonArray canvas;
    bool pass=true;
    QHash<QString,QString> tmpHash;
    //No canva with same name
    foreach(Canva *v, getCanvas()){
        QString name = tmpHash[v->getName()];
        int i = 0;
        while(!name.isNull()) {
            QString newName = v->getName() + QString::number(i++);
            v->setName(newName);
            name = tmpHash[v->getName()];
            qDebug() << "Name : " + v->getName();
        }
        tmpHash.insert(v->getName(),"None");

    }
    foreach(Canva *v, getCanvas()){
        if(pass) {
            pass = false;
            continue;

        }
        QString baseFolder;
        QJsonObject canva;
        canva["name"] = v->getName();
        QJsonObject feature;
        QJsonArray files;

        if(v->modified()) {
            v->setModified(false);
            feature["name"] = v->getName().remove(QRegExp("[ \"'_-]|[+]"));
            QString trimName = v->getName().remove(QRegExp("[ \"'_-]|[+]"));
            baseFolder = trimName;
            QString filename = base + trimName +".jpg";
            qDebug() << "Filename : " + filename;
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            v->getPix().save(&file, "JPG");
            QProcess *process = new QProcess();
#ifdef Q_OS_WIN
            QString prog = "../bin/genTexData.exe " + filename + " -level=2 -leveli=1 -dpi=100 -min_dpi=20 -max_dpi=100";
#else
            QString prog = "../bin/genTexData " + filename + " -level=2 -leveli=1 -dpi=100 -min_dpi=20 -max_dpi=100";
#endif
            process->start(prog);
            process->waitForFinished(-1); //Timeout 20min
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
                filup.seek(0);
                file["path"] = cex->upload(QString(baseFolder + "/" + trimName + "." + ext[i]),filup.readAll());
                file["MD5"] = QString(strHash.toHex());
                files.append(file);
            }
        } else {
            feature["name"] = v->getName().remove(QRegExp("[ \"'_-]|[+]"));
            QString trimName = v->getName().remove(QRegExp("[ \"'_-]|[+]"));
            baseFolder = trimName;
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
        int zModel = 0; //First model will be on 0 axis and next on n*2 axis
        foreach(Model *m, v->getItems()){
            QJsonObject model;
            QString tmp;
            model["name"] = m->name;
            model["tlc"] = setZ(m->tlc,zModel);
            model["trc"] = setZ(m->trc,zModel);
            model["blc"] = setZ(m->blc,zModel);
            model["brc"] = setZ(m->brc,zModel);
            zModel+=2;
            QJsonArray textures;
            int indexTex = 0;
            foreach(Texture *t, m->getTextures()){
                if(t->modified()) {
                    qDebug() << "Modified texture" << endl;
                    t->setModified(false);
                    if(t->getType() == Texture::TEXT) {
                        TextureTXT *ttxt =  dynamic_cast<TextureTXT *>(t);
                        QJsonObject tex;
                        tex["text"] = ttxt->getData();
                        tex["type"] = "texte";
                        textures.append(tex);
                    }
                    if(t->getType() == Texture::IMG) {
                        TextureIMG *timg =  dynamic_cast<TextureIMG *>(t);
                        QJsonObject tex;
                        QByteArray bArray;
                        QBuffer buffer(&bArray);
                        buffer.open(QIODevice::WriteOnly);
                        timg->getData().save(&buffer, "PNG");
                        QCryptographicHash hash(QCryptographicHash::Md5);
                        QByteArray strHash;
                        hash.addData(bArray);
                        strHash = hash.result();
                        QString tmpName = m->name.remove(QRegExp("[ \"'_-]|[+]")) + "%1" + ".png";
                        tex["name"] = tmpName.arg(++indexTex);
                        tex["path"] = QString(cex->upload(QString(baseFolder + "/" + tmpName.arg(indexTex)),bArray));
                        tex["MD5"] = QString(strHash.toHex());
                        tex["type"] = "image";
                        textures.append(tex);
                    }
                } else {
                    if(t->getType() == Texture::TEXT) {
                        TextureTXT *ttxt =  dynamic_cast<TextureTXT *>(t);
                        QJsonObject tex;
                        tex["text"] = ttxt->getData();
                        tex["type"] = "texte";
                        textures.append(tex);
                    }
                    if(t->getType() == Texture::IMG) {
                        TextureIMG *timg =  dynamic_cast<TextureIMG *>(t);
                        QJsonObject tex;
                        tex["type"] = "image";
                        QString tmpName = m->name.remove(QRegExp("[ \"'_-]")) + "%1" + ".png";
                        tex["name"] = tmpName.arg(++indexTex);
                        tex["path"] = timg->getUrl();
                        tex["MD5"] =  timg->getMD5();
                        textures.append(tex);
                    }
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
    int NO_ERROR = 0;
    emit configExported(NO_ERROR);
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
        Canva *c = new Canva(QPixmap("../img/plus.png"),"Nouveau","");
        canvas.append(c);
        first = false;
    } else {
        Canva *c = new Canva(QPixmap("../img/mba.png"),"Nouveau+","");
        c->setModified(true);
        canvas.append(c);
    }
}

void ConfigHolder::delCanva(int i){
    canvas.remove(i);
}
