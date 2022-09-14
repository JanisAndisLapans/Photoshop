#include "transformtool.h"
#include "editframe.h"


TransformTool::TransformTool(EditFrame* editFrame)
   : EditTool(editFrame)
{
    menu = new TransformToolMenu();
    connect(menu, SIGNAL(cancelTransform()), this, SLOT(onCancel()));
    connect(menu, SIGNAL(finishTransform()), this, SLOT(onFinish()));
    connect(menu, SIGNAL(rotated(double)), this, SLOT(onRotationChanged(double)));
    connect(menu, SIGNAL(dimsChanged(const QSize&)), this, SLOT(onDimsChanged(const QSize&)));
    connect(menu, SIGNAL(posChanged(const QPoint&)), this, SLOT(onPosChanged(const QPoint&)));
}

TransformToolMenu* TransformTool::getMenu()
{
    return menu;
}

void TransformTool::setCursor()
{
    editFrame->setCursor(Qt::ArrowCursor);
}

bool TransformTool::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::MouseMove:
            onMoveMouse(dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonPress:
            onDownMouse(dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            onReleaseMouse(dynamic_cast<QMouseEvent*>(event));
            break;    
    }
    return QObject::eventFilter(obj, event);
}

void TransformTool::startLayerTransform(const QVector<Layer*>& layers)
{
    int minTop = INT_MAX, minLeft = INT_MAX, maxBot = 0, maxRight = 0;
    for(auto& l : layers)
    {
        minTop = min(l->top(), minTop);
        minLeft = min(l->left(), minLeft);
        maxBot = max(l->bottom(), maxBot);
        maxRight = max(l->right(), maxRight);
    }
    transformingRect = QRect(minLeft, minTop,0,0);
    transformingRect.setBottom(maxBot);
    transformingRect.setRight(maxRight);
    baseTransformingRect = transformingRect;
    originalTransformingRect = transformingRect;
    transformingRectRotation = 0.0;
    menu->setRotation(0.0);
    menu->setSize(transformingRect.size());
    menu->setPos(transformingRect.topLeft());
    currType = Resize;
    for(auto& l : layers)
    {
        l->setTransforming(true);
        LayerInfo linfo{l, *l, *l};
        transformingLayers.append(linfo);
    }
}

void TransformTool::onReleaseMouse(QMouseEvent *event)
{
    if(transforming)
    {
        transforming = false;
        editFrame->adjustSize();
        baseTransformingRect = transformingRect;
        for(auto& layer : transformingLayers)
        {
            layer.resizingBase = *layer.layer;
        }
        for(auto layer : transformingLayers)
        {
            layer.layer->setTransforming(true);
        }
    }
}
void TransformTool::onDownMouse(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        QMenu contextMenu("Action type", editFrame);

        QAction action1("Resize", editFrame);
        connect(&action1, SIGNAL(triggered()), this, SLOT(onChooseResize()));
        contextMenu.addAction(&action1);
        QAction action2("Move", editFrame);
        connect(&action2, SIGNAL(triggered()), this, SLOT(onChooseMove()));
        contextMenu.addAction(&action2);
        QAction action3("Rotate", editFrame);
        connect(&action3, SIGNAL(triggered()), this, SLOT(onChooseRotate()));
        contextMenu.addAction(&action3);

        contextMenu.exec(QCursor::pos());
        return;
    }
    if(transformingAvailable)
    {
        transforming = true;
        if(currType == Rotate)
        {
            startMouse = event->pos()/editFrame->getZoom();
            for(auto layer : transformingLayers)
            {
                layer.layer->setTransforming(true);
                layer.prevRotation = layer.layer->getRotationDegrees();
            }
        }
        else if(currType == Move)
        {
            startMouse = event->pos();
        }
        else if(currType == Resize)
        {
            baseTransformingRect = transformingRect;
            for(auto layer : transformingLayers)
            {
                layer.resizingBase = *layer.layer;
            }
        }
    }
}

