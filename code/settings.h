#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    Q_INVOKABLE bool darkTheme();
    Q_INVOKABLE void setDarkTheme(bool newValue);

    Q_INVOKABLE bool aasFeed();
    Q_INVOKABLE void setAasFeed(bool newValue);

    //Q_INVOKABLE bool unsignedDialog();
    //Q_INVOKABLE void setUnsignedDialog(bool newValue);
    
signals:
    
public slots:
    
};

#endif // SETTINGS_H
