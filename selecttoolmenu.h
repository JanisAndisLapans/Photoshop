#ifndef SELECTTOOLMENU_H
#define SELECTTOOLMENU_H

#include <QWidget>
#include "ImageAlgorithms.h"

namespace Ui {
class selectToolMenu;
}

class selectToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit selectToolMenu(QWidget *parent = nullptr);

    int getBrushSize() const;
    int getStopAdd() const;
    int getStopSub() const;
    int getFreeDist() const;

    ~selectToolMenu();

private:
    Ui::selectToolMenu *ui;

private slots:
    void onSizeChange(int changedVal);
    void onDistChange(double changedVal);
    void onFillSensChange(int changedVal);
    void onSubSensChange(int changedVal);

private:
    const double maxDiffAbs = ImageAlgorithms::colorDifference(qRgb(0,0,0), qRgb(255,255,255));
    int brushSize = 15;
    int stopAdd = maxDiffAbs*0.05;
    int stopSub = maxDiffAbs*0.03;
    int freeDist = 2*brushSize;

signals:
    void cursorChanged();
};

#endif // SELECTTOOLMENU_H
