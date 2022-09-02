#include "layer.h"

Layer::Layer(const QPoint& pos)
    :QRect(pos, QSize(0,0))
{

}

Layer::Layer(const QString& path, int rank, const QPoint& pos)
    :img(new QImage(path)), rank(rank)
{
    *img = img->convertToFormat(QImage::Format_ARGB32);
    setSize(img->size());
    setPos(pos);
    auto fileNameFull = QFile(path).fileName();
    name = fileNameFull.mid(fileNameFull.lastIndexOf("/")+1);
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

int Layer::getRank() const
{
    return rank;
}

void Layer::setRank(int rank)
{
    this->rank = rank;
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
