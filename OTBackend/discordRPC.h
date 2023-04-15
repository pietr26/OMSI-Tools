#include <array>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/DiscordGameSDK/include/discord.h"

struct DiscordState
{
    discord::User currentUser;
    std::unique_ptr<discord::Core> core;
};

class OTDiscordRPC
{
public:
    OTDiscordRPC()
    {
        qInfo() << "Init DiscordRPC...";
        auto result = discord::Core::Create(1050919021685526620, DiscordCreateFlags_Default, &core);
        state.core.reset(core);
        if (!state.core) {
            qInfo() << "Failed to instantiate discord core! (err " << static_cast<int>(result) << ")";
            std::exit(-1);
        }

        state.core->SetLogHook(
            discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) {
                qWarning() << "Log(" << static_cast<uint32_t>(level) << "): " << message;
            });
    }

    void updateActivity()
    {
        activity.SetDetails("Test");
        activity.SetState("Es geht endlich...");
        //        activity.GetAssets().SetLargeText("Miau.");
        //        activity.GetParty().GetSize().SetCurrentSize(1);
        //        activity.GetParty().GetSize().SetMaxSize(5);
        //        activity.GetParty().SetPrivacy(discord::ActivityPartyPrivacy::Public);
        activity.SetType(discord::ActivityType::Playing);
        state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
            qInfo().noquote() << ((result == discord::Result::Ok) ? "Succeeded" : QString("[%1] Failed").arg(static_cast<std::underlying_type<discord::Result>::type>(result)))
                              << "updating activity!";
        });
    }

    DiscordState state{};
    discord::Core* core{};
    discord::Activity activity{};
};

//discord::Activity activity{};
//activity.SetDetails("Fruit Tarts");
//activity.SetState("Pop Snacks");
//activity.GetAssets().SetSmallImage("the");
//activity.GetAssets().SetSmallText("i mage");
//activity.GetAssets().SetLargeImage("the");
//activity.GetAssets().SetLargeText("u mage");
//activity.GetSecrets().SetJoin("join secret");
//activity.GetParty().GetSize().SetCurrentSize(1);
//activity.GetParty().GetSize().SetMaxSize(5);
//activity.GetParty().SetId("party id");
//activity.GetParty().SetPrivacy(discord::ActivityPartyPrivacy::Public);
//activity.SetType(discord::ActivityType::Playing);
//state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
//    std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
//              << " updating activity!\n";
//});
