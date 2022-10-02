#include "textdrawingtool.h"
#include "editframe.h"

TextDrawingTool::TextDrawingTool(EditFrame *editFrame, QVector<Layer*>* layers)
    :EditTool(editFrame), layers(layers)
{
    menu = new TextDrawingToolMenu();
    connect(menu, SIGNAL(paramsChanged()), this, SLOT(onParamsChanged()));
    connect(menu, SIGNAL(colorChanged(const QColor&)), this, SLOT(onColorChanged(const QColor&)));
    connect(menu, SIGNAL(finish()), this, SLOT(onFinish()));
    connect(menu, SIGNAL(cancel()), this, SLOT(onCancel()));
    edit = new QTextEdit(editFrame);
    edit->acceptRichText();
    edit->setVisible(false);
    edit->setFocusPolicy(Qt::StrongFocus);
    edit->viewport()->setAutoFillBackground(false);
    edit->setStyleSheet("border: 1px solid blue");
    connect(edit, SIGNAL(cursorPositionChanged()), this, SLOT(onEditCursorPositionChanged()));
    onParamsChanged();
}

void TextDrawingTool::onFinish()
{
    editFrame->saveState();
    QImage txtImg(edit->width(),edit->height(), QImage::Format_ARGB32);
    txtImg.fill(Qt::transparent);
    QPainter painter(&txtImg);
    edit->document()->drawContents(&painter, edit->rect());
    editFrame->addImg(txtImg, edit->toPlainText().first(min(static_cast<int>(edit->toPlainText().length()), 20)), edit->pos());
    end();
    start();
    setCursor();
}

void TextDrawingTool::onCancel()
{
    end();
    start();
    setCursor();
}

TextDrawingToolMenu* TextDrawingTool::getMenu()
{
    return menu;
}

void TextDrawingTool::setCursor()
{
    if(drawn)
        editFrame->setCursor(Qt::ArrowCursor);
    else
        editFrame->setCursor(Qt::IBeamCursor);
}

void TextDrawingTool::onMousePress(QMouseEvent *ev)
{
    static const QString lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas varius aliquam elit a congue. Fusce tempus ante ac dui finibus viverra. Interdum et malesuada fames ac ante ipsum primis in faucibus. Vestibulum luctus, metus non convallis venenatis,"
                                 " magna ipsum sodales mi, nec tempor dui lacus eu turpis. Maecenas vulputate interdum lectus, feugiat blandit nisi. Duis enim mi, luctus ut mattis a, lobortis id lorem. Aliquam congue ante vitae leo efficitur, eu aliquet ante tempus. Etiam consequat a velit ut aliquam. Nunc ultrices leo nulla, sit amet maximus libero dignissim a. Morbi consequat nisi mattis finibus mattis. Aliquam erat volutpat. Etiam quis mi ultricies, vehicula lorem a, rhoncus ligula.     Sed quis elit velit. Integer vehicula tempor nisi eget maximus. Ut venenatis eget augue at tristique. Sed dictum enim a tellus dictum ornare. "
                                 "Quisque volutpat blandit lacinia. Morbi risus lacus, mollis et hendrerit ac, egestas sed velit. Sed egestas vitae urna consequat eleifend. Etiam vel finibus dui, non hendrerit nisl. Curabitur consequat tempus neque eu tristique. Duis facilisis molestie diam scelerisque dignissim. Quisque ut purus tellus. In et sapien commodo, mollis velit sit amet, mollis augue. Mauris elementum nisi nec mi vehicula vestibulum.    Sed efficitur, est eget accumsan interdum, quam mauris cursus nibh, at aliquam est risus at ipsum. Proin sed dolor euismod, pellentesque augue vitae, sagittis massa. Nulla commodo ut nisl ac iaculis. "
                                 "Curabitur quis consectetur est. Aliquam a ex ac nibh facilisis imperdiet nec et nisl. Donec quis turpis at felis dictum elementum. Vestibulum pretium, lectus eu porttitor sagittis, nunc mi suscipit metus, quis iaculis justo quam at nisi. Maecenas posuere nunc eu odio vehicula, ut posuere risus pretium. Ut at sagittis augue. Maecenas aliquet nisl quis magna vulputate luctus. Nam pretium rutrum diam. Suspendisse potenti. Quisque erat metus, facilisis id commodo eu, interdum a leo. Sed imperdiet pharetra egestas";
    if(ev->button() & Qt::LeftButton && !drawn)
    {
        drawingStarted = true;
        edit->setText(lorem);
        edit->resize(0,0);
        edit->move(ev->pos());
        edit->setVisible(true);

        drawingStartPos = ev->pos();
    }
    else if(ev->button() & Qt::RightButton)
    {
        drawingStarted = false;
        edit->setVisible(false);
    }
    if(transformingAvailable) transforming = true;
}

