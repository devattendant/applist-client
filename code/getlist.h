#ifndef GETLIST_H
#define GETLIST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QEvent>
#include "roleitemmodel.h"
#include "appmanagement.h"
#include "updatemanagement.h"
#include "language.h"
#include "downloader.h"

class GetList : public QObject
{
    Q_PROPERTY(RoleItemModel *_mainList READ _mainList NOTIFY mainListChanged)
    Q_PROPERTY(RoleItemModel *_categoryList READ _categoryList NOTIFY categoryListChanged)
    Q_PROPERTY(RoleItemModel *_searchList READ _searchList NOTIFY searchListChanged)
    Q_PROPERTY(RoleItemModel *_personalList READ _personalList NOTIFY personalListChanged)
    Q_PROPERTY(RoleItemModel *_updateList READ _updateList NOTIFY updateListChanged)
    Q_PROPERTY(int _updates READ _updates NOTIFY updateCountChanged)
    Q_PROPERTY(int _downloadProgress READ _downloadProgress NOTIFY progressChanged)
    Q_OBJECT

public:
    explicit GetList(QObject *parent = 0);
    explicit GetList(Language *__language, QObject *parent = 0);
    
    Q_INVOKABLE void loadMainList();
    Q_INVOKABLE void loadCategoryList(int categoryId, int start, bool clean);
    Q_INVOKABLE void loadSearchList(QString searchTerm, int start, bool clean);
    Q_INVOKABLE QString getLastSearchTerm() { return lastSearchTerm; }
    Q_INVOKABLE void loadPersonalList(int start, bool clean);
    Q_INVOKABLE void loadUpdateList(int start, bool clean);
    Q_INVOKABLE void loadUpdateList(int id);

    void loadList(QUrl url, int listId);
    void parseList(QString html, int listId);

    Q_INVOKABLE void setCurrentList(int i) { currentList = i; }

    Q_INVOKABLE int buttonAction(int index);
    /* ActionId to do when downloadButton pressed
       1 = Download/update self-signed
       2 = Download/update unsigned, 3 = Download/update Store, 4 = Launch*/
    Q_INVOKABLE bool buttonPressed(int actionId);

    int _downloadProgress() { return downloadProgress; }
    Q_INVOKABLE bool isDownloading() {
        if (downloadingItem == emptyItem) return false;
        else return true;
    }
    Q_INVOKABLE bool cancelDownload() {
        if (downloader.cancelDownload()) {
            downloadingItem = emptyItem;
            return true;
        } else {
            return false;
        }
    }

    bool useUnsigned;
    Q_INVOKABLE bool getUseUnsigned () { return useUnsigned; }
    Q_INVOKABLE void setUseUnsigned (bool newSetting) {
        QSettings settings("schumi1331", "AppList");
        useUnsigned = newSetting;
        settings.setValue("apps/unsigned", newSetting);
    }

    Q_INVOKABLE void setStoreNote (bool newSetting) {
        QSettings settings("schumi1331", "AppList");
        settings.setValue("settings/storeNote", newSetting);
    }
    Q_INVOKABLE bool getStoreNote() {
        // true = display Store download explanation, false = don't display
        QSettings settings("schumi1331", "AppList");
        return settings.value("settings/storeNote", true).toBool();
    }

protected:
    bool eventFilter(QObject *object, QEvent *event) {
        //qDebug() << "eventFilter";
        if (event->type() == QEvent::ApplicationDeactivate) {
            // The application deactivation can be handled here
            qDebug() << event->type();
            return true; // The event is handled
        }

        if (event->type() == QEvent::ApplicationActivate) {
            // The application activation can be handled here
            qDebug() << event->type();
            if (activateCount == 0) {
                // Workaround for being called after using QDesktopServices::openUrl()
                activateCount++;
            } else if (downloadFromStoreStarted) {
                installServerFinishedReceived();
                downloadFromStoreStarted = false;
            }
            return true;
        }
        return QObject::eventFilter(object, event); // Unhandled events are passed to the base class
    }

private:
    Language *_language;
    QNetworkAccessManager* n_manager;

    RoleItemModel *list[5];
    RoleItemModel* _mainList() { return list[0]; }
    RoleItemModel* _categoryList() { return list[1]; }
    RoleItemModel* _searchList() { return list[2]; }
    QString lastSearchTerm;
    RoleItemModel* _personalList() { return list[3]; }
    RoleItemModel* _updateList() { return list[4]; }

