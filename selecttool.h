#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "edittool.h"

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QPoint>

class SelectTool : public EditTool
{
public:
    SelectTool();
    virtual void rerouteEvent(QEvent *event, QVector<Layer>& layers) override;
private:
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event);
    void onMoveMouse(QMouseEvent *event, QVector<Layer>& layers);
    void onLeaveMouse(QHoverEvent *event);
    bool mouseDown = false;
    QPoint startMouse;
};

#endif // SELECTTOOL_H