void TransformTool::onMoveMouse(QMouseEvent *event)
{
    auto posNoZoom = event->pos() / editFrame->getZoom();
    auto pos = ImageAlgorithms::rotatePos(posNoZoom, transformingRectRotation, transformingRect.center());
    switch(currType){
        case Resize:
            if(!transforming)
            {
                auto activateTreshhold = 10;
                if(ImageAlgorithms::pointDistance(pos, transformingRect.topLeft()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::topLeftResize, this, _1);
                    editFrame->setCursor(Qt::SizeFDiagCursor);
                }
                else if(ImageAlgorithms::pointDistance(pos, transformingRect.topRight()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::topRightResize, this, _1);
                    editFrame->setCursor(Qt::SizeBDiagCursor);
                }
                else if(ImageAlgorithms::pointDistance(pos, transformingRect.bottomLeft())  < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::bottomLeftResize, this, _1);
                    editFrame->setCursor(Qt::SizeBDiagCursor);
                }
                else if(ImageAlgorithms::pointDistance(pos, transformingRect.bottomRight())  < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::bottomRightResize, this, _1);
                    editFrame->setCursor(Qt::SizeFDiagCursor);
                }
                else if(abs(pos.x() - transformingRect.left()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::leftResize, this, _1);
                    editFrame->setCursor(Qt::SizeHorCursor);
                }
                else if(abs(pos.x() - transformingRect.right()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::rightResize, this, _1);
                    editFrame->setCursor(Qt::SizeHorCursor);
                }
                else if(abs(pos.y() - transformingRect.top()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::topResize, this, _1);
                    editFrame->setCursor(Qt::SizeVerCursor);
                }
                else if(abs(pos.y() - transformingRect.bottom()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::bottomResize, this, _1);
                    editFrame->setCursor(Qt::SizeVerCursor);
                }
                else
                {
                    transformingAvailable = false;
                    editFrame->setCursor(Qt::ArrowCursor);
                    return;
                }
                transformingAvailable = true;
            }
            else
            {
                currResizeMethod(pos);
                menu->setSize(transformingRect.size());
                menu->setPos(transformingRect.topLeft());
                editFrame->lookAt(event->pos());
                editFrame->adjustSize(true);
                editFrame->update();
            }
            break;
        case Move:
            if(!transforming)
            {
                if(transformingRect.contains(pos))
                {
                    editFrame->setCursor(Qt::SizeAllCursor);
                    transformingAvailable = true;
                }
                else
                {
                    editFrame->setCursor(Qt::ArrowCursor);
                    transformingAvailable = false;
                }
            }
            else
            {
                auto dif = event->pos() - startMouse;
                startMouse = event->pos();
                for(auto& layer : transformingLayers)
                {
                    auto newPos = layer.layer->getPos() + dif;
                    newPos.setX(max(newPos.x(),0));
                    newPos.setY(max(newPos.y(),0));
                    layer.layer->setPos(newPos);
                }
                auto newPos = transformingRect.topLeft() + dif;
                newPos.setX(max(newPos.x(),0));
                newPos.setY(max(newPos.y(),0));
                transformingRect.translate(newPos - transformingRect.topLeft());
                menu->setPos(transformingRect.topLeft());
                editFrame->adjustSize(true);
                editFrame->update();
            }
            break;
        case Rotate:
            auto activateTreshhold = 30/editFrame->getZoom();
            if(!transforming)
            {
               if(abs(pos.x() - transformingRect.left()) < activateTreshhold)
               {
                   editFrame->setCursor(clockwiseRotateCursor);
               }
               else if(abs(pos.x() - transformingRect.right()) < activateTreshhold)
               {
                   editFrame->setCursor(anticlockwiseRotateCursor);
               }
               else if(abs(pos.y() - transformingRect.top()) < activateTreshhold)
               {
                   editFrame->setCursor(clockwiseRotateCursor);
               }
               else if(abs(pos.y() - transformingRect.bottom()) < activateTreshhold)
               {
                   editFrame->setCursor(anticlockwiseRotateCursor);
               }
               else
               {
                   transformingAvailable = false;
                   editFrame->setCursor(Qt::ArrowCursor);
                   return;
               }
               transformingAvailable = true;
            }
            else
            {
                auto amount = ImageAlgorithms::angleBetweenPoints(startMouse, posNoZoom, transformingRect.center());
                transformingRectRotation += amount;
                for(auto& layer : transformingLayers)
                {
                    layer.layer->setRotationDegress(layer.layer->getRotationDegrees() + amount);
                }
                startMouse = posNoZoom;
                editFrame->update();
                menu->setRotation(transformingRectRotation);
            }
            break;
    }
}


