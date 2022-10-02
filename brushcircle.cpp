#include "brushcircle.h"

void BrushCircle::initialize(const QColor& color)
{
    BrushShape::initialize(color);
    img = QImage(QSize(size,size),QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter(&img);
    painter.setPen(Qt::transparent);
    painter.setBrush(color);
    painter.drawEllipse(QRect(0,0,size,size));
}

void BrushCircle::initialize(const QImage& img)
{
    BrushShape::initialize(img);
    this->img = QImage(QSize(size,size),QImage::Format_ARGB32);
    this->img.fill(Qt::transparent);
    QPainter painter(&this->img);
    painter.setPen(Qt::transparent);
    painter.setBrush(QBrush(img));
    painter.drawEllipse(0,0,size,size);
}

BrushCircle::BrushCircle(int size, const QColor& solidColor)
    : BrushShape(size, solidColor)
{
    initialize(solidColor);
}

BrushCircle::BrushCircle(int size, const QImage& img)
    : BrushShape(size, img)
{
    initialize(img);
}
