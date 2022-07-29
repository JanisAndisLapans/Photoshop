#include "layerbox.h"

LayerBox::LayerBox(const Layer& layer, QWidget *parent)
    :QWidget(parent), layer(layer)
{
    name = new QLabel(layer.getName());
    image = new QLabel();
    image->setPixmap(QPixmap::fromImage(layer.getImg()));

    image->setParent(this);
    name->setParent(this);
    show();
}
