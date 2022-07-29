#include "brushtool.h"
#include "editframe.h"

BrushTool::BrushTool(EditFrame *editFrame, const unique_ptr<BrushShape>* brush)
    :EditTool(editFrame), brushShape(brush)
{

}

bool BrushTool::draw(Layer& l, const QPoint& currPos)
{
    auto brushImg = (*brushShape)->getImg();
    auto brushRectOnDisplay = brushImg.rect();
    auto resizedRect = l;
    resizedRect.setSize(resizedRect.size()*editFrame->getZoom());
    brushRectOnDisplay.translate(currPos);
    if(!brushRectOnDisplay.intersects(resizedRect)) return false;
    auto brushImgRef = l.getImgRef();
    auto brushRect = brushImg.rect();
    auto translateAmount = -l.getPos();
    auto preciseXTranslate = static_cast<int>(static_cast<double>(currPos.x())/editFrame->getZoom()),
         preciseYTranslate = static_cast<int>(static_cast<double>(currPos.y())/editFrame->getZoom());
    translateAmount += QPoint(preciseXTranslate, preciseYTranslate);
    brushRect.translate(translateAmount);
    QPainter painter(brushImgRef);
    painter.drawImage(brushRect, brushImg);
    return true;
}

void BrushTool::onDownMouse(QMouseEvent *eventPress, QVector<Layer>& layers)
{
    mouseDown = true;
    auto currPos = eventPress->pos();
    for(auto riter = layers.rbegin(); riter!=layers.rend(); riter++)
    {
        auto& l = *riter;
        if(draw(l, currPos)) break;
    }
    editFrame->update();
}

void BrushTool::onMoveMouse(QMouseEvent *eventMove, QVector<Layer>& layers)
{
    if(!mouseDown) return;
    auto currPos = eventMove->pos();
    for(auto riter = layers.rbegin(); riter!=layers.rend(); riter++)
    {
        auto& l = *riter;
        if(draw(l, currPos)) break;
    }
    editFrame->update();
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
