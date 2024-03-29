#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editframe.h"
#include "edittool.h"
#include "brushtool.h"
#include "movetool.h"
#include "selecttool.h"
#include "zoomtool.h"
#include "pointertool.h"
#include "stamptool.h"
#include "transformtool.h"
#include "solidcolorlayerdialog.h"
#include "textdrawingtool.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <QStandardPaths>
#include <QKeyEvent>
#include <QMap>

#include <utility>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_pushButton_clicked();

    void on_resizeButton_clicked();

    void on_brushButton_clicked();

    void on_selectionButton_clicked();

    void on_stampPushButton_clicked();

    void on_pushButton_2_clicked();
    void on_actionSolid_color_triggered();

    void on_textPushButton_clicked();

    void on_actionRedio_triggered();

    void on_actionUndo_triggered();

    void onUndoEnable(bool);
    void onRedoEnable(bool);

    void on_actionRedo_triggered();

    void on_actionSave_triggered();

protected:
    virtual void keyPressEvent(QKeyEvent* ev) override;
    virtual bool eventFilter(QObject* obj, QEvent* event) override;
private:

    void saveFile();

    Ui::MainWindow *ui;
    QColor pickedColor = Qt::green;
    unordered_map<string,EditTool*> tools;
};
#endif // MAINWINDOW_H
