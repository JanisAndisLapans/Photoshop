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

        painter.setCompositionMode(layer->getCompositionMode());
        painter.drawImage(imageRect, layer->getImg());
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
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

    if(layers->size()>index && index >= 0)
    {
        if(event->button() == Qt::LeftButton)
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
        else if(event->button() == Qt::RightButton)
        {
            auto layer = (*layers)[index];
            auto currMode = layer->getCompositionMode();

            QMenu contextMenu("Action type", editFrame);

            auto modeMenu = contextMenu.addMenu("Mode");

            QAction overAction("Over", editFrame);
            if(currMode == QPainter::CompositionMode_SourceOver)
            {
                 overAction.setCheckable(true);
                 overAction.setChecked(true);
            }
            else
            {
                connect(&overAction, &QAction::triggered, [&]()
                {
                    editFrame->saveState();
                    layer->setCompositionMode(QPainter::CompositionMode_SourceOver);
                    editFrame->update();
                });
            }
            modeMenu->addAction(&overAction);

            QAction overlayAction("Overlay", editFrame);
            if(currMode == QPainter::CompositionMode_Overlay)
            {
                overlayAction.setCheckable(true);
                overlayAction.setChecked(true);
            }
            else
            {
                connect(&overlayAction, &QAction::triggered, [&]()
                {
                    editFrame->saveState();
                    layer->setCompositionMode(QPainter::CompositionMode_Overlay);
                    editFrame->update();
                });
            }
            modeMenu->addAction(&overlayAction);

            QAction multipyAction("Multiply", editFrame);
            if(currMode == QPainter::CompositionMode_Multiply)
            {
                multipyAction.setCheckable(true);
                multipyAction.setChecked(true);
            }
            else
            {
                connect(&multipyAction, &QAction::triggered, [&]()
                {
                    editFrame->saveState();
                    layer->setCompositionMode(QPainter::CompositionMode_Multiply);
                    editFrame->update();
                });
            }
            modeMenu->addAction(&multipyAction);

            QAction screenAction("Screen", editFrame);
            if(currMode == QPainter::CompositionMode_Screen)
            {
                screenAction.setCheckable(true);
                screenAction.setChecked(true);
            }
            else
            {
                connect(&screenAction, &QAction::triggered, [&]()
                {
                    editFrame->saveState();
                    layer->setCompositionMode(QPainter::CompositionMode_Screen);
                    editFrame->update();
                });
            }
            modeMenu->addAction(&screenAction);

            contextMenu.exec(QCursor::pos());
        }
    }
}

void LayerFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
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
