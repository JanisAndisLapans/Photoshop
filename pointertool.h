#ifndef POINTERTOOL_H
#define POINTERTOOL_H

#include "edittool.h"
#include "pointertoolmenu.h"

#include <QEvent>
#include <QMouseEvent>
#include <QStack>

class PointerTool : public EditTool
{
public:
    PointerTool(EditFrame* editFrame);
    virtual PointerToolMenu* getMenu() override;
    virtual void setCursor() override;

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private:
    void selectLocation(const QPoint& from);

    PointerToolMenu *menu;
};

#endif // POINTERTOOL_H
