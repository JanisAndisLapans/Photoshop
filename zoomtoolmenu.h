#ifndef ZOOMTOOLMENU_H
#define ZOOMTOOLMENU_H

#include <QWidget>

namespace Ui {
class ZoomToolMenu;
}

class ZoomToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomToolMenu(QWidget *parent = nullptr);

    double getZoomAmount() const;
    void setZoom(double amount);
    ~ZoomToolMenu();

private:
    double zoomAmount = 1.0;

    Ui::ZoomToolMenu *ui;

public slots:
    void onZoomChange(int changedValue);
signals:
    void changedZoom(double changedValue);
};

#endif // ZOOMTOOLMENU_H
