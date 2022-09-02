#ifndef STAMPTOOL_H
#define STAMPTOOL_H

#include "brushshape.h"
#include "ImageAlgorithms.h"
#include "stamptoolmenu.h"
#include "brushcircle.h"
#include "brushsquare.h"
#include "brushfadedcircle.h"
#include "brushtool.h"

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QPoint>
#include <QPainter>
#include <QCursor>

class StampTool : public EditTool
{
    Q_OBJECT
public:
    StampTool(EditFrame *editFrame, QVector<Layer*>* layers);
    virtual StampToolMenu* getMenu() override;
    virtual void setCursor() override;
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void onCursorChanged();
private:
    bool draw(Layer& l, const QPoint& point);
    void sampleAnew(const QPoint& point);
    void sampleInDraw(const QPoint& point);
    void adjustBrush();
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event);
    void onMoveMouse(QMouseEvent *event);
    void onLeaveMouse(QHoverEvent *event);
    void onTurnAltCursor();
    void onTurnPrimaryCursor();

    void drawPixels(const QPoint& pos, const QPoint& realPos, QImage* img, const QImage& brushImg);
    void adjustCursor();
    bool mouseDown = false;
    QVector<Layer*>* layers;
    StampToolMenu *menu;
    BrushShape *brushShape = nullptr;
    bool notSampledYet = true;
    QImage srcArea;
    QPoint prevMouse;
    QPoint samplePos;
    QPoint startMouse;
    bool *drawnArea;
    bool isClone = true;
    bool dragResizing = false;
    QCursor cloneCursor, sampleCursor;
};

#endif // STAMPTOOL_H
