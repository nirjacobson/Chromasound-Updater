#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl url, QObject *parent)
    : QObject{parent}
{
    connect(&_webCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(url);
    _webCtrl.get(request);
}

FileDownloader::~FileDownloader()
{

}

QByteArray FileDownloader::downloadedData()
{
    return _downloadedData;
}

void FileDownloader::fileDownloaded(QNetworkReply* reply)
{
    _downloadedData = reply->readAll();
    reply->deleteLater();
    emit downloaded();
}


