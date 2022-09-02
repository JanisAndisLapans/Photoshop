#include "transformtoolmenu.h"
#include "ui_transformtoolmenu.h"

TransformToolMenu::TransformToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransformToolMenu)
{
    ui->setupUi(this);
    connect(ui->endTransformPushButton, SIGNAL(clicked()), this, SLOT(onEndTransformButtonPress()));
    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelTransformButtonPress()));
}

TransformToolMenu::~TransformToolMenu()
{
    delete ui;
}

void TransformToolMenu::onCancelTransformButtonPress()
{
    emit cancelTransform();
}

void TransformToolMenu::onEndTransformButtonPress()
{
    emit finishTransform();
}
