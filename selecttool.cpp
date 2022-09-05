#include "selecttool.h"
#include "editframe.h"

SelectTool::SelectTool(EditFrame *editFrame, QVector<Layer*> *layers)
    :EditTool(editFrame), selectedArea(editFrame->getSelectedAreaRef()), layers(layers)
{
    menu = new selectToolMenu();
    connect(menu, SIGNAL(cursorChanged()), this, SLOT(onCursorChanged()));
}


void SelectTool::onDownMouse(QMouseEvent *eventPress)
{
    mouseDown = true;
    if(dragResizing)
    {
        startMouse = eventPress->pos();
        return;
    }
    auto currPos = eventPress->pos();
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto l = *riter;
        if(l->contains(ImageAlgorithms::rotatePos(currPos, l->getRotationDegrees(), l->center() /2*(editFrame->getZoom() + 1.0))))
        {
            if(!isAdd)
                makeDeselectionAt(ImageAlgorithms::rotatePos(currPos/editFrame->getZoom(), l->getRotationDegrees(), l->center()), l->getImg(), l->getPos(), l->center(), l->getRotationDegrees());
            else
                makeSelectionAt(ImageAlgorithms::rotatePos(currPos/editFrame->getZoom(), l->getRotationDegrees(), l->center()), l->getImg(), l->getPos(), l->center(), l->getRotationDegrees());
            break;
        }
    }
}

void SelectTool::onMoveMouse(QMouseEvent *eventMove)
{
    if(!mouseDown) return;
    auto currPos = eventMove->pos();
    if(dragResizing)
    {
        int radius = sqrt(pow(abs(currPos.x()-startMouse.x()),2) + pow(abs(currPos.y()-startMouse.y()),2));
        editFrame->drawResizeBall(radius, startMouse);
        return;
    }
    for(auto riter = layers->rbegin(); riter!=layers->rend(); riter++)
    {
        auto l = *riter;
        if(l->contains(ImageAlgorithms::rotatePos(currPos, l->getRotationDegrees(), l->center() /2*(editFrame->getZoom() + 1.0))))
        {
            if(!isAdd)
                makeDeselectionAt(ImageAlgorithms::rotatePos(currPos/editFrame->getZoom(), l->getRotationDegrees(), l->center()), l->getImg(), l->getPos(), l->center(), l->getRotationDegrees());
            else
                makeSelectionAt(ImageAlgorithms::rotatePos(currPos/editFrame->getZoom(), l->getRotationDegrees(), l->center()), l->getImg(), l->getPos(), l->center(), l->getRotationDegrees());
            break;
        }
    }
}

void SelectTool::onReleaseMouse(QMouseEvent *releaseEvent)
{
    mouseDown = false;
    if(dragResizing)
    {
        editFrame->finishDrawingResizeBall();
        auto currPos = releaseEvent->pos();
        int newSize = sqrt(pow(abs(currPos.x()-startMouse.x()),2) + pow(abs(currPos.y()-startMouse.y()),2))*2;
        menu->setBrushSize(newSize);
    }
}

void SelectTool::onLeaveMouse(QHoverEvent *hoverEvent)
{
    onReleaseMouse(reinterpret_cast<QMouseEvent*>(hoverEvent));
}

bool SelectTool::eventFilter(QObject *obj, QEvent *event)
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
                isAdd = false;
                onTurnAltCursor();
            }
            else if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Control)
            {
                dragResizing = true;
                editFrame->setCursor(Qt::BlankCursor);
            }
            else
            {
                dragResizing = false;
                if(!isAdd) onTurnPrimaryCursor();
                isAdd = true;
            }
            break;
        case QEvent::KeyRelease:
            if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Alt)
            {
                isAdd = true;
                onTurnPrimaryCursor();
            }
            else if(static_cast<QKeyEvent*>(event)->key()==Qt::Key_Control)
            {
                dragResizing = false;
                adjustCursor();
            }
            else
            {
                dragResizing = false;
                if(!isAdd) onTurnPrimaryCursor();
                isAdd = true;
            }
            break;
    }
    return QObject::eventFilter(obj, event);
}

