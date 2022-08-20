#ifndef IMAGEALGORITHMS_H
#define IMAGEALGORITHMS_H

#include <QColor>

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
        uchar A = 255 - (255-qAlpha(dest))*(255-qAlpha(src))/255,
                R, G, B;
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


        return qRgba(R,G,B,A);
    }

    inline QRgb multiply(QRgb src, QRgb dest)
    {
        uchar R = qRed(src)*qRed(dest)/255;
        uchar G = qGreen(src)*qGreen(dest)/255;
        uchar B = qBlue(src)*qBlue(dest)/255;
        uchar A = qAlpha(src)*qAlpha(dest)/255;
        return qRgba(R,G,B,A);
    }

    inline QRgb screen(QRgb src, QRgb dest)
    {
        uchar R = 255 - (255-qRed(dest)) * (255-qRed(src))/255;
        uchar G = 255 - (255-qGreen(dest)) * (255-qGreen(src))/255;
        uchar B = 255 - (255-qBlue(dest)) * (255-qBlue(src))/255;
        uchar A = 255 - (255-qAlpha(dest)) * (255-qAlpha(src))/255;
        return qRgba(R,G,B,A);
    }
}

#endif // IMAGEALGORITHMS_H
