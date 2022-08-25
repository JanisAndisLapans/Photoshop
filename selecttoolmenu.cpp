#include "selecttoolmenu.h"
#include "ui_selecttoolmenu.h"

selectToolMenu::selectToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectToolMenu)
{
    ui->setupUi(this);
    connect(ui->sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSizeChange(int)));
    connect(ui->distDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onDistChange(double)));
    connect(ui->fillSensSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFillSensChange(int)));
    connect(ui->subSensSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSubSensChange(int)));
}

void selectToolMenu::onSizeChange(int changedVal)
{
    brushSize = changedVal;
    freeDist = brushSize*2;
    emit cursorChanged();
}
void selectToolMenu::onDistChange(double changedVal)
{
    freeDist = brushSize*changedVal;
}
void selectToolMenu::onFillSensChange(int changedVal)
{
    stopAdd = maxDiffAbs*changedVal/100;
}
void selectToolMenu::onSubSensChange(int changedVal)
{
    stopSub = maxDiffAbs*changedVal/100;
}


int selectToolMenu::getBrushSize() const
{
    return brushSize;
}
int selectToolMenu::getStopAdd() const
{
    return stopAdd;
}
int selectToolMenu::getStopSub() const
{
    return stopSub;
}
int selectToolMenu::getFreeDist() const
{
    return freeDist;
}

void selectToolMenu::setBrushSize(int newSize)
{
    brushSize = newSize;
    freeDist = brushSize*2;
    ui->sizeSpinBox->blockSignals(true);
    ui->sizeSpinBox->setValue(newSize);
    ui->sizeSpinBox->blockSignals(false);
}

selectToolMenu::~selectToolMenu()
{
    delete ui;
}
