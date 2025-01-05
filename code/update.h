#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include "downloader.h"

class Update : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString changelog READ changelog NOTIFY changelogChanged)
public:
    explicit Update(QObject *parent = 0);

    Q_INVOKABLE void loadUpdateData(bool all = false);
    Q_INVOKABLE void startDownload();

    QString changelog() { return _changelog; }

signals:
    void loadingDone(bool loadResult, QString message);
    void progressChanged(int progress);

    void changelogChanged();

public slots:
    void parseData();

    void progressChangedReceived(int percent) {
        emit progressChanged(percent);
    }

private:
    QNetworkAccessManager* n_manager;
    QString fileUrl;

    QString _changelog;

    Downloader downloader;
};

#endif // UPDATE_H
