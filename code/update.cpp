#include "update.h"

Update::Update(QObject *parent) :
    QObject(parent)
{
    n_manager = new QNetworkAccessManager(this);
    fileUrl = "";

    connect(&downloader, SIGNAL(progressChanged(int)),
            this, SLOT(progressChangedReceived(int)));
}

void Update::loadUpdateData(bool all) {
    //qDebug() << "loading started";
    QNetworkReply *reply = n_manager->get(QNetworkRequest(QUrl((all ? "http://<YOUR_APPLIST_DOMAIN>/changelog.xml" : "http://<YOUR_APPLIST_DOMAIN>/version.xml"))));
    if (all) reply->setObjectName("all");
    connect(reply, SIGNAL(finished()),
            this, SLOT(parseData()));
}

void Update::startDownload() {
    downloader.startDownloadWithoutStats(fileUrl);
}

void Update::parseData() {
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data=pReply->readAll();
    QString html = QString::fromUtf8(data);

    if (html.length() > 0) {
        int lastPos = html.indexOf("<url>") + 5;
        fileUrl = html.mid(lastPos, html.indexOf("</url>", lastPos)-lastPos);

        lastPos = html.indexOf("<message>") + 9;
        if (pReply->objectName() == "all") {
            _changelog = html.mid(lastPos, html.indexOf("</message>", lastPos)-lastPos);
            emit changelogChanged();
        } else {
            emit loadingDone(true, html.mid(lastPos, html.indexOf("</message>", lastPos)-lastPos));
        }
    } else {
        if (pReply->objectName() == "all") {
            emit changelogChanged();
        } else {
            emit loadingDone(false, "");
        }
    }
}
