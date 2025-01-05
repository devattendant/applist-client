#ifndef INSTALLSERVERTHREAD_H
#define INSTALLSERVERTHREAD_H

#include <QThread>
#include <apacmdln.h>
#include <apgcli.h>
#include <apgtask.h>
#include <e32std.h>
#include <w32std.h>

class InstallServerThread : public QThread
{
    Q_OBJECT
public:
    InstallServerThread();

    static void sleep(unsigned long ms) { QThread::msleep(ms); }
protected:
    void run();
};

#endif // INSTALLSERVERTHREAD_H
