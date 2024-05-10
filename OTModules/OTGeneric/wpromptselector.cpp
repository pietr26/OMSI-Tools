#include "wpromptselector.h"
#include "ui_wpromptselector.h"

wPromptSelector::wPromptSelector(QString title, QList<QPair<QString, QString>> commandLinkButtons, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wPromptSelector)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    // -----------

    setWindowTitle(title);

    for (int i = 0; i < commandLinkButtons.length(); i++)
    {
        QCommandLinkButton *clb = new QCommandLinkButton();
        clb->setText(commandLinkButtons[i].first);
        clb->setDescription(commandLinkButtons[i].second);
        clb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        connect(clb, &QCommandLinkButton::clicked, this, [=](){ emit sendButtonClick(i); close(); });

        ui->vlaMain->addWidget(clb);
    }

    qInfo().noquote() << objectName() + " started";
}

wPromptSelector::~wPromptSelector()
{
    delete ui;
}
