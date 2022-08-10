#include "selectedarea.h"

SelectedArea::SelectedArea()
{
    size = 0;
}

void SelectedArea::generateImgFast(QRect rectAffected)
{
    static const auto black = qRgb(0,0,0), white = qRgb(255,255,255), transparent = qRgba(0,0,0,0);

    bool state = true;
    auto pixels1 = reinterpret_cast<QRgb*>(contour1.bits()),
         pixels2 = reinterpret_cast<QRgb*>(contour2.bits());
    for(int i = max(0, rectAffected.topLeft().x() + rectAffected.topLeft().y()*size);
        i<size*size && i<=rectAffected.bottomRight().x() + rectAffected.bottomRight().y()*size; i++)
    {
        if(selectedArea[i]==2)
        {
            if(state)
            {
                pixels1[i] = black;
                pixels2[i] = white;
            }
            else
            {
                pixels2[i] = black;
                pixels1[i] = white;
            }
            state = !state;
        }
        else
        {
            pixels1[i] = transparent;
            pixels2[i] = transparent;
        }
    }

    //contour1 = QImage(reinterpret_cast<uchar*>(pixels1), size, size, QImage::Format_ARGB32).copy();
    //contour2 = QImage(reinterpret_cast<uchar*>(pixels2), size, size, QImage::Format_ARGB32).copy();
}

void SelectedArea::generateImgAnew()
{
    static const auto black = qRgb(0,0,0), white = qRgb(255,255,255), transparent = qRgba(0,0,0,0);

    bool state = true;
    auto pixels1 = new QRgb[size*size],
         pixels2 = new QRgb[size*size];
    for(int i = 0; i<size*size; i++)
    {
        if(selectedArea[i]==2)
        {
            if(state)
            {
                pixels1[i] = black;
                pixels2[i] = white;
            }
            else
            {
                pixels2[i] = black;
                pixels1[i] = white;
            }
            state = !state;
        }
        else
        {
            pixels1[i] = transparent;
            pixels2[i] = transparent;
        }

    }

    contour1 = QImage(reinterpret_cast<uchar*>(pixels1), size, size, QImage::Format_ARGB32);
    contour2 = QImage(reinterpret_cast<uchar*>(pixels2), size, size, QImage::Format_ARGB32);
}

SelectedArea::SelectedArea(int width, int height)
    :size(max(width, height))
{
    selectedArea = new uchar[size*size];
    memset(selectedArea, 0, size*size);
}

SelectedArea::SelectedArea(uchar* selectedArea,int width, int height, long long pixelCount)
    :selectedArea(selectedArea), size(max(width, height))
{
    if(pixelCount!=-1) this->selectedPixelCount = pixelCount;
    generateImgAnew();
}
void SelectedArea::change(QRect rectAffected, long long pixelCount)
{
    if(pixelCount!=-1) this->selectedPixelCount = pixelCount;
    generateImgFast(rectAffected);
}
const QImage& SelectedArea::getContourImg1() const
{
    return contour1;
}

const QImage& SelectedArea::getContourImg2() const
{
    return contour2;
}

uchar* SelectedArea::getSelectedArea()
{
    return selectedArea;
}

bool SelectedArea::hasSelected() const
{
    return selectedPixelCount > 0;
}

void SelectedArea::resize(int width, int height)
{
    if(size<width || size<height)
    {
        auto newSize = max(width, height);
        auto allocated = new uchar[newSize*newSize];
        memset(allocated, 0, newSize*newSize);
        for(int y = 0; y<size; y++)
        {
            for(int x = 0; x<size; x++)
            {
                allocated[x + y*newSize] = selectedArea[x + y*size];
            }
        }
        delete[] selectedArea;
        selectedArea = allocated;
        size = newSize;
        generateImgAnew();
    }
}

long long SelectedArea::getPixelCount() const
{
    return selectedPixelCount;
}


int SelectedArea::getSize() const
{
    return size;
}


SelectedArea::~SelectedArea()
{
    delete[] selectedArea;
}


