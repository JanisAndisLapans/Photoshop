#ifndef EDITTOOL_H
#define EDITTOOL_H

#include "layer.h"

#include <vector>

#include <QEvent>

using namespace std;

class EditFrame;

class EditTool
{
public:
    EditTool(EditFrame *editFrame);
    virtual void rerouteEvent(QEvent *event, QVector<Layer>& layers) = 0;
protected:
    EditFrame *editFrame;
};

#endif // EDITTOOL_H
