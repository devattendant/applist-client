#include "hswidget.h"
#include <QDebug>
#include <apgcli.h> // for RApaLsSession
#include <apacmdln.h> // for CApaCommandLine
#include <apgtask.h> // bring to foreground
#include <eikenv.h> // bring to foreground

const QString sw_type = "tworows";
const QString sw_name = "AppList";
const QString sw_desc = "With the AppList homescreen widget you will not miss any updates for your app. Visit the AppList help menu to see how it works.";
const QString sw_id = "HSWidgetxAppListSchumi1331";
const QString sw_icon = QString("uid(%1)").arg(APP_UID); //App UID to set the widget icon

const QString sw_image ("image1");
const QString row1 ("text1");
const QString row2 ("text2");

HSWidget::HSWidget(QObject *parent) : QObject(parent) {
    updateManagement = new UpdateManagement(true);
    connect(updateManagement, SIGNAL(loadingFinished(int, int)),
            this, SLOT(dataUpdated(int,int)));
    connect(updateManagement, SIGNAL(updateWidgetCount(int)),
            this, SLOT(dataUpdated(int)));

    qDebug() << "init";

    //appListVersion = QApplication::instance()->applicationVersion();

    widget = QHSWidget::create(sw_type, sw_name, sw_id, sw_desc, sw_icon, this);
    connect(widget, SIGNAL(handleEvent(QHSWidget*, QHSEvent)), this, SLOT(handleEvent(QHSWidget*, QHSEvent) ));
    connect(widget, SIGNAL(handleItemEvent(QHSWidget*, QString, QHSItemEvent)), this, SLOT(handleItemEvent(QHSWidget*, QString, QHSItemEvent)));
}

void HSWidget::registerWidget() {
    qDebug() << "register";
    widget->RegisterWidget();
}

void HSWidget::publishWidget() {
    qDebug() << "loadList";
    //loadList();
    qDebug() << "publish";
    widget->PublishWidget();
}

void HSWidget::removeWidget() {
    qDebug() << "remove";
    QSettings settings("schumi1331", "AppList");
    settings.remove("widget");
    //settings.sync();
    widget->RemoveWidget();
}

void HSWidget::handleEvent( QHSWidget* /*aSender*/, QHSEvent aEvent ) {
    qDebug() << "handleEvent";
    switch(aEvent) {
        case EActivate: {
                qDebug() << "handleActivate";
                widget->SetItem(sw_image, "c:\\AppList.png");
                //updateWidget("c:\\data\\Images\\Pictures\\QtSunny2.JPG", "a", "b");
                //publishWidget();
                updateManagement->loadList();
            }
            break;
        case EResume: {
                qDebug() << "handlePublish";
                //publishWidget();
                updateManagement->loadList();
                qDebug() << "handlePublishAfter";
            }
            break;
        case EDeactivate: {
                qDebug() << "handleDeactivate";
                QSettings settings("schumi1331", "AppList");
                settings.remove("widget/lastUpdateTime");
            }
            break;
        default:
            break;
    }
}

void HSWidget::handleItemEvent( QHSWidget* /*aSender*/, QString aTemplateItemName, QHSItemEvent aEvent) {
    qDebug() << "handleItemEvent";
    if(aTemplateItemName.compare(sw_image)==0) {
        this->bringToFront();
        //updateWidget("c:\\data\\Images\\Pictures\\QtSunny2.JPG", "a", "b");
        //publishWidget();
        //updateManagement->loadList();
    }
}

void HSWidget::bringToFront() {
    /*TApaTask task( CEikonEnv::Static()->WsSession() );
    task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
    task.BringToForeground();*/

    RProcess proc;
    User::LeaveIfError(proc.Create(_L("AppList.exe"),KNullDesC));
    proc.Resume();
    proc.Close();

    /*TApaTaskList taskList(CCoeEnv::Static()->WsSession());
    const TUid aAppUid = TUid::Uid(0xA1331002);
    TApaTask task = taskList.FindApp(aAppUid);
    if (task.Exists()) {
        task.BringToForeground();
    }

    TApaTaskList taskList(CCoeEnv::Static()->WsSession());
    TApaTask task = taskList.FindApp(aAppUid);
    if (task.Exists()) {
        task.BringToForeground();
    } else {
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

            TApaTask task = taskList.FindApp(aAppUid);
            if (task.Exists()) {
                task.BringToForeground();
            }

            CleanupStack::PopAndDestroy(cmdLine);
        }

        CleanupStack::PopAndDestroy(&apaLsSession);
    }*/
}

void HSWidget::updateWidget( QString icon, QString nRow1, QString nRow2 ) {
    widget->SetItem(sw_image, icon);
    widget->SetItem(row1, nRow1);
    widget->SetItem(row2, nRow2);
    publishWidget();
}

void HSWidget::dataUpdated(int updates, int newApps) {
    if (updates == 1) {
        widget->SetItem(row1, "1 update available");
    } else {
        widget->SetItem(row1, QString::number(updates) + " updates available");
    }

    if (newApps == 1) {
        widget->SetItem(row2, "1 new app");
    } else if (newApps >= 0) {
        widget->SetItem(row2, QString::number(newApps) + " new apps");
    } else {
        widget->SetItem(row2, "0 new apps");
    }

    publishWidget();
}

void HSWidget::dataUpdated(int updates) {
    if (updates == 1) {
        widget->SetItem(row1, "1 update available");
    } else {
        widget->SetItem(row1, QString::number(updates) + " updates available");
    }

    widget->SetItem(row2, "0 new apps");

    publishWidget();
}
