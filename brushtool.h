#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include "edittool.h"
#include "brushshape.h"

#include <utility>
#include <chrono>
#include <memory>

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QPoint>
#include <QPainter>

using namespace std;

class BrushTool : public EditTool
{
public:
    BrushTool(EditFrame *editFrame, const shared_ptr<BrushShape>& brushColor);
    virtual void rerouteEvent(QEvent *event, QVector<Layer>& layers) override;
private:
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event, QVector<Layer>& layers);
    void onMoveMouse(QMouseEvent *event, QVector<Layer>& layers);
    void onLeaveMouse(QHoverEvent *event);
    bool mouseDown = false;
    shared_ptr<BrushShape> brushShape;
};

#endif // MOVETOOL_H
