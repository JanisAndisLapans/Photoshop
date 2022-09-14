#ifndef TRANSFORMTOOLMENU_H
#define TRANSFORMTOOLMENU_H

#include <QWidget>

#include <cmath>

using namespace std;

namespace Ui {
class TransformToolMenu;
}

class TransformToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit TransformToolMenu(QWidget *parent = nullptr);
    ~TransformToolMenu();
    void setRotation(double degrees);
    void setSize(const QSize& newSize);
    void setPos(const QPoint& newPos);

private:
    Ui::TransformToolMenu *ui;

private slots:
    void onEndTransformButtonPress();
    void onCancelTransformButtonPress();
    void onRotationChanged(double degrees);
    void onWidthChange(int width);
    void onHeightChange(int height);
    void onXChanged(int x);
    void onYChanged(int y);

signals:
    void finishTransform();
    void cancelTransform();
    void rotated(double degrees);
    void dimsChanged(const QSize& newSize);
    void posChanged(const QPoint& newPos);

};

#endif // TRANSFORMTOOLMENU_H
