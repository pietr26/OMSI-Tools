#include "DiscordGameSDK.h"

#ifdef OT_NO_DISCORD

/*
    The Discord Game SDK is only shipped as a Windows library with this repository. If no
    library for the current platform is available, the rich presence is skipped
    completely - all entry points stay no-ops.

    To enable it on Linux, drop Discord's 'discord_game_sdk.so' into
    OTBackend/DiscordGameSDK/ and re-run qmake.
*/

DiscordGameSDK::DiscordGameSDK() { qInfo() << "DiscordGameSDK is not available on this platform - skipping rich presence."; }

void DiscordGameSDK::exec() { }
void DiscordGameSDK::stop() { }
void DiscordGameSDK::update() { }
void DiscordGameSDK::clearActivity() { }
void DiscordGameSDK::setModule(QString name) { Q_UNUSED(name); }
void DiscordGameSDK::setStatus(QString action) { Q_UNUSED(action); }
void DiscordGameSDK::setStart(QDateTime epochTimestamp) { Q_UNUSED(epochTimestamp); }
void DiscordGameSDK::setStart(bool enable) { Q_UNUSED(enable); }
void DiscordGameSDK::setEnd(QDateTime epochTimestamp) { Q_UNUSED(epochTimestamp); }
void DiscordGameSDK::setEnd(bool enable) { Q_UNUSED(enable); }
void DiscordGameSDK::setIcon(QString key, QString tooltip) { Q_UNUSED(key); Q_UNUSED(tooltip); }
void DiscordGameSDK::setImage(QString key, QString tooltip) { Q_UNUSED(key); Q_UNUSED(tooltip); }

bool DiscordGameSDK::_blockExcecution = true;
bool DiscordGameSDK::_blockUpdate = true;

#else

DiscordGameSDK::DiscordGameSDK()
{
    if (_blockExcecution) return;

    qInfo() << "Initialize DiscordGameSDK...";
    try
    {
        discord::Core::Create(1244025640064127017, DiscordCreateFlags_NoRequireDiscord, &_core);
        _state.core.reset(_core);
    }
    catch (...)
    {
        qCritical() << "Exception: Could not initialize DiscordGameSDK. Maybe you run two Discord instances at once?";
        _blockExcecution = true;
        return;
    }


    if (!_state.core) {
        qWarning() << "Failed to instantiate DiscordGameSDK!";
        _blockExcecution = true;
        return;
    }

    clearActivity();

    qInfo() << "Initialized DiscordGameSDK.";
}

void DiscordGameSDK::exec()
{
    if (_blockExcecution) return;

    qInfo() << "Excecuting DiscordGameSDK...";
    update();

    while (!_blockExcecution)
    {
        _state.core->RunCallbacks();
        QThread::msleep(500);
    }
}

void DiscordGameSDK::stop()
{
    if (_blockExcecution) return;

    qInfo() << "Stopping DiscordGameSDK...";
    _blockExcecution = true;
}

void DiscordGameSDK::update()
{
    if (_blockExcecution) return;

    _state.core->ActivityManager().UpdateActivity(_activity, [](discord::Result result)
    {
        qInfo() << ((result == discord::Result::Ok) ? "Succeeded" : "Failed") << "updating Discord Activity!";
    });
}

void DiscordGameSDK::clearActivity()
{
    if (_blockExcecution) return;

    _activity.SetDetails("---");
    _activity.SetState("---");
    _activity.GetAssets().SetLargeImage("logo");
    _activity.GetAssets().SetLargeText("");
    _activity.GetAssets().SetSmallImage("");
    _activity.SetType(discord::ActivityType::Playing);
    setStart(false);
    update();
}

void DiscordGameSDK::setModule(QString name)
{
    if (_blockUpdate || _blockExcecution) return;

    _activity.SetDetails(name.toUtf8());
}

void DiscordGameSDK::setStatus(QString action)
{
    if (_blockUpdate || _blockExcecution) return;

    _activity.SetState(action.toUtf8()); }

void DiscordGameSDK::setStart(QDateTime epochTimestamp)
{
    if (_blockUpdate || _blockExcecution) return;

    _activity.GetTimestamps().SetStart(epochTimestamp.currentSecsSinceEpoch()); }

void DiscordGameSDK::setStart(bool enable)
{
    if (_blockUpdate || _blockExcecution) return;

    if (enable) _activity.GetTimestamps().SetStart(QDateTime::currentSecsSinceEpoch());
    else _activity.GetTimestamps().SetStart(std::int64_t());
}

void DiscordGameSDK::setEnd(QDateTime epochTimestamp)
{
    if (_blockUpdate || _blockExcecution) return;

    _activity.GetTimestamps().SetEnd(epochTimestamp.currentSecsSinceEpoch());
}

void DiscordGameSDK::setEnd(bool enable)
{
    if (_blockUpdate || _blockExcecution) return;

    if (!enable) _activity.GetTimestamps().SetEnd(std::int64_t());
}

void DiscordGameSDK::setIcon(QString key, QString tooltip)
{
    if (_blockUpdate || _blockExcecution) return;

    _activity.GetAssets().SetSmallImage(key.toUtf8()); _activity.GetAssets().SetSmallText(tooltip.toUtf8());
}

void DiscordGameSDK::setImage(QString key, QString tooltip)
{
    if (_blockUpdate || _blockExcecution) return;

    _activity.GetAssets().SetLargeImage(key.toUtf8()); _activity.GetAssets().SetLargeText(tooltip.toUtf8());
}

discord::Activity DiscordGameSDK::_activity = {};
DiscordState DiscordGameSDK::_state = {};
discord::Core* DiscordGameSDK::_core = {};
bool DiscordGameSDK::_blockExcecution = false;
bool DiscordGameSDK::_blockUpdate = false;

#endif // OT_NO_DISCORD
