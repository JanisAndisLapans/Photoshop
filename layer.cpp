#include "layer.h"

Layer::Layer(const QPoint& pos)
    :QRect(pos, QSize(0,0))
{

}

Layer::Layer(const QString& path, const QPoint& pos)
    :img(new QImage(path))
{
    *img = img->convertToFormat(QImage::Format_ARGB32);
    setSize(img->size());
    setPos(pos);
    auto fileNameFull = QFile(path).fileName();
    name = fileNameFull.mid(fileNameFull.lastIndexOf("/")+1);
    origSize = size();
}

QPoint Layer::getPos() const
{
    return QRect::topLeft();
}

void Layer::setPos(const QPoint& pos)
{
    translate(pos-topLeft());
}

QString Layer::getName() const
{
    return name;
}

QImage* Layer::getImgRef()
{
    return img;
}

QImage Layer::getImg() const
{
    return *img;
}

bool Layer::isTransforming() const
{
    return transforming;
}

void Layer::setTransforming(bool state)
{
    transforming = state;
}

bool Layer::isSelected() const
{
    return selected;
}
void Layer::setSelected(bool state)
{
    selected = state;
}

qreal Layer::getRotationDegrees() const
{
    if(isResizing) return 0;
    else return rotationDegrees;
}

void Layer::setRotationDegress(qreal degrees)
{
    rotationDegrees = degrees;
}
void Layer::setResizingState(bool state)
{
    isResizing = true;
}

double Layer::sizePercentageW() const
{
    return static_cast<double>(width())/origSize.width();
}
double Layer::sizePercentageH() const
{
    return static_cast<double>(height())/origSize.height();
}
