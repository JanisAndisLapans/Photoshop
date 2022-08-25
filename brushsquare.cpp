#include "brushsquare.h"

void BrushSquare::initialize(const QColor& color)
{
    BrushShape::initialize(color);
    img = QImage(QSize(size,size),QImage::Format_RGB32);
    img.fill(color);
}

void BrushSquare::initialize(const QImage& img)
{
    BrushShape::initialize(img);
    this->img = img.scaled(QSize(size,size),Qt::KeepAspectRatio);
}



BrushSquare::BrushSquare(int size, const QColor& solidColor)
    : BrushShape(size, solidColor)
{
    initialize(solidColor);
}

BrushSquare::BrushSquare(int size, const QImage& img)
    : BrushShape(size, img)
{
    initialize(img);
}
