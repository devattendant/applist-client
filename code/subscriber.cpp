#include "subscriber.h"
#include <QDebug>

Subscriber::Subscriber(QObject *parent) :
    QObject(parent)
{
    /* Constructs a QValueSpaceSubscriber with the specified parent that refers to path /mypublisher. */
    qsubscriber = new QValueSpaceSubscriber("/appList",this);

    connect(qsubscriber, SIGNAL(contentsChanged()), this, SLOT(getValueChanged()));
}


void Subscriber::getValueChanged() {
    qDebug() << "subscriber";

    /* Returns a list of sub-paths under the current path */
    QStringList subPaths = qsubscriber->subPaths();

    for (int i = 0; i < subPaths.count(); ++i)
    {
        /*Returns the value of the subPath under this subscriber path */
        QString qstring = qsubscriber->value(subPaths.at(i)).toString();
        if (qstring.startsWith("appListId:")) {
            emit appListIdReceived(qstring.mid(10).toInt());
            break;
        }
    }
}