void TextDrawingTool::onMouseRelease(QMouseEvent *ev)
{
    if(ev->button() & Qt::LeftButton && drawingStarted)
    {
        menu->setFinishButtonsEnabled(true);
        drawingStarted = false;
        drawn = true;
        setCursor();
        editFrame->blockUndo(true);
    }
    if(transforming) transforming = false;
}

void TextDrawingTool::onMouseMove(QMouseEvent *ev)
{
    if(drawingStarted)
    {
        auto dif = ev->pos() - drawingStartPos;
        edit->resize(max(0,dif.x()), max(0, dif.y()));
    }
    else if(!transforming && drawn)
    {
        auto activateTreshhold = 10;
        auto rect = edit->rect();
        rect.translate(edit->pos());
        if(!rect.contains(ev->pos())) goto pass;
        if(ImageAlgorithms::pointDistance(ev->pos() , rect.topLeft()) < activateTreshhold)
        {
            currResizeMethod = bind(&TextDrawingTool::topLeftResize, this, _1);
            editFrame->setCursor(Qt::SizeFDiagCursor);
        }
        else if(ImageAlgorithms::pointDistance(ev->pos() , rect.topRight()) < activateTreshhold)
        {
            currResizeMethod = bind(&TextDrawingTool::topRightResize, this, _1);
            editFrame->setCursor(Qt::SizeBDiagCursor);
        }
        else if(ImageAlgorithms::pointDistance(ev->pos() , rect.bottomLeft())  < activateTreshhold)
        {
            currResizeMethod = bind(&TextDrawingTool::bottomLeftResize, this, _1);
            editFrame->setCursor(Qt::SizeBDiagCursor);
        }
        else if(ImageAlgorithms::pointDistance(ev->pos() , rect.bottomRight())  < activateTreshhold)
        {
            currResizeMethod = bind(&TextDrawingTool::bottomRightResize, this, _1);
            editFrame->setCursor(Qt::SizeFDiagCursor);
        }
        else if(abs(ev->pos().x() - rect.left()) < activateTreshhold)
        {
            currResizeMethod = bind(&TextDrawingTool::leftResize, this, _1);
            editFrame->setCursor(Qt::SizeHorCursor);
        }
        else if(abs(ev->pos().x() - rect.right()) < activateTreshhold)
        {
            currResizeMethod = bind(&TextDrawingTool::rightResize, this, _1);
            editFrame->setCursor(Qt::SizeHorCursor);
        }
        else if(abs(ev->pos().y() - rect.top()) < activateTreshhold)
        {
            currResizeMethod = bind(&TextDrawingTool::topResize, this, _1);
            editFrame->setCursor(Qt::SizeVerCursor);
        }
        else if(abs(ev->pos().y() - rect.bottom()) < activateTreshhold)
        {
            currResizeMethod = bind(&TextDrawingTool::bottomResize, this, _1);
            editFrame->setCursor(Qt::SizeVerCursor);
        }
        else
        {
            pass:
            transformingAvailable = false;
            setCursor();
            return;
        }
        transformingAvailable = true;
    }
    else if(transforming)
    {
        currResizeMethod(ev->pos());
    }
}

bool TextDrawingTool::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::MouseButtonPress:
            onMousePress(dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseMove:
            onMouseMove(dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            onMouseRelease(dynamic_cast<QMouseEvent*>(event));
        case QEvent::KeyPress:
            if(edit->hasFocus()) return false;
        break;
    }
    return QObject::eventFilter(obj, event);
}

