#include "zoomtool.h"

ZoomTool::ZoomTool(EditFrame* editFrame)
   : EditTool(editFrame)
{
    zoom = editFrame->getZoom();
    menu = new ZoomToolMenu();
    connect(menu, SIGNAL(changedZoom(double)), this, SLOT(onZoomChange(double)));
}

ZoomToolMenu* ZoomTool::getMenu()
{
    return menu;
}

void ZoomTool::onZoomChange(double amount)
{
    zoom = amount;
    editFrame->changeZoom(amount);
}

bool ZoomTool::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QMouseEvent::MouseButtonPress)
    {
        lastMouse = QCursor::pos();
        startPos = static_cast<QMouseEvent*>(event)->pos()/zoom;
        mouseDown = true;
    }
    else if(event->type() == QMouseEvent::MouseButtonRelease)
    {
        mouseDown = false;
    }
    else if(event->type() == QMouseEvent::MouseMove)
    {
        #define ZOOM_DIST_RATIO 200.0
        if(mouseDown)
        {
            auto mouseNow = QCursor::pos();
            auto dist = mouseNow - lastMouse;
            auto totalDist = dist.y() - dist.x();
            auto zoomChange = totalDist / ZOOM_DIST_RATIO;
            lastMouse = mouseNow;
            if(zoom + zoomChange<0.01 || zoom + zoomChange>10.0) goto end;
            zoom += zoomChange;
            editFrame->changeZoom(zoom);
            editFrame->lookAt(startPos*zoom);
            menu->setZoom(zoom);
        }
    } 
    end:
    return QObject::eventFilter(obj, event);
}

void ZoomTool::setCursor()
{
    editFrame->setCursor(Qt::SizeBDiagCursor);
}
