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
    for(const auto& l : layers)
    {
        auto pos = l.getPos();
        auto xr = static_cast<int>(pos.x() + l.width()*zoom);
        auto yr = static_cast<int>(pos.y() + l.height()*zoom);

        maxX = max(maxX, xr);
        maxY = max(maxY, yr);
    }

    setMinimumWidth(maxX);
    setMinimumHeight(maxY);
}

void EditFrame::setImg(QString path)
{
    layers.append(Layer(path, layers.size()));
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
    auto img = brushShape->getImg();
    brushCursor = QCursor(QPixmap::fromImage(img), 0, 0);
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
                brushShape = make_unique<BrushSquare>(brushShape->getSize(), brushShape->getColor());
            //else
               // brushShape = make_shared<BrushSquare>(brushShape->getSize(), brushShape->getSrcImg());
            break;

        case Circle:
            if(brushShape->isColorSrc())
                brushShape = make_unique<BrushCircle>(brushShape->getSize(), brushShape->getColor());
            //else
               // brushShape = make_shared<BrushCircle>(brushShape->getSize(), brushShape->getSrcImg());
            break;

        case FadedCircle:
            if(brushShape->isColorSrc())
                brushShape = make_unique<BrushFadedCircle>(brushShape->getSize(), brushShape->getColor());
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
            editTool = make_unique<BrushTool>(this, &brushShape);
            break;
        case Resize:
            setCursor(Qt::SizeBDiagCursor);
            editTool = nullptr;
            break;
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

void EditFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    for(const auto& l : layers)
    {
        auto resizedLayer = l;
        resizedLayer.setSize(resizedLayer.size()*zoom);
        painter.drawImage(resizedLayer,l.getImg());
    }
}

void EditFrame::changeZoomBy(double amount)
{
    zoom += amount;
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

QVector<Layer>* EditFrame::getLayersRef()
{
    return &layers;
}
