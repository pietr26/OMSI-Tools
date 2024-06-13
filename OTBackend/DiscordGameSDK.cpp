#include "DiscordGameSDK.h"


DiscordGameSDK::DiscordGameSDK()
{
    qInfo() << "Initialize DiscordGameSDK...";
    discord::Core::Create(1244025640064127017, DiscordCreateFlags_NoRequireDiscord, &_core);
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
    _activity.SetDetails("---");
    _activity.SetState("---");
    _activity.GetAssets().SetLargeImage("logo");
    _activity.GetAssets().SetLargeText("");
    _activity.GetAssets().SetSmallImage("");
    _activity.SetType(discord::ActivityType::Playing);
    setStart(false);
    update();
}

void DiscordGameSDK::setModule(QString name) {
    if (_blockUpdate) return;

    _activity.SetDetails(name.toUtf8());
}

void DiscordGameSDK::setStatus(QString action) {
    if (_blockUpdate) return;

    _activity.SetState(action.toUtf8()); }

void DiscordGameSDK::setStart(QDateTime epochTimestamp) {
    if (_blockUpdate) return;

    _activity.GetTimestamps().SetStart(epochTimestamp.currentSecsSinceEpoch()); }

void DiscordGameSDK::setStart(bool enable) {
    if (_blockUpdate) return;

    if (enable) _activity.GetTimestamps().SetStart(QDateTime::currentSecsSinceEpoch());
    else _activity.GetTimestamps().SetStart(std::int64_t());
}

void DiscordGameSDK::setEnd(QDateTime epochTimestamp) {
    if (_blockUpdate) return;

    _activity.GetTimestamps().SetEnd(epochTimestamp.currentSecsSinceEpoch());
}

void DiscordGameSDK::setEnd(bool enable) {
    if (_blockUpdate) return;

    if (!enable) _activity.GetTimestamps().SetEnd(std::int64_t());
}

void DiscordGameSDK::setIcon(QString key, QString tooltip) {
    if (_blockUpdate) return;

    _activity.GetAssets().SetSmallImage(key.toUtf8()); _activity.GetAssets().SetSmallText(tooltip.toUtf8());
}

void DiscordGameSDK::setImage(QString key, QString tooltip) {
    if (_blockUpdate) return;

    _activity.GetAssets().SetLargeImage(key.toUtf8()); _activity.GetAssets().SetLargeText(tooltip.toUtf8());
}

discord::Activity DiscordGameSDK::_activity = {};
DiscordState DiscordGameSDK::_state = {};
discord::Core* DiscordGameSDK::_core = {};
bool DiscordGameSDK::_blockExcecution = false;
bool DiscordGameSDK::_stopped = false;
bool DiscordGameSDK::_blockUpdate = false;
