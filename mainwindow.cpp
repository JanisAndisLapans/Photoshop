#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->editFrame->installEventFilter(this);

    auto pointer = new PointerTool(ui->editFrame);
    tools["pointer"] = pointer;
    auto move = new MoveTool(ui->editFrame, ui->editFrame->getLayersRef());
    tools["move"] = move;
    auto select = new SelectTool(ui->editFrame, ui->editFrame->getLayersRef());
    tools["select"] = select;
    auto paint = new BrushTool(ui->editFrame, ui->editFrame->getLayersRef());
    tools["paint"] = paint;
    auto zoom = new ZoomTool(ui->editFrame);
    tools["zoom"] = zoom;
    auto stamp = new StampTool(ui->editFrame, ui->editFrame->getLayersRef());
    tools["stamp"] = stamp;
    auto text = new TextDrawingTool(ui->editFrame, ui->editFrame->getLayersRef());
    tools["text"] = text;
    auto transform = new TransformTool(ui->editFrame);
    tools["transfrom"] = transform;
    ui->editFrame->setTransformTool(transform);

    ui->OptionLayout->layout()->addWidget(select->getMenu());
    select->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(paint->getMenu());
    paint->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(zoom->getMenu());
    zoom->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(move->getMenu());
    move->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(stamp->getMenu());
    stamp->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(text->getMenu());
    text->getMenu()->setVisible(false);
    ui->OptionLayout->layout()->addWidget(transform->getMenu());
    transform->getMenu()->setVisible(false);

    ui->OptionLayout->layout()->addWidget(pointer->getMenu());
    ui->editFrame->enableTool(pointer);


    ui->layerFrame->setEditFrame(ui->editFrame);
    ui->menubar->setFocusProxy(ui->editFrame);

    ui->editFrame->setScrollArea(ui->scrollArea);

    connect(ui->editFrame, SIGNAL(enableUndo(bool)), this, SLOT(onUndoEnable(bool)));
    connect(ui->editFrame, SIGNAL(enableRedo(bool)), this, SLOT(onRedoEnable(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) , tr("Image Files (*.png *.jpg *.bmp)"));

    ui->editFrame->addImg(fileName);
    ui->layerFrame->update();
}

void MainWindow::on_pushButton_clicked()
{
    ui->editFrame->enableTool(tools["move"]);
}


void MainWindow::on_resizeButton_clicked()
{
    ui->editFrame->enableTool(tools["zoom"]);
}


void MainWindow::on_brushButton_clicked()
{
     ui->editFrame->enableTool(tools["paint"]);
}

void MainWindow::on_selectionButton_clicked()
{
     ui->editFrame->enableTool(tools["select"]);
}

void MainWindow::on_stampPushButton_clicked()
{
     ui->editFrame->enableTool(tools["stamp"]);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->editFrame->enableTool(tools["pointer"]);
}

void MainWindow::on_textPushButton_clicked()
{
    ui->editFrame->enableTool(tools["text"]);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* kev = dynamic_cast<QKeyEvent*>(event);
        keyPressEvent(kev);
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent* ev)
{
    if(ev->modifiers() & Qt::ControlModifier)
    {
        if(ev->key() == Qt::Key_Z)
        {
            if(ev->modifiers() & Qt::ShiftModifier)
                ui->editFrame->redo();
            else
                ui->editFrame->undo();
        }
    }
    else
    {
        switch(ev->key())
        {
            case Qt::Key_Z:
                ui->editFrame->enableTool(tools["zoom"]);
                break;
            case Qt::Key_W:
                ui->editFrame->enableTool(tools["select"]);
                break;
            case Qt::Key_S:
                 ui->editFrame->enableTool(tools["stamp"]);
                break;
            case Qt::Key_B:
                ui->editFrame->enableTool(tools["paint"]);
                break;
            case Qt::Key_M:
                ui->editFrame->enableTool(tools["move"]);
                break;
            case Qt::Key_P:
                ui->editFrame->enableTool(tools["pointer"]);
                break;
            case Qt::Key_T:
                ui->editFrame->enableTool(tools["text"]);
                break;
        }
    }
}

void MainWindow::on_actionSolid_color_triggered()
{
    SolidColorLayerDialog d(this);
    d.setModal(true);
    ui->editFrame->saveState();
    if(d.exec() == QDialog::Accepted)
    {
        ui->editFrame->addSolidLayer(d.getColor(), d.getSize());
    }
}

void MainWindow::on_actionRedio_triggered()
{

}

void MainWindow::on_actionUndo_triggered()
{
    ui->editFrame->undo();
}

void MainWindow::onUndoEnable(bool val)
{
    ui->actionUndo->setEnabled(val);
}

void MainWindow::onRedoEnable(bool val)
{
    ui->actionRedo->setEnabled(val);
}


void MainWindow::on_actionRedo_triggered()
{
    ui->editFrame->redo();
}

