#ifndef APPITEM_H
#define APPITEM_H

#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <reviewitem.h>

#define commonMember(type,x) \
  protected: \
     type m_ ## x;  \
  public: \
     type get ## x () { return m_ ## x;} \
     void set ## x (type a){ m_ ## x = a; emit appItemChanged(); }

class AppItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ getid WRITE setid NOTIFY appItemChanged)
    Q_PROPERTY(QString name READ getname WRITE setname NOTIFY appItemChanged)

    Q_PROPERTY(QString uid READ getuid WRITE setuid NOTIFY appItemChanged)
    Q_PROPERTY(QString uidStore READ getuidStore WRITE setuidStore NOTIFY appItemChanged)
    Q_PROPERTY(QString uidUnsigned READ getuidUnsigned WRITE setuidUnsigned NOTIFY appItemChanged)
    Q_PROPERTY(QString uidInstalled READ getuidInstalled WRITE setuidInstalled NOTIFY appItemChanged)
    Q_PROPERTY(QString version READ getversion WRITE setversion NOTIFY appItemChanged)
    Q_PROPERTY(QString versionStore READ getversionStore WRITE setversionStore NOTIFY appItemChanged)
    Q_PROPERTY(QString versionUnsigned READ getversionUnsigned WRITE setversionUnsigned NOTIFY appItemChanged)
    Q_PROPERTY(QDateTime versiondate READ getversiondate WRITE setversiondate NOTIFY appItemChanged)
    Q_PROPERTY(QDateTime versiondateStore READ getversiondateStore WRITE setversiondateStore NOTIFY appItemChanged)
    Q_PROPERTY(QDateTime versiondateUnsigned READ getversiondateUnsigned WRITE setversiondateUnsigned NOTIFY appItemChanged)
    Q_PROPERTY(QString download READ getdownload WRITE setdownload NOTIFY appItemChanged)
    Q_PROPERTY(QString downloadStore READ getdownloadStore WRITE setdownloadStore NOTIFY appItemChanged)
    Q_PROPERTY(QString downloadUnsigned READ getdownloadUnsigned WRITE setdownloadUnsigned NOTIFY appItemChanged)
    Q_PROPERTY(QString downloadsize READ getdownloadsize WRITE setdownloadsize NOTIFY appItemChanged)
    Q_PROPERTY(QString downloadsizeUnsigned READ getdownloadsizeUnsigned WRITE setdownloadsizeUnsigned NOTIFY appItemChanged)
    Q_PROPERTY(bool update READ getupdate WRITE setupdate NOTIFY appItemChanged)

    Q_PROPERTY(int category READ getcategory WRITE setcategory NOTIFY appItemChanged)
    Q_PROPERTY(QString icon READ geticon WRITE seticon NOTIFY appItemChanged)
    Q_PROPERTY(QString description READ getdescription WRITE setdescription NOTIFY appItemChanged)
    Q_PROPERTY(QString language READ getlanguage WRITE setlanguage NOTIFY appItemChanged)
    Q_PROPERTY(QString os READ getos WRITE setos NOTIFY appItemChanged)
    Q_PROPERTY(QString developer READ getdeveloper WRITE setdeveloper NOTIFY appItemChanged)
    Q_PROPERTY(QString mail READ getmail WRITE setmail NOTIFY appItemChanged)
    Q_PROPERTY(QString website READ getwebsite WRITE setwebsite NOTIFY appItemChanged)
    Q_PROPERTY(QString twitter READ gettwitter WRITE settwitter NOTIFY appItemChanged)
    Q_PROPERTY(QString facebook READ getfacebook WRITE setfacebook NOTIFY appItemChanged)
    Q_PROPERTY(QString donation READ getdonation WRITE setdonation NOTIFY appItemChanged)
    Q_PROPERTY(QString aas READ getaas WRITE setaas NOTIFY appItemChanged)
    Q_PROPERTY(QString tags READ gettags WRITE settags NOTIFY appItemChanged)
    Q_PROPERTY(QString changelog READ getchangelog WRITE setchangelog NOTIFY appItemChanged)
    Q_PROPERTY(QString price READ getprice WRITE setprice NOTIFY appItemChanged)
    Q_PROPERTY(QString unsignednote READ getunsignednote WRITE setunsignednote NOTIFY appItemChanged)

    Q_PROPERTY(QString image1 READ getimage1 NOTIFY appItemChanged)
    Q_PROPERTY(QString image2 READ getimage2 NOTIFY appItemChanged)
    Q_PROPERTY(QString image3 READ getimage3 NOTIFY appItemChanged)
    Q_PROPERTY(QString image4 READ getimage4 NOTIFY appItemChanged)
    Q_PROPERTY(QString image5 READ getimage5 NOTIFY appItemChanged)

    commonMember(int, id)
    commonMember(QString, name)
    commonMember(QString, uid)
    commonMember(QString, uidStore)
    commonMember(QString, uidUnsigned)
    commonMember(QString, uidInstalled)
    commonMember(QString, version)
    commonMember(QString, versionStore)
    commonMember(QString, versionUnsigned)
    commonMember(QDateTime, versiondate)
    commonMember(QDateTime, versiondateStore)
    commonMember(QDateTime, versiondateUnsigned)
    commonMember(QString, download)
    commonMember(QString, downloadStore)
    commonMember(QString, downloadUnsigned)
    commonMember(QString, downloadsize)
    commonMember(QString, downloadsizeUnsigned)
    commonMember(bool, update)

    commonMember(int, category)
    commonMember(QString, icon)
    commonMember(QString, description)
    commonMember(QString, language)
    commonMember(QString, os)
    commonMember(QString, developer)
    commonMember(QString, mail)
    commonMember(QString, website)
    commonMember(QString, twitter)
    commonMember(QString, facebook)
    commonMember(QString, donation)
    commonMember(QString, aas)
    commonMember(QString, tags)
    commonMember(QString, changelog)
    commonMember(QString, price)
    commonMember(QString, unsignednote)

    commonMember(double, rating)
    commonMember(int, ratingCount)
    commonMember(int, commentCount)

public:
    explicit AppItem(QObject *parent = 0);

    void setImages(QStringList __images);

    void addReview(ReviewItem* __item);
    void clearReviews();

private:
    QStringList _images;
    QString getimage1();
    QString getimage2();
    QString getimage3();
    QString getimage4();
    QString getimage5();

    QList<ReviewItem*> _reviews;

signals:
    void appItemChanged();
    void reviewsChanged();

public slots:
    
};

#endif // APPITEM_H
