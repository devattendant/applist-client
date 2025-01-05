#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <qmobilityglobal.h>
#include "qvaluespacesubscriber.h"

QTM_BEGIN_NAMESPACE
class QValueSpaceSubscriber;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE

class Subscriber : public QObject
{
    Q_OBJECT
public:
    explicit Subscriber(QObject *parent = 0);
    
signals:
    void appListIdReceived(int appListId);
    
public slots:
    void getValueChanged();

private:
    QValueSpaceSubscriber *qsubscriber;
    
};

#endif // SUBSCRIBER_H
