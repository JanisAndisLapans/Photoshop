#ifndef LAYER_H
#define LAYER_H

#include <QImage>
#include <QPoint>
#include <QRect>
#include <QFile>

class Layer : public QRect
{
public:
    Layer(const QPoint& pos = QPoint(0,0));
    Layer(const QString& path, const QPoint& pos = QPoint(0,0));
    void setPos(const QPoint& pos);
    QPoint getPos() const;
    bool isSelected() const;
    void toggleSelected();
    QString getName() const;
    QImage* getImgRef();
    QImage getImg() const;
private:
    QImage *img;
    bool selected = true;
    QString name;
};

#endif // LAYER_H
