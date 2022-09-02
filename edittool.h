#ifndef EDITTOOL_H
#define EDITTOOL_H

#include "layer.h"

#include <vector>

using namespace std;

class EditFrame;

class EditTool : public QObject
{
    Q_OBJECT

public:
    EditTool(EditFrame *editFrame);
    virtual QWidget* getMenu() = 0;
    virtual void setCursor() = 0;
protected:
    EditFrame *editFrame;
    virtual bool eventFilter(QObject *obj, QEvent *event) override = 0;
};

#endif // EDITTOOL_H
