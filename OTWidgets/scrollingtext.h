#ifndef SCROLLINGTEXT_H
#define SCROLLINGTEXT_H

#include <QWidget>

#include <QTimer>

namespace Ui {
class ScrollingText;
}

class ScrollingText : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString   text      READ text      WRITE setText      NOTIFY textChanged)
    Q_PROPERTY(int       speed     READ speed     WRITE setSpeed     NOTIFY speedChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QFont     font      READ font      WRITE setFont      NOTIFY fontChanged)
    Q_PROPERTY(int       fps       READ fps       WRITE setFps       NOTIFY fpsChanged)

public:
    explicit ScrollingText(QWidget *parent = nullptr);
    ~ScrollingText();

    enum Direction {
        LeftDirection,
        RightDirection
    };

    QString text() const;
    void setText(const QString &);

    int speed() const;
    void setSpeed(const int &);

    Direction direction() const;
    void setDirection(const Direction &);

    QFont font() const;
    void setFont(const QFont & = QFont());

    int fps() const;
    void setFps(const int &);

signals:
    void textChanged(const QString &);
    void speedChanged(const int &);
    void directionChanged(const Direction &);
    void fontChanged(const QFont &);
    void fpsChanged(const int &);

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
