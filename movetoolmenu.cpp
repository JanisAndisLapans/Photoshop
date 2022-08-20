#include "movetoolmenu.h"
#include "ui_movetoolmenu.h"

MoveToolMenu::MoveToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveToolMenu)
{
    ui->setupUi(this);
}

MoveToolMenu::~MoveToolMenu()
{
    delete ui;
}
