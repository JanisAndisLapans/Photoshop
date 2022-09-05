#include "transformtoolmenu.h"
#include "ui_transformtoolmenu.h"

TransformToolMenu::TransformToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransformToolMenu)
{
    ui->setupUi(this);
    connect(ui->endTransformPushButton, SIGNAL(clicked()), this, SLOT(onEndTransformButtonPress()));
    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelTransformButtonPress()));
    connect(ui->rotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onRotationChanged(double)));

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

void TransformToolMenu::setRotation(double degrees)
{
    ui->rotationSpinBox->blockSignals(true);
    ui->rotationSpinBox->setValue(fmod(degrees,360.0));
    ui->rotationSpinBox->blockSignals(false);
}

void TransformToolMenu::onRotationChanged(double degrees)
{
    emit rotated(degrees);
}
