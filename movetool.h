#ifndef MOVETOOL_H
#define MOVETOOL_H

#include "edittool.h"

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QPoint>

class MoveTool : public EditTool
{
public:
    MoveTool(EditFrame *editFrame);
    virtual void rerouteEvent(QEvent *event, QVector<Layer>& layers) override;
private:
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event, QVector<Layer>& layers);
    void onMoveMouse(QMouseEvent *event);
    void onLeaveMouse(QHoverEvent *event);
    bool mouseDown = false;
    QPoint startMouse;
    Layer* currMoving = nullptr;
};

#endif // MOVETOOL_H
