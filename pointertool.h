#ifndef POINTERTOOL_H
#define POINTERTOOL_H

#include "edittool.h"
#include "pointertoolmenu.h"

#include <QEvent>
#include <QMouseEvent>

class PointerTool : public EditTool
{
public:
    PointerTool(EditFrame* editFrame);
    virtual PointerToolMenu* getMenu() override;
    virtual void setCursor() override;

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private:
    PointerToolMenu *menu;
};

#endif // POINTERTOOL_H
