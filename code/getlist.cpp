#include "getlist.h"
#include <QApplication>
#include <QDesktopServices>
#include <QStringList>
#include <QDesktopWidget>
#include <QSystemInfo>

GetList::GetList(QObject *parent) :
    QObject(parent)
{
}

GetList::GetList(Language *__language, QObject *parent) :
    QObject(parent)
{
    _language = __language;
    n_manager = new QNetworkAccessManager(this);

    appListVersion = QApplication::instance()->applicationVersion();

    downloadProgress = 0;
    connect(&downloader, SIGNAL(progressChanged(int)),
            this, SLOT(progressChangedReceived(int)));
    connect(&downloader, SIGNAL(downloadDone()),
            this, SLOT(downloadDoneReceived()));
    connect(&downloader, SIGNAL(downloadError(int)),
            this, SLOT(downloadErrorReceived(int)));

    os=""; resolution="";

    downloadFromStoreStarted = false;
    activateCount = 0;

    QtMobility::QSystemInfo si;
    os = si.version(QtMobility::QSystemInfo::Os);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    int width = desktopWidget->screenGeometry().width();
    qDebug() << width;
    if (width == 480) resolution = "1"; //E6
    else {
        width = desktopWidget->screenGeometry().height();
        if (width == 480) resolution = "1"; //E6 landscape
        else resolution = "0";
    }

    updateManagement = new UpdateManagement(false);
    connect(updateManagement, SIGNAL(loadingFinished(QStringList)),
            this, SLOT(updateDataUpdated(QStringList)));
    updateManagement->loadList();

    QSettings settings("schumi1331", "AppList");
    updateIds = settings.value("apps/updateIds", updateIds).toStringList();

    useUnsigned = settings.value("apps/unsigned", false).toBool();

    struct ListEntry {
        enum ListRoles {
            IdRole = Qt::UserRole + 1,
            NameRole = Qt::UserRole + 2,
            UidRole = Qt::UserRole + 3,
            UidStoreRole = Qt::UserRole + 4,
            UidUnsignedRole = Qt::UserRole + 5,
            IconRole = Qt::UserRole + 6,
            VersionRole = Qt::UserRole + 7,
            VersionStoreRole = Qt::UserRole + 8,
            VersionUnsignedRole = Qt::UserRole + 9,
            VersionDateRole = Qt::UserRole + 10,
            VersionDateStoreRole = Qt::UserRole + 11,
            VersionDateUnsignedRole = Qt::UserRole + 12,
            InstalledUidRole = Qt::UserRole + 13,
            CategoryRole = Qt::UserRole + 14,
            LanguageRole = Qt::UserRole + 15,
            OsRole = Qt::UserRole + 16,
            DeveloperRole = Qt::UserRole + 17,
            MailRole = Qt::UserRole + 18,
            WebsiteRole = Qt::UserRole + 19,
            TwitterRole = Qt::UserRole + 20,
            FacebookRole = Qt::UserRole + 21,
            DonationRole = Qt::UserRole + 22,
            AASRole = Qt::UserRole + 23,
            PriceRole = Qt::UserRole + 24,
            DescriptionRole = Qt::UserRole + 25,
            Image1Role = Qt::UserRole + 26,
            Image2Role = Qt::UserRole + 27,
            Image3Role = Qt::UserRole + 28,
            TagsRole = Qt::UserRole + 29,
            ChangelogRole = Qt::UserRole + 30,
            UnsignedNoteRole = Qt::UserRole + 31,
            DownloadRole = Qt::UserRole + 32,
            DownloadStoreRole = Qt::UserRole + 33,
            DownloadUnsignedRole = Qt::UserRole + 34,
            UpdateRole = Qt::UserRole + 35,
            Image4Role = Qt::UserRole + 36,
            Image5Role = Qt::UserRole + 37,
            DownloadSizeRole = Qt::UserRole + 38,
            DownloadUnsignedSizeRole = Qt::UserRole + 39
        };
    };
    QHash<int, QByteArray> roleNames;
    roleNames[ListEntry::IdRole] = "id";
    roleNames[ListEntry::NameRole] =  "name";
    roleNames[ListEntry::UidRole] = "uid";
    roleNames[ListEntry::UidStoreRole] = "uidstore";
    roleNames[ListEntry::UidUnsignedRole] = "uidunsigned";
    roleNames[ListEntry::IconRole] = "icon";
    roleNames[ListEntry::VersionRole] = "version";
    roleNames[ListEntry::VersionStoreRole] = "versionstore";
    roleNames[ListEntry::VersionUnsignedRole] = "versionunsigned";
    roleNames[ListEntry::VersionDateRole] = "versiondate";
    roleNames[ListEntry::VersionDateStoreRole] = "versiondatestore";
    roleNames[ListEntry::VersionDateUnsignedRole] = "versiondateunsigned";
    roleNames[ListEntry::InstalledUidRole] = "installed";
    roleNames[ListEntry::CategoryRole] = "category";
    roleNames[ListEntry::LanguageRole] = "language";
    roleNames[ListEntry::OsRole] = "os";
    roleNames[ListEntry::DeveloperRole] = "developer";
    roleNames[ListEntry::MailRole] = "mail";
    roleNames[ListEntry::WebsiteRole] = "website";
    roleNames[ListEntry::TwitterRole] = "twitter";
    roleNames[ListEntry::FacebookRole] = "facebook";
    roleNames[ListEntry::DonationRole] = "donation";
    roleNames[ListEntry::AASRole] = "aas";
    roleNames[ListEntry::PriceRole] = "price";
    roleNames[ListEntry::DescriptionRole] = "description";
    roleNames[ListEntry::Image1Role] = "image1";
    roleNames[ListEntry::Image2Role] = "image2";
    roleNames[ListEntry::Image3Role] = "image3";
    roleNames[ListEntry::TagsRole] = "tags";
    roleNames[ListEntry::ChangelogRole] = "changelog";
    roleNames[ListEntry::UnsignedNoteRole] = "unsignednote";
    roleNames[ListEntry::DownloadRole] = "download";
    roleNames[ListEntry::DownloadStoreRole] = "downloadstore";
    roleNames[ListEntry::DownloadUnsignedRole] = "downloadunsigned";
    roleNames[ListEntry::UpdateRole] = "updateAvailable";
    roleNames[ListEntry::Image4Role] = "image4";
    roleNames[ListEntry::Image5Role] = "image5";
    roleNames[ListEntry::DownloadSizeRole] = "downloadsize";
    roleNames[ListEntry::DownloadUnsignedSizeRole] = "downloadunsignedsize";

    //RoleItemModel *_list[2] = {new RoleItemModel(roleNames), new RoleItemModel(roleNames), new RoleItemModel(roleNames)};
    //list = _list;
    list[0] = new RoleItemModel(roleNames);
    list[1] = new RoleItemModel(roleNames);
    list[2] = new RoleItemModel(roleNames);
    list[3] = new RoleItemModel(roleNames);
    list[4] = new RoleItemModel(roleNames);

    currentList = 0;
    cleanList[0] = false;
    cleanList[1] = false;
    cleanList[2] = false;
    cleanList[3] = false;
    cleanList[4] = false;

    lastSearchTerm = "";
}

