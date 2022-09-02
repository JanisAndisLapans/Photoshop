#ifndef MOVETOOL_H
#define MOVETOOL_H

#include "edittool.h"
#include "movetoolmenu.h"

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QPoint>

class MoveTool : public EditTool
{
public:
    MoveTool(EditFrame *editFrame, QVector<Layer*>* layers);
    virtual MoveToolMenu* getMenu() override;
    virtual void setCursor() override;
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private:
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event);
    void onMoveMouse(QMouseEvent *event);
    void onLeaveMouse(QHoverEvent *event);
    bool mouseDown = false;
    QPoint startMouse;
    Layer* currMoving = nullptr;
    QVector<Layer*>* layers;
    MoveToolMenu* menu;
};

#endif // MOVETOOL_H
