#include "brushtool.h"
#include "editframe.h"

BrushTool::BrushTool(EditFrame *editFrame, QVector<Layer*>* layers)
    :EditTool(editFrame), layers(layers)
{
    menu = new BrushToolMenu();
    connect(menu, SIGNAL(cursorChanged()), this, SLOT(onCursorChanged()));
}

void BrushTool::drawPixels(const QPoint& pos, const QPoint& realPos, QImage* img, const QImage& brushImg)
{
    auto bits = reinterpret_cast<QRgb*>(img->bits());
    auto brushBits = reinterpret_cast<const QRgb*>(brushImg.constBits());
    for(auto y = 0; y<brushImg.height(); y++)
    {
        auto inImgY = y + pos.y();
        auto inAreaY = y + realPos.y();
        if(inImgY >=img->height() || inImgY<0) break;
        for(auto x = 0; x<brushImg.width(); x++)
        {
            auto inImgX = x + pos.x();
            auto inAreaX = x + realPos.x();
            if(inImgX>=img->width() || inImgX<0) break;
            if(editFrame->hasSelectedArea() && !editFrame->getSelectedAreaRef()->isSelected(inAreaX, inAreaY)
                    || drawnArea[inAreaX + editFrame->getSelectedAreaRef()->getSize()*inAreaY]) continue;
            QRgb newVal;
            switch(menu->getMode())
            {
                case BrushToolMenu::Over:
                    newVal = ImageAlgorithms::sourceOver(brushBits[x + y*brushImg.width()], bits[inImgX+inImgY*img->width()]);
                    break;
                case BrushToolMenu::Multiply:
                    newVal = ImageAlgorithms::multiply(brushBits[x + y*brushImg.width()], bits[inImgX+inImgY*img->width()]);
                    break;
                case BrushToolMenu::Screen:
                    newVal = ImageAlgorithms::screen(brushBits[x + y*brushImg.width()], bits[inImgX+inImgY*img->width()]);
                    break;
                case BrushToolMenu::Overlay:
                    newVal = ImageAlgorithms::overlay(brushBits[x + y*brushImg.width()], bits[inImgX+inImgY*img->width()]);
                    break;
            }
            if(newVal != bits[inImgX+inImgY*img->width()])
                drawnArea[inAreaX + editFrame->getSelectedAreaRef()->getSize()*inAreaY] = true;
            bits[inImgX+inImgY*img->width()] = newVal;
        }
    }
}

bool BrushTool::draw(Layer& l, const QPoint& currPos)
{
    auto brushImg = menu->getBrushShape().getImg();
    auto brushRectOnDisplay = brushImg.rect();
    auto resizedRect = l;
    resizedRect.setSize(resizedRect.size()*editFrame->getZoom());
    brushRectOnDisplay.translate(currPos);
    if(!brushRectOnDisplay.intersects(resizedRect)) return false;
    auto canvasImgRef = l.getImgRef();
    auto translateAmount = -l.getPos();
    auto preciseXTranslate = static_cast<int>(static_cast<double>(currPos.x())/editFrame->getZoom()),
         preciseYTranslate = static_cast<int>(static_cast<double>(currPos.y())/editFrame->getZoom());
    translateAmount += QPoint(preciseXTranslate, preciseYTranslate);
    drawPixels(translateAmount, currPos/editFrame->getZoom(), canvasImgRef, brushImg);
    return true;
}

void BrushTool::onDownMouse(QMouseEvent *eventPress)
{
    mouseDown = true;
    if(dragResizing)
    {
        startMouse = eventPress->pos();
        return;
    }
    auto areaSize = editFrame->getSelectedAreaRef()->getSize();
    drawnArea = new bool[areaSize*areaSize];
    memset(drawnArea, false, areaSize*areaSize);
    auto currPos = eventPress->pos();
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto& l = *riter;
        if(draw(*l, currPos)) break;
    }
    editFrame->update();
}

void BrushTool::onMoveMouse(QMouseEvent *eventMove)
{
    if(!mouseDown) return;
    if(dragResizing)
    {
        auto currPos = eventMove->pos();
        int radius = sqrt(pow(abs(currPos.x()-startMouse.x()),2) + pow(abs(currPos.y()-startMouse.y()),2));
        editFrame->drawResizeBall(radius, startMouse);
        return;
    }
    auto currPos = eventMove->pos();
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto& l = *riter;
        if(draw(*l, currPos)) break;
    }
    editFrame->update();
}

void BrushTool::onReleaseMouse(QMouseEvent *releaseEvent)
{
    if(mouseDown)
    {
        mouseDown = false;
        if(dragResizing)
        {
            editFrame->finishDrawingResizeBall();
            auto currPos = releaseEvent->pos();
            int newSize = sqrt(pow(abs(currPos.x()-startMouse.x()),2) + pow(abs(currPos.y()-startMouse.y()),2))*2;
            menu->setBrushSize(newSize);
        }
        else
            delete[] drawnArea;
    }
}

void BrushTool::onLeaveMouse(QHoverEvent *hoverEvent)
{
    onReleaseMouse(reinterpret_cast<QMouseEvent*>(hoverEvent));
}

bool BrushTool::eventFilter(QObject *obj, QEvent *event)
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
        case QEvent::KeyPress:
            if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Control)
            {
                dragResizing = true;
                editFrame->setCursor(Qt::BlankCursor);
            }
            break;
        case QEvent::KeyRelease:

            if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Control && dragResizing)
            {
                dragResizing = false;
                adjustCursor();
            }
            break;
    }
    return QObject::eventFilter(obj, event);
}

BrushToolMenu* BrushTool::getMenu()
{
    return menu;
}

void BrushTool::adjustCursor()
{
    auto zoom = editFrame->getZoom();
    int zoomedSize = menu->getBrushShape().getSize()*zoom;
    auto img = menu->getBrushShape().getImg().scaled(zoomedSize, zoomedSize, Qt::KeepAspectRatio);
    editFrame->setCursor(QCursor(QPixmap::fromImage(img), 0, 0));
}

void BrushTool::onCursorChanged()
{
    adjustCursor();
}

void BrushTool::setCursor()
{
    adjustCursor();
}
