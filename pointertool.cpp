#include "pointertool.h"

PointerTool::PointerTool(EditFrame* editFrame)
    :EditTool(editFrame)
{
    menu = new PointerToolMenu();
}

PointerToolMenu* PointerTool::getMenu()
{
    return menu;
}
void PointerTool::setCursor()
{
    editFrame->setCursor(Qt::ArrowCursor);
}

bool PointerTool::eventFilter(QObject *obj, QEvent *event)
{
    return QObject::eventFilter(obj, event);
}
