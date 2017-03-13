#include "threadexport.h"
#include "Config/dropboxexporter.h"
ThreadExport::ThreadExport(QString filename, ConfigHolder* holder)
{
    this->holder = holder;
    this->filename = filename;
}

void ThreadExport::run() {
    //Quick and dirty hack to remove error, QNetworkAccessManager used in DropboxExporter need to be created inside the same thread as this->run()
    this->exporter = new DropboxExporter();
    qDebug() << "Thread start" << endl;
    holder->ExportToJSONFile(filename,exporter);
    delete(exporter);
}
