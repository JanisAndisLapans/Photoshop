#include "brushsquare.h"

void BrushSquare::initialize(const QColor& color)
{
    BrushShape::initialize(color);
    img = QImage(QSize(size,size),QImage::Format_RGB16);
    img.fill(color);
}

BrushSquare::BrushSquare(int size, const QColor& solidColor)
    : BrushShape(size, solidColor)
{
    initialize(solidColor);
}
