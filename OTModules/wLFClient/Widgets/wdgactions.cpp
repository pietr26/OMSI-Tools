#include "wdgactions.h"
#include "ui_wdgactions.h"

wdgActions::wdgActions(QWidget *parent, LFClientAPIInterface *api)
    : QWidget(parent)
    , ui(new Ui::wdgActions),
    api(api)
{
    ui->setupUi(this);

    timer5s = new QTimer(this);
    connect(timer5s, &QTimer::timeout, this, &wdgActions::reloadUi5s);
    timer5s->start(5000);

    reloadUi5s();

    connect(api, &LFClientAPIInterface::loginStatusChanged, this, &wdgActions::reloadUi5s);
}

wdgActions::~wdgActions()
{
    delete ui;
}

void wdgActions::on_btnSpeak_clicked()
{
    reloadUi(ui->btnSpeak->isChecked() ? true : false, false, true);
}

void wdgActions::on_btnCrash_clicked()
{
    reloadUi(false, ui->btnCrash->isChecked() ? true : false, true);
}

void wdgActions::reloadUi(bool speak, bool crash, bool sendRequest)
{
    if (api->isLoggedIn())
    {
        if (speak)
        {
            ui->btnSpeak->setChecked(true);
            ui->btnCrash->setChecked(false);
            ui->btnSpeak->setEnabled(true);
            ui->btnCrash->setEnabled(false);

            ui->btnSpeak->setStyleSheet("border: 3px solid #70AD47");
            ui->btnCrash->setStyleSheet("");

            if (sendRequest) api->requestSpeak(LFCApiSpeakRequest::speak);
        }
        else if (crash)
        {
            ui->btnSpeak->setChecked(false);
            ui->btnCrash->setChecked(true);
            ui->btnSpeak->setEnabled(false);
            ui->btnCrash->setEnabled(true);

            ui->btnSpeak->setStyleSheet("");
            ui->btnCrash->setStyleSheet("border: 3px solid #C00000");

            if (sendRequest) api->requestSpeak(LFCApiSpeakRequest::crash);
        }
        else
        {
            ui->btnSpeak->setChecked(false);
            ui->btnCrash->setChecked(false);
            ui->btnSpeak->setEnabled(true);
            ui->btnCrash->setEnabled(true);

            ui->btnSpeak->setStyleSheet("");
            ui->btnCrash->setStyleSheet("");

            if (sendRequest) api->requestSpeak(LFCApiSpeakRequest::none);
        }
    }
}

void wdgActions::reloadUi5s()
{
    if (api->isLoggedIn())
    {
        LFCApiSpeakRequest request = api->getMySpeakRequest();

        if (request == LFCApiSpeakRequest::speak)
        {
            reloadUi(true, false);
            ui->lblTalkInProgress->setVisible(false);
        }
        else if (request == LFCApiSpeakRequest::crash)
        {
            reloadUi(false, true);
            ui->lblTalkInProgress->setVisible(false);
        }
        else if (request == LFCApiSpeakRequest::talking)
        {
            reloadUi(false, false);
            ui->btnSpeak->setEnabled(false);
            ui->btnCrash->setEnabled(false);
            ui->lblTalkInProgress->setVisible(true);
        }
        else // none
        {
            reloadUi(false, false);
            ui->lblTalkInProgress->setVisible(false);
        }
    }
    else // no connection
    {
        reloadUi(false, false);
        ui->lblTalkInProgress->setVisible(false);
    }
}

