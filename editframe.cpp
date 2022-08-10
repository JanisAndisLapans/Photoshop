#include "editframe.h"


EditFrame::EditFrame(QWidget *parent)
    :QFrame(parent)
{
    setMouseTracking(true);
    adjustBrushCursor();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EditFrame::selectionDisplaySwitch);
    timer->start(750);
    setFocusPolicy(Qt::StrongFocus);

}

void EditFrame::selectionDisplaySwitch()
{
    if(hasSelectedArea())
    {
        selectedAreaDisplayState = !selectedAreaDisplayState;
        update();
    }
}

void EditFrame::adjustSize(bool quick)
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
    if(!quick) selectedArea.resize(maxX, maxY);
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
    int zoomedBrushSize = brushShape->getSize()*zoom;
    auto img = brushShape->getImg().scaled(zoomedBrushSize, zoomedBrushSize, Qt::KeepAspectRatio);
    brushCursor = QCursor(QPixmap::fromImage(img), 0, 0);
    if(tool == Brush)
    {
        setCursor(brushCursor);
    }

    QImage selectImg(zoomedBrushSize+1, zoomedBrushSize+1, QImage::Format_ARGB32);
    selectImg.fill(Qt::transparent);
    QPainter painter(&selectImg);
    painter.setPen(Qt::black);
    painter.drawEllipse(QPoint(zoomedBrushSize/2, zoomedBrushSize/2), zoomedBrushSize/2, zoomedBrushSize/2);
    painter.drawLine(zoomedBrushSize/6, zoomedBrushSize/2, zoomedBrushSize - zoomedBrushSize/6, zoomedBrushSize/2);
    selectionSubCursor = QCursor(QPixmap::fromImage(selectImg));
    painter.drawLine(zoomedBrushSize/2, zoomedBrushSize/6, zoomedBrushSize/2, zoomedBrushSize - zoomedBrushSize/6);
    selectionAddCursor = QCursor(QPixmap::fromImage(selectImg));
    if(tool == Selection)
    {
        if(toolSpecial)
            setCursor(selectionSubCursor);
        else
            setCursor(selectionAddCursor);

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
        case Selection:
            setCursor(selectionAddCursor);
            editTool = make_unique<SelectTool>(this, &brushShape);
            break;
    }

}

bool EditFrame::event(QEvent *event)
{
    if(editTool)
    {
        editTool->rerouteEvent(event, layers);
    }
    if(event->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Alt)
    {
        toolSpecial = true;
        if(tool == Selection) setCursor(selectionSubCursor);
    }
    if(event->type() == QEvent::KeyRelease && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Alt)
    {
        toolSpecial = false;
        if(tool == Selection) setCursor(selectionAddCursor);
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
    if(hasSelectedArea())
    {
        auto img = selectedAreaDisplayState ? selectedArea.getContourImg1() : selectedArea.getContourImg2();
        painter.drawImage(QRect(0,0,img.width()*zoom, img.height()*zoom), img);
    }
}

#define ZOOM_DIST_RATIO 200.0

void EditFrame::mouseMoveEvent(QMouseEvent *event)
{   
    setFocus();
    if(tool == Resize && mouseDown)
    {
        auto mouseNow = event->pos();
        auto dist = mouseNow - lastMouse;
        auto totalDist = dist.y() - dist.x();
        auto zoomChange = totalDist / ZOOM_DIST_RATIO;
        zoom += zoomChange;
        update();
        adjustSize(true);
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
        adjustBrushCursor();
    }
}

QVector<Layer>* EditFrame::getLayersRef()
{
    return &layers;
}

SelectedArea* EditFrame::getSelectedAreaRef()
{
    return &selectedArea;
}

bool EditFrame::hasSelectedArea() const
{
    return selectedArea.hasSelected();
}

bool EditFrame::isSpecialTool() const
{
    return toolSpecial;
}

