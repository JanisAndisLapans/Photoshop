#ifndef BRUSHCIRCLE_H
#define BRUSHCIRCLE_H

#include <QPainter>

#include "brushshape.h"

class BrushCircle : public BrushShape
{
public:
    BrushCircle(int size, QColor solidColor);
protected:
    virtual void initialize(QColor color);    
};

#endif // BRUSHCIRCLE_H
