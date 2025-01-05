#include "installserverthread.h"
#include <QDebug>

InstallServerThread::InstallServerThread()
{
}

void InstallServerThread::run() {
    bool situationFlag = true;

    qDebug() << "before sleep";

    sleep(10000);

    qDebug() << "after sleep";

    //make sure that the name of executable is without extension. i.e helloworld not helloworld.exe
    TFileName executableName;
    executableName.Copy( _L("installserver") );
    executableName.Trim();
    executableName.Append(_L("*"));

    while (situationFlag) {

        //QString data = "installserver";
        //TPtrC8 aExeName(reinterpret_cast<const TUint8*>(data.toUtf8().constData()));

        qDebug() << "situationFlag: " + situationFlag;

        TFindProcess processSearch;
        TFullName processFullName;

        bool found = false;
        while( processSearch.Next( processFullName ) == KErrNone) {
            if( processFullName.Match(executableName) != KErrNotFound ) {
                situationFlag = true;
                found = true;
                break;
            }
        }
        if (!found) situationFlag = false;

        if (situationFlag) sleep(1000);
    }
}
