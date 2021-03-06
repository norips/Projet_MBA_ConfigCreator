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
#include "texturemov.h"

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
    QJsonArray rootA = rootO[QString("canvas")].toArray();
    FileDownloader *fileD = NULL;
    QString baseB = "../tmp/";
    foreach(const QJsonValue &v, rootA){
        QJsonObject obj = v.toObject();
        qDebug() << obj[QString("name")] << endl;
        QString name = obj[QString("name")].toString();
        QString base = baseB + name;
        Canva *c = new Canva(QPixmap(),name,base + name);

        QJsonObject features = obj[QString("feature")].toObject();
        QJsonArray featureFiles = features[QString("files")].toArray();
        if(featureFiles.size()<3) {
            int ERROR_ISET = 1;
            emit configImported(ERROR_ISET);
        }
        Feature *feature = new Feature(name);
        QEventLoop pause;
        foreach(const QJsonValue &jfilev, featureFiles) {
            QJsonObject jfileo = jfilev.toObject();
            File f(jfileo[QString("name")].toString(),jfileo[QString("path")].toString(),jfileo[QString("MD5")].toString());
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

        QJsonArray models = obj[QString("models")].toArray();
        int modInd = 0;
        foreach (const QJsonValue &mod, models) {
           QJsonObject modObj = mod.toObject();
           QString modName = modObj[QString("name")].toString();
           Model *m = new Model(modName,modInd++);
           m->tlc = modObj[QString("tlc")].toString();
           m->trc = modObj[QString("trc")].toString();
           m->blc = modObj[QString("blc")].toString();
           m->brc = modObj[QString("brc")].toString();
           QJsonArray textures = modObj[QString("textures")].toArray();
           foreach (const QJsonValue &t, textures) {
               qDebug() << "Texture found" << endl;
               QJsonObject tobj = t.toObject();
               if(tobj[QString("type")].toString().compare("image") == 0) {
                   QString url = tobj[QString("path")].toString();
                   name = tobj[QString("name")].toString();
                   fileD = new FileDownloader(QUrl(url),base + "/" + name);
                   qDebug() << "URL : " << url << endl;
                   QObject::connect(fileD, SIGNAL (downloaded()), &pause, SLOT (quit()));
                   pause.exec();
                   QImage img;
                   img.loadFromData(fileD->downloadedData());
                   QPixmap pix = QPixmap::fromImage(img);
                   QString md5 = tobj[QString("MD5")].toString();
                   TextureIMG *tmp = new TextureIMG(pix,url,md5);
                   tmp->setLocalPath(base + "/" + name);
                   tmp->setModified(false);
                   File f(tobj[QString("name")].toString(),tobj[QString("path")].toString(),tobj[QString("MD5")].toString());
                   tmp->setFile(f);
                   m->addTexture(tmp);
                   qDebug() << "Texture added" << endl;
               } else if(tobj[QString("type")].toString().compare("texte") == 0 ) {
                   TextureTXT *tmp = new TextureTXT(tobj[QString("text")].toString());
                   tmp->setModified(false);
                   m->addTexture(tmp);
                   qDebug() << "Texture added :" << tobj[QString("text")].toString() << endl;
               } else if(tobj[QString("type")].toString().compare("video") == 0 ) {
                   TextureMOV *tmp = new TextureMOV;

                   QString url = tobj[QString("path")].toString();
                   name = tobj[QString("name")].toString();
                   fileD = new FileDownloader(QUrl(url),base + "/" + name);
                   qDebug() << "URL : " << url << endl;
                   QObject::connect(fileD, SIGNAL (downloaded()), &pause, SLOT (quit()));
                   pause.exec();


                   File f(tobj[QString("name")].toString(),tobj[QString("path")].toString(),tobj[QString("MD5")].toString());
                   tmp->setFile(f);
                   tmp->setLocalPath(base + "/" + name);
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

static QRect convertToQRect(QString tlc,QString brc) {
    QStringList posTLC = tlc.split(",");
    QStringList posBRC = brc.split(",");
    QPoint pTLC(posTLC.at(0).toDouble(),posTLC.at(0).toDouble());
    QPoint pBRC(posBRC.at(0).toDouble(),posBRC.at(0).toDouble());
    return QRect(pTLC,pBRC);
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
    foreach(Canva *canv, getCanvas()){
        if(pass) {
            pass = false;
            continue;

        }
        QString baseFolder;
        QJsonObject canva;
        canva[QString("name")] = canv->getName();
        QJsonObject feature;
        QJsonArray files;

        if(canv->modified()) {
            canv->setModified(false);
            feature[QString("name")] = canv->getName().remove(QRegExp("[ \"'_-]|[+]"));
            Feature *featureCanva = new Feature(feature[QString("name")].toString());
            QString trimName = canv->getName().remove(QRegExp("[ \"'_-]|[+]"));
            baseFolder = trimName;
            QString filename = base + trimName +".jpg";
            qDebug() << "Filename : " + filename;
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            canv->getPix().save(&file, "JPG");
            QProcess *process = new QProcess();
#ifdef Q_OS_WIN
            QString prog = "../bin/genTexData.exe " + filename + " -level=2 -leveli=1 -dpi=100 -min_dpi=20 -max_dpi=100";
#else
            QString prog = "../bin/genTexData " + filename + " -level=2 -leveli=1 -dpi=100 -min_dpi=20 -max_dpi=100";
#endif
            process->start(prog);
            process->waitForFinished(-1); //Timeout 20min
            QString ext[] = {"iset","fset3","fset"};
            QString folderIset = "";
            for(int i = 0; i < 3; i++) {
                QJsonObject file;
                QFile filup(base + trimName + "." + ext[i]);
                filup.open(QFile::ReadOnly);
                QCryptographicHash hash(QCryptographicHash::Md5);
                QByteArray strHash;
                if (hash.addData(&filup)) {
                    strHash = hash.result();
                }
                if(i==0)
                    folderIset = QString(strHash.toHex());
                file[QString("name")] = trimName + "." + ext[i];
                filup.seek(0);
                file[QString("path")] = cex->upload(QString(baseFolder + "/" + folderIset + "/" + trimName + "." + ext[i]),filup.readAll());
                file[QString("MD5")] = QString(strHash.toHex());
                File fileFeature(file[QString("name")].toString(),file[QString("path")].toString(),file[QString("MD5")].toString());
                if(ext[i].compare("iset") == 0) {
                    featureCanva->setIset(fileFeature);
                } else if(ext[i].compare("fset") == 0) {
                    featureCanva->setFset(fileFeature);
                } else if(ext[i].compare("fset3") == 0) {
                    featureCanva->setFset3(fileFeature);
                }
                files.append(file);
            }
            canv->setFeature(featureCanva);
        } else {
            feature[QString("name")] = canv->getName().remove(QRegExp("[ \"'_-]|[+]"));
            QString trimName = canv->getName().remove(QRegExp("[ \"'_-]|[+]"));
            baseFolder = trimName;
            QJsonObject iset;
            iset[QString("name")] = canv->getFeature()->getIset().getName();
            iset[QString("path")] = canv->getFeature()->getIset().getPath();
            iset[QString("MD5")] = canv->getFeature()->getIset().getMD5();
            files.append(iset);
            QJsonObject fset;
            fset[QString("name")] = canv->getFeature()->getFset().getName();
            fset[QString("path")] = canv->getFeature()->getFset().getPath();
            fset[QString("MD5")] = canv->getFeature()->getFset().getMD5();
            files.append(fset);
            QJsonObject fset3;
            fset3[QString("name")] = canv->getFeature()->getFset3().getName();
            fset3[QString("path")] = canv->getFeature()->getFset3().getPath();
            fset3[QString("MD5")] = canv->getFeature()->getFset3().getMD5();
            files.append(fset3);
        }





        feature[QString("files")] = files;
        canva[QString("feature")] = feature;
        QJsonArray models;
        QVector<QRect> listRect;
        foreach(Model *m, canv->getItems()){
            QJsonObject model;

            int zModel = 0; //First model will be on 0 axis and next on n*2 axis
            model[QString("name")] = m->name;

            QRect currentRect = convertToQRect(m->tlc,m->brc);
            foreach(QRect r, listRect) {
                if(r.intersects(currentRect)) {
                    zModel+=2;
                }
            }
            listRect.append(currentRect);

            model[QString("tlc")] = setZ(m->tlc,zModel);
            model[QString("trc")] = setZ(m->trc,zModel);
            model[QString("blc")] = setZ(m->blc,zModel);
            model[QString("brc")] = setZ(m->brc,zModel);
            QJsonArray textures;
            foreach(Texture *t, m->getTextures()){
                if(t->modified()) {
                    qDebug() << "Modified texture" << endl;
                    t->setModified(false);
                    if(t->getType() == Texture::TEXT) {
                        TextureTXT *ttxt =  static_cast<TextureTXT *>(t);
                        QJsonObject tex;
                        tex[QString("text")] = ttxt->getData();
                        tex[QString("type")] = QString("texte");
                        textures.append(tex);
                    }
                    if(t->getType() == Texture::IMG) {
                        TextureIMG *timg =  static_cast<TextureIMG *>(t);
                        QFileInfo fileName(timg->getLocalPath());
                        QString fileExt = fileName.suffix();
                        QJsonObject tex;
                        QByteArray bArray;
                        QBuffer buffer(&bArray);
                        buffer.open(QIODevice::WriteOnly);
                        if(fileExt.compare("jpg",Qt::CaseInsensitive) == 0 || fileExt.compare("jpeg",Qt::CaseInsensitive) == 0) {
                            timg->getData().save(&buffer, "JPG");
                        } else if (fileExt.compare("png",Qt::CaseInsensitive) == 0) {
                            timg->getData().save(&buffer, "PNG");
                        } else {
                            timg->getData().save(&buffer, "BMP");
                        }
                        QCryptographicHash hash(QCryptographicHash::Md5);
                        QByteArray strHash;
                        hash.addData(bArray);
                        strHash = hash.result();
                        QString nameTex = strHash.toHex() + "." + fileExt;
                        tex[QString("name")] = nameTex;
                        tex[QString("path")] = QString(cex->upload(QString(baseFolder + "/" + nameTex),bArray));
                        tex[QString("MD5")] = QString(strHash.toHex());
                        tex[QString("type")] = QString("image");
                        File f(tex[QString("name")].toString(),tex[QString("path")].toString(),tex[QString("MD5")].toString());
                        timg->setFile(f);
                        textures.append(tex);
                    }
                    if(t->getType() == Texture::MOV) {
                        TextureMOV *tmov =  static_cast<TextureMOV *>(t);
                        QString filePath = tmov->getLocalPath();
                        QFileInfo fileName(filePath);
                        QString fileExt = fileName.suffix();
                        QJsonObject tex;


                        QFile file(filePath);

                        if (!file.open(QIODevice::ReadOnly)) {
                            qDebug() << "Can't open movie" << endl;
                            continue;
                        }
                        QByteArray bArray = file.readAll();
                        QBuffer buffer(&bArray);
                        buffer.open(QIODevice::WriteOnly);


                        QCryptographicHash hash(QCryptographicHash::Md5);
                        QByteArray strHash;
                        hash.addData(bArray);
                        strHash = hash.result();
                        QString nameTex = strHash.toHex() + "." + fileExt;
                        tex[QString("name")] = nameTex;
                        tex[QString("path")] = QString(cex->upload(QString(baseFolder + "/" + nameTex),bArray));
                        tex[QString("MD5")] = QString(strHash.toHex());
                        tex[QString("type")] = QString("video");
                        File f(tex[QString("name")].toString(),tex[QString("path")].toString(),tex[QString("MD5")].toString());
                        tmov->setFile(f);
                        textures.append(tex);
                    }
                } else {
                    if(t->getType() == Texture::TEXT) {
                        TextureTXT *ttxt =  static_cast<TextureTXT *>(t);
                        QJsonObject tex;
                        tex[QString("text")] = ttxt->getData();
                        tex[QString("type")] = QString("texte");
                        textures.append(tex);
                    }
                    if(t->getType() == Texture::IMG) {
                        TextureIMG *timg =  static_cast<TextureIMG *>(t);
                        QJsonObject tex;
                        tex[QString("type")] = QString("image");
                        tex[QString("name")] = timg->getFile().getName();
                        tex[QString("path")] = timg->getUrl();
                        tex[QString("MD5")] =  timg->getMD5();
                        textures.append(tex);
                    }
                    if(t->getType() == Texture::MOV) {
                        TextureMOV *tmov =  static_cast<TextureMOV *>(t);
                        QJsonObject tex;
                        tex[QString("type")] = QString("video");
                        tex[QString("name")] = tmov->getFile().getName();
                        tex[QString("path")] = tmov->getUrl();
                        tex[QString("MD5")] =  tmov->getMD5();
                        textures.append(tex);
                    }
                }
            }
            model[QString("textures")] = textures;
            models.append(model);
        }
        canva[QString("models")] = models;
        canvas.append(canva);
    }
    QJsonObject canvasRoot;
    canvasRoot[QString("canvas")] = canvas;
    doc.setObject(canvasRoot);
    file.write(doc.toJson());
    file.close();
    QFile fileR(filepath);
    fileR.open(QIODevice::ReadOnly | QIODevice::Text);
    QCryptographicHash hash(QCryptographicHash::Md5);
    QByteArray strHash;
    if (hash.addData(&fileR)) {
        strHash = hash.result();
    }
    fileR.seek(0);
    QString file_url = cex->upload(QString(strHash.toHex() + ".json"),fileR.readAll());
    QFile fileURL(filepath + ".url");
    fileURL.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&fileURL);
    out << file_url; //Write url to .url file
    fileURL.close();
    fileR.close();
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
