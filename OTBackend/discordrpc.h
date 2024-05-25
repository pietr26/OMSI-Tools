#ifndef DISCORDRPC_H
#define DISCORDRPC_H

#include "DiscordRPC/discord.h"
#include <csignal>
#include "OTGlobal.h"

struct DiscordState {
    std::unique_ptr<discord::Core> core;
};

namespace {
volatile bool interrupted{ false };
}

class DiscordRPC
{
public:
    DiscordRPC()
    {
        auto response = discord::Core::Create(1244025640064127017, DiscordCreateFlags_Default, &core);
        state.core.reset(core);

        if (!state.core) {
            qWarning() << "Failed to instantiate Discord!";
            return;
        }
    }

    void update()
    {
        discord::Activity activity{};
        activity.SetDetails("Fruit Tarts");
        activity.SetState("Pop Snacks");
        activity.GetAssets().SetSmallImage("the");
        activity.GetAssets().SetSmallText("i mage");
        activity.GetAssets().SetLargeImage("the");
        activity.GetAssets().SetLargeText("u mage");
        activity.SetType(discord::ActivityType::Playing);
        state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
            qInfo() << ((result == discord::Result::Ok) ? "Succeeded" : "Failed") << "updating activity!";
        });


        std::signal(SIGINT, [](int) {
            interrupted = true;
        });
        do {
            state.core->RunCallbacks();
            QThread::msleep(16);
        } while (!interrupted);
    }

    DiscordState state{};
    discord::Core* core{};
};

#endif // DISCORDRPC_H
