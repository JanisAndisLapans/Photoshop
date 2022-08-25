#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "edittool.h"
#include "selectedarea.h"
#include "brushshape.h"
#include "selecttoolmenu.h"

#include <memory>
#include <tuple>

#include <QEvent>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QPoint>
#include <QStack>
#include <QLabel>
#include <QPoint>

using namespace std;

class SelectTool : public EditTool
{
    Q_OBJECT
public:
    SelectTool(EditFrame *editFrame, QVector<Layer>* layers);
    virtual selectToolMenu* getMenu() override;
    virtual void setCursor() override;

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private:
    void onReleaseMouse(QMouseEvent *event);
    void onDownMouse(QMouseEvent *event);
    void onMoveMouse(QMouseEvent *event);
    void onLeaveMouse(QHoverEvent *event);
    void onTurnAltCursor();
    void onTurnPrimaryCursor();
    void adjustCursor();
    void makeSelectionAt(const QPoint& from, const QImage& img, const QPoint& realPos);
    void makeDeselectionAt(const QPoint& from, const QImage& img, const QPoint& realPos);

    bool mouseDown = false;
    QPoint startMouse;
    SelectedArea *selectedArea;
    QVector<Layer>* layers;
    selectToolMenu *menu;
    bool isAdd = true;
    bool dragResizing = false;
    QPoint startMouseResize;
    QCursor subCursor, addCursor;
private slots:
    void onCursorChanged();
};

#endif // SELECTTOOL_H
