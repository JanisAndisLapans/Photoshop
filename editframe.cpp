#include "editframe.h"

EditFrame::EditFrame(QWidget *parent)
    :QFrame(parent)
{
    setMouseTracking(true);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EditFrame::selectionDisplaySwitch);
    timer->start(750);
    setFocusPolicy(Qt::StrongFocus);
    setAcceptDrops(true);
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
        auto pos = l->getPos();
        auto xr = static_cast<int>(pos.x() + l->width()*zoom);
        auto yr = static_cast<int>(pos.y() + l->height()*zoom);

        maxX = max(maxX, xr);
        maxY = max(maxY, yr);
    }
    setMinimumWidth(maxX);
    setMinimumHeight(maxY);
    if(!quick) selectedArea.resize(maxX/zoom, maxY/zoom);
}

void EditFrame::addImages(const QVector<QString>& paths)
{
    selectedArea.unselectAll();
    for(const auto& l : layers)
        if(l->isTransforming()) return;
    for(auto& l : layers)
        l->setSelected(false);


    QVector<Layer*> transforming;
    for(const auto& path : paths)
    {
        Layer *l = new Layer(path);
        layers.append(l);
        l->setSelected(true);
        transforming.append(l);
    }
    ttool->startLayerTransform(transforming);
    prevTool = currTool;
    enableTool(ttool);
    adjustSize();
    update();
}

void EditFrame::addImg(QString path)
{
    selectedArea.unselectAll();
    for(const auto& l : layers)
        if(l->isTransforming()) return;
    for(auto& l : layers)
        l->setSelected(false);

    auto l = new Layer(path);
    layers.append(l);
    l->setSelected(true);
    ttool->startLayerTransform({l});
    prevTool = currTool;
    enableTool(ttool);
    adjustSize();
    update();
}

void EditFrame::addSolidLayer(QColor color, QSize size)
{
    static auto solidLayerNumber = 1;

    selectedArea.unselectAll();
    for(const auto& l : layers)
        if(l->isTransforming()) return;
    for(auto& l : layers)
        l->setSelected(false);

    QImage img(size,QImage::Format_ARGB32);
    img.fill(color);

    auto l = new Layer(img, "Solid layer " + QString::number(solidLayerNumber));
    solidLayerNumber++;
    layers.append(l);
    l->setSelected(true);
    ttool->startLayerTransform({l});
    prevTool = currTool;
    enableTool(ttool);
    adjustSize();
    update();
}

double EditFrame::getZoom() const
{
    return zoom;
}

void EditFrame::drawResizeBall(int size, const QPoint& resizeBallCenter)
{
    drawingResizeBall = true;
    this->centerResizeBall = resizeBallCenter;
    resizeBallSize = size;
    update();
}

void EditFrame::finishDrawingResizeBall()
{
    drawingResizeBall = false;
    update();
}

void EditFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    bool isTransforming = false;
    for(const auto& l : layers)
    {
        isTransforming = l->isTransforming();
        if(isTransforming) break;
    }
    painter.setPen(Qt::transparent);
    auto notTransformingForeground = QBrush(QColor(122,122,122,122));
    for(const auto& l : layers)
    {
        auto resizedLayer = *l;
        resizedLayer.setSize(resizedLayer.size()*zoom);
        auto xc = resizedLayer.width() * 0.5  + resizedLayer.topLeft().x();
        auto yc = resizedLayer.height() * 0.5 + resizedLayer.topLeft().y();
        painter.translate(xc, yc);
        painter.rotate(l->getRotationDegrees());
        resizedLayer.translate(QPoint(-xc, -yc));
        painter.drawImage(resizedLayer,l->getImg());
        if(isTransforming && !l->isTransforming())
        {
            painter.fillRect(resizedLayer, notTransformingForeground);
        }
        painter.resetTransform();
    }
    if(hasSelectedArea())
    {
        auto img = selectedAreaDisplayState ? selectedArea.getContourImg1() : selectedArea.getContourImg2();
        painter.drawImage(QRect(0,0,img.width()*zoom, img.height()*zoom), img);
    }
    if(drawingResizeBall)
    {
        painter.setBrush(Qt::red);
        painter.drawEllipse(centerResizeBall, resizeBallSize, resizeBallSize);
    }
    if(isTransforming)
    {
        auto rect =  ttool->getWorkedAreaRect();
        rect.setSize(rect.size()*zoom);
        auto xc = rect.width() * 0.5 + rect.topLeft().x();
        auto yc = rect.height() * 0.5 + rect.topLeft().y();
        painter.translate(xc, yc);
        painter.rotate(ttool->getWorkedAreaRectRotation());
        rect.translate(QPoint(-xc, -yc));
        painter.setPen(QPen(Qt::blue,2));
        painter.drawRect(rect);
        painter.resetTransform();
    }
}

QVector<Layer*>* EditFrame::getLayersRef()
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


void EditFrame::changeZoom(double amount)
{
    zoom = amount;
    adjustSize(true);
    update();
}

void EditFrame::mouseMoveEvent(QMouseEvent * ev)
{
    setFocus();
}

void EditFrame::lookAt(QPoint point)
{
    scrollArea->ensureVisible(point.x(),point.y());
}

void EditFrame::setScrollArea(QScrollArea *scrollArea)
{
    this->scrollArea = scrollArea;
}

void EditFrame::dragEnterEvent(QDragEnterEvent *ev)
{
    if(ev->mimeData()->hasUrls())
        ev->accept();
    else
        ev->ignore();
}
void EditFrame::dragMoveEvent(QDragMoveEvent *ev)
{
    if(ev->mimeData()->hasUrls())
    {
        ev->setDropAction(Qt::CopyAction);
        ev->accept();
    }
    else
        ev->ignore();
}
void EditFrame::dropEvent(QDropEvent *ev)
{
    if(ev->mimeData()->hasUrls())
    {
        ev->setDropAction(Qt::CopyAction);
        ev->accept();
        static const auto acceptedTypes = QVector<QString>{"png", "jpg", "bmp"};

        QVector<QString> imagesToAdd;
        for(auto& url : ev->mimeData()->urls())
        {
            if(url.isLocalFile() && acceptedTypes.contains(QFileInfo(url.fileName()).completeSuffix()))
            {
                imagesToAdd.append(url.toLocalFile());
            }
        }
        addImages(imagesToAdd);
    }
}

void EditFrame::setTransformTool(TransformTool* tool)
{
    this->ttool = tool;
    connect(tool, SIGNAL(endTransform()), this, SLOT(onEndTransform()));
}


void EditFrame::enableTool(EditTool* tool)
{
    if(tool != ttool) ttool->terminateTransform();

    if(currTool!=nullptr)
    {
        currTool->getMenu()->setVisible(false);
        removeEventFilter(currTool);
    }
    currTool = tool;
    currTool->getMenu()->setVisible(true);
    currTool->setCursor();
    installEventFilter(currTool);
}

void EditFrame::onEndTransform()
{
    enableTool(prevTool);
}

void EditFrame::keyPressEvent(QKeyEvent* event)
{
    if(currTool == ttool) return;

    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_T)
    {
        QVector<Layer*> layersToTransform;
        for(auto layer : layers)
        {
            if(layer->isSelected())
            {
                layersToTransform.append(layer);
            }
        }
        ttool->startLayerTransform(layersToTransform);
        prevTool = currTool;
        enableTool(ttool);
    }
}


