#include "layerbox.h"

LayerBox::LayerBox(Layer* layer, QWidget *parent)
    :QBoxLayout(LeftToRight, parent), layer(layer)
{
    name = new QLabel(layer->getName());
    image = new QLabel;
    image->setPixmap(QPixmap::fromImage(layer->getImg()));
}
