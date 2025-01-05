#include "appmanagement.h"

AppManagement::AppManagement(QObject *parent) :
    QObject(parent)
{
}

QString AppManagement::getVersion(QString uid) {
    QString version = "";

    bool ok = false;
    uint uidInt = uid.toUInt(&ok, 16);
    Swi::RSisRegistrySession iSisRegSession;

    if (KErrNone != iSisRegSession.Connect() ) {
    }
    CleanupClosePushL( iSisRegSession );

    //Add here you application UID to access your installed application PKG file.
    const TUid packageId = TUid::Uid((TInt)uidInt);
    Swi::RSisRegistryEntry packageEntry;

    if( KErrNone == packageEntry.Open( iSisRegSession, packageId ) ) {
        CleanupClosePushL( packageEntry );

        int major = packageEntry.VersionL().iMajor;
        int minor = packageEntry.VersionL().iMinor;
        int build = packageEntry.VersionL().iBuild;

        version = QString::number(major) + "." + QString::number(minor) + "." + QString::number(build);
        // Add here your application version numbers as in your project PKG file.
        //version = packageEntry.VersionL().iMajor; // + "." + packageEntry.VersionL().iMinor + std::string(".") + packageEntry.VersionL().iBuild;

        CleanupStack::PopAndDestroy( &packageEntry );
    }

    CleanupStack::PopAndDestroy( &iSisRegSession );

    return version;
}

bool AppManagement::compareVersions(QString appVersion, QString webVersion) {
    //appVersion -> Installiert, webVersion -> In Liste die neueste
    //Ergebnis ist true, wenn webVersion neuer
    if ((appVersion != webVersion) && (appVersion.length() > 0) && (webVersion.length() > 0)) {
        //Versionen sind nicht identisch, prüfen, ob neuer
        int webVersion1, webVersion2, webVersion3;
        webVersion1 = webVersion.mid(0,webVersion.indexOf(".", 0)).toInt();
        webVersion2 = webVersion.mid(webVersion.indexOf(".", 0)+1,webVersion.indexOf(".", webVersion.indexOf(".", 0)+1)-(webVersion.indexOf(".", 0)+1)).toInt();
        webVersion3 = webVersion.mid(webVersion.indexOf(".", webVersion.indexOf(".", 0)+1)+1,webVersion.length()-webVersion.indexOf(".", webVersion.indexOf(".", 0)+1)+1).toInt();

        int appVersion1, appVersion2, appVersion3;
        appVersion1 = appVersion.mid(0,appVersion.indexOf(".", 0)).toInt();
        appVersion2 = appVersion.mid(appVersion.indexOf(".", 0)+1,appVersion.indexOf(".", appVersion.indexOf(".", 0)+1)-(appVersion.indexOf(".", 0)+1)).toInt();
        appVersion3 = appVersion.mid(appVersion.indexOf(".", appVersion.indexOf(".", 0)+1)+1,appVersion.length()-appVersion.indexOf(".", appVersion.indexOf(".", 0)+1)+1).toInt();

        if (appVersion1 < webVersion1) {
            //Hauptversionsnummer ist neuer
            return true;
        } else if (appVersion1 > webVersion1) {
            //Hauptversionsnummer ist älter
            return false;
        } else {
            if (appVersion2 < webVersion2) {
                //Subversionsnummer ist neuer
                return true;
            } else if (appVersion2 > webVersion2) {
                //Subversionsnummer ist älter
                return false;
            } else {
                if (appVersion3 < webVersion3) {
                    //Build-Nummer ist neuer
                    return true;
                } else {
                    //Build-Nummer ist älter
                    return false;
                }
            }
        }
    } else {
        //Versionen sind identisch, Ergebnis false
        return false;
    }
}

