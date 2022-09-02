#include "transformtool.h"
#include "editframe.h"


TransformTool::TransformTool(EditFrame* editFrame)
   : EditTool(editFrame)
{
    menu = new TransformToolMenu();
    connect(menu, SIGNAL(cancelTransform()), this, SLOT(onCancel()));
    connect(menu, SIGNAL(finishTransform()), this, SLOT(onFinish()));
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
    originalTransformingRect = transformingRect;
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
        originalTransformingRect = transformingRect;
        for(auto& layer : transformingLayers)
        {
            layer.resizingBase = *layer.layer;
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
        startMouse = event->pos();
        transforming = true;
    }
}

void TransformTool::onMoveMouse(QMouseEvent *event)
{
    switch(currType){
        case Resize:
            if(!transforming)
            {
                auto pos = event->pos()/editFrame->getZoom();
                auto activateTreshhold = 10/editFrame->getZoom();
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
                else if(abs(event->pos().x() - transformingRect.left()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::leftResize, this, _1);
                    editFrame->setCursor(Qt::SizeHorCursor);
                }
                else if(abs(event->pos().x() - transformingRect.right()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::rightResize, this, _1);
                    editFrame->setCursor(Qt::SizeHorCursor);
                }
                else if(abs(event->pos().y() - transformingRect.top()) < activateTreshhold)
                {
                    currResizeMethod = bind(&TransformTool::topResize, this, _1);
                    editFrame->setCursor(Qt::SizeVerCursor);
                }
                else if(abs(event->pos().y() - transformingRect.bottom()) < activateTreshhold)
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
                currResizeMethod(event->pos());;
                editFrame->lookAt(event->pos());
                editFrame->adjustSize(true);
                editFrame->update();
            }
            break;
        case Move:
            if(!transforming)
            {
                if(transformingRect.contains(event->pos()))
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
                editFrame->adjustSize(true);
                editFrame->update();
            }
            break;
    }
}

void TransformTool::topLeftResize(QPoint pos)
{ 
    if(pos.x() < 0  || pos.y() <0)
    {
        auto shortest = min(originalTransformingRect.top(), originalTransformingRect.left());
        pos = QPoint(originalTransformingRect.left() - shortest, originalTransformingRect.top() - shortest);
    }
    pos.setX(min(pos.x(), originalTransformingRect.bottomRight().x()));
    pos.setY(min(pos.y(), originalTransformingRect.bottomRight().y()));
    const auto max = ImageAlgorithms::pointDistance(originalTransformingRect.topLeft(), originalTransformingRect.bottomRight());   
    auto current = ImageAlgorithms::pointDistance(pos, originalTransformingRect.bottomRight());
    auto percentage = static_cast<double>(current)/max;
    auto percentageReverse = 1.0 - percentage;
    transformingRect.setSize(QSize(originalTransformingRect.width()*percentage, originalTransformingRect.height()*percentage));
    transformingRect.translate(-transformingRect.topLeft() + originalTransformingRect.topLeft() + QPoint(originalTransformingRect.width(), originalTransformingRect.height())*percentageReverse);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        auto offset = (orig.topLeft() - originalTransformingRect.topLeft())*percentage;

        curr->setSize(QSize(orig.width()*percentage, orig.height()*percentage));
        curr->setPos(offset + originalTransformingRect.topLeft() + QPoint(originalTransformingRect.width(), originalTransformingRect.height())*percentageReverse);
    }
}

void TransformTool::topRightResize(QPoint pos)
{
    if(pos.y() <0)
    {
        const auto proportion = static_cast<double>(originalTransformingRect.width())/originalTransformingRect.height();
        pos = QPoint(originalTransformingRect.right() + originalTransformingRect.top()*proportion, 0);
    }
    pos.setX(max(pos.x(), originalTransformingRect.bottomLeft().x()));
    pos.setY(min(pos.y(), originalTransformingRect.bottomLeft().y()));
    const auto max = ImageAlgorithms::pointDistance(originalTransformingRect.topRight(), originalTransformingRect.bottomLeft());
    auto current = ImageAlgorithms::pointDistance(pos, originalTransformingRect.bottomLeft());
    auto percentage = static_cast<double>(current)/max;
    auto percentageReverse = 1.0 - percentage;

    transformingRect.setSize(QSize(originalTransformingRect.width()*percentage, originalTransformingRect.height()*percentage));
    transformingRect.translate(-transformingRect.topLeft() + QPoint(transformingRect.x(),originalTransformingRect.top() + originalTransformingRect.height()*percentageReverse));

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        auto offset = (orig.topLeft() - originalTransformingRect.topLeft())*percentage;

        curr->setSize(QSize(orig.width()*percentage, orig.height()*percentage));
        curr->setPos(offset + QPoint(transformingRect.x(), originalTransformingRect.top() + originalTransformingRect.height()*percentageReverse));
    }
}

