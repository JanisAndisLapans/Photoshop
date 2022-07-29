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
    Layer(const QString& path, int rank , const QPoint& pos = QPoint(0,0));
    void setPos(const QPoint& pos);
    QPoint getPos() const;
    QString getName() const;
    QImage* getImgRef();
    QImage getImg() const;
    int getRank() const;
    void setRank(int rank);
private:
    QImage  *img;
    QString name;
    int rank;
};

#endif // LAYER_H
