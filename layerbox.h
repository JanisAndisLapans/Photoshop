#ifndef LAYERBOX_H
#define LAYERBOX_H

#include "layer.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPixmap>

class LayerBox : public QBoxLayout
{
public:
    LayerBox(Layer* layer, QWidget *parent = nullptr);
private:
    QLabel *name, *image;
    Layer *layer;
};

#endif // LAYERBOX_H
