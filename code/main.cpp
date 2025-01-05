#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include "qmlapplicationviewer.h"

#include "getlist.h"
#include "roleitemmodel.h"
#include "hswidget.h"
#include "news.h"
#include "update.h"
#include "language.h"
#include "subscriber.h"
#include "settings.h"
#include "symbian.h"

#include <QDebug>

#include <fstream>
#include <QTime>

using namespace std;
ofstream logfile;

void SimpleLoggingHandler(QtMsgType type, const char *msg) {
    switch (type) {
        case QtDebugMsg:
            logfile << QTime::currentTime().toString().toAscii().data() << " Debug: " << msg << "\n";
            logfile.flush();
            break;
        case QtCriticalMsg:
            logfile << QTime::currentTime().toString().toAscii().data() << " Critical: " << msg << "\n";
            logfile.flush();
            break;
        case QtWarningMsg:
            logfile << QTime::currentTime().toString().toAscii().data() << " Warning: " << msg << "\n";
            logfile.flush();
            break;
        case QtFatalMsg:
            logfile << QTime::currentTime().toString().toAscii().data() <<  " Fatal: " << msg << "\n";
            abort();
    }
}


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    //QString _appVersion = "1.0.266";
    app->setApplicationVersion(APP_Version);

    //logfile.open("c:\\data\\applistlog.txt", ios::app);
    //qInstallMsgHandler(SimpleLoggingHandler);

    // Manage, if an app has started AppList
    int launchAppId = -1;
    const QStringList list = app->arguments();
    for (int i=0; i<list.length(); i++) {
        if (list.at(i).startsWith("appListId:")) {
            launchAppId = list.at(i).mid(10).toInt();
            break;
        }
    }

    qDebug() << "registerWidgetBefore";
    HSWidget widget;
    widget.registerWidget();
    qDebug() << "registerWidgetAfter";

    AppManagement appManagement;
    if (!appManagement.appsScanned()) appManagement.scanApps();

    News news;
    Update update;
    Language *language = new Language();
    Subscriber subscriber;
    Settings settings;
    GetList getList(language);
    Symbian symbianFunctions;

    app->installEventFilter(&getList);

    QmlApplicationViewer viewer;

    qmlRegisterType<RoleItemModel>("com.applist.RoleItemModel", 1, 0, "RoleItemModel");

    viewer.rootContext()->setContextProperty("getList", &getList);
    viewer.rootContext()->setContextProperty("appManagement", &appManagement);
    viewer.rootContext()->setContextProperty("news", &news);
    viewer.rootContext()->setContextProperty("update", &update);
    viewer.rootContext()->setContextProperty("l", language);
    viewer.rootContext()->setContextProperty("subscriber", &subscriber);
    viewer.rootContext()->setContextProperty("settings", &settings);
    viewer.rootContext()->setContextProperty("symbianFunctions", &symbianFunctions);

    viewer.rootContext()->setContextProperty("launchAppId", launchAppId);

    viewer.setSource(QUrl("qrc:/qml/AppList/MainAppLoader.qml"));
    //viewer.setMainQmlFile(QLatin1String("qml/AppList/MainAppLoader.qml"));
    viewer.showExpanded();

    viewer.raise();

    return app->exec();
}