    int currentList;
    int listLoading;
    bool cleanList[5];

    AppManagement appManagement;

    UpdateManagement* updateManagement;
    QStringList updateIds;
    int _updates() { return updateIds.count(); }

    Downloader downloader;
    int downloadProgress;
    QString os, resolution;
    bool downloadFromStoreStarted;
    int activateCount;

    QString appListVersion;

    QVariantMap currentItem, downloadingItem, emptyItem;
    QString parseItem(QString html, int& lastPos, QString itemName);

    //  http://spechard.wordpress.com/2009/07/23/tip-handling-an-http-redirection-with-qnetworkaccessmanager/
    QUrl redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const {
        QUrl redirectUrl;
        if(!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != oldRedirectUrl)
            redirectUrl = possibleRedirectUrl;
        return redirectUrl;
    }

    //QString debug;

signals:
    void mainListChanged();
    void searchListChanged();
    void categoryListChanged();
    void personalListChanged();
    void updateListChanged();
    //To display that a new AppList version is required
    void appListUpdate();
    void loadingError(QString error);

    void updateCountChanged();

    void downloadDone();
    void downloadError(int code);
    void installServerFinished(int statusCode);

    void progressChanged();
public slots:
    void listLoaded();
    void progressChangedReceived(int percent) {
        downloadProgress = percent;
        emit progressChanged();
    }
    void downloadDoneReceived() {
        qDebug() << "downloadDone";

        connect(&appManagement, SIGNAL(installServerFinished()),
                this, SLOT(installServerFinishedReceived()));
        appManagement.startInstallServerListening();

        emit downloadDone();
    }
    void downloadErrorReceived(int code) {
        emit downloadError(code);
    }
    void updateDataUpdated(QStringList _updateIds) {
        updateIds = _updateIds;
        emit updateCountChanged();
    }

