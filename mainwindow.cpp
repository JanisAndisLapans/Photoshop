#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currTool = new PointerTool(ui->editFrame);
    tools["pointer"] = currTool;
    auto move = new MoveTool(ui->editFrame,ui->editFrame->getLayersRef());
    tools["move"] = move;
    auto select = new SelectTool(ui->editFrame, ui->editFrame->getLayersRef());
    tools["select"] = select;
    auto paint = new BrushTool(ui->editFrame, ui->editFrame->getLayersRef());
    tools["paint"] = paint;
    auto zoom = new ZoomTool(ui->editFrame);
    tools["zoom"] = zoom;

    ui->OptionLayout->layout()->addWidget(select->getMenu());
    select->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(paint->getMenu());
    paint->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(zoom->getMenu());
    zoom->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(move->getMenu());
    move->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(currTool->getMenu());
    currTool->setCursor();
    ui->editFrame->installEventFilter(currTool);

    ui->layerFrame->setEditFrame(ui->editFrame);
    ui->menubar->setFocusProxy(ui->editFrame);

    ui->editFrame->setScrollArea(ui->scrollArea);
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
    ui->layerFrame->update();

}

void MainWindow::enableTool(const string& name)
{
    currTool->getMenu()->setVisible(false);
    ui->editFrame->removeEventFilter(currTool);
    currTool = tools[name];
    currTool->getMenu()->setVisible(true);
    currTool->setCursor();
    ui->editFrame->installEventFilter(currTool);
}

void MainWindow::on_pushButton_clicked()
{
    enableTool("move");
}


void MainWindow::on_resizeButton_clicked()
{
    enableTool("zoom");
}


void MainWindow::on_brushButton_clicked()
{
    enableTool("paint");
}

void MainWindow::on_selectionButton_clicked()
{
    enableTool("select");
}

void MainWindow::keyPressEvent(QKeyEvent* ev)
{
    switch(ev->key())
    {
        case Qt::Key_Z:
            enableTool("zoom");
            break;
        case Qt::Key_S:
            enableTool("select");
            break;
        case Qt::Key_B:
            enableTool("paint");
            break;
        case Qt::Key_M:
            enableTool("move");
            break;
    }
}




