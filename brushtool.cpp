#include "brushtool.h"
#include "editframe.h"

BrushTool::BrushTool(EditFrame *editFrame, const shared_ptr<BrushShape>& brushShape)
    :EditTool(editFrame), brushShape(brushShape)
{
}

void draw(Layer& l, const QPoint& currPos, const BrushShape& brushShape, EditFrame *editFrame)
{
    auto img = brushShape.getImg();
    auto brushRectOnDisplay = img.rect();
    brushRectOnDisplay.translate(currPos);
    if(!brushRectOnDisplay.intersects(l)) return;
    auto imgRef = l.getImgRef();
    auto brushRect = img.rect();
    brushRect.translate(currPos/editFrame->getZoom()-l.getPos());
    QPainter painter(imgRef);
    painter.drawImage(brushRect, img);
}

void BrushTool::onDownMouse(QMouseEvent *eventPress, QVector<Layer>& layers)
{
    mouseDown = true;
    auto currPos = eventPress->pos();
    for(Layer& l : layers)
    {
        if(l.isSelected())
        {
            draw(l, currPos, *brushShape, editFrame);
        }
    }
    editFrame->update();
}

void BrushTool::onMoveMouse(QMouseEvent *eventMove, QVector<Layer>& layers)
{
    if(!mouseDown) return;
    auto start = chrono::steady_clock::now();
    auto currPos = eventMove->pos();
    for(Layer& l : layers)
    {
        if(l.isSelected())
        {
            draw(l, currPos, *brushShape, editFrame);
        }
    }
    editFrame->update();
    auto elapsed = chrono::steady_clock::now() - start;
}

void BrushTool::onReleaseMouse(QMouseEvent *releaseEvent)
{
    mouseDown = false;
}

void BrushTool::onLeaveMouse(QHoverEvent *hoverEvent)
{
    onReleaseMouse(reinterpret_cast<QMouseEvent*>(hoverEvent));
}

void BrushTool::rerouteEvent(QEvent *event, QVector<Layer>& layers)
{
    switch(event->type())
    {
        case QEvent::MouseButtonPress:
            onDownMouse(static_cast<QMouseEvent*>(event), layers);
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
