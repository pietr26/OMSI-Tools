#include "wlfclientdispatcher.h"
#include "ui_wlfclientdispatcher.h"

wLFClientDispatcher::wLFClientDispatcher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wLFClientDispatcher)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Control center trip") + " - " + tr("dispatcher dashboard"));

    centralWidget()->setVisible(false);
    createDockWidgets();

    qInfo().noquote() << objectName() + " started";
}

wLFClientDispatcher::~wLFClientDispatcher()
{
    delete ui;
}

void wLFClientDispatcher::on_actionBackToHome_triggered()
{
    close();
    backToHome();
}

void wLFClientDispatcher::on_actionClose_triggered()
{
    QApplication::quit();
}

void wLFClientDispatcher::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->show();
}

void wLFClientDispatcher::on_actionHelp_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::lfClient);
    WFEEDBACK->show();
}

void wLFClientDispatcher::createDockWidgets()
{
    QDockWidget *dock = new QDockWidget(tr("Information", "plural form"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(WDGINFORMATION);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}
