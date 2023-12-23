#ifndef WPROMPTSELECTOR_H
#define WPROMPTSELECTOR_H

#include <QMainWindow>
#include <QCommandLinkButton>
#include <OTBackend/OTGlobal.h>

namespace Ui {
class wPromptSelector;
}

class wPromptSelector : public QMainWindow
{
    Q_OBJECT

public:
    explicit wPromptSelector(QString title, QList<QPair<QString, QString>> commandLinkButtons, QWidget *parent = nullptr);
    ~wPromptSelector();

signals:
    void sendButtonClick(int id);

private:
    Ui::wPromptSelector *ui;

    OTSettings set;
};

#endif // WPROMPTSELECTOR_H