void GetList::loadMainList() {
    QString language = _language->languagesForUrl();
    if (language != "") language = "&language="+language;
    QUrl url("http://<YOUR_APPLIST_DOMAIN>/applist.php?latest=true&count=7"+language+"&os="+os+"&resolution="+resolution+"&unsigned="+(useUnsigned ? "true" : "false"));
    cleanList[0] = true;
    loadList(url,0);
}

void GetList::loadCategoryList(int categoryId, int start, bool clean) {
    QString language = _language->languagesForUrl();
    if (language != "") language = "&language="+language;
    QUrl url("http://<YOUR_APPLIST_DOMAIN>/applist.php?category=" + QString::number(categoryId) + "&start=" + QString::number(start) + language+"&os="+os+"&resolution="+resolution+"&unsigned="+(useUnsigned ? "true" : "false"));
    cleanList[1] = clean;
    loadList(url,1);
}

void GetList::loadSearchList(QString searchTerm, int start, bool clean) {
    QString language = _language->languagesForUrl();
    if (language != "") language = "&language="+language;
    QUrl url("http://<YOUR_APPLIST_DOMAIN>/applist.php?search=" + searchTerm + "&start=" + QString::number(start) + language+"&os="+os+"&resolution="+resolution+"&unsigned="+(useUnsigned ? "true" : "false"));
    cleanList[2] = clean;
    lastSearchTerm = searchTerm;
    loadList(url,2);
}

