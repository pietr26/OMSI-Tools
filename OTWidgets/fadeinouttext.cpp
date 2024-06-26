#include "fadeinouttext.h"
#include "ui_fadeinouttext.h"

fadeInOutText::fadeInOutText(QList<QPair<QString, unsigned int>> textList, unsigned int msFadeInOutTime, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fadeInOutText)
{
    ui->setupUi(this);

    texts = textList;
    fadeInOutTime = msFadeInOutTime;

    ui->lblText->clear();

    ui->lblText->setProperty("opacity", 0);
    labelEffect = new QGraphicsOpacityEffect(this);
    ui->lblText->setGraphicsEffect(labelEffect);

    labelAnimation = new QPropertyAnimation(labelEffect, "opacity");
    labelAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    labelAnimation->setDuration(fadeInOutTime);

    if (!texts.isEmpty())
        showText();
}

fadeInOutText::~fadeInOutText()
{
    delete ui;
}

void fadeInOutText::hideText()
{
    labelAnimation->setStartValue(1);
    labelAnimation->setEndValue(0);
    labelAnimation->start();

    QTimer::singleShot(fadeInOutTime + 200, this, &fadeInOutText::showText);
}

void fadeInOutText::showText()
{
    int randomIndex;

    do {
        randomIndex = QRandomGenerator::global()->bounded(0, texts.size());
    } while (prevRandomIndex == randomIndex);


    prevRandomIndex = randomIndex;
    ui->lblText->setText(texts[randomIndex].first);

    labelAnimation->setStartValue(0);
    labelAnimation->setEndValue(1);
    labelAnimation->start();

    if (texts[randomIndex].second < fadeInOutTime + 200)
        QTimer::singleShot(fadeInOutTime + 200, this, &fadeInOutText::hideText);
    else
        QTimer::singleShot(texts[randomIndex].second, this, &fadeInOutText::hideText);
}