void TransformTool::bottomLeftResize(QPoint pos)
{
    if(pos.x() < 0)
    {
        const auto proportion = static_cast<double>(originalTransformingRect.height())/originalTransformingRect.width();
        pos = QPoint(0, originalTransformingRect.bottom() + originalTransformingRect.left()*proportion);
    }
    pos.setX(min(pos.x(), originalTransformingRect.topRight().x()));
    pos.setY(max(pos.y(), originalTransformingRect.topRight().y()));
    const auto max = ImageAlgorithms::pointDistance(originalTransformingRect.topRight(), originalTransformingRect.bottomLeft());
    auto current = ImageAlgorithms::pointDistance(pos, originalTransformingRect.topRight());
    auto percentage = static_cast<double>(current)/max;
    auto percentageReverse = 1.0 - percentage;

    transformingRect.setSize(originalTransformingRect.size()*percentage);
    transformingRect.translate(-transformingRect.topLeft() + QPoint(originalTransformingRect.left() + originalTransformingRect.width()*percentageReverse,transformingRect.y()));

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        auto offset = (orig.topLeft() - originalTransformingRect.topLeft())*percentage;

        curr->setSize(orig.size() * percentage);
        curr->setPos(offset + QPoint(originalTransformingRect.left() + originalTransformingRect.width()*percentageReverse,transformingRect.y()));
    }
}

void TransformTool::bottomRightResize(QPoint pos)
{
    pos.setX(max(pos.x(), originalTransformingRect.topLeft().x()));
    pos.setY(max(pos.y(), originalTransformingRect.topLeft().y()));
    const auto max = ImageAlgorithms::pointDistance(originalTransformingRect.topLeft(), originalTransformingRect.bottomRight());
    auto current = ImageAlgorithms::pointDistance(pos, originalTransformingRect.topLeft());
    auto percentage = static_cast<double>(current)/max;
//  auto percentageReverse = 1.0 - percentage;

    transformingRect.setSize(originalTransformingRect.size()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
//      auto offset = (orig.topLeft() - originalTransformingRect.topLeft())*percentage;
        curr->setSize(orig.size()*percentage);
    }
}

void TransformTool::leftResize(QPoint pos)
{
    pos.setX(min(max(pos.x(), 0), originalTransformingRect.right()));
    const auto max = originalTransformingRect.right() - originalTransformingRect.left();
    auto current =  pos.x() - originalTransformingRect.left();
    auto percentage = static_cast<double>(current)/max;

    transformingRect.setLeft(originalTransformingRect.left() + originalTransformingRect.width()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        curr->setLeft(orig.left() + orig.width()*percentage);
    }
}

void TransformTool::rightResize(QPoint pos)
{
    pos.setX(max(pos.x(), originalTransformingRect.left()));
    const auto max = originalTransformingRect.right() - originalTransformingRect.left();
    auto current = originalTransformingRect.right() - pos.x();
    auto percentage = 1.0 - static_cast<double>(current)/max;

    transformingRect.setRight(originalTransformingRect.left() + originalTransformingRect.width()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        curr->setRight(orig.left() + orig.width()*percentage);
    }
}
void TransformTool::topResize(QPoint pos)
{
    pos.setY(max(min(pos.y(), originalTransformingRect.bottom()), 0));
    const auto max = originalTransformingRect.bottom() - originalTransformingRect.top();
    auto current = pos.y() - originalTransformingRect.top();
    auto percentage = static_cast<double>(current)/max;

    transformingRect.setTop(originalTransformingRect.top() + originalTransformingRect.height()*percentage);

    for(auto layer : transformingLayers)
    {
        auto curr = layer.layer;
        const auto& orig = layer.resizingBase;
        curr->setTop(orig.top() + orig.height()*percentage);
    }
}
void TransformTool::bottomResize(QPoint pos)
{
    pos.setY(max(pos.y(), originalTransformingRect.top()));
    const auto max = originalTransformingRect.bottom() - originalTransformingRect.top();
    auto current =  originalTransformingRect.bottom() - pos.y();
    auto percentage = 1.0 - static_cast<double>(current)/max;

    transformingRect.setBottom(originalTransformingRect.top() + originalTransformingRect.height()*percentage);

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
