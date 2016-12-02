#include "filedownloader.h"
#include <QFile>
FileDownloader::FileDownloader(QUrl imageUrl,QString fileName, QObject *parent) :
 QObject(parent)
{
 connect(
  &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
  this, SLOT (fileDownloaded(QNetworkReply*))
  );

 QNetworkRequest request(imageUrl);
 m_fileName = fileName;
 m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = QByteArray(pReply->readAll());
    //emit a signal
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
