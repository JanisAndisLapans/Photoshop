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

BrushCircle::BrushCircle(int size, QColor solidColor)
    : BrushShape(size, solidColor)
{
    initialize(solidColor);
}
