#ifndef IMAGEALGORITHMS_H
#define IMAGEALGORITHMS_H

#include "layer.h"

#include <cmath>
#include <utility>

#include <QColor>
#include <QPoint>

using namespace std;

namespace ImageAlgorithms
{
    inline double colorDifference(QRgb rgb1, QRgb rgb2)
    {
        const auto deltar = abs(qRed(rgb1) - qRed(rgb2));
        const auto deltag = abs(qGreen(rgb1) - qGreen(rgb2));
        const auto deltab = abs(qBlue(rgb1) - qBlue(rgb2));

        if(deltar < 128)
        {
            return sqrt(2*deltar*deltar + 4*deltag*deltag + 3*deltab*deltab);
        }
        else
        {
            return sqrt(3*deltar*deltar + 4*deltag*deltag + 2*deltab*deltab);
        }
    }

    inline QRgb sourceOver(QRgb src, QRgb dest)
    {   
        uchar A = qAlpha(src) + qAlpha(dest)*(255-qAlpha(src))/255;
        uchar R = (qRed(src)*qAlpha(src) + qRed(dest)*(255-qAlpha(src)))/255;
        uchar G = (qGreen(src)*qAlpha(src) + qGreen(dest)*(255-qAlpha(src)))/255;
        uchar B = (qBlue(src)*qAlpha(src) + qBlue(dest)*(255-qAlpha(src)))/255;
        return qRgba(R,G,B,A);
    }

    inline QRgb overlay(QRgb src, QRgb dest)
    {
        uchar R, G, B;
        if(qRed(dest)>127)
            R = (255 - (255-2*(qRed(dest)-127)) * (255-qRed(src)));
        else
            R = ((2*qRed(dest)) * qRed(src))/255;
        if(qGreen(dest)>127)
            G = (255 - (255-2*(qGreen(dest)-127)) * (255-qGreen(src)));
        else
            G = ((2*qGreen(dest)) * qGreen(src))/255;
        if(qBlue(dest)>127)
            B = (255 - (255-2*(qBlue(dest)-127)) * (255-qBlue(src)));
        else
            B = ((2*qBlue(dest)) * qBlue(src))/255;

        return sourceOver(qRgba(R,G,B,qAlpha(src)), dest);
    }

    inline QRgb multiply(QRgb src, QRgb dest)
    {
        uchar R = qRed(src)*qRed(dest)/255;
        uchar G = qGreen(src)*qGreen(dest)/255;
        uchar B = qBlue(src)*qBlue(dest)/255;

        return sourceOver(qRgba(R,G,B,qAlpha(src)), dest);
    }

    inline QRgb screen(QRgb src, QRgb dest)
    {
        uchar R = 255 - (255-qRed(dest)) * (255-qRed(src))/255;
        uchar G = 255 - (255-qGreen(dest)) * (255-qGreen(src))/255;
        uchar B = 255 - (255-qBlue(dest)) * (255-qBlue(src))/255;

        return sourceOver(qRgba(R,G,B,qAlpha(src)), dest);
    }

    inline int pointDistance(const QPoint& p1, const QPoint& p2)
    {
        return sqrt(abs(p1.x()-p2.x())*abs(p1.x()-p2.x())+abs(p1.y()-p2.y())*abs(p1.y()-p2.y()));
    }

    inline qreal degreesToRadians(qreal degs)
    {
        return (degs / 180.0) * (static_cast<double>(M_PI));
    }

    inline qreal radiansToDegrees(qreal rads)
    {
        return rads * 180.0 / static_cast<double>(M_PI);
    }

    inline QPoint rotatePos(QPoint pos, double rotation, const QPoint& center = QPoint(0,0))
    {
        rotation = degreesToRadians(rotation);

        pos -= center;

        auto x = pos.x() * cos(rotation) + pos.y() * sin(rotation);
        pos.setY(pos.x() * -sin(rotation) + pos.y() * cos(rotation));
        pos.setX(x);

        pos += center;

        return pos;
    }

    inline qreal angleBetweenPoints(const QPoint& p1, const QPoint& p2, const QPoint& origin = QPoint(0,0))
    {
//        auto ret =
//             static_cast<double>(QPoint::dotProduct(p1 - origin, p2 - origin)) /
//            (ImageAlgorithms::pointDistance(origin, p1) * ImageAlgorithms::pointDistance(origin, p2));
//        qDebug() <<ret << (-1.0 < ret && ret < 1.0 ? radiansToDegrees(acos(ret)) : radiansToDegrees(acos(round(ret)))) << QPoint::dotProduct(p1 - origin, p2 - origin) << ImageAlgorithms::pointDistance(origin, p1) << ImageAlgorithms::pointDistance(origin, p2);
//        return -1.0 < ret && ret < 1.0 ? radiansToDegrees(acos(ret)) : radiansToDegrees(acos(round(ret)));
        auto pcor1 = p1 - origin, pcor2 = p2 - origin;
        auto det = pcor1.x()*pcor2.y() - pcor1.y()*pcor2.x();
        auto dot = QPoint::dotProduct(pcor1, pcor2);
        return radiansToDegrees(atan2(det, dot));
    }

    inline QPoint mousePointInLayer(const QPoint& p, const Layer& l, double zoom)
    {
        QRect resized(l);
        resized.setSize(resized.size()*zoom);
        auto zoomShift = resized.topLeft()*(zoom-1.0);
        return rotatePos(p-zoomShift, l.getRotationDegrees(), l.center() / 2 * (zoom + 1.0) - zoomShift) / zoom;
    }
}




#endif // IMAGEALGORITHMS_H
