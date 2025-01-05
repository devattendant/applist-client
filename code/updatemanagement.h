#ifndef UPDATEMANAGEMENT_H
#define UPDATEMANAGEMENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>
#include <QStringList>
#include <QDateTime>
#include "appmanagement.h"

class UpdateManagement : public QObject
{
    Q_OBJECT
public:
    explicit UpdateManagement(bool _widget, QObject *parent = 0);

    void loadList();

    // Update count sync for widget
    int updateCount();

    bool checkForUpdate(QString uid, QString uidstore, QString uidunsigned, QString version, QString versionstore, QString versionunsigned);
    
signals:
    void loadingFinished(int updates, int newApps);
    void loadingFinished(QStringList updateIds);
    void updateWidgetCount(int updates);
    
public slots:
    void listLoaded();

private:
    AppManagement appManagement;
    QNetworkAccessManager* n_manager;
    void parseList(QString html);
    QString parseItem(QString html, int& lastPos, QString itemName);

    bool widget, useUnsigned;

    QString settingsKey;
    bool lastUpdateMoreThanADayAgo();

    //  http://spechard.wordpress.com/2009/07/23/tip-handling-an-http-redirection-with-qnetworkaccessmanager/
    QUrl redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const {
        QUrl redirectUrl;
        if(!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != oldRedirectUrl)
            redirectUrl = possibleRedirectUrl;
        return redirectUrl;
    }
    
};

#endif // UPDATEMANAGEMENT_H