    void installServerFinishedReceived() {
        // Delete file after installation
        downloader.deleteFile();

        /*debug += "\niSFR: " + QString::number(currentItem.value("id").toInt()) + " - " + QString::number(downloadingItem.value("id").toInt()) + " - " + currentItem.value("uid").toString() + " - " + downloadingItem.value("uid").toString() + (downloadingItem == emptyItem ? " - empty" : "");

        debug += "\n\nself: "+ appManagement.getVersion(downloadingItem.value("uid").toString()) + " - " + downloadingItem.value("version").toString();
        debug += "\nunsigned: "+ appManagement.getVersion(downloadingItem.value("uidunsigned").toString()) + " - " + downloadingItem.value("versionunsigned").toString();
        debug += "\nstore: "+ appManagement.getVersion(downloadingItem.value("uidstore").toString()) + " - " + downloadingItem.value("versionstore").toString();
        debug += "\ninstalled: " + downloadingItem.value("installed").toString();*/

        // If installation was successful
        // Get installed version and check if it matches the equal version and if this version is installed
        if ( (QString::compare(appManagement.getVersion(downloadingItem.value("uid").toString()), downloadingItem.value("version").toString()) == 0 && appManagement.getVersion(downloadingItem.value("uid").toString()).length() > 0 )
         || ( QString::compare(appManagement.getVersion(downloadingItem.value("uidunsigned").toString()), downloadingItem.value("versionunsigned").toString()) == 0 && appManagement.getVersion(downloadingItem.value("uidunsigned").toString()).length() > 0 )
         || ( QString::compare(appManagement.getVersion(downloadingItem.value("uidstore").toString()), downloadingItem.value("versionstore").toString()) == 0 && appManagement.getVersion(downloadingItem.value("uidstore").toString()).length() > 0 )) {

            // Get installed UID
            QString uid = downloadingItem.value("installed").toString();
            // Get source of old installation (0=not installed, 1=selfsigned, 2=unsigned, 3=store)
            int source = uid.mid(uid.length()-1).toInt();

            // Compare old source and new version for every source. If changed update list item(s)
            // 1) App was not installed at all
            if (true) { //(source == 0)
            // 2) Old source was not self-signed, new one is self-signed
        //     || (source != 1 && appManagement.getVersion(downloadingItem.value("uid").toString()) != "")
            // 3) Old source was not selfsigned, new is
        //     || (source != 2 && appManagement.getVersion(downloadingItem.value("uidunsigned").toString()) != "")
            // 4) Old source was not store, new is store
        //     || (source != 3 && appManagement.getVersion(downloadingItem.value("uidstore").toString()) != "")) {
                // Go through all 5 lists
                for (int i=0; i<5; i++) {
                    // Go through all list items until found or all items checked
                    for (int l=0; l<list[i]->rowCount(); l++) {
                        // Find updated/installed item in all lists and update "installed" field
                        if (list[i]->getModelData(l).value("id").toString() == downloadingItem.value("id").toString()) {
                            QStandardItem *tempItem = list[i]->item(l);

                            // Update installed UID data in the item
                            QString uid = downloadingItem.value("uid").toString();
                            QString installedVersion = appManagement.getVersion(uid);
                            if (installedVersion.length() > 0 && uid.length() > 0 && QString::compare(uid, downloadingItem.value("uidunsigned").toString()) == 0) {
                                /* Prüfe ob uid und uidunsigned gleich sind. Wenn ja, muss die unsigned Version neuer sein
                                  und die installierte Version gehört entweder in den Bereich <= selfsigned oder >= unsigned
                                  */
                                if (appManagement.compareVersions(downloadingItem.value("version").toString(), installedVersion)) {
                                    // Installierte Versionsnummer ist höher als letzte selfsigned, damit ist unsigned installiert
                                    tempItem->setData(downloadingItem.value("uidunsigned").toString()+"2", Qt::UserRole + 13);
                                } else {
                                    tempItem->setData(uid+"1", Qt::UserRole + 13);
                                }
                            }
                            else if (installedVersion.length() > 0) tempItem->setData(uid+"1", Qt::UserRole + 13);
                            else {
                                uid = downloadingItem.value("uidunsigned").toString();
                                installedVersion = appManagement.getVersion(uid);

                                if (installedVersion.length() > 0) tempItem->setData(uid+"2", Qt::UserRole + 13);
                                else {
                                    uid = downloadingItem.value("uidstore").toString();
                                    installedVersion = appManagement.getVersion(uid);
                                    if (installedVersion.length() > 0) tempItem->setData(uid+"3", Qt::UserRole + 13);
                                    else tempItem->setData("0", Qt::UserRole + 13);
                                }
                            }

                            // Set update value to false
                            tempItem->setData("false", Qt::UserRole + 35);

                            // Update item
                            list[i]->setItem(l, tempItem);

                            // Update currentItem
                            if (currentItem == downloadingItem) {
                                currentItem = list[i]->getModelData(l);
                                //debug += "tI: " + tempItem->data(Qt::UserRole + 13).toString();
                            }

                            // Notify list changes
                            switch (i) {
                                case 0: mainListChanged(); break;
                                case 1: categoryListChanged(); break;
                                case 2: searchListChanged(); break;
                                case 3: personalListChanged(); break;
                                case 4: updateListChanged(); break;
                            }

                            break;
                        }
                    }
                }
            }

            // Update order in installed IDs list
            QSettings settings("schumi1331", "AppList");
            QStringList installedIds = settings.value("apps/installedIds").toStringList();
            if (installedIds.contains(QString::number(downloadingItem.value("id").toInt()))) installedIds.removeAll(QString::number(downloadingItem.value("id").toInt()));
            installedIds.append(QString::number(downloadingItem.value("id").toInt()));
            settings.setValue("apps/installedIds", installedIds);

            // Remove item from updateIds list
            if (updateIds.contains(downloadingItem.value("id").toString())) {
                updateIds.removeAll(downloadingItem.value("id").toString());
                settings.setValue("apps/updateIds", updateIds);
                emit updateCountChanged();
            }

            downloadingItem = emptyItem;

            emit installServerFinished(0);
        } else {
            // Update order in installed IDs list
            QSettings settings("schumi1331", "AppList");
            QStringList installedIds = settings.value("apps/installedIds").toStringList();
            if (installedIds.contains(QString::number(downloadingItem.value("id").toInt()))) installedIds.removeAll(QString::number(downloadingItem.value("id").toInt()));
            installedIds.append(QString::number(downloadingItem.value("id").toInt()));
            settings.setValue("apps/installedIds", installedIds);

            downloadingItem = emptyItem;

            // Installation failed, return Error
            emit installServerFinished(1);
        }
    }
};

#endif // GETLIST_H
