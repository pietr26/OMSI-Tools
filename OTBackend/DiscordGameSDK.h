#ifndef DISCORDGAMESDK_H
#define DISCORDGAMESDK_H

#include "DiscordRPC/discord.h"
#include <csignal>
#include <QString>

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

    static void setStart(discord::Timestamp epochTimestamp);

    static void setEnd(discord::Timestamp epochTimestamp);

    static void setIcon(QString key, QString tooltip = "");

    static void setImage(QString key, QString tooltip = "");

private:
    static discord::Activity activity;
    static DiscordState state;
    static discord::Core* core;
};

#endif // DISCORDGAMESDK_H
