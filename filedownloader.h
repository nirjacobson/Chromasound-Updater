#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QtNetwork/QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl url, QObject *parent = nullptr);
    virtual ~FileDownloader();

    QByteArray downloadedData();

signals:
    void downloaded();

private:
    QNetworkAccessManager _webCtrl;
    QByteArray _downloadedData;

private slots:
    void fileDownloaded(QNetworkReply* reply);
};

#endif // FILEDOWNLOADER_H
