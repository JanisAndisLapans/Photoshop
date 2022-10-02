#ifndef COLORPICKERLABEL_H
#define COLORPICKERLABEL_H

#include <utility>

#include <QLabel>
#include <QColor>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPainter>

using namespace std;

class ColorPickerLabel : public QLabel
{
    Q_OBJECT
public:
    ColorPickerLabel(QWidget* parent);
    QColor getColor() const;
    void setColor(const QColor& color);

protected:
    virtual void mousePressEvent(QMouseEvent* ev) override;
    virtual void paintEvent(QPaintEvent* ev) override;

private:
    QColor color = Qt::green;

signals:
    void colorChanged(const QColor& color);
};

#endif // COLORPICKERLABEL_H
