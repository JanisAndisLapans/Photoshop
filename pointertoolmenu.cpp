#include "pointertoolmenu.h"
#include "ui_pointertoolmenu.h"

PointerToolMenu::PointerToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointerToolMenu)
{
    ui->setupUi(this);
}

PointerToolMenu::~PointerToolMenu()
{
    delete ui;
}
