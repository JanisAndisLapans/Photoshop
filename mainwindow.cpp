#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->layerFrame->setEditFrame(ui->editFrame);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) , tr("Image Files (*.png *.jpg *.bmp)"));

    ui->editFrame->setImg(fileName);
    ui->editFrame->update();
    ui->layerFrame->update();
}


void MainWindow::on_pushButton_clicked()
{
    ui->editFrame->setTool(EditFrame::Move);
}


void MainWindow::on_resizeButton_clicked()
{
    ui->editFrame->setTool(EditFrame::Resize);
}


void MainWindow::on_brushButton_clicked()
{
    ui->editFrame->setTool(EditFrame::Brush);
}


void MainWindow::on_brushSizeBox_valueChanged(int arg1)
{
    ui->editFrame->setBrushSize(arg1);
}


void MainWindow::on_pushButton_3_clicked()
{
    pickedColor = QColorDialog::getColor(pickedColor, this);
    auto css = "QLabel { background-color :" + pickedColor.name() + "}";
    ui->colorLabel->setStyleSheet(css);
    ui->editFrame->setBrushColor(pickedColor);
}


void MainWindow::on_ShapeComboBox_currentTextChanged(const QString &arg1)
{

}


void MainWindow::on_ShapeComboBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0: //square
            ui->editFrame->setBrushShape(EditFrame::Square);
            break;
        case 1: //circle
            ui->editFrame->setBrushShape(EditFrame::Circle);
            break;
        case 2: //faded circle
            ui->editFrame->setBrushShape(EditFrame::FadedCircle);
            break;
    }
}

