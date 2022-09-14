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
    connect(ui->heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onHeightChange(int)));
    connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onWidthChange(int)));
    connect(ui->ySpinBox, SIGNAL(valueChanged(int)), this, SLOT(onYChanged(int)));
    connect(ui->xSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onXChanged(int)));
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

void TransformToolMenu::onWidthChange(int width)
{
    emit dimsChanged(QSize(width, ui->heightSpinBox->value()));
}

void TransformToolMenu::onHeightChange(int height)
{
    emit dimsChanged(QSize(ui->widthSpinBox->value(), height));
}

void TransformToolMenu::onXChanged(int x)
{
    emit posChanged(QPoint(x, ui->ySpinBox->value()));
}

void TransformToolMenu::onYChanged(int y)
{
    emit posChanged(QPoint(ui->xSpinBox->value(), y));
}

void TransformToolMenu::setSize(const QSize& newSize)
{
    ui->widthSpinBox->blockSignals(true);
    ui->heightSpinBox->blockSignals(true);
    ui->widthSpinBox->setValue(newSize.width());
    ui->heightSpinBox->setValue((newSize.height()));
    ui->widthSpinBox->blockSignals(false);
    ui->heightSpinBox->blockSignals(false);
}

void TransformToolMenu::setPos(const QPoint& newPoint)
{
    ui->xSpinBox->blockSignals(true);
    ui->ySpinBox->blockSignals(true);
    ui->xSpinBox->setValue(newPoint.x());
    ui->ySpinBox->setValue(newPoint.y());
    ui->xSpinBox->blockSignals(false);
    ui->ySpinBox->blockSignals(false);
}
