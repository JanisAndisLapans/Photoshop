#ifndef BRUSHCIRCLE_H
#define BRUSHCIRCLE_H

#include <QPainter>
#include <QDebug>

#include "brushshape.h"

class BrushCircle : public BrushShape
{
public:
    BrushCircle(int size, const QColor& solidColor);
    BrushCircle(int size, const QImage& img);
protected:
    virtual void initialize(const QColor& color) override;
    virtual void initialize(const QImage& img) override;
};

#endif // BRUSHCIRCLE_H
