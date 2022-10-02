#ifndef TEXTDRAWINGTOOL_H
#define TEXTDRAWINGTOOL_H

#include "edittool.h"
#include "textdrawingtoolmenu.h"
#include "layer.h"
#include "ImageAlgorithms.h"

#include <utility>
#include <functional>

#include <QTextEdit>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QKeyEvent>

using namespace std;
using namespace placeholders;

class TextDrawingTool : public EditTool
{
    Q_OBJECT

public:
    TextDrawingTool(EditFrame *editFrame, QVector<Layer*>* layers);
    virtual TextDrawingToolMenu* getMenu() override;
    virtual void setCursor() override;
    virtual void start() override;
    virtual void end() override;
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private:
    void onMousePress(QMouseEvent *ev);
    void onMouseMove(QMouseEvent *ev);
    void onMouseRelease(QMouseEvent *ev);

    void topLeftResize(const QPoint& pos);
    void topRightResize(const QPoint& pos);
    void bottomLeftResize(const QPoint& pos);
    void bottomRightResize(const QPoint& pos);
    void leftResize(const QPoint& pos);
    void rightResize(const QPoint& pos);
    void topResize(const QPoint& pos);
    void bottomResize(const QPoint& pos);

private slots:
    void onParamsChanged();
    void onColorChanged(const QColor& color);
    void onEditCursorPositionChanged();
    void onFinish();
    void onCancel();

private:
    function<void(const QPoint&)> currResizeMethod;
    bool transforming = false, transformingAvailable = false;
    QTextEdit *edit;
    QVector<Layer*>* layers;
    TextDrawingToolMenu *menu;
    bool drawingStarted = false;
    bool drawn = false;
    QPoint drawingStartPos;

};

#endif // TEXTDRAWINGTOOL_H
