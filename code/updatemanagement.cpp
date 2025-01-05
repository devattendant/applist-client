#include "updatemanagement.h"
#include <QDebug>

UpdateManagement::UpdateManagement(bool _widget, QObject *parent) :
    QObject(parent)
{
    n_manager = new QNetworkAccessManager(this);

    widget = _widget;
    if (widget) {
        settingsKey = "widget/lastUpdateTime";
    } else {
        settingsKey = "personal/lastUpdateTime";
    }

    useUnsigned = false;
}

int UpdateManagement::updateCount() {
    QSettings settings("schumi1331", "AppList");
    return settings.value("apps/updateIds").toStringList().count();
}

void UpdateManagement::loadList() {
    if (lastUpdateMoreThanADayAgo() || !widget) {
        QSettings settings("schumi1331", "AppList");
        QStringList installedIds = settings.value("apps/installedIds").toStringList();
        QString ids = "";
        for (int i=0; i<installedIds.length(); i++) {
            ids += installedIds.at(i);
            if (i+1<installedIds.length()) ids += ",";
        }

        useUnsigned = settings.value("apps/unsigned", false).toBool();

        QUrl url("http://<YOUR_APPLIST_DOMAIN>/applist.php?id=" + ids + "&widget=true&unsigned=" + (useUnsigned ? "true" : "false"));

        qDebug() << url.toString();

        QNetworkReply *reply = n_manager->get(QNetworkRequest(url));
        connect(reply, SIGNAL(finished()),
                this, SLOT(listLoaded()));
    } else {
        if (widget) emit updateWidgetCount(updateCount());
    }
}

void UpdateManagement::listLoaded() {
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());

    //  http://spechard.wordpress.com/2009/07/23/tip-handling-an-http-redirection-with-qnetworkaccessmanager/
    QVariant possibleRedirectUrl = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    // We'll deduct if the redirection is valid in the redirectUrl function
    QUrl urlRedirectedTo = redirectUrl(possibleRedirectUrl.toUrl(), urlRedirectedTo);

    // If the URL is not empty, we're being redirected.
    if(!urlRedirectedTo.isEmpty()) {
        pReply = n_manager->get(QNetworkRequest(urlRedirectedTo));

        connect(pReply, SIGNAL(finished()),
                this, SLOT(listLoaded()));
    } else {
        QByteArray data=pReply->readAll();
        QString html = QString::fromUtf8(data);
        parseList(html);
    }
}

void UpdateManagement::parseList(QString html) {
    qDebug() << html;
    if (html.length() > 0) {
        int lastPos = 0;
        QString minVersion = parseItem(html, lastPos, "minversion");

        if (true) {
            QSettings settings("schumi1331", "AppList");
            int highestSeenId = settings.value("apps/highestSeenId", -1).toInt();
            QStringList updateIds;

            int updateAvailable = 0;
            int highestId = 0;
            int newApps = 0;

            while (html.indexOf("<app>", lastPos) != -1) {
                int id = parseItem(html, lastPos, "id").toInt();
                // ID hasn't been seen
                if (id > highestId) highestId = id;

                QString uid = parseItem(html, lastPos, "uid");
                QString uidstore = parseItem(html, lastPos, "uidstore");
                QString uidunsigned = parseItem(html, lastPos, "uidunsigned");

                QString version = parseItem(html, lastPos, "version");
                QString versionstore = parseItem(html, lastPos, "versionstore");
                QString versionunsigned = parseItem(html, lastPos, "versionunsigned");

                if (checkForUpdate(uid, uidstore, (useUnsigned ? uidunsigned : "update"), version, versionstore, (useUnsigned ? versionunsigned : "update"))) {
                    updateAvailable++;
                    updateIds.append(QString::number(id));
                }
            }

            newApps = highestId - highestSeenId;

            settings.setValue(settingsKey, QDateTime::currentDateTime());
            settings.setValue("apps/updateIds", updateIds);
            if (widget) emit loadingFinished(updateAvailable, newApps);
            else emit loadingFinished(updateIds);
        } else {
            // Eine neuere Version von AppList wird vorausgesetzt
        }
    } else {
        // Konnte nicht geladen werden
    }
}

QString UpdateManagement::parseItem(QString html, int& lastPos, QString itemName) {
    int begin = html.indexOf("<"+itemName+">", lastPos);
    if (begin != -1) {
        int end = html.indexOf("</"+itemName+">", begin);
        if (end != -1) {
            lastPos = end + 1;
            begin = begin + itemName.length() + 2;
            QString result = html.mid(begin, (end-begin));
            //qDebug() << itemName + ": " + result;
            return result;
        }
    }
    return "";
}

bool UpdateManagement::lastUpdateMoreThanADayAgo() {
    QSettings settings("schumi1331", "AppList");
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime lastUpdateTime = settings.value(settingsKey, currentTime).toDateTime();

    if (abs(lastUpdateTime.daysTo(currentTime)) >= 1 || lastUpdateTime == currentTime || !widget) {
        return true;
    } else {
        return false;
    }
}

bool UpdateManagement::checkForUpdate(QString uid, QString uidstore, QString uidunsigned, QString version, QString versionstore, QString versionunsigned) {
    QString installedVersion = "";

    qDebug() << "checkForUpdate: " + uid + " - " + uidunsigned + " - " + versionunsigned;

    if ((installedVersion = appManagement.getVersion(uid)).length() > 0) {
        // Check for selfsigned install if there are updates

        // Check if selfsigned was updated later
        if (!version.isEmpty() && appManagement.compareVersions(installedVersion, version)) return true;
        // Check if unsigned version is newer
        if (!versionunsigned.isEmpty() && appManagement.compareVersions(installedVersion, versionunsigned)) return true;

    }
    if ((installedVersion = appManagement.getVersion(uidunsigned)).length() > 0) {
        // Check for unsigned install if there are updates

        // Check if unsigned version is newer
        if (!versionunsigned.isEmpty() && appManagement.compareVersions(installedVersion, versionunsigned)) return true;
    }
    if ((installedVersion = appManagement.getVersion(uidstore)).length() > 0) {
        // Check for Store version install if there are updates

        // Check if selfsigned was updated later
        if (!version.isEmpty() && appManagement.compareVersions(installedVersion, version)) return true;
        // Check if unsigned version is newer
        if (!versionunsigned.isEmpty() && appManagement.compareVersions(installedVersion, versionunsigned)) return true;
        // Check if Store version is newer
        if (!versionstore.isEmpty() && appManagement.compareVersions(installedVersion, versionstore)) return true;

    }

    return false;
}
