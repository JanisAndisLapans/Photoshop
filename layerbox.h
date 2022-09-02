#ifndef LAYERBOX_H
#define LAYERBOX_H

#include "layer.h"

#include <memory>

#include <QBoxLayout>
#include <QLabel>
#include <QPixmap>

using namespace std;

class LayerBox : public QWidget
{
public:
    LayerBox(const Layer* layer, QWidget *parent = nullptr);
private:
    QLabel *name, *image;
    const Layer& layer;
};

#endif // LAYERBOX_H
