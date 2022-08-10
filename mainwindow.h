#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editframe.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <QStandardPaths>
#include <QKeyEvent>

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

    void on_brushSizeBox_valueChanged(int arg1);

    void on_pushButton_3_clicked();

    void on_ShapeComboBox_currentTextChanged(const QString &arg1);

    void on_ShapeComboBox_currentIndexChanged(int index);

    void on_selectionButton_clicked();

protected:

private:
    Ui::MainWindow *ui;
    QColor pickedColor = Qt::green;
};
#endif // MAINWINDOW_H
