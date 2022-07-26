#include "editframe.h"


EditFrame::EditFrame(QWidget *parent)
    :QFrame(parent)
{
    setMouseTracking(true);
    adjustBrushCursor();
}

void EditFrame::adjustSize()
{
    auto maxX = -1, maxY = -1;
    for(const Layer& l : layers)
    {
        auto pos = l.getPos();
        auto xr = pos.x() + l.width();
        auto yr = pos.y() + l.height();

        maxX = max(maxX, xr);
        maxY = max(maxY, yr);
    }

    setMinimumWidth(maxX);
    setMinimumHeight(maxY);
}

void EditFrame::setImg(QString path)
{
    layers.append(Layer(path));
    adjustSize();
}

void EditFrame::setBrushSize(int size)
{
    brushShape->resize(size);
    adjustBrushCursor();
}

void EditFrame::setBrushColor(const QColor& color)
{
    brushShape->changeSource(color);
    adjustBrushCursor();
}

void EditFrame::adjustBrushCursor()
{
    const auto sourceImg = brushShape->getImg();
    auto image = QPixmap(sourceImg.size()*(zoom*2.11));
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setPen(Qt::transparent);
    auto brushAdjustedToCorner = sourceImg.rect();
    brushAdjustedToCorner.translate(brushAdjustedToCorner.bottomRight()*(zoom*1.11));
    painter.drawImage(brushAdjustedToCorner, sourceImg);
    brushCursor = QCursor(image);
    if(tool == Brush)
    {
        setCursor(brushCursor);
    }
}

void EditFrame::setBrushShape(BrushType type)
{
    this->brushType = type;
    switch(type)
    {
        case Square:
            if(brushShape->isColorSrc())
                *brushShape = BrushSquare(brushShape->getSize(), brushShape->getColor());
            //else
               // brushShape = make_shared<BrushSquare>(brushShape->getSize(), brushShape->getSrcImg());
            break;

        case Circle:
            qDebug() <<"123";
            if(brushShape->isColorSrc())
                *brushShape = BrushCircle(brushShape->getSize(), brushShape->getColor());
            //else
               // brushShape = make_shared<BrushCircle>(brushShape->getSize(), brushShape->getSrcImg());
            break;

        case FadedCircle:
            if(brushShape->isColorSrc())
                *brushShape = BrushFadedCircle(brushShape->getSize(), brushShape->getColor());
            //else
               // brushShape = make_shared<BrushFadedCircle>(brushShape->getSize(), brushShape->getSrcImg());
            break;
    }
    adjustBrushCursor();
}

void EditFrame::setTool(Tools tool)
{
    this->tool = tool;
    switch(tool)
    {
        case Cursor:
            setCursor(Qt::ArrowCursor);
            editTool = nullptr;
            break;
        case Move:
            setCursor(Qt::SizeAllCursor);
            editTool = make_unique<MoveTool>(this);
            break;
        case Brush:
            setCursor(brushCursor);
            editTool = make_unique<BrushTool>(this, brushShape);
            break;
        case Resize:
            setCursor(Qt::SizeBDiagCursor);
            editTool = nullptr;
            break;
    }

}

void EditFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    for(const Layer& l : layers)
    {
        painter.drawImage(l,l.getImg());
    }
}


bool EditFrame::event(QEvent *event)
{
    if(editTool)
    {
        editTool->rerouteEvent(event, layers);
    }

    return QWidget::event(event);
}

double EditFrame::getZoom() const
{
    return zoom;
}

void EditFrame::changeZoomBy(double amount)
{
    zoom += amount;
    for(Layer& l : layers)
    {
        auto newDims = l.getImgRef()->size()*zoom;
        l.setSize(newDims);
    }
    update();
    adjustSize();
    adjustBrushCursor();
}

#define ZOOM_DIST_RATIO 200.0

void EditFrame::mouseMoveEvent(QMouseEvent *event)
{   
    if(tool == Resize && mouseDown)
    {
        auto mouseNow = event->pos();
        auto dist = mouseNow - lastMouse;
        auto totalDist = dist.y() - dist.x();
        auto zoomChange = totalDist / ZOOM_DIST_RATIO;
        changeZoomBy(zoomChange);
    }
    lastMouse = event->pos();
}

void EditFrame::mousePressEvent(QMouseEvent *event)
{
    if(tool == Resize)
    {
        mouseDown = true;
    }
}

void EditFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if(tool == Resize && mouseDown)
    {
        mouseDown = false;
    }
}
