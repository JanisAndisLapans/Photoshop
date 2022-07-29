#ifndef BRUSHSHAPE_H
#define BRUSHSHAPE_H

#include <utility>

#include <QImage>
#include <QColor>

using namespace std;

class BrushShape
{
public:
    BrushShape(int size, const QColor& color);

    void resize(int newSize);
    void changeSource(QColor newColor);
    QImage getImg() const;
    QColor getColor() const;
    const QImage* getSrcImg() const;
    bool isColorSrc() const;
    int getSize() const;

protected:
    virtual void initialize(const QColor& color);
    QImage img;
    int size;
    QImage *srcImg;
    QPoint srcImgPos;
    QColor sourceColor;
    bool isColor;
};

#endif // BRUSHSHAPE_H
