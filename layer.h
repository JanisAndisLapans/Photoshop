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
    Layer(const QPoint& pos = QPoint(0,0));
    Layer(const QString& path, int rank, const QPoint& pos = QPoint(0,0));
    void setPos(const QPoint& pos);
    QPoint getPos() const;
    QString getName() const;
    QImage* getImgRef();
    QImage getImg() const;
    int getRank() const;
    void setRank(int rank);
    void resize(uint w, uint height);
    bool isTransforming() const;
    void setTransforming(bool state);
    bool isSelected() const;
    void setSelected(bool state);
    QWidget* getTransformToolMenu();
    qreal getRotationDegrees() const;
    void setRotationDegress(qreal degrees);
    void setResizingState(bool state);

private:
    QImage  *img;
    QString name;
    int rank;
    bool transforming = false;
    bool selected = false;
    qreal rotationDegrees = 0;
    bool isResizing = false;

};

#endif // LAYER_H
