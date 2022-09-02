#include "stamptool.h"
#include "editframe.h"


StampTool::StampTool(EditFrame *editFrame, QVector<Layer*>* layers)
    :EditTool(editFrame), layers(layers)
{
    menu = new StampToolMenu();
    connect(menu, SIGNAL(cursorChanged()), this, SLOT(onCursorChanged()));
    adjustBrush();

}

void StampTool::drawPixels(const QPoint& pos, const QPoint& realPos, QImage* img, const QImage& brushImg)
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
                case StampToolMenu::Over:
                    newVal = ImageAlgorithms::sourceOver(brushBits[x + y*brushImg.width()], bits[inImgX+inImgY*img->width()]);
                    break;
                case StampToolMenu::Multiply:
                    newVal = ImageAlgorithms::multiply(brushBits[x + y*brushImg.width()], bits[inImgX+inImgY*img->width()]);
                    break;
                case StampToolMenu::Screen:
                    newVal = ImageAlgorithms::screen(brushBits[x + y*brushImg.width()], bits[inImgX+inImgY*img->width()]);
                    break;
                case StampToolMenu::Overlay:
                    newVal = ImageAlgorithms::overlay(brushBits[x + y*brushImg.width()], bits[inImgX+inImgY*img->width()]);
                    break;
            }
            if(newVal != bits[inImgX+inImgY*img->width()])
                drawnArea[inAreaX + editFrame->getSelectedAreaRef()->getSize()*inAreaY] = true;
            bits[inImgX+inImgY*img->width()] = newVal;
        }
    }
}

bool StampTool::draw(Layer& l, const QPoint& currPos)
{
    const auto& brushImg = brushShape->getImg();
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

void StampTool::sampleAnew(const QPoint& point)
{
    auto wholeAreaSize = editFrame->getSelectedAreaRef()->getSize();
    srcArea = QImage(wholeAreaSize,wholeAreaSize,QImage::Format_ARGB32);
    QPainter painter(&srcArea);
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto l = *riter;
        if(l->contains(point))
        {
            painter.drawImage(l->getPos(), l->getImg());
            break;
        }
    }

    auto size = QSize(brushShape->getSize(),brushShape->getSize());
    brushShape->changeSource(srcArea.copy(QRect(point, size)));
    adjustCursor();
}

void StampTool::sampleInDraw(const QPoint& point)
{
    auto size = QSize(brushShape->getSize(),brushShape->getSize());
    brushShape->changeSource(srcArea.copy(QRect(point, size)));
    adjustCursor();
}

void StampTool::onDownMouse(QMouseEvent *eventPress)
{
    mouseDown = true;
    if(dragResizing)
    {
        startMouse = eventPress->pos();
        return;
    }
    if(!isClone)
    {
        sampleAnew(eventPress->pos()/editFrame->getZoom());
        samplePos = eventPress->pos()/editFrame->getZoom();
        notSampledYet = false;
        return;
    }
    if(notSampledYet) return;
    auto areaSize = editFrame->getSelectedAreaRef()->getSize();
    drawnArea = new bool[areaSize*areaSize];
    memset(drawnArea, false, areaSize*areaSize);
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto l = *riter;
        if(draw(*l,  eventPress->pos()))
        {
            prevMouse = eventPress->pos();
            editFrame->update();
            break;
        }
    }
}

void StampTool::onMoveMouse(QMouseEvent *eventMove)
{
    if(!mouseDown || !isClone) return;
    if(dragResizing)
    {
        auto currPos = eventMove->pos();
        int radius = sqrt(pow(abs(currPos.x()-startMouse.x()),2) + pow(abs(currPos.y()-startMouse.y()),2));
        editFrame->drawResizeBall(radius, startMouse);
        return;
    }
    if(notSampledYet) return;
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto l = *riter;
        if(draw(*l, eventMove->pos()))
        {
            samplePos += (eventMove->pos() - prevMouse)/editFrame->getZoom();
            sampleInDraw(samplePos);
            prevMouse = eventMove->pos();
            editFrame->update();
            break;
        }
    }
}

void StampTool::onReleaseMouse(QMouseEvent *releaseEvent)
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
        else if(isClone)
            delete[] drawnArea;
    }
}

void StampTool::onLeaveMouse(QHoverEvent *hoverEvent)
{
    onReleaseMouse(reinterpret_cast<QMouseEvent*>(hoverEvent));
}

bool StampTool::eventFilter(QObject *obj, QEvent *event)
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
            if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Alt)
            {
                isClone = false;
                onTurnAltCursor();
            }
            else if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Control)
            {
                dragResizing = true;
                editFrame->setCursor(Qt::BlankCursor);
            }
            break;
        case QEvent::KeyRelease:
            if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Alt)
            {
                isClone = true;
                onTurnPrimaryCursor();
            }
            else if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Control && dragResizing)
            {
                dragResizing = false;
                adjustBrush();
                adjustCursor();
            }
            break;
    }
    return QObject::eventFilter(obj, event);
}

StampToolMenu* StampTool::getMenu()
{
    return menu;
}

void StampTool::adjustCursor()
{
    auto zoom = editFrame->getZoom();
    int zoomedSize = brushShape->getSize()*zoom;
    auto img = brushShape->getImg().scaled(zoomedSize, zoomedSize, Qt::KeepAspectRatio);
    cloneCursor = QCursor(QPixmap::fromImage(move(img)), 0, 0);
    img = QImage(zoomedSize,zoomedSize,QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter(&img);
    painter.setPen(Qt::black);
    painter.drawLine(0, zoomedSize/2, zoomedSize - 0, zoomedSize/2);
    painter.drawLine(zoomedSize/2, 0, zoomedSize/2, zoomedSize);
    sampleCursor = QCursor(QPixmap::fromImage(move(img)), 0, 0);

    if(isClone)
    {
        editFrame->setCursor(cloneCursor);
    }
    else
    {
        editFrame->setCursor(sampleCursor);
    }
}

void StampTool::adjustBrush()
{
    int  size = menu->getBrushSize();
    QImage img;
    if(notSampledYet)
    {
        static QImage blank(15, 15, QImage::Format_ARGB32);
        img = blank;
    }
    else
    {
        img = brushShape->getImg();
    }
    switch(menu->getBrushType())
    {
        case StampToolMenu::Circle:
            brushShape = new BrushCircle(size,img);
            break;
        case StampToolMenu::Square:
                brushShape = new BrushSquare(size,img);
            break;
        case StampToolMenu:: FadedCircle:
                brushShape = new BrushFadedCircle(size,img);
            break;
    }
}

void StampTool::onCursorChanged()
{
    adjustBrush();
    adjustCursor();
}

void StampTool::setCursor()
{
    adjustCursor();
}

void StampTool::onTurnAltCursor()
{
    editFrame->setCursor(sampleCursor);
}
void StampTool::onTurnPrimaryCursor()
{
    editFrame->setCursor(cloneCursor);
}
