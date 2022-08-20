#include "editframe.h"


EditFrame::EditFrame(QWidget *parent)
    :QFrame(parent)
{
    setMouseTracking(true);
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
    //qDebug() << maxX << maxY;
    if(!quick) selectedArea.resize(maxX/zoom, maxY/zoom);
}

void EditFrame::setImg(QString path)
{
    layers.append(Layer(path, layers.size()));
    adjustSize();
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