void TransformTool::topLeftResize(QPoint pos)
{ 
    if(pos.x() < 0  || pos.y() <0)
    {
        auto shortest = min(baseTransformingRect.top(), baseTransformingRect.left());
        pos = QPoint(baseTransformingRect.left() - shortest, baseTransformingRect.top() - shortest);
    }
    pos.setX(min(pos.x(), baseTransformingRect.bottomRight().x()));
    pos.setY(min(pos.y(), baseTransformingRect.bottomRight().y()));
    const auto max = ImageAlgorithms::pointDistance(baseTransformingRect.topLeft(), baseTransformingRect.bottomRight());
    auto current = ImageAlgorithms::pointDistance(pos, baseTransformingRect.bottomRight());
    auto percentage = static_cast<double>(current)/max;
    auto percentageReverse = 1.0 - percentage;
    transformingRect.setSize(QSize(baseTransformingRect.width()*percentage, baseTransformingRect.height()*percentage));
    transformingRect.translate(-transformingRect.topLeft() + baseTransformingRect.topLeft() + QPoint(baseTransformingRect.width(), baseTransformingRect.height())*percentageReverse);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        auto offset = (orig.topLeft() - baseTransformingRect.topLeft())*percentage;

        curr->setSize(QSize(orig.width()*percentage, orig.height()*percentage));
        curr->setPos(offset + baseTransformingRect.topLeft() + QPoint(baseTransformingRect.width(), baseTransformingRect.height())*percentageReverse);
    }
}

void TransformTool::topRightResize(QPoint pos)
{
    if(pos.y() <0)
    {
        const auto proportion = static_cast<double>(baseTransformingRect.width())/baseTransformingRect.height();
        pos = QPoint(baseTransformingRect.right() + baseTransformingRect.top()*proportion, 0);
    }
    pos.setX(max(pos.x(), baseTransformingRect.bottomLeft().x()));
    pos.setY(min(pos.y(), baseTransformingRect.bottomLeft().y()));
    const auto max = ImageAlgorithms::pointDistance(baseTransformingRect.topRight(), baseTransformingRect.bottomLeft());
    auto current = ImageAlgorithms::pointDistance(pos, baseTransformingRect.bottomLeft());
    auto percentage = static_cast<double>(current)/max;
    auto percentageReverse = 1.0 - percentage;

    transformingRect.setSize(QSize(baseTransformingRect.width()*percentage, baseTransformingRect.height()*percentage));
    transformingRect.translate(-transformingRect.topLeft() + QPoint(transformingRect.x(),baseTransformingRect.top() + baseTransformingRect.height()*percentageReverse));

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        auto offset = (orig.topLeft() - baseTransformingRect.topLeft())*percentage;

        curr->setSize(QSize(orig.width()*percentage, orig.height()*percentage));
        curr->setPos(offset + QPoint(transformingRect.x(), baseTransformingRect.top() + baseTransformingRect.height()*percentageReverse));
    }
}

void TransformTool::bottomLeftResize(QPoint pos)
{
    if(pos.x() < 0)
    {
        const auto proportion = static_cast<double>(baseTransformingRect.height())/baseTransformingRect.width();
        pos = QPoint(0, baseTransformingRect.bottom() + baseTransformingRect.left()*proportion);
    }
    pos.setX(min(pos.x(), baseTransformingRect.topRight().x()));
    pos.setY(max(pos.y(), baseTransformingRect.topRight().y()));
    const auto max = ImageAlgorithms::pointDistance(baseTransformingRect.topRight(), baseTransformingRect.bottomLeft());
    auto current = ImageAlgorithms::pointDistance(pos, baseTransformingRect.topRight());
    auto percentage = static_cast<double>(current)/max;
    auto percentageReverse = 1.0 - percentage;

    transformingRect.setSize(baseTransformingRect.size()*percentage);
    transformingRect.translate(-transformingRect.topLeft() + QPoint(baseTransformingRect.left() + baseTransformingRect.width()*percentageReverse,transformingRect.y()));

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        auto offset = (orig.topLeft() - baseTransformingRect.topLeft())*percentage;

        curr->setSize(orig.size() * percentage);
        curr->setPos(offset + QPoint(baseTransformingRect.left() + baseTransformingRect.width()*percentageReverse,transformingRect.y()));
    }
}

void TransformTool::bottomRightResize(QPoint pos)
{
    pos.setX(max(pos.x(), baseTransformingRect.topLeft().x()));
    pos.setY(max(pos.y(), baseTransformingRect.topLeft().y()));
    const auto max = ImageAlgorithms::pointDistance(baseTransformingRect.topLeft(), baseTransformingRect.bottomRight());
    auto current = ImageAlgorithms::pointDistance(pos, baseTransformingRect.topLeft());
    auto percentage = static_cast<double>(current)/max;
//  auto percentageReverse = 1.0 - percentage;

    transformingRect.setSize(baseTransformingRect.size()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
//      auto offset = (orig.topLeft() - baseTransformingRect.topLeft())*percentage;
        curr->setSize(orig.size()*percentage);
    }
}

