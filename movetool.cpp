#include "movetool.h"
#include "editframe.h"

MoveTool::MoveTool(EditFrame *editFrame)
    :EditTool(editFrame)
{

}

void MoveTool::onDownMouse(QMouseEvent *eventPress)
{
    mouseDown = true;
    startMouse = eventPress->pos();
}

void MoveTool::onMoveMouse(QMouseEvent *eventMove, QVector<Layer>& layers)
{
        if(!mouseDown) return;
        auto dif = eventMove->pos() - startMouse;
        startMouse = eventMove->pos();

        for(Layer& l : layers)
        {
            if(l.isSelected())
            {
                auto newPos = l.getPos() + dif;
                newPos.setX(max(newPos.x(),0));
                newPos.setY(max(newPos.y(),0));
                l.setPos(newPos);
            }
        }
        editFrame->adjustSize();
        editFrame->update();
}

void MoveTool::onReleaseMouse(QMouseEvent *releaseEvent)
{
    mouseDown = false;
}

void MoveTool::onLeaveMouse(QHoverEvent *hoverEvent)
{
    onReleaseMouse(reinterpret_cast<QMouseEvent*>(hoverEvent));
}

void MoveTool::rerouteEvent(QEvent *event, QVector<Layer>& layers)
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
            onMoveMouse(static_cast<QMouseEvent*>(event), layers);
            break;
        case QEvent::HoverLeave:
            onLeaveMouse(static_cast<QHoverEvent*>(event));
            break;
    }
}
