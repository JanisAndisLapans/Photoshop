#include "stamptoolmenu.h"
#include "ui_stamptoolmenu.h"

StampToolMenu::StampToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StampToolMenu)
{
    ui->setupUi(this);
    connect(ui->sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSizeChange(int)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChange(int)));
    connect(ui->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onModeChanged(int)));
}

void StampToolMenu::onSizeChange(int changedVal)
{
    size = changedVal;
    emit cursorChanged();
}

void StampToolMenu::onTypeChange(int index)
{
    switch(index)
    {
        case 0:
            currType = Circle;
            emit cursorChanged();
            break;
        case 1:
            currType = Square;
            emit cursorChanged();
            break;
        case 2:
            currType = FadedCircle;
            emit cursorChanged();
            break;
    }
}


void StampToolMenu::onModeChanged(int index)
{
    switch(index)
    {
        case 0:
            currMode = Over;
            break;
        case 1:
            currMode = Multiply;
            break;
        case 2:
            currMode = Screen;
            break;
        case 3:
            currMode = Overlay;
            break;
    }
}

StampToolMenu::Mode StampToolMenu::getMode() const
{
    return currMode;
}

StampToolMenu::BrushType StampToolMenu::getBrushType() const
{
    return currType;
}

int StampToolMenu::getBrushSize() const
{
    return size;
}

void StampToolMenu::setBrushSize(int newSize)
{
    size = newSize;
    ui->sizeSpinBox->blockSignals(true);
    ui->sizeSpinBox->setValue(newSize);
    ui->sizeSpinBox->blockSignals(false);
}


StampToolMenu::~StampToolMenu()
{
    delete ui;
}
