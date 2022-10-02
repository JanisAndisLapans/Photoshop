#include "colorpickerlabel.h"

ColorPickerLabel::ColorPickerLabel(QWidget* parent)
    : QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}


void ColorPickerLabel::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    painter.setPen(Qt::transparent);
    painter.fillRect(rect(), color);
    auto shortestSide = min(height(), width());
    painter.setPen(QPen(Qt::black, min(shortestSide/3, 14))); //border
    painter.drawRect(rect());
}


QColor ColorPickerLabel::getColor() const
{
    return color;
}

void ColorPickerLabel::setColor(const QColor& color)
{
    this->color = color;
    update();
}

void ColorPickerLabel::mousePressEvent(QMouseEvent* ev)
{
    setColor(QColorDialog::getColor(color, this));
    emit colorChanged(color);
}

