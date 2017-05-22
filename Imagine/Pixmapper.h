#pragma once
#include "qobject.h"
#include "qimage.h"
#include "qpixmap.h"
#include "qpen.h"

class Pixmapper : public QObject {
    Q_OBJECT
public:
    Pixmapper(QObject *parent = 0);
    ~Pixmapper();
private:
    // checked in handleImg to see if we're ready for another img
    QPen pen;
public slots:
    // null execution if an img is already being processed
    void handleImg(const QByteArray &ba, const int imageW, const int imageH,
        const double scaleFactor, const double dAreaSize,
        const int dLeft, const int dTop, const int dWidth, const int dHeight,
        const int xDown, const int xCur, const int yDown, const int yCur,
        const int minPixVal, const int maxPixVal, bool colorizeSat);
    void handleColorImg(const QByteArray &ba1, const QByteArray &ba2,
        QColor color1, QColor color2, int alpha, const int imageW, const int imageH,
        const double scaleFactor1, const double scaleFactor2, const double dAreaSize,
        const int dLeft, const int dTop, const int dWidth, const int dHeight,
        const int xDown, const int xCur, const int yDown, const int yCur,
        const int minPixVal1, const int maxPixVal1, const int minPixVal2,
        const int maxPixVal2, bool colorizeSat);
signals:
    void pixmapReady(const QPixmap &pxmp, const QImage &img);
    void pixmapColorReady(const QPixmap &pxmp, const QImage &img);
};