void TransformTool::leftResize(QPoint pos)
{
    pos.setX(min(max(pos.x(), 0), baseTransformingRect.right()));
    const auto max = baseTransformingRect.right() - baseTransformingRect.left();
    auto current =  pos.x() - baseTransformingRect.left();
    auto percentage = static_cast<double>(current)/max;

    transformingRect.setLeft(baseTransformingRect.left() + baseTransformingRect.width()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        curr->setLeft(orig.left() + orig.width()*percentage);
    }
}

void TransformTool::rightResize(QPoint pos)
{
    pos.setX(max(pos.x(), baseTransformingRect.left()));
    const auto max = baseTransformingRect.right() - baseTransformingRect.left();
    auto current = baseTransformingRect.right() - pos.x();
    auto percentage = 1.0 - static_cast<double>(current)/max;

    transformingRect.setRight(baseTransformingRect.left() + baseTransformingRect.width()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        curr->setRight(orig.left() + orig.width()*percentage);
    }
}
void TransformTool::topResize(QPoint pos)
{
    pos.setY(max(min(pos.y(), baseTransformingRect.bottom()), 0));
    const auto max = baseTransformingRect.bottom() - baseTransformingRect.top();
    auto current = pos.y() - baseTransformingRect.top();
    auto percentage = static_cast<double>(current)/max;

    transformingRect.setTop(baseTransformingRect.top() + baseTransformingRect.height()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        curr->setTop(orig.top() + orig.height()*percentage);
    }
}
void TransformTool::bottomResize(QPoint pos)
{
    pos.setY(max(pos.y(), baseTransformingRect.top()));
    const auto max = baseTransformingRect.bottom() - baseTransformingRect.top();
    auto current =  baseTransformingRect.bottom() - pos.y();
    auto percentage = 1.0 - static_cast<double>(current)/max;

    transformingRect.setBottom(baseTransformingRect.top() + baseTransformingRect.height()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        curr->setBottom(orig.top() + orig.height()*percentage);
    }
}

const QRect& TransformTool::getWorkedAreaRect() const
{
    return transformingRect;
}

void TransformTool::stopTransform()
{
    for(auto& layer : transformingLayers)
    {
        layer.layer->setTransforming(false);
    }
    transformingLayers.clear();
}


void TransformTool::onCancel()
{
    for(auto& layer : transformingLayers)
    {
        *layer.layer = layer.original;
    }
    stopTransform();
    emit endTransform();
}
void TransformTool::onFinish()
{
    stopTransform();
    emit endTransform();
}

void TransformTool::terminateTransform()
{
    stopTransform();
}

void TransformTool::onChooseRotate()
{
    currType = Rotate;
}
void TransformTool::onChooseMove()
{
    currType = Move;
}
void TransformTool::onChooseResize()
{
    currType = Resize;
}

qreal TransformTool::getWorkedAreaRectRotation() const
{
    return transformingRectRotation;
}

void TransformTool::onRotationChanged(double degrees)
{
    transformingRectRotation = degrees;
    for(auto& layer : transformingLayers)
    {
        layer.layer->setRotationDegress(layer.original.getRotationDegrees() + degrees);
    }
    editFrame->update();
}

void TransformTool::onDimsChanged(const QSize& newSize)
{
    auto percentageW = static_cast<double>(newSize.width())/originalTransformingRect.width(),
         percentageH = static_cast<double>(newSize.height())/originalTransformingRect.height();
    transformingRect.setSize(newSize);
    for(auto& layer : transformingLayers)
    {
        layer.layer->setSize(QSize(layer.original.width() * percentageW, layer.original.height() * percentageH));
    }
    editFrame->adjustSize();
    editFrame->update();
}

void TransformTool::onPosChanged(const QPoint& newPoint)
{
    for(auto& layer : transformingLayers)
    {
        auto offset = layer.layer->topLeft() - transformingRect.topLeft();
        layer.layer->setPos(newPoint + offset);
    }
    transformingRect.translate(newPoint - transformingRect.topLeft());
    editFrame->adjustSize();
    editFrame->update();
}

