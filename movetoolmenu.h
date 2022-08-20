#ifndef MOVETOOLMENU_H
#define MOVETOOLMENU_H

#include <QWidget>

namespace Ui {
class MoveToolMenu;
}

class MoveToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MoveToolMenu(QWidget *parent = nullptr);
    ~MoveToolMenu();

private:
    Ui::MoveToolMenu *ui;
};

#endif // MOVETOOLMENU_H
