#include "brushshape.h"

BrushShape::BrushShape(int size, const QColor& color)
    : size(size), sourceColor(color)
{
}

QImage BrushShape::getImg() const
{
    return img;
}

QColor BrushShape::getColor() const
{
    return sourceColor;
}

bool BrushShape::isColorSrc() const
{
    return isColor;
}

QImage BrushShape::getSrcImg() const
{
    return sourceImg;
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

void BrushShape::initialize(const QColor& color)
{
    isColor = true;
}

int BrushShape::getSize() const
{
    return size;
}


