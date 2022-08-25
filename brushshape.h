#ifndef BRUSHSHAPE_H
#define BRUSHSHAPE_H

#include <utility>

#include <QImage>
#include <QColor>
#include <QObject>

using namespace std;

class BrushShape : public QObject
{
    Q_OBJECT

public:
    BrushShape(int size, const QColor& color);
    BrushShape(int size, const QImage& img);

    void resize(int newSize);
    void changeSource(QColor newColor);
    void changeSource(const QImage& newImage);
    QImage getImg() const;
    QColor getColor() const;
    bool isColorSrc() const;
    int getSize() const;

protected:
    virtual void initialize(const QColor& color);
    virtual void initialize(const QImage& img);
    QImage img;
    int size;
    QColor sourceColor;
    bool isColor;
};

#endif // BRUSHSHAPE_H
