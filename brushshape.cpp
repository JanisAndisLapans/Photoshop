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

const QImage* BrushShape::getSrcImg() const
{
    return srcImg;
}

void BrushShape::resize(int newSize)
{
    size = newSize;
    img = img.scaled(size,size);
}

void BrushShape::changeSource(QColor color)
{
    this->sourceColor = color;
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


