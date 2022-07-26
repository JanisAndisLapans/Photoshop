#include "brushshape.h"

BrushShape::BrushShape(int size)
    : size(size)
{
}

QImage BrushShape::getImg() const
{
    return img;
}

void BrushShape::resize(int newSize)
{
    size = newSize;
    img = img.scaled(size,size);
}

void BrushShape::changeSource(QColor color)
{
    initialize(color);
}
