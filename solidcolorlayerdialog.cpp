#include "solidcolorlayerdialog.h"
#include "ui_solidcolorlayerdialog.h"

SolidColorLayerDialog::SolidColorLayerDialog(QWidget *parent) :
    QDialog(parent), parent(parent),
    ui(new Ui::SolidColorLayerDialog)
{
    ui->setupUi(this);
    ui->colorLabel->installEventFilter(this);
}

bool SolidColorLayerDialog::eventFilter(QObject *obj, QEvent *ev)
{
    if ((ev->type() == QEvent::MouseButtonPress
            || ev->type() == QEvent::MouseButtonRelease
            || ev->type() == QEvent::MouseButtonDblClick)
            && obj->isWidgetType() && obj==ui->colorLabel)
    {
        pickedColor = QColorDialog::getColor(pickedColor, this);
        auto css = "QLabel { background-color :" + pickedColor.name() + "}";
        ui->colorLabel->setStyleSheet(css);
    }
    return false;
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
