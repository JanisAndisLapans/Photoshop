#ifndef ZOOMTOOL_H
#define ZOOMTOOL_H

#include "edittool.h"
#include "zoomtoolmenu.h"

#include <QCursor>
#include <QMouseEvent>
#include <QPoint>

class ZoomTool : public EditTool
{
    Q_OBJECT
public:
    ZoomTool(EditFrame *editFrame);
    virtual ZoomToolMenu* getMenu() override;
    virtual void setCursor() override;

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private:
    ZoomToolMenu *menu;
    QPoint lastMouse;
    QPoint startPos;
    double zoom;
    bool mouseDown = false;
private slots:
    void onZoomChange(double amount);
};

#endif // ZOOMTOOL_H
