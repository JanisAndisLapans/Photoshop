#ifndef LAYER_H
#define LAYER_H

#include <QImage>
#include <QPoint>
#include <QRect>
#include <QFile>
#include <QDebug>

class Layer : public QRect
{

public:
    Layer(const Layer& l);
    Layer(Layer&& l);
    Layer& operator=(const Layer& l);
    Layer& operator=(Layer&& l);
    Layer(const QPoint& pos = QPoint(0,0));
    Layer(const QString& path, const QPoint& pos = QPoint(0,0));
    Layer(const QImage& img, const QString& name, const QPoint& pos = QPoint(0,0));
    void setPos(const QPoint& pos);
    QPoint getPos() const;
    QString getName() const;
    QImage* getImgRef();
    QImage getImg() const;
    void resize(uint w, uint height);
    bool isTransforming() const;
    void setTransforming(bool state);
    bool isSelected() const;
    void setSelected(bool state);
    QWidget* getTransformToolMenu();
    qreal getRotationDegrees() const;
    void setRotationDegress(qreal degrees);
    void setResizingState(bool state);
    double sizePercentageW() const;
    double sizePercentageH() const;

private:
    QImage  *img;
    QString name;
    bool transforming = false;
    bool selected = false;
    qreal rotationDegrees = 0;
    bool isResizing = false;
    QSize origSize;

};

#endif // LAYER_H
