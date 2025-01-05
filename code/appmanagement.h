#ifndef APPMANAGEMENT_H
#define APPMANAGEMENT_H

#include <QObject>
#include <installserverthread.h>
#include <sisregistrysession.h>
#include <sisregistryentry.h>
#include <QDebug>
#include <QSettings>

#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include <apgcli.h> // for RApaLsSession
#include <apacmdln.h> // for CApaCommandLine

class AppManagement : public QObject
{
    Q_OBJECT
public:
    explicit AppManagement(QObject *parent = 0);
    
    QString getVersion(QString uid);
    bool compareVersions(QString appVersion, QString webVersion);

    bool launchApp(QString uid);
    Q_INVOKABLE bool checkAppInstalled(QString uid);
    Q_INVOKABLE bool checkAppInstalled(QString installed, bool withSource);
    Q_INVOKABLE void startInstallServerListening();

    bool appsScanned();
    void scanApps();

    static void openInstaller(QString path);
signals:
    void installServerFinished();
public slots:
    void receiveThreadSignal() {
        qDebug() << "receiveThreadSignal";
        emit installServerFinished();
    }

private:
    InstallServerThread installServerThread;

    // Get all installed UIDs
    QStringList applications() const;
};

#endif // APPMANAGEMENT_H
