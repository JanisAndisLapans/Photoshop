#include "layerframe.h"



LayerFrame::LayerFrame(QWidget *parent)
    :QFrame(parent)
{
    mouseDownTimer = new QTimer(this);
    connect(mouseDownTimer, &QTimer::timeout, this, &LayerFrame::startDrag);
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
    auto imageRect = QRect(0, 0, width()/2, height);
    auto nameRect = QRect(width()/2, 0, width()/2, height);
    const auto shiftVector = QPoint(0, height);
    for(auto riter = (*layers).rbegin(); riter!=(*layers).rend(); riter++)
    {
        const auto& layer = *riter;
        painter.drawImage(imageRect, layer.getImg());
        painter.drawText(nameRect, Qt::TextWrapAnywhere | Qt::AlignCenter, layer.getName());
        imageRect.translate(shiftVector);
        nameRect.translate(shiftVector);
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
    const auto& currLayer = (*layers)[draggedLayerIndex];
    painter.drawImage(imageRect, currLayer.getImg());
    painter.drawText(nameRect, Qt::AlignHCenter, currLayer.getName());
    dragCursor = QCursor(QPixmap::fromImage(draggedImage));
    setCursor(dragCursor);
    mouseDragging = true;
}

int LayerFrame::getIndexOfLayerAtY(int y)
{
    const auto height = min(width()/2, BOX_HEIGHT_MAX);
    return y/height;
}

void LayerFrame::mousePressEvent(QMouseEvent *event)
{
    auto index = getIndexOfLayerAtY(event->position().y());
    if(layers->size()>index)
    {
        draggedLayerIndex = index;
        mouseDownTimer->start(800);
    }
}

void LayerFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if(mouseDragging)
    {
        mouseDownTimer->stop();
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
