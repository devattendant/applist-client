#include "symbian.h"

#include <QDebug>

Symbian::Symbian(QObject *parent) :
    QObject(parent)
{
}

/*
Following code is used to open a URL from a Qt app in the default browser that can
be selected in the OS' settings (Settings > Program settings > Default programs >
Internet). By default when using Qt.openUrlExternally(), the Symbian Web Browser
is opened independent if the set browser is Opera, etc.
Maybe this will be fixed by Nokia sometimes but hard to believe (and then it
wouldn't work in older OS versions I think).

Further informations/sources:
http://www.developer.nokia.com/Community/Wiki/Launch_default_web_browser_using_Symbian_C%2B%2B
http://www.developer.nokia.com/Community/Wiki/How_to_Launch_Browser_in_3rd_edition
http://qml-tieba-client.googlecode.com/git-history/e9036669bbf1eda33c2cd7c41a7d29232e66dac5/utility.cpp

This code will not work if the Symbian Web Browser is already opened when trying to
open a page from your app. To solve this, add "SwEvent" capability in your pro
file, see the comments in the bug report:
https://bugreports.qt-project.org/browse/QTBUG-15725?page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel
*/
void Symbian::openUrlInDefaultBrowser(QString url) {
    // Convert QString to TBuf to use it within Symbian C++
    TPtrC myUrl (reinterpret_cast<const TText*>(url.constData()),url.length());

    RApaLsSession lsSession;
    // create a session with apparc server.
    User::LeaveIfError(lsSession.Connect());
    CleanupClosePushL(lsSession);
    TDataType mimeDatatype(_L8("application/x-web-browse"));
    TUid handlerUID;
    // get the default application uid for application/x-web-browse
    lsSession.AppForDataType(mimeDatatype,handlerUID);
    // there may not be a mime-type handler defined, especially on S60 3.x
    // in such case we default to the built-in browser
    if (handlerUID.iUid == 0) {
        const TInt KWmlBrowserUid = 0x10008D39;
        TUid id( TUid::Uid( KWmlBrowserUid ) );
        handlerUID = id;
    }
    // Finally launch default browser
    LaunchBrowserL(myUrl, handlerUID);
    // Cleanup
    CleanupStack::PopAndDestroy(&lsSession);
}
// ----------------------------------------------------
// CBrowserAppUi::LaunchBrowserL(const TDesC& aUrl, TUid& aUid)
// Used for launching the default browser with provided url.
// (modified, see second link)
// ----------------------------------------------------
//
void Symbian::LaunchBrowserL(const TDesC& aUrl, TUid& _id)
{
    TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
    TApaTask task = taskList.FindApp( _id );
    if ( task.Exists() )
    {
        HBufC8* param = HBufC8::NewLC( aUrl.Length() + 2);
                //"4 " is to Start/Continue the browser specifying a URL
        param->Des().Append(_L("4 "));
        param->Des().Append(aUrl);
        task.SendMessage( TUid::Uid( 0 ), *param );// Uid is not used
        CleanupStack::PopAndDestroy(param);
    }
    else
    {
        HBufC16* param = HBufC16::NewLC( aUrl.Length() + 2);
                //"4 " is to Start/Continue the browser specifying a URL
        param->Des().Append(_L("4 "));
        param->Des().Append(aUrl);
        RApaLsSession appArcSession;
                // connect to AppArc server
        User::LeaveIfError(appArcSession.Connect());
        TThreadId id;
        appArcSession.StartDocument( *param, _id, id );
        appArcSession.Close();
        CleanupStack::PopAndDestroy(param);
    }
}
