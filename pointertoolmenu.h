#ifndef POINTERTOOLMENU_H
#define POINTERTOOLMENU_H

#include <QWidget>

namespace Ui {
class PointerToolMenu;
}

class PointerToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PointerToolMenu(QWidget *parent = nullptr);
    ~PointerToolMenu();

private:
    Ui::PointerToolMenu *ui;
};

#endif // POINTERTOOLMENU_H
