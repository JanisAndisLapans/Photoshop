#include "stamptool.h"
#include "editframe.h"


StampTool::StampTool(EditFrame *editFrame, QVector<Layer*>* layers)
    :EditTool(editFrame), layers(layers)
{
    menu = new StampToolMenu();
    connect(menu, SIGNAL(cursorChanged()), this, SLOT(onCursorChanged()));
    adjustBrush();
}

void StampTool::start()
{
    adjustBrush();
    adjustCursor();
}

void StampTool::end()
{
    notClonedYet = true;
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
            drawingDone = true;
        }
    }
}

bool StampTool::canDraw(Layer& l, const QPoint& currPos)
{
    auto brushImg = brushShape->getImg();
    auto brushRectOnDisplay = brushImg.rect();
    auto realPos = ImageAlgorithms::mousePointInLayer(currPos, l, editFrame->getZoom());
    brushRectOnDisplay.translate(realPos);
    if(!brushRectOnDisplay.intersects(l)) return false;
    return true;
}

void StampTool::draw(Layer& l, const QPoint& currPos)
{
    auto brushImg = brushShape->getImg();
    auto realPos = ImageAlgorithms::mousePointInLayer(currPos, l, editFrame->getZoom());
    brushImg = brushImg.scaled(brushImg.width()/l.sizePercentageW(),brushImg.height()/l.sizePercentageH(),Qt::IgnoreAspectRatio);
    auto canvasImgRef = l.getImgRef();
    auto translateAmount =  realPos - l.getPos()/editFrame->getZoom();
    translateAmount = QPoint(translateAmount.x() / l.sizePercentageW(), translateAmount.y() / l.sizePercentageH());
    drawPixels(translateAmount, currPos / editFrame->getZoom(), canvasImgRef, brushImg);
}

void StampTool::sampleAnew(const QPoint& point)
{
    auto wholeAreaSize = editFrame->getSelectedAreaRef()->getSize();
    srcArea = QImage(wholeAreaSize,wholeAreaSize,QImage::Format_ARGB32);
    srcArea.fill(Qt::black);
    QPainter painter(&srcArea);
    QPoint posRotated = point;
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto l = *riter;
        if(l->contains(ImageAlgorithms::rotatePos(point, l->getRotationDegrees(), l->center())))
        {
            painter.drawImage(l->getPos(), l->getImg());
            posRotated = ImageAlgorithms::rotatePos(point, l->getRotationDegrees(), l->center());
            cursorRotation = l->getRotationDegrees();
            break;
        }
    }

    auto size = QSize(brushShape->getSize(),brushShape->getSize());
    brushShape->changeSource(srcArea.copy(QRect(posRotated, size)));
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
        sampleStartPos = eventPress->pos()/editFrame->getZoom();
        sampleAnew(sampleStartPos);
        notClonedYet = true;
        notSampledYet = false;
        return;
    }
    if(notSampledYet)
    {
        mouseDown = false;
        return;
    }
    auto areaSize = editFrame->getSelectedAreaRef()->getSize();
    drawnArea = new bool[areaSize*areaSize];
    memset(drawnArea, false, areaSize*areaSize);
    drawingDone = false;
    editFrame->saveState();
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto l = *riter;
        auto prevsampleStartPos = sampleStartPos;
        if(notClonedYet)
        {
            stampStartPos = eventPress->pos();
        }
        if(canDraw(*l,  eventPress->pos()))
        {
            sampleNowPos = sampleStartPos - (stampStartPos - eventPress->pos());
            sampleInDraw(sampleNowPos);
            draw(*l,  eventPress->pos());
            notClonedYet = false;
            editFrame->update();
            break;
        }
        else
            sampleStartPos = prevsampleStartPos;
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
        if(canDraw(*l, eventMove->pos()))
        {
            sampleNowPos = sampleStartPos - (stampStartPos - eventMove->pos());
            sampleInDraw(sampleNowPos);
            draw(*l, eventMove->pos());
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
            int newSize = sqrt(pow(abs(currPos.x()-startMouse.x()),2) + pow(abs(currPos.y()-startMouse.y()),2))*2 / editFrame->getZoom();;
            menu->setBrushSize(newSize);
        }
        else if(isClone)
        {
            delete[] drawnArea;
            if(!drawingDone) editFrame->undo();
        }
        editFrame->update();
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
    auto img = QImage(zoomedSize, zoomedSize, QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter(&img);
    painter.translate(QPoint(zoomedSize/2, zoomedSize/2));
    painter.rotate(cursorRotation);
    painter.translate(-QPoint(zoomedSize/2, zoomedSize/2));
    painter.drawImage(0, 0, brushShape->getImg().scaled(zoomedSize, zoomedSize, Qt::KeepAspectRatio));
    painter.end();
    cloneCursor = QCursor(QPixmap::fromImage(move(img)), 0, 0);
    img = QImage(zoomedSize,zoomedSize,QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter2(&img);
    painter2.setPen(Qt::black);
    painter2.drawLine(0, zoomedSize/2, zoomedSize - 0, zoomedSize/2);
    painter2.drawLine(zoomedSize/2, 0, zoomedSize/2, zoomedSize);
    samplerImage = move(img);
    sampleCursor = QCursor(QPixmap::fromImage(samplerImage), 0, 0);

    if(isClone)
    {
        editFrame->setCursor(cloneCursor);
    }
    else
    {
        editFrame->setCursor(sampleCursor);
    }
}

bool StampTool::isDrawing() const
{
    return !notSampledYet && mouseDown;
}

QImage StampTool::getSamplerImage() const
{
    return samplerImage;
}

QPoint StampTool::getSampleCoords() const
{
    return sampleNowPos;
}

void StampTool::adjustBrush()
{
    int size = menu->getBrushSize();
    QImage img;
    static QImage blank(15, 15, QImage::Format_ARGB32);
    img = blank;
    notSampledYet = true;
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
