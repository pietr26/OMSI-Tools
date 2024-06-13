#include "wfonts.h"
#include "ui_wfonts.h"

wFonts::wFonts(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wFonts)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Font creation"));

    DiscordGameSDK::setModule(tr("Font creation"));
    DiscordGameSDK::setStatus(tr("Creating a font"));
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wfonts");
    DiscordGameSDK::update();

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->pgbProgress->setVisible(false);
    centralWidget()->setVisible(false);

    createDockWidgets();

    qInfo().noquote() << objectName() + " started";
}

wFonts::~wFonts()
{
    delete ui;
}

void wFonts::on_actionBackToHome_triggered()
{
    emit backToHome();
    close();
}

void wFonts::on_actionClose_triggered()
{
    QApplication::quit();
}

void wFonts::createDockWidgets()
{
    QDockWidget *dockGeneral = new QDockWidget(tr("General"), this);
    dockGeneral->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockGeneral->setWidget(WDGGENERAL);
    addDockWidget(Qt::LeftDockWidgetArea, dockGeneral);

    QDockWidget *dockChars = new QDockWidget(tr("Characters"), this);
    dockChars->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockChars->setWidget(WDGCHARS);
    addDockWidget(Qt::RightDockWidgetArea, dockChars);

    QDockWidget *dockPreview = new QDockWidget(tr("Preview"), this);
    dockPreview->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockPreview->setWidget(WDGPREVIEW);
    addDockWidget(Qt::BottomDockWidgetArea, dockPreview);
}
