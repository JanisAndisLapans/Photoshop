#ifndef SOLIDCOLORLAYERDIALOG_H
#define SOLIDCOLORLAYERDIALOG_H

#include <utility>

#include <QDialog>
#include <QColorDialog>
#include <QSize>

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
    ~SolidColorLayerDialog();
protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    Ui::SolidColorLayerDialog *ui;
    QColor pickedColor = Qt::green;
    QWidget* parent = nullptr;
};

#endif // SOLIDCOLORLAYERDIALOG_H