void GetList::loadPersonalList(int start, bool clean) {
    QSettings settings("schumi1331", "AppList");
    QStringList installedIds = settings.value("apps/installedIds").toStringList();
    QString ids = "";
    for (int i=(installedIds.length()-1); i>=0; i--) {
        ids += installedIds.at(i);
        if (i-1>=0) ids += ",";
    }

    QUrl url("http://<YOUR_APPLIST_DOMAIN>/applist.php?id=" + ids + "&start=" + QString::number(start)+"&os="+os+"&resolution="+resolution+"&unsigned="+(useUnsigned ? "true" : "false"));
    cleanList[3] = clean;
    //qDebug() << url.toString();
    loadList(url,3);
}

void GetList::loadUpdateList(int start, bool clean) {
    QSettings settings("schumi1331", "AppList");
    QString ids = "";
    for (int i=0; i<updateIds.length(); i++) {
        ids += updateIds.at(i);
        if (i+1<updateIds.length()) ids += ",";
    }

    QUrl url("http://<YOUR_APPLIST_DOMAIN>/applist.php?id=" + ids + "&start=" + QString::number(start)+"&os="+os+"&resolution="+resolution+"&unsigned="+(useUnsigned ? "true" : "false"));
    cleanList[4] = clean;
    //qDebug() << url.toString();
    loadList(url,4);
}

void GetList::loadUpdateList(int id) {
    QUrl url("http://<YOUR_APPLIST_DOMAIN>/applist.php?id=" + QString::number(id) + "&start=0&os="+os+"&resolution="+resolution+"&unsigned="+(useUnsigned ? "true" : "false"));
    cleanList[4] = true;
    //qDebug() << url.toString();
    loadList(url,4);
}

void GetList::loadList(QUrl url, int listId) {
    //qDebug() << "loading started";
    QNetworkReply *reply = n_manager->get(QNetworkRequest(url));
    reply->setObjectName(QString::number(listId));
    connect(reply, SIGNAL(finished()),
            this, SLOT(listLoaded()));
}

void GetList::listLoaded() {
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
        //qDebug() << html;
        parseList(html, pReply->objectName().toInt());
    }
}

