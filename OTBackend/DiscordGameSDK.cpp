#include "DiscordGameSDK.h"


DiscordGameSDK::DiscordGameSDK()
{
    qInfo() << "Initialize DiscordGameSDK...";
    discord::Core::Create(1244025640064127017, DiscordCreateFlags_Default, &_core);
    _state.core.reset(_core);

    if (!_state.core) {
        qWarning() << "Failed to instantiate Discord!";
        _blockExcecution = true;
        return;
    }

    clearActivity();

    qInfo() << "Initialized DiscordGameSDK.";
}

void DiscordGameSDK::exec()
{
    if (!_blockExcecution)
    {
        qInfo() << "Excecuting DiscordGameSDK...";
        update();

        do
        {
            _state.core->RunCallbacks();
            QThread::msleep(500);
        }
        while (!_stopped);
    }
}

void DiscordGameSDK::stop()
{
    qInfo() << "Stopping DiscordGameSDK...";
    _stopped = true;
}

void DiscordGameSDK::update()
{
    if (!_blockExcecution)
    {
        _state.core->ActivityManager().UpdateActivity(_activity, [](discord::Result result)
        {
            qInfo() << ((result == discord::Result::Ok) ? "Succeeded" : "Failed") << "updating Discord RPC!";
        });
    }
}

void DiscordGameSDK::clearActivity()
{
    DiscordGameSDK::_activity.SetDetails("---");
    DiscordGameSDK::_activity.SetState("---");
    DiscordGameSDK::_activity.GetAssets().SetLargeImage("logo");
    DiscordGameSDK::_activity.GetAssets().SetLargeText("");
    DiscordGameSDK::_activity.SetType(discord::ActivityType::Playing);
    DiscordGameSDK::start();
    DiscordGameSDK::update();
}

void DiscordGameSDK::setModule(QString name) { if (!_blockUpdate) _activity.SetDetails(name.toUtf8()); }

void DiscordGameSDK::setStatus(QString action) { if (!_blockUpdate) _activity.SetState(action.toUtf8()); }

void DiscordGameSDK::start() { if (!_blockUpdate) _activity.GetTimestamps().SetStart(QDateTime::currentDateTime().currentSecsSinceEpoch()); }

void DiscordGameSDK::setEnd(QDateTime epochTimestamp) { if (!_blockUpdate) _activity.GetTimestamps().SetEnd(epochTimestamp.currentSecsSinceEpoch()); }

void DiscordGameSDK::setIcon(QString key, QString tooltip) { if (!_blockUpdate) { _activity.GetAssets().SetSmallImage(key.toUtf8()); _activity.GetAssets().SetSmallText(tooltip.toUtf8()); } }

void DiscordGameSDK::setImage(QString key, QString tooltip) { if (!_blockUpdate) { _activity.GetAssets().SetLargeImage(key.toUtf8()); _activity.GetAssets().SetLargeText(tooltip.toUtf8()); } }

discord::Activity DiscordGameSDK::_activity = {};
DiscordState DiscordGameSDK::_state = {};
discord::Core* DiscordGameSDK::_core = {};
bool DiscordGameSDK::_blockExcecution = false;
bool DiscordGameSDK::_stopped = false;
bool DiscordGameSDK::_blockUpdate = false;
