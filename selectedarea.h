#ifndef SELECTEDAREA_H
#define SELECTEDAREA_H

#include "ImageAlgorithms.h"

#include <cstring>
#include <utility>

#include <QPoint>
#include <QImage>
#include <QDebug>

using namespace std;

class SelectedArea
{
public:
    SelectedArea();
    SelectedArea(int width, int height);
    SelectedArea(uchar* selectedArea, int width, int height, long long pixelCount=-1);
    void change(QRect rectAffected, long long pixelCount=-1);
    const QImage& getContourImg1() const;
    const QImage& getContourImg2() const;
    uchar* getSelectedArea();
    bool hasSelected() const;
    void resize(int width, int height);
    long long getPixelCount() const;
    int getSize() const;
    ~SelectedArea();
private:
    void generateImgAnew();
    void generateImgFast(QRect rectAffected);
    uchar* selectedArea = nullptr;
    long long selectedPixelCount = 0;
    int size;
    QImage contour1, contour2;
};

#endif // SELECTEDAREA_H
