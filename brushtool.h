#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include "edittool.h"
#include "brushshape.h"
#include "ImageAlgorithms.h"
#include "brushtoolmenu.h"

#include <utility>

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QPoint>
#include <QPainter>
#include <QCursor>

using namespace std;

class BrushTool : public EditTool
{
    Q_OBJECT
public:
    BrushTool(EditFrame *editFrame, QVector<Layer*>* layers);
    virtual BrushToolMenu* getMenu() override;
    virtual void setCursor() override;
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void onCursorChanged();
private:
    bool draw(Layer& l, const QPoint& point);
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event);
    void onMoveMouse(QMouseEvent *event);
    void onLeaveMouse(QHoverEvent *event);

    void drawPixels(const QPoint& pos, const QPoint& realPos, QImage* img, const QImage& brushImg);
    void adjustCursor();
    bool mouseDown = false;
    QVector<Layer*>* layers;
    BrushToolMenu *menu;
    bool dragResizing = false;
    QPoint startMouse;
    bool *drawnArea;
    bool drawingDone;
};

#endif // MOVETOOL_H
