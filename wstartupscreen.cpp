#include "wstartupscreen.h"
#include "ui_wstartupscreen.h"

wStartUpScreen::wStartUpScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wStartUpScreen)
{
    qDebug() << "Show startup screen...";
    ui->setupUi(this);

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());
    ui->lblStatus->setText(tr("Start-up...", "Note #1"));
    setWindowTitle(OTName + " - " + ui->lblStatus->text());

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(finished()));
    timer->start(startUpScreenDuration);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->lblTitle->setText(OTName);
    ui->lblVersion->setText(OTVersion);
    ui->lblCopyright->setText("© Bamp");
    ui->lblCopyright->setVisible(false);

    ui->btnClose->setEnabled(false);

    setWindowOpacity(0);
    QPropertyAnimation *windowAnimation = new QPropertyAnimation(this, "windowOpacity");
    windowAnimation->setDuration(1000);
    windowAnimation->setStartValue(0);
    windowAnimation->setEndValue(1);
    windowAnimation->setEasingCurve(QEasingCurve::OutCubic);
    windowAnimation->start(QPropertyAnimation::DeleteWhenStopped);

    // ---------------

//    QGraphicsOpacityEffect *labelEffect = new QGraphicsOpacityEffect(this);
//    ui->lblVersion->setGraphicsEffect(labelEffect);

//    QPropertyAnimation *labelAnimation = new QPropertyAnimation(labelEffect, "opacity");
//    labelAnimation->setDuration(2500);
//    labelAnimation->setStartValue(-1);
//    labelAnimation->setEndValue(1);
//    labelAnimation->setEasingCurve(QEasingCurve::InOutCubic);
//    labelAnimation->start(QPropertyAnimation::DeleteWhenStopped);

//    // ---------------

//    QGraphicsOpacityEffect *prefixlabelEffect = new QGraphicsOpacityEffect(this);
//    ui->lblVersionPrefix->setGraphicsEffect(prefixlabelEffect);

//    QPropertyAnimation *prefixLabelAnimation = new QPropertyAnimation(prefixlabelEffect, "opacity");
//    prefixLabelAnimation->setDuration(2500);
//    prefixLabelAnimation->setStartValue(-1);
//    prefixLabelAnimation->setEndValue(1);
//    prefixLabelAnimation->setEasingCurve(QEasingCurve::InOutCubic);
//    prefixLabelAnimation->start(QPropertyAnimation::DeleteWhenStopped);
}

wStartUpScreen::~wStartUpScreen()
{
    delete timer;
    delete ui;
}

/// \brief Shows first setup section
void wStartUpScreen::finished()
{
    timer->stop();
    ui->lblStatus->setText(tr("Finished."));
    setCursor(Qt::ArrowCursor);

    if (set.read("main", "language").isValid())
        QTimer::singleShot(1000, this, &wStartUpScreen::openWStart);
    else
    {
        ui->btnClose->setEnabled(true);
        FIRSTSETUP = new firstSetup(this);
        ui->vlaFirstSetup->addWidget(FIRSTSETUP);

        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        ui->lblStatus->setGraphicsEffect(effect);

        QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(2500);
        animation->setStartValue(1);
        animation->setEndValue(0);
        animation->setEasingCurve(QEasingCurve::InOutCubic);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
}

void wStartUpScreen::on_btnClose_clicked()
{
    close();
}

void wStartUpScreen::openWStart()
{
    WSTART = new wStart();
    WSTART->show();
    hide();
}
