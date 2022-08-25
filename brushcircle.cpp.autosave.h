#include "brushcircle.h"

void BrushCircle::initialize(QColor color)
{
    img = QImage(QSize(size,size),QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter(&img);
    
    painter.drawEllipse(QRect(0,0,size,size));
}

void BrushCircle::initialize(const Qimage& color)
{
    img = QImage(QSize(size,size),QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter(&img);

    painter.drawEllipse(QRect(0,0,size,size));
}

BrushCircle::BrushCircle(int size, QColor solidColor)
    : BrushShape(size)
{
    initialize(solidColor);
}
