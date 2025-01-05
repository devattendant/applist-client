#ifndef REVIEWITEM_H
#define REVIEWITEM_H

#include <QObject>
#include <QDateTime>

#define commonMember(type,x) \
  protected: \
     type m_ ## x;  \
  public: \
     type get ## x () { return m_ ## x;} \
     void set ## x (type a){ m_ ## x = a; emit reviewChanged(); }

class ReviewItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ getid WRITE setid NOTIFY reviewChanged)
    Q_PROPERTY(QString name READ getname WRITE setname NOTIFY reviewChanged)
    Q_PROPERTY(int rating READ getrating WRITE setrating NOTIFY reviewChanged)
    Q_PROPERTY(QString comment READ getcomment WRITE setcomment NOTIFY reviewChanged)
    Q_PROPERTY(QDateTime date READ getdate WRITE setdate NOTIFY reviewChanged)
    Q_PROPERTY(QString device READ getdevice WRITE setdevice NOTIFY reviewChanged)
    Q_PROPERTY(QString version READ getversion WRITE setversion NOTIFY reviewChanged)

    commonMember(int, id)
    commonMember(QString, name)
    commonMember(int, rating)
    commonMember(QString, comment)
    commonMember(QDateTime, date)
    commonMember(QString, device)
    commonMember(QString, version)

public:
    explicit ReviewItem(QObject *parent = 0);
    
signals:
    void reviewChanged();

public slots:
    
};

#endif // REVIEWITEM_H
