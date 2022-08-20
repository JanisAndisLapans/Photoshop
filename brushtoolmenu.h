#ifndef BRUSHTOOLMENU_H
#define BRUSHTOOLMENU_H

#include "brushshape.h"
#include "brushcircle.h"
#include "brushsquare.h"
#include "brushfadedcircle.h"

#include <QWidget>
#include <QMouseEvent>
#include <QColorDialog>
#include <QObject>

namespace Ui {
class BrushToolMenu;
}

class BrushToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit BrushToolMenu(QWidget *parent = nullptr);

    const BrushShape& getBrushShape() const;

    enum Mode{
        Over, Multiply, Screen, Overlay
    };

    Mode getMode() const;

    ~BrushToolMenu();
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void onSizeChange(int changedVal);
    void onTypeChange(int index);
    void onModeChanged(int index);
signals :
    void cursorChanged();
private:
    QColor pickedColor = Qt::green;
    int size = 15;
    Ui::BrushToolMenu *ui;
    BrushShape *brushShape;
    Mode currMode = Over;
};

#endif // BRUSHTOOLMENU_H
