#ifndef BRUSHSQUARE_H
#define BRUSHSQUARE_H

#include "brushshape.h"
#include <QColor>

class BrushSquare : public BrushShape
{
public:
    BrushSquare(int size, QColor solidColor);
protected:
    virtual void initialize(QColor color);
};

#endif // BRUSHSQUARE_H
