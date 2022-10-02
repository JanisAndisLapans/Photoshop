#ifndef TEXTDRAWINGTOOLMENU_H
#define TEXTDRAWINGTOOLMENU_H

#include <QWidget>

namespace Ui {
class TextDrawingToolMenu;
}

class TextDrawingToolMenu : public QWidget
{
    Q_OBJECT

public:
    explicit TextDrawingToolMenu(QWidget *parent = nullptr);
    QColor getColor() const;
    QFont getFont() const;
    int getSize() const;
    bool isBold() const;
    bool isItalic() const;
    bool isUnderline() const;
    void setColor(const QColor& color);
    void setFont(const QFont& font);
    void setSize(int size);
    void setBold(bool bold);
    void setItalic(bool italic);
    void setUnderline(bool underline);
    void setFinishButtonsEnabled(bool value);
    ~TextDrawingToolMenu();

private:
    Ui::TextDrawingToolMenu *ui;

private slots:
    void onParamsChanged();
    void onColorChanged(const QColor& color);
    void onCancelButtonPress();
    void onFinishButtonPress();

signals:
    void paramsChanged();
    void colorChanged(const QColor& color);
    void cancel();
    void finish();

};

#endif // TEXTDRAWINGTOOLMENU_H
