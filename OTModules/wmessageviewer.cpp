#include "wmessageviewer.h"
#include "ui_wmessageviewer.h"

wMessageViewer::wMessageViewer(OTInAppMessage mData, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wMessageViewer)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog);
    resize(misc.sizeWindow(0.5, 0.75));
    move(misc.centerPosition(this));
    messageData = mData;

    ui->statusbar->addPermanentWidget(ui->pgbLoadingImage);
    ui->pgbLoadingImage->setVisible(false);

    if (set.read("main", "language") == "1")
    {
        setWindowTitle(tr("Message: %1").arg(messageData.deTitle));
        ui->lblDescription->setText(messageData.deDescription);
    }
    else
    {
        setWindowTitle(tr("Message: %1").arg(messageData.enTitle));
        ui->lblDescription->setText(messageData.enDescription);
    }

    ui->lblPublishDate->setText(tr("Published on %1", "e.g. Published on 28.10.2022").arg(messageData.start.toString("dd.MM.yyyy, HH:mm")));

    if (!messageData.image.isEmpty())
        QTimer::singleShot(1, this, SLOT(loadImage()));
}

wMessageViewer::~wMessageViewer()
{
    delete ui;
}

void wMessageViewer::loadImage()
{
    QTimer timer;
    timer.setInterval(1);
    timer.start();

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateDownloadProgress()));

    QPixmap pixmap;
    ui->statusbar->showMessage(tr("Loading image..."));
    ui->pgbLoadingImage->setVisible(true);
    qApp->processEvents();

    pixmap.loadFromData(nc.post(QUrl(messageData.image)));
    ui->lblPicture->setPixmap(pixmap);

    ui->lblPicture->setMaximumWidth(ui->scrollArea->width() * 0.95);

    float a = ui->scrollArea->width() * 0.95;
    float b = pixmap.width();

    ui->lblPicture->setMaximumHeight((a / b) * pixmap.height());

    ui->statusbar->showMessage(tr("Finished."), 2000);
    ui->pgbLoadingImage->setVisible(false);
}

void wMessageViewer::updateDownloadProgress()
{
    ui->pgbLoadingImage->setMaximum(nc.maxProgress);
    ui->pgbLoadingImage->setValue(nc.currentProgress);
}

void wMessageViewer::on_actionClose_triggered()
{
    close();
}

