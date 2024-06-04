#ifndef SCROLLINGTEXT_H
#define SCROLLINGTEXT_H

#include <QWidget>

#include <QTimer>

namespace Ui {
class ScrollingText;
}

class ScrollingText : public QWidget {
    Q_OBJECT

public:
    explicit ScrollingText(QWidget *parent = nullptr);
    ~ScrollingText();

    enum Direction {
        LeftDirection,
        RightDirection
    };

    void setText(const QString &);
    void setSpeed(const int &);
    void setDirection(const Direction &);
    void setFontSize(const int &);
    void setFps(const int &);


private:
    Ui::ScrollingText *ui;

    void recalcTextWidthCache();
    void recalcMovePixels();
    void recalcFontDescent();
    void paintEvent(QPaintEvent *) override;

    QString _text;
    int _speed;
    int _currentPosition;
    int _fps;
    int _movePixels;
    Direction _direction;
    QFont _font;

    int _textWidthCache;
    int _fontDescendCache;

    QTimer timer;
};

#endif // SCROLLINGTEXT_H