bool AppManagement::launchApp(QString uid) {
    bool ok = false;
    uint uidInt = uid.toUInt(&ok, 16);
    const TUid aAppUid = TUid::Uid((TInt)uidInt);

    RApaLsSession apaLsSession;
    User::LeaveIfError(apaLsSession.Connect());
    CleanupClosePushL(apaLsSession);

    TApaAppInfo appInfo;
    TInt retVal = apaLsSession.GetAppInfo(appInfo, aAppUid);

    if(retVal == KErrNone) {
        CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
        cmdLine->SetExecutableNameL(appInfo.iFullName);
        cmdLine->SetCommandL(EApaCommandRun);
        User::LeaveIfError( apaLsSession.StartApp(*cmdLine) );

        CleanupStack::PopAndDestroy(cmdLine);

        ok = true;
    } else {
        // The application not found!
        ok = false;
    }

    CleanupStack::PopAndDestroy(&apaLsSession);

    return ok;
}

bool AppManagement::checkAppInstalled(QString uid) {
    qDebug() << "checkAppInstalled";

    if (uid.length() == 0) return false;

    bool ok = false;
    uint uidInt = uid.toUInt(&ok, 16);
    const TUid aAppUid = TUid::Uid((TInt)uidInt);

    RApaLsSession apaLsSession;
    User::LeaveIfError(apaLsSession.Connect());
    CleanupClosePushL(apaLsSession);

    TApaAppInfo appInfo;
    TInt retVal = apaLsSession.GetAppInfo(appInfo, aAppUid);

    CleanupStack::PopAndDestroy(&apaLsSession);

    if(retVal == KErrNone) {
        return true;
    } else {
        return false;
    }
}

bool AppManagement::checkAppInstalled(QString installed, bool withSource) {
    if (withSource) {
        installed.chop(1);
        return checkAppInstalled(installed);
    }
}

void AppManagement::startInstallServerListening() {
    qDebug() << "listening started";
    connect(&installServerThread, SIGNAL(finished()),
            this, SLOT(receiveThreadSignal()));
    installServerThread.start();
}


void AppManagement::openInstaller(QString path) {
    path.replace("/", "\\");
    TPtrC exe(reinterpret_cast<const TText*>(path.data()));

    RApaLsSession apaLsSession;
    CleanupClosePushL(apaLsSession );
    User::LeaveIfError(apaLsSession.Connect());
    TThreadId threadId;
    apaLsSession.StartDocument(exe, threadId);
    CleanupStack::PopAndDestroy(&apaLsSession );
}


// App scanning
bool AppManagement::appsScanned() {
    QSettings settings("schumi1331", "AppList");
    return settings.value("settings/appsScanned", false).toBool();
}

void AppManagement::scanApps() {
    QStringList apps = applications();

    QByteArray liste;
    QString filePath = ":/apps.txt";
    if (QFile::exists(filePath)) {
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
        liste = file.readAll();
    }

    QStringList installed;
    QSettings settings("schumi1331", "AppList");
    if (settings.contains("apps/installedIds")) installed = settings.value("apps/installedIds").toStringList();

    for (int i=0; i<apps.count(); i++) {
        int indexOf = liste.indexOf(apps.at(i));
        if (indexOf != -1) {
            indexOf = liste.indexOf("<id>", indexOf)+4;

            QString id = liste.mid(indexOf, (liste.indexOf("</id>", indexOf)-indexOf));
            if (!installed.contains(id)) installed.append(id);
        }
    }

    settings.setValue("apps/installedIds", installed);
    settings.setValue("settings/appsScanned", true);
}

QStringList AppManagement::applications() const
{
    RApaLsSession lsSession;
    QStringList applications;

    // Connect to application architecture server
    User::LeaveIfError(lsSession.Connect());
    CleanupClosePushL(lsSession);

    TApaAppInfo appInfo;
    lsSession.GetAllApps();

    while (lsSession.GetNextApp(appInfo) == KErrNone) {
        TApaAppCapabilityBuf capability;
        User::LeaveIfError(lsSession.GetAppCapability(capability,
            appInfo.iUid));
        if (appInfo.iCaption.Length() > 0 && !capability().iAppIsHidden) {
            applications.append("0x"+QString::number((TUint)appInfo.iUid.iUid, 16).toUpper());
        }
    }
    CleanupStack::PopAndDestroy(&lsSession);

    return applications;
}
