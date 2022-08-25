#include "brushfadedcircle.h"

void BrushFadedCircle::initialize(const QColor& color)
{
    BrushShape::initialize(color);
    img = QImage(QSize(size,size),QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter(&img);
    painter.setPen(Qt::transparent);
    painter.setBrush(color);

    auto center = QPoint(size/2,size/2);

    painter.setOpacity(0.25);
    painter.drawEllipse(center, size/2, size/2);
    painter.setOpacity(0.6);
    painter.drawEllipse(center, size/4, size/4);

}

void BrushFadedCircle::initialize(const QImage& img)
{
    BrushShape::initialize(img);
    this->img = QImage(QSize(size,size),QImage::Format_ARGB32);
    this->img.fill(Qt::transparent);
    QPainter painter(&this->img);
    painter.setPen(Qt::transparent);
    painter.setBrush(QBrush(img));

    auto center = QPoint(size/2,size/2);

    painter.setOpacity(0.25);
    painter.drawEllipse(center, size/2, size/2);
    painter.setOpacity(0.6);
    painter.drawEllipse(center, size/4, size/4);

}

BrushFadedCircle::BrushFadedCircle(int size, const QColor& solidColor)
    : BrushShape(size, solidColor)
{
    initialize(solidColor);
}

BrushFadedCircle::BrushFadedCircle(int size, const QImage& img)
    : BrushShape(size, img)
{
    initialize(img);
}
