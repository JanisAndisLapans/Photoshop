#ifndef BRUSHFADEDCIRCLE_H
#define BRUSHFADEDCIRCLE_H

#include "brushshape.h"

#include <QPainter>
#include <QImage>

class BrushFadedCircle : public BrushShape
{
public:
    BrushFadedCircle(int size, const QColor& solidColor);
    BrushFadedCircle(int size, const QImage& img);
protected:
    virtual void initialize(const QColor& color) override;
        virtual void initialize(const QImage& img) override;
};

#endif // BRUSHFADEDCIRCLE_H
