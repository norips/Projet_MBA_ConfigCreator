#include "filedownloader.h"
#include <QFile>
#include <QDir>
FileDownloader::FileDownloader(QUrl imageUrl,QString fileName, QObject *parent) :
 QObject(parent)
{
    m_WebCtrl = new QNetworkAccessManager();
 connect(
  m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
  this, SLOT (fileDownloaded(QNetworkReply*))
  );
 QNetworkRequest request(imageUrl);
 m_fileName = fileName;
 m_WebCtrl->get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    if(pReply->error()) {
        qDebug() << pReply->errorString() << endl;
    }
    int statusCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode == 302)
    {
        QUrl newUrl = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        qDebug() << "redirected from to " + newUrl.toString();
        QNetworkRequest newRequest(newUrl);
        m_WebCtrl->get(newRequest);
        return;
    }
    m_DownloadedData = QByteArray(pReply->readAll());
    //emit a signal
    QFileInfo fi(m_fileName);
    QDir().mkpath(fi.path());
    QFile file(m_fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qDebug() << "Can't open" << endl;
    } else {
        file.write(m_DownloadedData);
        file.close();
    }
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
 return m_DownloadedData;
}
