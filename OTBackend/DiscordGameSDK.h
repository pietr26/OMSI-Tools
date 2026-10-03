#ifndef DISCORDGAMESDK_H
#define DISCORDGAMESDK_H

#define NOMINMAX
#ifndef OT_NO_DISCORD
#include "DiscordGameSDK/discord.h"
#endif
#include <csignal>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QDateTime>

#ifndef OT_NO_DISCORD
struct DiscordState {
    std::unique_ptr<discord::Core> core;
};
#endif

class DiscordGameSDK
{
public:
    explicit DiscordGameSDK();

    static void exec();

    static void stop();

    static void update();

    static void clearActivity();

    static void setModule(QString name);

    static void setStatus(QString action);

    static void setStart(QDateTime epochTimestamp = QDateTime());

    static void setStart(bool enable);

    static void setEnd(QDateTime epochTimestamp);

    static void setEnd(bool enable);

    static void setIcon(QString key, QString tooltip = "");

    static void setImage(QString key, QString tooltip = "");

    static void setBlockUpdate(bool blockUpdate) { _blockUpdate = blockUpdate; }

private:
#ifndef OT_NO_DISCORD
    static discord::Activity _activity;
    static DiscordState _state;
    static discord::Core* _core;
#endif

    static bool _blockExcecution;

    static bool _blockUpdate;
};

#endif // DISCORDGAMESDK_H
