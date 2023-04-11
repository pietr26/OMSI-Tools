#include <QtCore>
#include <QtNetwork>
#include "OTBackend/DiscordGameSDK/include/discord.h"

class OTDiscordRP {
public:
    OTDiscordRP() {
        discordInstance->Create(1050919021685526620, DiscordCreateFlags_NoRequireDiscord, &discordInstance);
        updateActivity("Starten...", "Wartet", "Es geht endlich");
    }

    void updateActivity(QString details, QString state, QString name) {
        activity.SetDetails(details.toUtf8().constData());
        activity.SetState(state.toUtf8().constData());
        activity.SetName(name.toUtf8().constData());

        discordInstance->ActivityManager().UpdateActivity(activity, [](discord::Result result)
                                                          {
                                                              if (result == discord::Result::Ok) qInfo() << "Discord:: Success";
                                                              else qInfo() << "Discord:: Failed!";
                                                          }
                                                          );
    }

private:
    discord::Activity activity;

//    DiscordCreateParams params;
//    discord::User user;
//    discord::AchievementManager achievement_manager;
//    discord::LobbyManager lobby_manager;
//    discord::OverlayManager overlay_manager;
//    discord::RelationshipManager relationship_manager;
//    discord::VoiceManager voice_manager;
    discord::Core* discordInstance;
};
