#ifndef BRUSHSQUARE_H
#define BRUSHSQUARE_H

#include "brushshape.h"
#include <QColor>
#include <QDebug>

class BrushSquare : public BrushShape
{
public:
    BrushSquare(int size, const QColor& solidColor);
    BrushSquare(int size, const QImage& img);
protected:
    virtual void initialize(const QColor& color) override;
    virtual void initialize(const QImage& img) override;
};

#endif // BRUSHSQUARE_H