void SelectTool::makeSelectionAt(const QPoint& from, const QImage& img, const QPoint& realPos, const QPoint& rotationCenter, qreal rotation)
{
    rotation *= -1;
    auto pixels = reinterpret_cast<const QRgb*>(img.constBits());
    auto selectedAreaGrid = selectedArea->getSelectedArea();
    auto x = from.x()-realPos.x(), y = from.y()-realPos.y();
    int count = 1;
    QStack<tuple<int, int, QRgb, int, int>> stack;
    const auto brushSize = menu->getBrushSize();
    auto currColor = pixels[x+y*img.width()];
    stack.push(make_tuple(x-1,y,currColor, x, y));
    stack.push(make_tuple(x+1,y,currColor, x, y));
    stack.push(make_tuple(x,y-1,currColor, x, y));
    stack.push(make_tuple(x,y+1,currColor, x, y));
    const auto maxDist = menu->getFreeDist();

    auto memo = new bool[img.width()*img.height()];
    memset(memo, false, img.width()*img.height());
    auto area = 0;
    auto real = ImageAlgorithms::rotatePos(from, rotation, rotationCenter);
    if(selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()] != 1)
    {
        area++;
    }
    selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()] = 1;
    memo[x+y*img.width()] = true;
    const auto startPos = QPoint(x,y);
    while(!stack.empty())
    {
        count++;
        auto curr = stack.pop();
        const auto x = get<0>(curr), y = get<1>(curr);
        auto prevColor = get<2>(curr);
        auto realPrev = ImageAlgorithms::rotatePos(QPoint(get<3>(curr), get<4>(curr)) + realPos, rotation, rotationCenter);
        auto real = ImageAlgorithms::rotatePos(QPoint(x, y) + realPos, rotation, rotationCenter);
        if(sqrt(abs(x-startPos.x())*abs(x-startPos.x()) + abs(y-startPos.y())*abs(y-startPos.y()))>maxDist ||
                x>=img.width() ||
                y>=img.height() ||
                x<0 || y<0)
        {
            if(selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()]==0)
            {
                if(selectedAreaGrid[realPrev.x() + realPrev.y()*selectedArea->getSize()] == 1) area--;
                selectedAreaGrid[realPrev.x() + realPrev.y()*selectedArea->getSize()] = 2;
            }
            continue;
        }
        if(memo[x+y*img.width()]) continue;
        auto currColor = pixels[x+y*img.width()];
        auto diff = ImageAlgorithms::colorDifference(currColor, prevColor);
        if(diff < menu->getStopAdd() || sqrt(abs(x-startPos.x())*abs(x-startPos.x()) + abs(y-startPos.y())*abs(y-startPos.y())) < brushSize/2)
        {
            if(selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()] != 1)
            {
                area++;
            }
            selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()] = 1;
            memo[x+y*img.width()] = true;
            stack.push(make_tuple(x-1,y,currColor, x, y));
            stack.push(make_tuple(x+1,y,currColor, x, y));
            stack.push(make_tuple(x,y-1,currColor, x, y));
            stack.push(make_tuple(x,y+1,currColor, x, y));
        }
        else if(selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()]==0)
        {
            if(selectedAreaGrid[realPrev.x() + realPrev.y()*selectedArea->getSize()] == 1) area--;
            selectedAreaGrid[realPrev.x() + realPrev.y()*selectedArea->getSize()] = 2;
        }
    }
    delete[] memo;
    selectedArea->change(QRect(ImageAlgorithms::rotatePos(startPos + realPos, rotation, rotationCenter) - QPoint(maxDist, maxDist), QSize(maxDist*2, maxDist*2)), selectedArea->getPixelCount()+area);
    editFrame->update();
}

