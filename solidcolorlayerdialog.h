#ifndef SOLIDCOLORLAYERDIALOG_H
#define SOLIDCOLORLAYERDIALOG_H

#include <utility>

#include <QDialog>
#include <QColorDialog>
#include <QSize>
#include <QPainter>

using namespace std;

namespace Ui {
class SolidColorLayerDialog;
}

class SolidColorLayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolidColorLayerDialog(QWidget *parent = nullptr);
    QSize getSize() const;
    QColor getColor() const;
    QPainter::CompositionMode getCompositionMode();

    ~SolidColorLayerDialog();

private:
    Ui::SolidColorLayerDialog *ui;
    QColor pickedColor = Qt::green;
    QWidget* parent = nullptr;

private slots:
    void onColorChanged(const QColor& color);
};

#endif // SOLIDCOLORLAYERDIALOG_H
