#include "selecttool.h"
#include "editframe.h"

SelectTool::SelectTool(EditFrame *editFrame, const unique_ptr<BrushShape>* brushShape)
    :EditTool(editFrame), selectedArea(editFrame->getSelectedAreaRef()), brushShape(brushShape)
{
}


void SelectTool::onDownMouse(QMouseEvent *eventPress, QVector<Layer>& layers)
{
    mouseDown = true;
    auto currPos = eventPress->pos();
    for(auto riter = layers.rbegin(); riter!=layers.rend(); riter++)
    {
        auto& l = *riter;
        if(l.contains(currPos))
        {
            if(editFrame->isSpecialTool())
                makeDeselectionAt(currPos/editFrame->getZoom(), l.getImg(), l.getPos());
            else
                makeSelectionAt(currPos/editFrame->getZoom(), l.getImg(), l.getPos());
            break;
        }
    }
}

void SelectTool::onMoveMouse(QMouseEvent *eventMove, QVector<Layer>& layers)
{
    auto currPos = eventMove->pos();
    if(!mouseDown) return;
    for(auto riter = layers.rbegin(); riter!=layers.rend(); riter++)
    {
        auto& l = *riter;
        if(l.contains(currPos))
        {
            if(editFrame->isSpecialTool())
                makeDeselectionAt(currPos/editFrame->getZoom(), l.getImg(), l.getPos());
            else
                makeSelectionAt(currPos/editFrame->getZoom(), l.getImg(), l.getPos());
            break;
        }
    }
}

void SelectTool::onReleaseMouse(QMouseEvent *releaseEvent)
{
    mouseDown = false;
}

void SelectTool::onLeaveMouse(QHoverEvent *hoverEvent)
{
    onReleaseMouse(reinterpret_cast<QMouseEvent*>(hoverEvent));
}
void SelectTool::rerouteEvent(QEvent *event, QVector<Layer>& layers)
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

void SelectTool::makeSelectionAt(const QPoint& from, const QImage& img, const QPoint& realPos)
{
    auto pixels = reinterpret_cast<const QRgb*>(img.constBits());
    auto selectedAreaGrid = selectedArea->getSelectedArea();
    auto x = from.x()-realPos.x(), y = from.y()-realPos.y();

    QStack<tuple<int, int, QRgb, int, int>> stack;
    const auto brushSize = (*brushShape)->getSize();
    auto currColor = pixels[x+y*img.width()];
    stack.push(make_tuple(x-1,y,currColor, x, y));
    stack.push(make_tuple(x+1,y,currColor, x, y));
    stack.push(make_tuple(x,y-1,currColor, x, y));
    stack.push(make_tuple(x,y+1,currColor, x, y));
    const auto maxDist = brushSize * 2;

    auto memo = new bool[img.width()*img.height()];
    memset(memo, false, img.width()*img.height());
    auto area = 0;
    if(selectedAreaGrid[from.x() + from.y()*selectedArea->getSize()] != 1)
    {
        area++;
    }
    selectedAreaGrid[from.x() + from.y()*selectedArea->getSize()] = 1;
    memo[x+y*img.width()] = true;
    const auto startPos = QPoint(x,y);
    while(!stack.empty())
    {
        auto curr = stack.pop();
        const auto x = get<0>(curr), y = get<1>(curr);
        auto prevColor = get<2>(curr);
        if(sqrt(abs(x-startPos.x())*abs(x-startPos.x()) + abs(y-startPos.y())*abs(y-startPos.y()))>=maxDist ||
                x>=img.width() ||
                y>=img.height() ||
                x<0 || y<0)
        {
            if(selectedAreaGrid[x+realPos.x() + (y+realPos.y())*selectedArea->getSize()]==0)
            {
                selectedAreaGrid[get<3>(curr)+realPos.x() + (get<4>(curr)+realPos.y())*selectedArea->getSize()] = 2;
            }
            continue;
        }
        if(memo[x+y*img.width()]) continue;
        auto currColor = pixels[x+y*img.width()];
        auto diff = ImageAlgorithms::colorDifference(currColor, prevColor);
        if(diff < 40 || sqrt(abs(x-startPos.x())*abs(x-startPos.x()) + abs(y-startPos.y())*abs(y-startPos.y())) < brushSize/2)
        {
            if(selectedAreaGrid[x + realPos.x() + (y+realPos.y())*selectedArea->getSize()]!=1)
            {
                area++;
            }
            selectedAreaGrid[x + realPos.x() + (y+realPos.y())*selectedArea->getSize()] = 1;
            memo[x+y*img.width()] = true;
            stack.push(make_tuple(x-1,y,currColor, x, y));
            stack.push(make_tuple(x+1,y,currColor, x, y));
            stack.push(make_tuple(x,y-1,currColor, x, y));
            stack.push(make_tuple(x,y+1,currColor, x, y));
        }
        else if(selectedAreaGrid[x+realPos.x() + (y+realPos.y())*selectedArea->getSize()]==0)
        {
            selectedAreaGrid[get<3>(curr)+realPos.x() + (get<4>(curr)+realPos.y())*selectedArea->getSize()] = 2;
        }
    }
    delete[] memo;
    selectedArea->change(QRect(startPos - QPoint(maxDist, maxDist), QSize(maxDist*2, maxDist*2)), selectedArea->getPixelCount()+area);
    editFrame->update();
}