void SelectTool::makeDeselectionAt(const QPoint& from, const QImage& img, const QPoint& realPos, const QPoint& rotationCenter, qreal rotation)
{
    rotation *= -1;
    auto pixels = reinterpret_cast<const QRgb*>(img.constBits());
    auto selectedAreaGrid = selectedArea->getSelectedArea();
    auto x = from.x()-realPos.x(), y = from.y()-realPos.y();

    QStack<tuple<int, int, QRgb, int, int>> stack;
    const auto brushSize = menu->getBrushSize();
    auto currColor = pixels[x+y*img.width()];
    stack.push(make_tuple(x-1,y,currColor, x, y));
    stack.push(make_tuple(x+1,y,currColor, x, y));
    stack.push(make_tuple(x,y-1,currColor, x, y));
    stack.push(make_tuple(x,y+1,currColor, x, y));
    const auto maxDist = menu->getFreeDist();

    auto memo = new bool[img.width()*img.height()];
    memset(memo, false, img.width()*img.height());
    auto area = 0;
    auto real = ImageAlgorithms::rotatePos(from, rotation, rotationCenter);
    if(selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()] == 1)
    {
        area--;
    }
    selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()] = 0;
    memo[x+y*img.width()] = true;
    const auto startPos = QPoint(x,y);
    while(!stack.empty())
    {
        auto curr = stack.pop();
        const auto x = get<0>(curr), y = get<1>(curr);
        auto realPrev = ImageAlgorithms::rotatePos(QPoint(get<3>(curr), get<4>(curr)) + realPos, rotation, rotationCenter);
        auto real = ImageAlgorithms::rotatePos(QPoint(x, y) + realPos, rotation, rotationCenter);
        auto prevColor = get<2>(curr);
        if(sqrt(abs(x-startPos.x())*abs(x-startPos.x()) + abs(y-startPos.y())*abs(y-startPos.y()))>maxDist ||
                x>=img.width() ||
                y>=img.height() ||
                x<0 || y<0)
        {
            if(selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()]==1)
            {
                selectedAreaGrid[realPrev.x() + realPrev.y()*selectedArea->getSize()] = 2;
            }
            continue;
        }
        if(memo[x+y*img.width()]) continue;
        auto currColor = pixels[x+y*img.width()];
        auto diff = ImageAlgorithms::colorDifference(currColor, prevColor);
        if(diff < menu->getStopSub() || sqrt(abs(x-startPos.x())*abs(x-startPos.x()) + abs(y-startPos.y())*abs(y-startPos.y())) < brushSize/2)
        {
            if(selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()]==1)
            {
                area--;
            }
            selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()] = 0;
            memo[x+y*img.width()] = true;
            stack.push(make_tuple(x-1,y,currColor, x, y));
            stack.push(make_tuple(x+1,y,currColor, x, y));
            stack.push(make_tuple(x,y-1,currColor, x, y));
            stack.push(make_tuple(x,y+1,currColor, x, y));
        }
        else if(selectedAreaGrid[real.x() + real.y()*selectedArea->getSize()]==1)
        {
            selectedAreaGrid[realPrev.x() + realPrev.y()*selectedArea->getSize()] = 2;
        }
    }
    delete[] memo;
    selectedArea->change(QRect(ImageAlgorithms::rotatePos(startPos + realPos, rotation, rotationCenter) - QPoint(maxDist, maxDist), QSize(maxDist*2, maxDist*2)), selectedArea->getPixelCount()+area);
    editFrame->update();
}

selectToolMenu* SelectTool::getMenu()
{
    return menu;
}

void SelectTool::onCursorChanged()
{
    adjustCursor();
}
void SelectTool::onTurnPrimaryCursor()
{
    editFrame->setCursor(addCursor);
}
void SelectTool::adjustCursor()
{
    int size = menu->getBrushSize()*editFrame->getZoom();
    QImage selectImg(size, size, QImage::Format_ARGB32);
    selectImg.fill(Qt::transparent);
    QPainter painter(&selectImg);
    painter.setPen(Qt::black);
    painter.drawEllipse(QPoint(size/2, size/2), size/2, size/2);
    painter.drawLine(size/6, size/2, size - size/6, size/2);
    subCursor = QCursor(QPixmap::fromImage(selectImg));
    painter.drawLine(size/2, size/6, size/2, size - size/6);
    addCursor = QCursor(QPixmap::fromImage(selectImg));
    if(isAdd)
        editFrame->setCursor(addCursor);
    else
        editFrame->setCursor(subCursor);
}

void SelectTool::onTurnAltCursor()
{
    editFrame->setCursor(subCursor);

}

void SelectTool::setCursor()
{
    adjustCursor();
}
