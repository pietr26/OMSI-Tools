#ifndef FADEINOUTTEXT_H
#define FADEINOUTTEXT_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QRandomGenerator>

namespace Ui {
class fadeInOutText;
}

class fadeInOutText : public QWidget
{
    Q_OBJECT

public:
    explicit fadeInOutText(QList<QPair<QString, unsigned int>> texts, unsigned int msFadeInOutTime = 1500, QWidget *parent = nullptr);
    ~fadeInOutText();

private slots:
    void hideText();

    void showText();

private:
    Ui::fadeInOutText *ui;

    QGraphicsOpacityEffect *labelEffect;
    QPropertyAnimation *labelAnimation;

    QList<QPair<QString, unsigned int>> texts;

    QRandomGenerator *randGen;

    unsigned int fadeInOutTime;
};

#endif // FADEINOUTTEXT_H
