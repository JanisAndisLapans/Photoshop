#ifndef TRANSFORMTOOL_H
#define TRANSFORMTOOL_H

#include "edittool.h"
#include "transformtoolmenu.h"
#include "ImageAlgorithms.h"

#include <functional>
#include <utility>
#include <QMenu>


using namespace std;
using std::placeholders::_1;

class TransformTool : public EditTool
{
    Q_OBJECT
public:
    TransformTool(EditFrame* editFrame);
    virtual TransformToolMenu* getMenu() override;
    virtual void setCursor() override;
    void startLayerTransform(const QVector<Layer*>& layers);
    const QRect& getWorkedAreaRect() const;
    void terminateTransform();
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private:
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event);
    void onMoveMouse(QMouseEvent *event);


    void topLeftResize(QPoint pos);
    void topRightResize(QPoint pos);
    void bottomLeftResize(QPoint pos);
    void bottomRightResize(QPoint pos);
    void leftResize(QPoint pos);
    void rightResize(QPoint pos);
    void topResize(QPoint pos);
    void bottomResize(QPoint pos);

    void stopTransform();

    struct LayerInfo
    {
        Layer* layer;
        Layer original;

        QRect resizingBase;
    };

    enum TransformType
    {
       Resize, Move, Rotate
    };

    function<void(const QPoint&)> currResizeMethod;
    bool transforming = false;
    bool transformingAvailable = false;
    QRect transformingRect, originalTransformingRect;
    QPoint startMouse;
    QVector<LayerInfo> transformingLayers = QVector<LayerInfo>();

    QVector<QRect> oldRects;
    TransformToolMenu *menu;
    TransformType currType;

private slots:
    void onCancel();
    void onFinish();

    void onChooseResize();
    void onChooseRotate();
    void onChooseMove();

signals:
    void endTransform();
};

#endif // TRANSFORMTOOL_H
