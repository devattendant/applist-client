#include "appitem.h"

AppItem::AppItem(QObject *parent) :
    QObject(parent)
{
}

void AppItem::setImages(QStringList __images) {
    _images = __images;
    emit appItemChanged();
}
QString AppItem::getimage1() { return _images.count() > 0 ? _images.at(0) : ""; }
QString AppItem::getimage2() { return _images.count() > 1 ? _images.at(1) : ""; }
QString AppItem::getimage3() { return _images.count() > 2 ? _images.at(2) : ""; }
QString AppItem::getimage4() { return _images.count() > 3 ? _images.at(3) : ""; }
QString AppItem::getimage5() { return _images.count() > 4 ? _images.at(4) : ""; }

void AppItem::addReview(ReviewItem *__item) {
    for (int i=0; i<_reviews.count(); i++) {
        if (_reviews.at(i)->getdate().msecsTo(__item->getdate()) < 0 || (i+1 == _reviews.count())) {
            _reviews.insert(i, __item);
            break;
        }
    }
    emit reviewsChanged();
}
void AppItem::clearReviews() {
    _reviews.clear();
    emit reviewsChanged();
}
