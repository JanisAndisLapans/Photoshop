#include "brushtoolmenu.h"
#include "ui_brushtoolmenu.h"

BrushToolMenu::BrushToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrushToolMenu)
{
    ui->setupUi(this);
    ui->colorLabel->setCursor(Qt::PointingHandCursor);
    ui->colorLabel->installEventFilter(this);
    connect(ui->sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSizeChange(int)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChange(int)));
    connect(ui->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onModeChanged(int)));
    connect(ui->colorLabel, SIGNAL(colorChanged(const QColor&)), this, SLOT(onColorChanged(const QColor&)));
    brushShape = new BrushCircle(size, pickedColor);
}

void BrushToolMenu::onColorChanged(const QColor& color)
{
    pickedColor = color;
    brushShape->changeSource(pickedColor);
    emit cursorChanged();
}

void BrushToolMenu::onSizeChange(int changedVal)
{
    size = changedVal;
    brushShape->resize(size);
    emit cursorChanged();
}

void BrushToolMenu::onTypeChange(int index)
{
    switch(index)
    {
        case 0:
            brushShape = new BrushCircle(size, pickedColor);
            emit cursorChanged();
            break;
        case 1:
            brushShape = new BrushSquare(size, pickedColor);
            emit cursorChanged();
            break;
        case 2:
            brushShape = new BrushFadedCircle(size, pickedColor);
            emit cursorChanged();
            break;
    }
}

const BrushShape& BrushToolMenu::getBrushShape() const
{
    return *brushShape;
}

void BrushToolMenu::onModeChanged(int index)
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

BrushToolMenu::Mode BrushToolMenu::getMode() const
{
    return currMode;
}

void BrushToolMenu::setBrushSize(int newSize)
{
    size = newSize;
    brushShape->resize(size);
    ui->sizeSpinBox->blockSignals(true);
    ui->sizeSpinBox->setValue(newSize);
    ui->sizeSpinBox->blockSignals(false);
}


BrushToolMenu::~BrushToolMenu()
{
    delete ui;
}
