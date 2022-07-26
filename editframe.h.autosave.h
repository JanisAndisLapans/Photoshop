#ifndef EDITFRAME_H
#define EDITFRAME_H

#include "layer.h"
#include "edittool.h"
#include "movetool.h"
#include "brushtool.h"
#include "brushshape.h"
#include "brushcircle.h"
#include "brushsquare.h"
#include "brushfadedcircle.h"

#include <memory>
#include <cmath>

#include <QFrame>
#include <QPainter>
#include <algorithm>
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

    EditFrame(QWidget *parent = nullptr);
    void setImg(QString path);
    virtual void paintEvent(QPaintEvent * event);
    void setTool(Tools tool);
    void adjustSize();
    void setBrushSize(int size);
    double getZoom() const;
    void setBrushColor(const QColor& color);


protected:
    bool event(QEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
     QVector<Layer> layers;
     Tools tool = Cursor;
     bool mouseDown = false;
     QPoint lastMouse;
     unique_ptr<EditTool> editTool;
     double zoom = 1.0;
     shared_ptr<BrushSquare> brushShape = make_shared<BrushSquare>(15,Qt::green);
     QCursor brushCursor;

     void changeZoomBy(double amount);
     void adjustBrushCursor();
};

#endif // EDITFRAME_H
