#ifndef HSWIDGET_H
#define HSWIDGET_H

#include <QObject>
#include "qhswidget.h"
#include "updatemanagement.h"

class HSWidget : public QObject
{
    Q_OBJECT
public:
    explicit HSWidget(QObject *parent = 0);
    Q_INVOKABLE void registerWidget();
    Q_INVOKABLE void publishWidget();
    Q_INVOKABLE void removeWidget();
    Q_INVOKABLE void updateWidget( QString icon, QString nRow1, QString nRow2 );
    void bringToFront();

public slots:
    void handleEvent(QHSWidget*, QHSEvent aEvent );
    void handleItemEvent(QHSWidget*, QString aTemplateItemName,
                         QHSItemEvent aEvent);

    void dataUpdated(int updates, int newApps);
    void dataUpdated(int updates);
private:
    QHSWidget* widget;

    UpdateManagement* updateManagement;

};

#endif // HSWIDGET_H
