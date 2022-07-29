#ifndef EDITFRAME_H
#define EDITFRAME_H

#include "layer.h"
#include "edittool.h"
#include "movetool.h"
#include "brushtool.h"
#include "brushshape.h"
#include "brushsquare.h"
#include "brushfadedcircle.h"
#include "brushcircle.h"

#include <memory>
#include <cmath>
#include <algorithm>

#include <QFrame>
#include <QPainter>
#include <QCursor>
#include <QHoverEvent>
#include <QException>


using namespace std;

class EditFrame : public QFrame
{
public:
    enum Tools{
        Cursor, Move, Resize, Brush
    };

    enum BrushType{
        Square, Circle, FadedCircle
    };

    EditFrame(QWidget *parent = nullptr);
    void setImg(QString path);
    virtual void paintEvent(QPaintEvent * event);
    void setTool(Tools tool);
    void adjustSize();
    void setBrushSize(int size);
    double getZoom() const;
    void setBrushColor(const QColor& color);
    void setBrushShape(BrushType type);
    QVector<Layer>* getLayersRef();

protected:
    bool event(QEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
     QVector<Layer> layers;
     Tools tool = Cursor;
     BrushType brushType;
     bool mouseDown = false;
     QPoint lastMouse;
     unique_ptr<EditTool> editTool;
     double zoom = 1.0;
     unique_ptr<BrushShape> brushShape = make_unique<BrushSquare>(15,Qt::green);
     QCursor brushCursor;

     void changeZoomBy(double amount);
     void adjustBrushCursor();
};

#endif // EDITFRAME_H
