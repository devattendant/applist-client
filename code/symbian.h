#ifndef SYMBIAN_H
#define SYMBIAN_H

#include <QObject>
#include <QSystemInfo>

/*
Includes of Symbian libraries. If you are building for multiple OSes, use
#ifdef Q_OS_SYMBIAN
...
#endif

DON'T FORGET TO ADD LIBS TO THE PRO FILE OF YOUR PROJECT
*/
//General
#include <eikenv.h>
//openUrlInDefaultBrowser
#include <apgcli.h>
#include <apgtask.h>

class Symbian : public QObject
{
    Q_OBJECT
public:
    Symbian(QObject *parent = 0);

    Q_INVOKABLE void openUrlInDefaultBrowser(QString url);

    void LaunchBrowserL(const TDesC& aUrl, TUid& _id);
};

#endif // SYMBIAN_H
