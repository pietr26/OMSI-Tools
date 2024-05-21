#include "wlogin.h"
#include "ui_wlogin.h"

wLogin::wLogin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wLogin)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    QAction *actionEnter  = this->addAction("");
    QAction *actionCancel = this->addAction("");
    actionEnter->setShortcuts({QKeySequence(Qt::Key_Enter), QKeySequence(Qt::Key_Return)});
    actionCancel->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(actionEnter,  &QAction::triggered, this, &wLogin::on_buttonBox_accepted);
    connect(actionCancel, &QAction::triggered, this, &wLogin::on_buttonBox_rejected);

    setWindowTitle(OTInformation::name + " - " + tr("Control center trip") + " - " + tr("login"));

    qInfo().noquote() << objectName() + " started";
}

wLogin::~wLogin()
{
    delete ui;
}

QString wLogin::username() const {
    return ui->ledUsername->text();
}

QString wLogin::password() const {
    return ui->ledPassword->text();
}

void wLogin::on_buttonBox_accepted() {
    emit accepted();
}

void wLogin::on_buttonBox_rejected() {
    close();
}

