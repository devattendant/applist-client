#ifndef NEWS_H
#define NEWS_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QTextDocument>

#include "roleitemmodel.h"

class News : public QObject
{
    Q_OBJECT

    Q_PROPERTY(RoleItemModel* _model READ _model NOTIFY listChanged)

private slots:
    void feedFetchFinished();
    void htmlFetchFinished();

public:
    explicit News(QObject *parent = 0);

    RoleItemModel* _model() {return model;}
    RoleItemModel *model;
    QNetworkAccessManager* comm_manager;

    QString url, htmlContent;

    bool updating;
    Q_INVOKABLE bool updateRunning () { return updating; }

    Q_INVOKABLE void fetch() {
        if (!updating) {
            qDebug() << "fetch feedUpdateStarted";
            qDebug() << "feedUrl: " + url;
            QNetworkReply *reply = comm_manager->get(QNetworkRequest(QUrl::fromPercentEncoding(url.toUtf8())));

            updating = true;
            connect(reply, SIGNAL(finished()),
                    this, SLOT(feedFetchFinished()));
        }
    }

    void parseFeed(QString html);

    Q_INVOKABLE void fetchHtml(QString link) {
        qDebug() << "fetch HTMLStarted " + link;
        QNetworkReply *reply = comm_manager->get(QNetworkRequest(QUrl(link)));
        connect(reply, SIGNAL(finished()),
                this, SLOT(htmlFetchFinished()));
    }
    Q_INVOKABLE QString getHtml() {
        return htmlContent;
    }

    Q_INVOKABLE void setFeedUrl (QString _url) {
    }

    Q_INVOKABLE QString feedUrl () {
        return url;
    }

    bool openInBrowser;
    Q_INVOKABLE bool getOpenInBrowser () { return openInBrowser; }
    Q_INVOKABLE void setOpenInBrowser (bool newSetting) {
        QSettings settings("schumi1331", "AppList");
        openInBrowser = newSetting;
        settings.setValue("news/openInBrowser", newSetting);
    }

    //  http://spechard.wordpress.com/2009/07/23/tip-handling-an-http-redirection-with-qnetworkaccessmanager/
    QUrl redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const {
        QUrl redirectUrl;
        if(!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != oldRedirectUrl)
            redirectUrl = possibleRedirectUrl;
        return redirectUrl;
    }
    
signals:
    void listChanged();
    void htmlChanged();
};

#endif // NEWS_H
