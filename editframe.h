#ifndef EDITFRAME_H
#define EDITFRAME_H

#include "layer.h"
#include "selectedarea.h"
#include "edittool.h"
#include "transformtool.h"
#include "stamptool.h"

#include <memory>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <deque>

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
    Q_OBJECT
public:
    EditFrame(QWidget *parent = nullptr);
    void addImg(QString path);
    void addImages(const QVector<QString>& paths);
    virtual void paintEvent(QPaintEvent * event) override;
    void adjustSize(bool quick=false);
    double getZoom() const;
    void changeZoom(double amount);
    QVector<Layer*>* getLayersRef();
    SelectedArea* getSelectedAreaRef();
    bool hasSelectedArea() const;
    void lookAt(QPoint point);
    void setScrollArea(QScrollArea *scrollArea);
    void drawResizeBall(int size, const QPoint& resizeBallCenter);
    void finishDrawingResizeBall();
    void enableTool(EditTool* tool);
    void setTransformTool(TransformTool* tool);
    void addSolidLayer(QColor color, QSize size, QPainter::CompositionMode mode);
    void addImg(const QImage& img, const QString& name, const QPoint& pos = QPoint(0,0));
    void saveState();
    void undo();
    void redo();
    void blockUndo(bool);
    QImage getResultingImg();

protected:
    virtual void mouseMoveEvent(QMouseEvent * ev) override;
    virtual void dragEnterEvent(QDragEnterEvent *ev) override;
    virtual void dragMoveEvent(QDragMoveEvent *ev) override;
    virtual void dropEvent(QDropEvent *ev) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    const int maxSaveStates = 20;

     deque<QVector<Layer*>> undos, redos;
     QVector<Layer*> layers;
     double zoom = 1.0;
     SelectedArea selectedArea;
     bool selectedAreaDisplayState = false;
     QScrollArea *scrollArea;
     int resizeBallSize;
     QPoint centerResizeBall;
     bool drawingResizeBall = false;
     EditTool* currTool = nullptr;
     EditTool* prevTool;
     TransformTool* ttool;
     bool blockedUndo = false;

private slots:
     void selectionDisplaySwitch();
     void onEndTransform();

signals:
     void enableUndo(bool);
     void enableRedo(bool);

};

#endif // EDITFRAME_H