void GetList::parseList(QString html, int listId) {
    if (html.length() > 0) {
        int lastPos = 0;
        QString minVersion = parseItem(html, lastPos, "minversion");

        if (!appManagement.compareVersions(appListVersion, minVersion)) {
            QSettings settings("schumi1331", "AppList");
            int highestSeenId = settings.value("apps/highestSeenId", 0).toInt();
            QStringList installedIds = settings.value("apps/installedIds").toStringList();

            if (cleanList[listId]) {
                list[listId]->clear();
                cleanList[listId] = false;
            }

            while (html.indexOf("<app>", lastPos) != -1) {
                QStandardItem* item = new QStandardItem();

                int id = parseItem(html, lastPos, "id").toInt();
                item->setData(id, Qt::UserRole + 1);
                // ID hasn't been seen, set as highest seen id
                if (id > highestSeenId) highestSeenId = id;

                item->setData(parseItem(html, lastPos, "name"), Qt::UserRole + 2);

                QString uid = parseItem(html, lastPos, "uid");
                item->setData(uid, Qt::UserRole + 3);

                QString uidstore = parseItem(html, lastPos, "uidstore");
                item->setData(uidstore, Qt::UserRole + 4);

                QString uidunsigned = parseItem(html, lastPos, "uidunsigned");
                item->setData(uidunsigned, Qt::UserRole + 5);

                item->setData(parseItem(html, lastPos, "icon"), Qt::UserRole + 6);

                QString version = parseItem(html, lastPos, "version");
                item->setData(version, Qt::UserRole + 7);
                QString versionstore = parseItem(html, lastPos, "versionstore");
                item->setData(versionstore, Qt::UserRole + 8);
                QString versionunsigned = parseItem(html, lastPos, "versionunsigned");
                item->setData(versionunsigned, Qt::UserRole + 9);

                QDateTime versiondate = QDateTime::fromString(parseItem(html, lastPos, "versiondate"), "yyyy-MM-dd hh:mm:ss");
                item->setData(versiondate, Qt::UserRole + 10);
                QDateTime versiondatestore = QDateTime::fromString(parseItem(html, lastPos, "versiondatestore"), "yyyy-MM-dd hh:mm:ss");
                item->setData(versiondatestore, Qt::UserRole + 11);
                QDateTime versiondateunsigned = QDateTime::fromString(parseItem(html, lastPos, "versiondateunsigned"), "yyyy-MM-dd hh:mm:ss");
                item->setData(versiondateunsigned, Qt::UserRole + 12);

                // Save installed UID
                QString installedVersion = appManagement.getVersion(uid);

                if (installedVersion.length() > 0 && uid.length() > 0 && QString::compare(uid, uidunsigned) == 0) {
                    /* Pr�fe ob uid und uidunsigned gleich sind. Wenn ja, muss die unsigned Version neuer sein
                      und die installierte Version geh�rt entweder in den Bereich <= selfsigned oder >= unsigned
                      */
                    if (appManagement.compareVersions(version, installedVersion)) {
                        // Installierte Versionsnummer ist h�her als letzte selfsigned, damit ist unsigned installiert
                        item->setData(uidunsigned+"2", Qt::UserRole + 13);
                    } else {
                        item->setData(uid+"1", Qt::UserRole + 13);
                    }
                }
                else if (installedVersion.length() > 0) item->setData(uid+"1", Qt::UserRole + 13);
                else {
                    installedVersion = appManagement.getVersion(uidunsigned);

                    if (installedVersion.length() > 0) item->setData(uidunsigned+"2", Qt::UserRole + 13);
                    else {
                        installedVersion = appManagement.getVersion(uidstore);
                        if (installedVersion.length() > 0) item->setData(uidstore+"3", Qt::UserRole + 13);
                        else item->setData("0", Qt::UserRole + 13);
                    }
                }

                // If installed add to installedIds-List
                if (installedVersion.length() > 0 && !installedIds.contains(QString::number(id))) {
                    // If not in installedId list check for update, otherwise it will be checked within updatelist
                    if (updateManagement->checkForUpdate(uid, uidstore, (useUnsigned ? uidunsigned : ""), version, versionstore, (useUnsigned ? versionunsigned : ""))) {
                        item->setData("true", Qt::UserRole + 35);
                        updateIds.append(QString::number(id));
                        emit updateCountChanged();
                    } else {
                        item->setData("false", Qt::UserRole + 35);
                    }

                    installedIds.append(QString::number(id));
                } else if (updateIds.contains(QString::number(id))) {
                    item->setData("true", Qt::UserRole + 35);
                } else {
                    item->setData("false", Qt::UserRole + 35);
                }

                item->setData(parseItem(html, lastPos, "category").toInt(), Qt::UserRole + 14);
                item->setData(parseItem(html, lastPos, "language"), Qt::UserRole + 15);
                item->setData(parseItem(html, lastPos, "os"), Qt::UserRole + 16);
                item->setData(parseItem(html, lastPos, "developer"), Qt::UserRole + 17);
                item->setData(parseItem(html, lastPos, "mail"), Qt::UserRole + 18);
                item->setData(parseItem(html, lastPos, "website"), Qt::UserRole + 19);
                item->setData(parseItem(html, lastPos, "twitter"), Qt::UserRole + 20);
                item->setData(parseItem(html, lastPos, "facebook"), Qt::UserRole + 21);
                item->setData(parseItem(html, lastPos, "donation"), Qt::UserRole + 22);
                item->setData(parseItem(html, lastPos, "aas"), Qt::UserRole + 23);
                item->setData(parseItem(html, lastPos, "price").toDouble(), Qt::UserRole + 24);
                item->setData(parseItem(html, lastPos, "description"), Qt::UserRole + 25);
                item->setData(parseItem(html, lastPos, "image1"), Qt::UserRole + 26);
                item->setData(parseItem(html, lastPos, "image2"), Qt::UserRole + 27);
                item->setData(parseItem(html, lastPos, "image3"), Qt::UserRole + 28);
                item->setData(parseItem(html, lastPos, "image4"), Qt::UserRole + 36);
                item->setData(parseItem(html, lastPos, "image5"), Qt::UserRole + 37);
                item->setData(parseItem(html, lastPos, "tags"), Qt::UserRole + 29);
                item->setData(parseItem(html, lastPos, "changelog"), Qt::UserRole + 30);
                item->setData(parseItem(html, lastPos, "unsignednote"), Qt::UserRole + 31);
                item->setData(parseItem(html, lastPos, "download"), Qt::UserRole + 32);
                item->setData(parseItem(html, lastPos, "downloadsize"), Qt::UserRole + 38);
                item->setData(parseItem(html, lastPos, "downloadstore"), Qt::UserRole + 33);
                item->setData(parseItem(html, lastPos, "downloadunsigned"), Qt::UserRole + 34);
                item->setData(parseItem(html, lastPos, "downloadunsignedsize"), Qt::UserRole + 39);


                // Is part of installed list but not installed (anymore) remove
                if (installedVersion.length() == 0 && installedIds.contains(QString::number(id))) {
                    installedIds.removeAll(QString::number(id));
                    // Add to list if not in personal list
                    if (listId != 3) list[listId]->appendRow(item);
                } else {
                    list[listId]->appendRow(item);
                }
            }

            //Save seen Ids
            settings.setValue("apps/highestSeenId", highestSeenId);
            settings.setValue("apps/installedIds", installedIds);

            switch (listId) {
            case 0: emit mainListChanged(); break;
            case 1: emit categoryListChanged(); break;
            case 2: emit searchListChanged(); break;
            case 3: emit personalListChanged(); break;
            case 4: emit updateListChanged(); break;
            }
        } else {
            // Eine neuere Version von AppList wird vorausgesetzt
            qDebug() << "newer version required";
            emit appListUpdate();
        }
    } else {
        // Konnte nicht geladen werden
        emit loadingError("The AppList couldn't be downloaded. Please check your internet connection.");
    }
}

