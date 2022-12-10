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
    ui->lblStatus->setText(tr("Start-up..."));
    setWindowTitle(OTInformation::name + " - " + ui->lblStatus->text());

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCheck()));
    timer->start(startUpScreenDuration);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->lblTitle->setText(OTInformation::name);
    ui->lblVersion->setText(OTInformation::versions::currentVersion.first);
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

/// Update check (if enabled)
void wStartUpScreen::updateCheck()
{
    timer->stop();

    ui->lblStatus->setText(tr("Check for updates..."));
    qApp->processEvents();

    QVariant checkVersion = set.read("main", "autoUpdateCheck");
    QVariant lastAutoUpdateCheck = set.read("main", "lastAutoUpdateCheck").toString();
    bool checkForUpdate = false;

    QDate lastCheck;
    lastCheck.setDate(lastAutoUpdateCheck.toString().remove(4, 4).toInt(),
                      lastAutoUpdateCheck.toString().remove(0, 4).remove(2, 2).toInt(),
                      lastAutoUpdateCheck.toString().remove(0, 6).toInt());

            // On start
    if (checkVersion == 1)
        checkForUpdate = true;

            // If updates enabled, but there's no lastAutoUpdateCheck
    else if (!lastAutoUpdateCheck.isValid() && (checkVersion != 0))
        checkForUpdate = true;

            // Daily
    else if ((checkVersion == 2) &&
             (lastAutoUpdateCheck.toString() != misc.getDate("yyyyMMdd")))
        checkForUpdate = true;

            // Weekly
    else if ((checkVersion == 3) &&
             (QDate::currentDate().toString("yyyyMMdd") >= lastCheck.addDays(7).toString("yyyyMMdd")))
        checkForUpdate = true;

            // Monthly
    else if ((checkVersion == 4) &&
             (QDate::currentDate().toString("yyyyMMdd") >= lastCheck.addMonths(1).toString("yyyyMMdd")))
        checkForUpdate = true;

    if (checkForUpdate)
    {
        update = misc.getUpdateInformation();
        set.write("main", "lastAutoUpdateCheck", misc.getDate("yyyyMMdd"));

        if (update.at(0) != "false")
        {
            ui->lblStatus->setText(tr("Found update"));
            ui->btnClose->setEnabled(true);

            ui->btnClose->setEnabled(true);
            NEWUPDATE = new newUpdate(update, this);
            ui->vlaFirstSetup->addWidget(NEWUPDATE);

            connect(NEWUPDATE, &newUpdate::goToStartScreen, this, &wStartUpScreen::finished);
        }
    }
    else
        finished();
}

/// Finished first setup
void wStartUpScreen::finished()
{
    timer->stop();
    ui->lblStatus->setText(tr("Finished."));
    setCursor(Qt::ArrowCursor);

    if (!set.read("main", "language").isValid())
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
    else if (!set.checkMainDir(this, set.read("main", "mainDir").toString(), false))
    {
        ui->btnClose->setEnabled(true);
        INVALIDMAINDIR = new invalidMainDir(this);
        ui->vlaFirstSetup->addWidget(INVALIDMAINDIR);

        connect(INVALIDMAINDIR, &invalidMainDir::goToStartScreen, this, [this](){ QTimer::singleShot(1000, this, &wStartUpScreen::openWStart); });
    }
    else
        QTimer::singleShot(1000, this, &wStartUpScreen::openWStart);
}

/// Closes the window (only available on first setup!)
void wStartUpScreen::on_btnClose_clicked()
{
    close();
}

/// Slot: Opens home screen
void wStartUpScreen::openWStart()
{
    WSTART = new wStart();
    WSTART->show();
    hide();
}
