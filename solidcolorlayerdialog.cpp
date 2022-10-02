#include "solidcolorlayerdialog.h"
#include "ui_solidcolorlayerdialog.h"

SolidColorLayerDialog::SolidColorLayerDialog(QWidget *parent) :
    QDialog(parent), parent(parent),
    ui(new Ui::SolidColorLayerDialog)
{
    ui->setupUi(this);
    ui->colorLabel->installEventFilter(this);
    connect(ui->colorLabel, SIGNAL(colorChanged(const QColor&)), this, SLOT(onColorChanged(const QColor&)));
}


void SolidColorLayerDialog::onColorChanged(const QColor& color)
{
    pickedColor = color;
}

QSize SolidColorLayerDialog::getSize() const
{
    return QSize(ui->spinBoxW->value(), ui->spinBoxH->value());
}

QColor SolidColorLayerDialog::getColor() const
{
    return pickedColor;
}

SolidColorLayerDialog::~SolidColorLayerDialog()
{
    delete ui;
}
