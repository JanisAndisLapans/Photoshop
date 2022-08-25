#include "movetool.h"

MoveTool::MoveTool(EditFrame *editFrame, QVector<Layer>* layers)
    :EditTool(editFrame), layers(layers)
{
    menu = new MoveToolMenu();
}

void MoveTool::onDownMouse(QMouseEvent *eventPress)
{
    mouseDown = true;
    startMouse = eventPress->pos();
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto& layer = *riter;
        auto rect = QRect(layer);
        rect.setSize(rect.size() * editFrame->getZoom());
        if(rect.contains(eventPress->pos()))
        {
            currMoving = &layer;
            break;
        }
    }
}

void MoveTool::onMoveMouse(QMouseEvent *eventMove)
{
        if(!mouseDown || currMoving == nullptr) return;
        auto dif = eventMove->pos() - startMouse;
        startMouse = eventMove->pos();
        auto newPos = currMoving->getPos() + dif;
        newPos.setX(max(newPos.x(),0));
        newPos.setY(max(newPos.y(),0));
        currMoving->setPos(newPos);
        editFrame->adjustSize(true);
        editFrame->update();
}

void MoveTool::onReleaseMouse(QMouseEvent *releaseEvent)
{
    mouseDown = false;
    editFrame->adjustSize();
}

void MoveTool::onLeaveMouse(QHoverEvent *hoverEvent)
{
    onReleaseMouse(reinterpret_cast<QMouseEvent*>(hoverEvent));
}

bool MoveTool::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::MouseButtonPress:
            onDownMouse(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            onReleaseMouse(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseMove:
            onMoveMouse(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::HoverLeave:
            onLeaveMouse(static_cast<QHoverEvent*>(event));
            break;
        default:
            return QObject::eventFilter(obj, event);
            break;
    }
    return true;
}

MoveToolMenu* MoveTool::getMenu()
{
    return menu;
}

void MoveTool::setCursor()
{
    editFrame->setCursor(Qt::SizeAllCursor);
}
