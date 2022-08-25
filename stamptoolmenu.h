#ifndef STAMPTOOLMENU_H
#define STAMPTOOLMENU_H

#include "brushshape.h"
#include "brushcircle.h"
#include "brushsquare.h"
#include "brushfadedcircle.h"

#include <QWidget>
#include <QMouseEvent>
#include <QColorDialog>
#include <QObject>


namespace Ui {
class StampToolMenu;
}

class StampToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit StampToolMenu(QWidget *parent = nullptr);

    enum Mode{
        Over, Multiply, Screen, Overlay
    };

    enum BrushType{
        Square, Circle, FadedCircle
    };

    Mode getMode() const;
    BrushType getBrushType() const;
    int getBrushSize() const;
    void setBrushSize(int newSize);
    ~StampToolMenu();
private slots:
    void onSizeChange(int changedVal);
    void onTypeChange(int index);
    void onModeChanged(int index);
signals :
    void cursorChanged();
private:
    int size = 15;
    Ui::StampToolMenu *ui;
    Mode currMode = Over;
    BrushType currType = Circle;
};

#endif // STAMPTOOLMENU_H
