#include "DiscordGameSDK.h"


 DiscordGameSDK::DiscordGameSDK()
{
    qInfo() << "Initialize DiscordGameSDK...";
    discord::Core::Create(1244025640064127017, DiscordCreateFlags_Default, &core);
    state.core.reset(core);

    if (!state.core) {
        qWarning() << "Failed to instantiate Discord!";
        return;
    }

    clearActivity();

    qInfo() << "Initialized DiscordGameSDK.";
}

void DiscordGameSDK::exec()
{
    qInfo() << "Excecuting DiscordGameSDK...";

    update();

    std::signal(SIGINT, [](int) { interrupted = true; });

    do
    {
        state.core->RunCallbacks();
        QThread::msleep(500);
    }
    while (!interrupted);
}

void DiscordGameSDK::update()
{
    state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        qInfo() << ((result == discord::Result::Ok) ? "Succeeded" : "Failed") << "updating Discord RPC!";
    });
}

void DiscordGameSDK::clearActivity()
{
    DiscordGameSDK::activity.SetDetails("---");
    DiscordGameSDK::activity.SetState("---");
    DiscordGameSDK::activity.GetAssets().SetLargeImage("logo");
    DiscordGameSDK::activity.GetAssets().SetLargeText(OTInformation::versions::currentVersion.first.toLatin1());
    DiscordGameSDK::activity.SetType(discord::ActivityType::Playing);
}

void DiscordGameSDK::setModule(QString name) { activity.SetDetails(name.toUtf8()); }

void DiscordGameSDK::setStatus(QString action) { activity.SetState(action.toUtf8()); }

void DiscordGameSDK::setStart(discord::Timestamp epochTimestamp) { activity.GetTimestamps().SetStart(epochTimestamp); }

void DiscordGameSDK::setEnd(discord::Timestamp epochTimestamp) { activity.GetTimestamps().SetEnd(epochTimestamp); }

void DiscordGameSDK::setIcon(QString key, QString tooltip) { activity.GetAssets().SetSmallImage(key.toUtf8()); activity.GetAssets().SetSmallText(tooltip.toUtf8()); }

void DiscordGameSDK::setImage(QString key, QString tooltip) { activity.GetAssets().SetLargeImage(key.toUtf8()); activity.GetAssets().SetLargeText(tooltip.toUtf8()); }

discord::Activity DiscordGameSDK::activity = {};
DiscordState DiscordGameSDK::state = {};
discord::Core* DiscordGameSDK::core = {};
