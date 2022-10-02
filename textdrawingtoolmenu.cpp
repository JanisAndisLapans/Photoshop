#include "textdrawingtoolmenu.h"
#include "ui_textdrawingtoolmenu.h"

TextDrawingToolMenu::TextDrawingToolMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextDrawingToolMenu)
{
    ui->setupUi(this);

    ui->colorLabel->setColor(Qt::black);

    connect(ui->fontComboBox, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(onParamsChanged()));
    connect(ui->sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onParamsChanged()));
    connect(ui->boldCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onParamsChanged()));
    connect(ui->italicCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onParamsChanged()));
    connect(ui->underlineCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onParamsChanged()));
    connect(ui->colorLabel, SIGNAL(colorChanged(const QColor&)), this, SLOT(onColorChanged(const QColor&)));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onCancelButtonPress()));
    connect(ui->finishButton, SIGNAL(clicked()), this, SLOT(onFinishButtonPress()));
}

void TextDrawingToolMenu::setFinishButtonsEnabled(bool value)
{
    ui->finishButton->setEnabled(value);
    ui->cancelButton->setEnabled(value);
}

QColor TextDrawingToolMenu::getColor() const
{
    return ui->colorLabel->getColor();
}

QFont TextDrawingToolMenu::getFont() const
{
    return ui->fontComboBox->currentFont();
}

int TextDrawingToolMenu::getSize() const
{
    return ui->sizeSpinBox->value();
}

bool TextDrawingToolMenu::isBold() const
{
    return ui->boldCheckBox->isChecked();
}

bool TextDrawingToolMenu::isItalic() const
{
    return ui->italicCheckBox->isChecked();
}

bool TextDrawingToolMenu::isUnderline() const
{
    return ui->underlineCheckBox->isChecked();
}

void TextDrawingToolMenu::setColor(const QColor& color)
{
    ui->colorLabel->blockSignals(true);
    ui->colorLabel->setColor(color);
    ui->colorLabel->blockSignals(false);
}

void TextDrawingToolMenu::setFont(const QFont& font)
{
    ui->fontComboBox->blockSignals(true);
    ui->fontComboBox->setCurrentFont(font);
    ui->fontComboBox->blockSignals(false);
}

void TextDrawingToolMenu::setSize(int size)
{
    ui->sizeSpinBox->blockSignals(true);
    ui->sizeSpinBox->setValue(size);
    ui->sizeSpinBox->blockSignals(false);
}

void TextDrawingToolMenu::setBold(bool bold)
{
    ui->boldCheckBox->blockSignals(true);
    ui->boldCheckBox->setChecked(bold);
    ui->boldCheckBox->blockSignals(false);
}

void TextDrawingToolMenu::setItalic(bool italic)
{
    ui->italicCheckBox->blockSignals(true);
    ui->italicCheckBox->setChecked(italic);
    ui->italicCheckBox->blockSignals(false);
}

void TextDrawingToolMenu::setUnderline(bool underline)
{
    ui->underlineCheckBox->blockSignals(true);
    ui->underlineCheckBox->setChecked(underline);
    ui->underlineCheckBox->blockSignals(false);
}

void TextDrawingToolMenu::onParamsChanged()
{
    emit paramsChanged();
}

void TextDrawingToolMenu::onColorChanged(const QColor& color)
{
    emit colorChanged(color);
}

void TextDrawingToolMenu::onCancelButtonPress()
{
    emit cancel();
}
void TextDrawingToolMenu::onFinishButtonPress()
{
    emit finish();
}

TextDrawingToolMenu::~TextDrawingToolMenu()
{
    delete ui;
}
