#ifndef TRANSFORMTOOLMENU_H
#define TRANSFORMTOOLMENU_H

#include <QWidget>

namespace Ui {
class TransformToolMenu;
}

class TransformToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit TransformToolMenu(QWidget *parent = nullptr);
    ~TransformToolMenu();

private:
    Ui::TransformToolMenu *ui;

private slots:
    void onEndTransformButtonPress();
    void onCancelTransformButtonPress();

signals:
    void finishTransform();
    void cancelTransform();

};

#endif // TRANSFORMTOOLMENU_H
