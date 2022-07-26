#ifndef BRUSHFADEDCIRCLE_H
#define BRUSHFADEDCIRCLE_H

#include "brushshape.h"

#include <QPainter>

class BrushFadedCircle : public BrushShape
{
public:
    BrushFadedCircle(int size, const QColor& solidColor);
protected:
    virtual void initialize(const QColor& color);
};

#endif // BRUSHFADEDCIRCLE_H
