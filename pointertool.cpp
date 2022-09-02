#include "pointertool.h"
#include "editframe.h"

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

void PointerTool::selectLocation(const QPoint& from)
{
    auto size = editFrame->getSelectedAreaRef()->getSize();
    auto newSelectedArea = new uchar[size*size];
    memset(newSelectedArea, 0, size*size);
    auto oldArea = editFrame->getSelectedAreaRef()->getSelectedArea();
    auto count = 0;
    QStack<QPoint> stack;
    stack.push(from);
    while(!stack.empty())
    {
        auto curr = stack.pop();
        auto x = curr.x(), y = curr.y();
        if(x>=0 && y>=0 && x<size && y<size && oldArea[x + y*size] != 0 && newSelectedArea[x + y*size] == 0)
        {
            if(oldArea[x + y*size] == 1) count++;
            newSelectedArea[x + y*size] = oldArea[x + y*size];
            stack.push(QPoint(x+1,y));
            stack.push(QPoint(x-1,y));
            stack.push(QPoint(x,y+1));
            stack.push(QPoint(x,y-1));
        }
    }

    memcpy(oldArea, newSelectedArea, size*size);
    delete[] newSelectedArea;
    editFrame->getSelectedAreaRef()->change(count);
    editFrame->update();
}

bool PointerTool::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::MouseButtonDblClick:
            if(!editFrame->hasSelectedArea()) break;
            editFrame->getSelectedAreaRef()->unselectAll();
            editFrame->update();
            break;
        case QEvent::MouseButtonPress:
            auto pos = dynamic_cast<QMouseEvent*>(event)->pos()/editFrame->getZoom();
            if(editFrame->getSelectedAreaRef()->isSelected(pos.x(),pos.y()))
                selectLocation(pos);
            break;
    }
    return QObject::eventFilter(obj, event);
}
