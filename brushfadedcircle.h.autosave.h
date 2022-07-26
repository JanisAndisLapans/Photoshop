#ifndef BRUSHFADEDCIRCLE_H
#define BRUSHFADEDCIRCLE_H

#include "brushshape.h"

#include <QPainter>

class BrushFadedCircle : BrushShape
{
public:
    BrushFadedCircle(int size, QColor solidColor);
protected:
    virtual void initialize(QColor color);    
};

#endif // BRUSHFADEDCIRCLE_H
