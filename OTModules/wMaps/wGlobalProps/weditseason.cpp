#include "weditseason.h"
#include "ui_weditseason.h"

wEditSeason::wEditSeason(OCMap::Global::Season s, int index, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wEditSeason)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(tr("Edit season"));

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    seasonIndex = index;
    if (seasonIndex != -1) season = s;

    auto* model = qobject_cast<QStandardItemModel*>(ui->cobxSeason->model());
    model->item(0)->setEnabled(false); // Summer

    reloadUI();

    qInfo().noquote() << objectName() + " started";
}

wEditSeason::~wEditSeason()
{
    delete ui;
}

void wEditSeason::reloadUI()
{
    ui->daeStart->setDate(season.start);
    ui->daeEnd->setDate(season.end);
    ui->cobxSeason->setCurrentIndex(season.type);
}

void wEditSeason::on_btnSave_clicked()
{
    emit returnSeason(season);
    close();
}

void wEditSeason::on_btnDiscard_clicked()
{
    close();
}

void wEditSeason::on_cobxSeason_currentIndexChanged(int index)
{
    season.type = OCMap::Global::Season::Type(index);
}


void wEditSeason::on_daeStart_userDateChanged(const QDate &date)
{
    season.start = date;
}


void wEditSeason::on_daeEnd_userDateChanged(const QDate &date)
{
    season.end = date;
}

