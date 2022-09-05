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

private:
    Ui::TransformToolMenu *ui;

private slots:
    void onEndTransformButtonPress();
    void onCancelTransformButtonPress();
    void onRotationChanged(double degrees);

signals:
    void finishTransform();
    void cancelTransform();
    void rotated(double degrees);

};

#endif // TRANSFORMTOOLMENU_H