void SelectTool::makeDeselectionAt(const QPoint& from, const QImage& img, const QPoint& realPos)
{
    auto pixels = reinterpret_cast<const QRgb*>(img.constBits());
    auto selectedAreaGrid = selectedArea->getSelectedArea();
    auto x = from.x()-realPos.x(), y = from.y()-realPos.y();

    QStack<tuple<int, int, QRgb, int, int>> stack;
    const auto brushSize = (*brushShape)->getSize();
    auto currColor = pixels[x+y*img.width()];
    stack.push(make_tuple(x-1,y,currColor, x, y));
    stack.push(make_tuple(x+1,y,currColor, x, y));
    stack.push(make_tuple(x,y-1,currColor, x, y));
    stack.push(make_tuple(x,y+1,currColor, x, y));
    const auto maxDist = brushSize * 3/2;

    auto memo = new bool[img.width()*img.height()];
    memset(memo, false, img.width()*img.height());
    auto area = 0;
    if(selectedAreaGrid[from.x() + from.y()*selectedArea->getSize()] == 1)
    {
        area--;
    }
    selectedAreaGrid[from.x() + from.y()*selectedArea->getSize()] = 0;
    memo[x+y*img.width()] = true;
    const auto startPos = QPoint(x,y);
    while(!stack.empty())
    {
        auto curr = stack.pop();
        const auto x = get<0>(curr), y = get<1>(curr);
        auto prevColor = get<2>(curr);
        if(sqrt(abs(x-startPos.x())*abs(x-startPos.x()) + abs(y-startPos.y())*abs(y-startPos.y()))>=maxDist ||
                x>=img.width() ||
                y>=img.height() ||
                x<0 || y<0)
        {
            if(selectedAreaGrid[x+realPos.x() + (y+realPos.y())*selectedArea->getSize()]==1)
            {
                selectedAreaGrid[get<3>(curr)+realPos.x() + (get<4>(curr)+realPos.y())*selectedArea->getSize()] = 2;
            }
            continue;
        }
        if(memo[x+y*img.width()]) continue;
        auto currColor = pixels[x+y*img.width()];
        auto diff = ImageAlgorithms::colorDifference(currColor, prevColor);
        if(diff < 20 || sqrt(abs(x-startPos.x())*abs(x-startPos.x()) + abs(y-startPos.y())*abs(y-startPos.y())) < brushSize/2)
        {
            if(selectedAreaGrid[x + realPos.x() + (y+realPos.y())*selectedArea->getSize()]==1)
            {
                area--;
            }
            selectedAreaGrid[x + realPos.x() + (y+realPos.y())*selectedArea->getSize()] = 0;
            memo[x+y*img.width()] = true;
            stack.push(make_tuple(x-1,y,currColor, x, y));
            stack.push(make_tuple(x+1,y,currColor, x, y));
            stack.push(make_tuple(x,y-1,currColor, x, y));
            stack.push(make_tuple(x,y+1,currColor, x, y));
        }
        else if(selectedAreaGrid[x+realPos.x() + (y+realPos.y())*selectedArea->getSize()]==1)
        {
            selectedAreaGrid[get<3>(curr)+realPos.x() + (get<4>(curr)+realPos.y())*selectedArea->getSize()] = 2;
        }
    }
    delete[] memo;
    selectedArea->change(QRect(startPos - QPoint(maxDist, maxDist), QSize(maxDist*2, maxDist*2)), selectedArea->getPixelCount()+area);
    editFrame->update();
}
