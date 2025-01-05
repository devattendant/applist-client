#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QFileInfo>
#include <QSystemStorageInfo>
#include <QDesktopServices>

#include "appmanagement.h"

#include <QDebug>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);
    
    void startDownload(QString _source, long long size, QString id, QString lan, QString os, QString resolution);
    void startDownloadWithoutStats(QString _source);

    bool cancelDownload();

    bool deleteFile();

signals:
    void downloadDone();
    void downloadError(int code); // 0 -> error receive reply from stat server; 1 -> not enough space
    void progressChanged(int percent);
    
public slots:
    void downloadFinished();
    void saveBytes() {
        //qDebug() << "saveBytes";
        file.write(reply->read(reply->bytesAvailable()));
    }
    void progress(qint64 bytesReceived, qint64 bytesTotal) {
        emit progressChanged((int)(((double)bytesReceived/bytesTotal)*100));
    }
    void startRealDownload();

private:
    QNetworkAccessManager* n_manager;
    QFile file;
    QNetworkReply* reply;
    QNetworkReply* statReply;

    bool downloading, aborted;
    QString sourcePath, source;
    
    //  http://spechard.wordpress.com/2009/07/23/tip-handling-an-http-redirection-with-qnetworkaccessmanager/
    QUrl redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const {
        QUrl redirectUrl;
        if(!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != oldRedirectUrl)
            redirectUrl = possibleRedirectUrl;
        return redirectUrl;
    }
};

#endif // DOWNLOADER_H
