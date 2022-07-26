#ifndef LAYERFRAME_H
#define LAYERFRAME_H

#include "layer.h"
#include "editframe.h"

#include <algorithm>

#include <QFrame>
#include <QVector>
#include <QPainter>
#include <QCursor>
#include <QTimer>
#include <QMouseEvent>
#include <QImage>
#include <QAction>
#include <QMenu>


using namespace std;

class LayerFrame : public QFrame
{
public:
    LayerFrame(QWidget *parent);
    void setEditFrame(EditFrame *editFrame);
    virtual void paintEvent(QPaintEvent * event) override;
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    void startDrag();
    int getIndexOfLayerAtY(int y);

    QVector<Layer*>* layers = nullptr;
    bool mouseDragging = false;
    int draggedLayerIndex;
    QCursor dragCursor;
    QTimer* mouseDownTimer;
    EditFrame* editFrame;
    bool isMultiSelect = false;
};

#endif // LAYERFRAME_H
