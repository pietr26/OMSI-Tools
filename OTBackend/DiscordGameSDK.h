#ifndef DISCORDGAMESDK_H
#define DISCORDGAMESDK_H

#include "DiscordGameSDK/discord.h"
#include <csignal>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QDateTime>

struct DiscordState {
    std::unique_ptr<discord::Core> core;
};

namespace {
volatile bool interrupted{ false };
}

class DiscordGameSDK
{
public:
    explicit DiscordGameSDK();

    static void exec();

    static void update();

    static void clearActivity();

    static void setModule(QString name);

    static void setStatus(QString action);

    static void setStart(QDateTime epochTimestamp = QDateTime());

    static void setEnd(QDateTime epochTimestamp = QDateTime());

    static void setIcon(QString key, QString tooltip = "");

    static void setImage(QString key, QString tooltip = "");

private:
    static discord::Activity activity;
    static DiscordState state;
    static discord::Core* core;

    static bool blockExcecution;
};

#endif // DISCORDGAMESDK_H
