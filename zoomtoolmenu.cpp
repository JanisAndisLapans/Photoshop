#include "zoomtoolmenu.h"
#include "ui_zoomtoolmenu.h"

ZoomToolMenu::ZoomToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZoomToolMenu)
{
    ui->setupUi(this);
    connect(ui->zoomSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onZoomChange(int)));
}

void ZoomToolMenu::onZoomChange(int changedValue)
{
    zoomAmount = changedValue/100.0;
    emit changedZoom(zoomAmount);
}

void ZoomToolMenu::setZoom(double amount)
{
    zoomAmount = amount;
    ui->zoomSpinBox->blockSignals(true);
    ui->zoomSpinBox->setValue(amount*100);
    ui->zoomSpinBox->blockSignals(false);
}

double ZoomToolMenu::getZoomAmount() const
{
    return zoomAmount;
}

ZoomToolMenu::~ZoomToolMenu()
{
    delete ui;
}
