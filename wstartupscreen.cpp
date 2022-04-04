#include "wstartupscreen.h"
#include "qpainter.h"
#include "ui_wstartupscreen.h"

wStartUpScreen::wStartUpScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wStartUpScreen)
{
    qDebug() << "Show startup screen...";
    ui->setupUi(this);

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());
    setWindowTitle(OTName + " - " + tr("Start...", "Note #1"));
    ui->lblStatus->setText(tr("Start-up..."));

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(finished()));
    timer->start(startUpScreenDuration);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->lblTitle->setText(OTName);
    ui->lblVersion->setText(OTVersion);
    ui->lblCopyright->setText("© Bamp");
    ui->lblCopyright->setVisible(false);

    // Sets up and plays opacity animation
    setWindowOpacity(0.0);
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(900);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
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
    {
        hide();
        WSTART = new wStart();
        WSTART->show();
    }
    else
    {
        FIRSTSETUP = new firstSetup(this);
        ui->vlaFirstSetup->addWidget(FIRSTSETUP);
    }
}


void wStartUpScreen::on_btnClose_clicked()
{
    close();
}

