#include "layerframe.h"

LayerFrame::LayerFrame(QWidget *parent)
    :QFrame(parent)
{
    mouseDownTimer = new QTimer(this);
    connect(mouseDownTimer, &QTimer::timeout, this, &LayerFrame::startDrag);
    setFocusPolicy(Qt::StrongFocus);
}

void LayerFrame::setEditFrame(EditFrame *editFrame)
{
    this->editFrame = editFrame;
    this->layers = editFrame->getLayersRef();
}

#define BOX_HEIGHT_MAX 200

void LayerFrame::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    const auto height = min(width()/2, BOX_HEIGHT_MAX);
    auto selectedThickness = 5;
    auto imageRect = QRect(0, 0, width()/2, height);
    auto nameRect = QRect(width()/2, 0, width()/2, height);
    auto selectedBorderRect = QRect(0,0,width(),height);
    const auto shiftVector = QPoint(0, height);
    for(auto riter = (*layers).rbegin(); riter!=(*layers).rend(); riter++)
    {
        painter.setPen(Qt::black);
        const auto& layer = *riter;

        painter.drawImage(imageRect, layer->getImg());
        painter.drawText(nameRect, Qt::TextWrapAnywhere | Qt::AlignCenter, layer->getName());
        imageRect.translate(shiftVector);
        nameRect.translate(shiftVector);
        if(layer->isSelected())
        {
            painter.setPen(QPen(Qt::red, selectedThickness));
            painter.drawRect(selectedBorderRect);
        }
        selectedBorderRect.translate(shiftVector);
    }
    setMinimumHeight(imageRect.top()+10);
}

void LayerFrame::startDrag()
{
    const auto height = min(width()/2, BOX_HEIGHT_MAX);
    auto imageRect = QRect(0, 0, width()/2, height);
    auto nameRect = QRect(width()/2, 0, width()/2, height);
    auto draggedImage = QImage(width(),height,QImage::Format_ARGB32);
    QPainter painter(&draggedImage);
    const auto currLayer = (*layers)[draggedLayerIndex];
    painter.drawImage(imageRect, currLayer->getImg());
    painter.drawText(nameRect, Qt::AlignHCenter, currLayer->getName());
    dragCursor = QCursor(QPixmap::fromImage(draggedImage));
    setCursor(dragCursor);
    mouseDragging = true;
}

int LayerFrame::getIndexOfLayerAtY(int y)
{
    const auto height = min(width()/2, BOX_HEIGHT_MAX);
    auto ind = y/height;
    return layers->size() - ind - 1;
}

void LayerFrame::mousePressEvent(QMouseEvent *event)
{
    auto index = getIndexOfLayerAtY(event->position().y());

    if(layers->size()>index)
    {
        draggedLayerIndex = index;
        auto wasSelected = (*layers)[index]->isSelected();
        if(!isMultiSelect)
            for(auto layer : *layers)
                layer->setSelected(false);
        if(!wasSelected) (*layers)[index]->setSelected(true);
        else (*layers)[index]->setSelected(false);
        update();
        mouseDownTimer->start(800);
    }
}

void LayerFrame::mouseReleaseEvent(QMouseEvent *event)
{
    mouseDownTimer->stop();
    if(mouseDragging)
    {
        mouseDragging = false;
        auto curr = getIndexOfLayerAtY(event->position().y());
        if(layers->size()>curr)
        {
            layers->swapItemsAt(curr, draggedLayerIndex);
            editFrame->update();
            update();
        }
        setCursor(Qt::ArrowCursor);
    }
}

void LayerFrame::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Control)
    {
        isMultiSelect = true;
    }
    else if(event->key() == Qt::Key_Delete && !mouseDragging)
    {
        layers->erase(remove_if(layers->begin(), layers->end(),
            [](Layer* l)
        {
            return l->isSelected();
        }), layers->end());
        update();
    }
}

void LayerFrame::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Control)
    {
        isMultiSelect = false;
    }
}


void LayerFrame::mouseMoveEvent(QMouseEvent *event)
{
    setFocus();
}