QString GetList::parseItem(QString html, int& lastPos, QString itemName) {
    int begin = html.indexOf("<"+itemName+">", lastPos);
    if (begin != -1) {
        int end = html.indexOf("</"+itemName+">", begin);
        if (end != -1) {
            lastPos = end + 1;
            begin = begin + itemName.length() + 2;
            QString result = html.mid(begin, (end-begin));
            //qDebug() << itemName + ": '" + result + "'";
            return result;
        }
    }
    return "";
}

int GetList::buttonAction(int index) {
    /*  onButtonPressed:
     *  0 - direct download (only one version available)
     *  1 - direct selfsigned update
     *  2 - direct unsigned update
     *  3 - direct Store update
     *  4 - launch app
     *  5 - update multiple options (open SelectVersion)
     *  6 - currently downloading item
     *  7 - download multiple options (open SelectVersion)
     */
    int onButtonPressed = -1;

    // Download/LaunchButton
    currentItem = list[currentList]->getModelData(index);

    // If this item is the downloadingItem, set 6
    if (downloadingItem == currentItem) {
        onButtonPressed = 6;
    } else {
        //Get installed UID
        QString uid = currentItem.value("installed").toString();
        //Get source of installation (0=not installed, 1=selfsigned, 2=unsigned, 3=store)
        int source = uid.mid(uid.length()-1).toInt();

        uid.chop(1);

        QString installedVersion = appManagement.getVersion(uid);

        if (source==0) {
            // Not installed, download
            onButtonPressed = 0;

            if (currentItem.value("download").toString() != "") onButtonPressed++;
            if (currentItem.value("downloadunsigned").toString() != "" && useUnsigned) onButtonPressed++;
            if (currentItem.value("downloadstore").toString() != "" && onButtonPressed == 0) onButtonPressed++;
            if (onButtonPressed > 1) onButtonPressed = 7;
            else onButtonPressed = 0;
        } else {
            int updateCount = 0;
            if (source == 1) {
                if (appManagement.compareVersions(installedVersion, currentItem.value("version").toString())) updateCount++;
                if (useUnsigned) if (appManagement.compareVersions(installedVersion, currentItem.value("versionunsigned").toString())) updateCount++;
            } else if (source == 2) {
                if (appManagement.compareVersions(installedVersion, currentItem.value("versionunsigned").toString())) updateCount++;
            } else if (source == 3) {
                if (appManagement.compareVersions(installedVersion, currentItem.value("version").toString())) updateCount++;
                if (appManagement.compareVersions(installedVersion, currentItem.value("versionunsigned").toString())) updateCount++;
                if (appManagement.compareVersions(installedVersion, currentItem.value("versionstore").toString())) updateCount++;
            }

            if (updateCount == 0) {
                // No update, launch
                onButtonPressed = 4;
            } else if (updateCount == 1) {
                // Direct updates, doesn't require SelectVersion to be displayed
                // source: self-signed, no update for unsigned available => 1 update self-signed to self-signed
                if (source == 1 && (!useUnsigned || !appManagement.compareVersions(installedVersion, currentItem.value("versionunsigned").toString()))) {
                    onButtonPressed = 1;
                } else if (source == 2) {
                    onButtonPressed = 2;
                } else if (source == 3 && !appManagement.compareVersions(installedVersion, currentItem.value("version").toString()) && (!useUnsigned || !appManagement.compareVersions(installedVersion, currentItem.value("versionunsigned").toString()))) {
                    onButtonPressed = 3;
                }
            }

            if (onButtonPressed == -1) {
                // Update selection required (QML -> open SelectVersion)
                onButtonPressed = 5;
                /*if (appManagement.compareVersions(installedVersion, currentItem.value("version").toString())) {
                    // Self-Signed is newer than installed, offer update if unsigned is not installed to selfsigned
                    if (source!=3 || useUnsigned) onButtonPressed = 1;
                    // else start unsigned
                    else onButtonPressed = 4;
                } else if (appManagement.compareVersions(installedVersion, currentItem.value("versionunsigned").toString())) {
                    // Unsigned version is newer than installed, offer update from unsigned or Store to unsigned
                    if (source>1) onButtonPressed = 2;
                    // else start Self-Signed
                    else onButtonPressed = 4;
                } else if (appManagement.compareVersions(installedVersion, currentItem.value("versionstore").toString())) {
                    // Store version is newer than installed, offer update from Store versions to Store
                    if (source == 2) onButtonPressed = 3;
                    // else start other versions
                    else onButtonPressed = 4;
                } else {
                    // No update available, but app installed => launch
                    onButtonPressed = 4;
                }*/
            }
        }
    }

    return onButtonPressed;
}

