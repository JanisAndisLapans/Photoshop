#include "layer.h"

Layer::Layer(const QPoint& pos)
    :QRect(pos, QSize(0,0))
{

}

Layer::Layer(const QString& path, const QPoint& pos)
    :img(new QImage(path))
{
    setSize(img->size());
    setPos(pos);
    name = QFile(path).fileName();
}

QPoint Layer::getPos() const
{
    return QRect::topLeft();
}

void Layer::setPos(const QPoint& pos)
{
    setRect(pos.x(), pos.y(), size().width(), size().height());
}

QString Layer::getName() const
{
    return name;
}

bool Layer::isSelected() const
{
    return selected;
}

void Layer::toggleSelected()
{
    this->selected = !this->selected;
}

QImage* Layer::getImgRef()
{
    return img;
}

QImage Layer::getImg() const
{
    return *img;
}


