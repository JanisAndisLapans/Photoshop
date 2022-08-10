#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "edittool.h"
#include "selectedarea.h"
#include "brushshape.h"

#include <memory>
#include <tuple>

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QPoint>
#include <QStack>
#include <QLabel>

using namespace std;

class SelectTool : public EditTool
{
public:
    SelectTool(EditFrame *editFrame, const unique_ptr<BrushShape>* brushShape);
    virtual void rerouteEvent(QEvent *event, QVector<Layer>& layers) override;
private:
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event, QVector<Layer>& layers);
    void onMoveMouse(QMouseEvent *event, QVector<Layer>& layers);
    void onLeaveMouse(QHoverEvent *event);

    void makeSelectionAt(const QPoint& from, const QImage& img, const QPoint& realPos);
    void makeDeselectionAt(const QPoint& from, const QImage& img, const QPoint& realPos);

    bool mouseDown = false;
    QPoint startMouse;
    SelectedArea *selectedArea;
    const unique_ptr<BrushShape>* brushShape;
};

#endif // SELECTTOOL_H
