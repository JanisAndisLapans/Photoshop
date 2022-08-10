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
}

#endif // IMAGEALGORITHMS_H
