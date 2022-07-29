#ifndef BRUSHSQUARE_H
#define BRUSHSQUARE_H

#include "brushshape.h"
#include <QColor>
#include <QDebug>

class BrushSquare : public BrushShape
{
public:
    BrushSquare(int size, const QColor& solidColor);
protected:
    virtual void initialize(const QColor& color);
};

#endif // BRUSHSQUARE_H
