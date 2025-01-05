#include "downloader.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    n_manager = NULL;
    downloading = false;
    aborted = false;
    sourcePath = "";

    qDebug() << "downloader";
}

void Downloader::startDownload(QString _source, long long size, QString id, QString lan, QString os, QString resolution) {
    QtMobility::QSystemStorageInfo sysStrgInfo;
    QStringList drives = sysStrgInfo.logicalDrives();

    QString driveToSave = "";
    qDebug() << sysStrgInfo.availableDiskSpace("C");
    qDebug() << size+size/10;
    qDebug() << size+(size/10);
    if (drives.contains("C") && sysStrgInfo.availableDiskSpace("C") > size+(size/10)) driveToSave = "c:/data/";
    else if (drives.contains("E") && sysStrgInfo.availableDiskSpace("E") > size+(size/10)) driveToSave = "e:/";
    else if (drives.contains("F") && sysStrgInfo.availableDiskSpace("F") > size+(size/10)) driveToSave = "f:/";
    else {
        emit downloadError(1);
        return;
    }

    if (!downloading) {
        downloading = true;
        source = _source;
        if (source.indexOf("http") != 0) source.prepend("http://<YOUR_REPO_DOMAIN>/apps/");
        if (n_manager == NULL) {
            n_manager = new QNetworkAccessManager(this);
        }

        QString statUrl = "http://<YOUR_APPLIST_DOMAIN>/applist-download.php?id="+id+"&language="+lan+"&os="+os+"&resolution="+resolution;

        statReply = n_manager->get(QNetworkRequest(QUrl(statUrl)));
        statReply->setObjectName(driveToSave);

        connect(statReply, SIGNAL(finished()),
                this, SLOT(startRealDownload()));
    }
}

void Downloader::startDownloadWithoutStats(QString _source) {
    if (!downloading) {
        downloading = true;
        source = _source;

        file.setFileName("c:/data/"+source.mid(source.lastIndexOf("/")+1));

        file.open(QIODevice::WriteOnly);
        sourcePath = "c:/data/" + source.mid(source.lastIndexOf("/")+1);
        //qDebug() << source;
        //qDebug() << source.mid(source.lastIndexOf("/")+1);
        //qDebug() << sourcePath;

        if (n_manager == NULL) {
            n_manager = new QNetworkAccessManager(this);
        }

        reply = n_manager->get(QNetworkRequest(QUrl(source)));
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(progress(qint64,qint64)));
        connect(reply, SIGNAL(readyRead()),
                this, SLOT(saveBytes()));
        connect(reply, SIGNAL(finished()),
                this, SLOT(downloadFinished()));
    }
}

void Downloader::startRealDownload() {
    QByteArray data=statReply->readAll();
    QString returnCode = QString::fromUtf8(data);

    if (returnCode == "0") {
        file.setFileName(statReply->objectName() + source.mid(source.lastIndexOf("/")+1));

        file.open(QIODevice::WriteOnly);
        sourcePath = statReply->objectName() + source.mid(source.lastIndexOf("/")+1);

        if (n_manager == NULL) {
            n_manager = new QNetworkAccessManager(this);
        }

        reply = n_manager->get(QNetworkRequest(QUrl(source)));
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(progress(qint64,qint64)));
        connect(reply, SIGNAL(readyRead()),
                this, SLOT(saveBytes()));
        connect(reply, SIGNAL(finished()),
                this, SLOT(downloadFinished()));
    } else {
        emit downloadError(0);
        downloading = false;
    }
}

void Downloader::downloadFinished() {
    //  http://spechard.wordpress.com/2009/07/23/tip-handling-an-http-redirection-with-qnetworkaccessmanager/
    QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    // We'll deduct if the redirection is valid in the redirectUrl function
    QUrl urlRedirectedTo = redirectUrl(possibleRedirectUrl.toUrl(), urlRedirectedTo);

    // If the URL is not empty, we're being redirected.
    if(!urlRedirectedTo.isEmpty()) {
        /*file.setFileName("c:/data/"+source.mid(source.lastIndexOf("/")+1));
        file.open(QIODevice::WriteOnly);

        reply = n_manager->get(QNetworkRequest(urlRedirectedTo));

        connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(progress(qint64,qint64)));
        connect(reply, SIGNAL(readyRead()),
                this, SLOT(saveBytes()));
        connect(reply, SIGNAL(finished()),
                this, SLOT(downloadFinished()));*/
    } else {
        file.close();

        if (!aborted) {
            qDebug() << "downloadFinished";

            //bool ok = QDesktopServices::openUrl(QUrl("file:///"+sourcePath));
            AppManagement::openInstaller(sourcePath);

            emit downloadDone();
        } else {
            aborted = false;
        }

        downloading = false;
    }
}

bool Downloader::cancelDownload() {
    if (downloading) {
        aborted = true;
        reply->abort();
        if (!reply->isRunning()) {
            deleteFile();
            aborted = false;
            return true;
        } else {
            aborted = false;
            return false;
        }
    } else {
        return false;
    }
}

bool Downloader::deleteFile() {
    return file.remove();
}
