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
#include "selectedarea.h"
#include "selecttool.h"

#include <memory>
#include <cmath>
#include <algorithm>

#include <QFrame>
#include <QPainter>
#include <QCursor>
#include <QHoverEvent>
#include <QException>
#include <QTimer>
#include <QKeyEvent>
#include <QScrollArea>
#include <QMimeData>
#include <QFileInfo>


using namespace std;

class EditFrame : public QFrame
{
public:
    EditFrame(QWidget *parent = nullptr);
    void setImg(QString path);
    virtual void paintEvent(QPaintEvent * event);
    void adjustSize(bool quick=false);
    double getZoom() const;
    void changeZoom(double amount);
    QVector<Layer>* getLayersRef();
    SelectedArea* getSelectedAreaRef();
    bool hasSelectedArea() const;
    void lookAt(QPoint point);
    void setScrollArea(QScrollArea *scrollArea);

protected:
    virtual void mouseMoveEvent(QMouseEvent * ev) override;
    virtual void dragEnterEvent(QDragEnterEvent *ev) override;
    virtual void dragMoveEvent(QDragMoveEvent *ev) override;
    virtual void dropEvent(QDropEvent *ev) override;

private:
     QVector<Layer> layers;
     EditTool* editTool;
     double zoom = 1.0;
     SelectedArea selectedArea;
     bool selectedAreaDisplayState = false;
     QScrollArea *scrollArea;

private slots:
     void selectionDisplaySwitch();

    };

#endif // EDITFRAME_H
