#include "threadexport.h"
#include "Config/dropboxexporter.h"
ThreadExport::ThreadExport(QString filename, ConfigHolder* holder)
{
    this->holder = holder;
    this->filename = filename;
}

void ThreadExport::run() {
    this->exporter = new DropboxExporter();
    qDebug() << "Thread start" << endl;
    holder->ExportToJSONFile(filename,exporter);
    delete(exporter);
}