bool GetList::buttonPressed(int actionId) {

    // Reset downloadProgress
    downloadProgress = 0;
    emit progressChanged();

    QSettings settings("schumi1331", "AppList");

    // Manipulate actionId, if 0; direct download
    if (actionId == 0) {
        if (currentItem.value("download").toString() != "") actionId = 1;
        else if (currentItem.value("downloadunsigned").toString() != "" && useUnsigned) actionId = 2;
        else if (currentItem.value("downloadstore").toString() != "") actionId = 3;
    }

    if (actionId == 1) {
        // Download/update self-signed
        downloader.startDownload(currentItem.value("download").toString(), currentItem.value("downloadsize").toLongLong(), QString::number(currentItem.value("id").toInt()), _language->currentLanguage(), os, resolution);
        downloadingItem = currentItem;
        //debug += "1: " + QString::number(downloadingItem.value("id").toInt());
    } else if (actionId == 2) {
        // Download/update unsigned
        downloader.startDownload(currentItem.value("downloadunsigned").toString(), currentItem.value("downloadunsignedsize").toLongLong(), QString::number(currentItem.value("id").toInt()), _language->currentLanguage(), os, resolution);
        downloadingItem = currentItem;
        //debug += "2: " + QString::number(downloadingItem.value("id").toInt());
    } else if (actionId == 3) {
        // Download/update Store version (open Store)
        downloadingItem = currentItem;
        downloadFromStoreStarted = true;
        activateCount = 0;
        //openInDefaultBrowser(currentItem.value("downloadstore").toString());
        QDesktopServices::openUrl(QUrl(currentItem.value("downloadstore").toString()));
        //debug += "3: " + QString::number(downloadingItem.value("id").toInt());
    } else if (actionId == 4) {
        // Launch app
        QString installedUID = currentItem.value("installed").toString();
        installedUID.chop(1);
        /*bool*/ appManagement.launchApp(installedUID);
    }

    // return if download progress shall be displayed
    if (actionId == 3) return false;
    else return true;
}