void TextDrawingTool::onParamsChanged()
{
    QTextCharFormat format;
    format.setFont(menu->getFont());
    format.setFontPointSize(menu->getSize());
    format.setFontItalic(menu->isItalic());
    format.setFontWeight(menu->isBold() ? 75 : 50);
    format.setUnderlineStyle(menu->isUnderline() ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline);
    edit->setCurrentCharFormat(format);
    edit->setTextColor(menu->getColor());
}

void TextDrawingTool::onColorChanged(const QColor& color)
{
    edit->setTextColor(color);
}

void TextDrawingTool::onEditCursorPositionChanged()
{
    auto posFormat = edit->currentCharFormat();
    menu->setFont(posFormat.font());
    menu->setSize(posFormat.fontPointSize());
    menu->setItalic(posFormat.fontItalic());
    menu->setBold(posFormat.fontWeight() > 50);
    menu->setUnderline(edit->fontUnderline() != QTextCharFormat::NoUnderline);
    menu->setColor(edit->textColor());
}

void TextDrawingTool::start()
{
    menu->setFinishButtonsEnabled(false);
}

void TextDrawingTool::end()
{
    edit->setVisible(false);
    drawn = false;
    drawingStarted = false;
    editFrame->blockUndo(false);
}

void TextDrawingTool::topLeftResize(const QPoint& pos)
{
    auto rect = edit->rect();
    rect.translate(edit->pos());
    if(pos.x() > rect.right() || pos.y() > rect.bottom() || pos.x() < 0 || pos.y() < 0) return;
    auto dif = pos - rect.topLeft();

    edit->resize(edit->width() - dif.x(), edit->height() - dif.y());
    edit->move(pos);
}

void TextDrawingTool::topRightResize(const QPoint& pos)
{
    auto rect = edit->rect();
    rect.translate(edit->pos());
    if(pos.x() < rect.left() || pos.y() > rect.bottom() || pos.y() < 0) return;
    auto dif = pos - rect.topRight();

    edit->resize(edit->width() + dif.x(), edit->height() - dif.y());
    edit->move(QPoint(rect.x(), pos.y()));

}

void TextDrawingTool::bottomLeftResize(const QPoint& pos)
{
    auto rect = edit->rect();
    rect.translate(edit->pos());
    if(pos.x() > rect.right() || pos.x() < 0 || pos.y() < rect.top()) return;
    auto dif = pos - rect.bottomLeft();

    edit->resize(edit->width() - dif.x(), edit->height() + dif.y());
    edit->move(QPoint(pos.x(), rect.y()));

}

void TextDrawingTool::bottomRightResize(const QPoint& pos)
{
    auto rect = edit->rect();
    rect.translate(edit->pos());
    if(pos.x() < rect.left() || pos.y() < rect.top()) return;
    auto dif = pos - rect.bottomRight();

    edit->resize(edit->width() + dif.x(), edit->height() + dif.y());
}

void TextDrawingTool::leftResize(const QPoint& pos)
{
    auto rect = edit->rect();
    rect.translate(edit->pos());
    if(pos.x() > rect.right() || pos.x() < 0) return;
    auto dif = pos.x() - rect.left();

    edit->resize(edit->width() - dif, edit->height());
    edit->move(QPoint(pos.x(), rect.top()));
}

void TextDrawingTool::rightResize(const QPoint& pos)
{
    auto rect = edit->rect();
    rect.translate(edit->pos());
    if(pos.x() < rect.left()) return;
    auto dif = pos.x() - rect.right();

    edit->resize(edit->width() + dif, edit->height());
}

void TextDrawingTool::topResize(const QPoint& pos)
{
    auto rect = edit->rect();
    rect.translate(edit->pos());
    if(pos.y() > rect.bottom() || pos.y() < 0) return;
    auto dif = pos.y() - rect.top();

    edit->resize(edit->width(), edit->height() - dif);
    edit->move(QPoint(rect.left(), pos.y()));
}

void TextDrawingTool::bottomResize(const QPoint& pos)
{
    auto rect = edit->rect();
    rect.translate(edit->pos());
    if(pos.y() < rect.top()) return;
    auto dif = pos.y() - rect.bottom();

    edit->resize(edit->width(), edit->height() + dif);
}
