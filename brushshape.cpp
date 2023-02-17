#include "brushshape.h"

BrushShape::BrushShape(int size, const QColor& color)
    : size(size), sourceColor(color)
{
}

BrushShape::BrushShape(int size, const QImage& img)
    : size(size)
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

void BrushShape::resize(int newSize)
{
    size = newSize;
    if(!isColor) img = img.scaled(size,size,Qt::KeepAspectRatio);
    else initialize(sourceColor);
}

void BrushShape::changeSource(QColor color)
{
    this->sourceColor = color;
    initialize(color);
}

void BrushShape::changeSource(const QImage& img)
{
    initialize(img);
}

void BrushShape::initialize(const QColor& color)
{
    isColor = true;
}

void BrushShape::initialize(const QImage& color)
{
    isColor = false;
}

int BrushShape::getSize() const
{
    return size;
}


