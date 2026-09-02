#ifndef OTLINKS_H
#define OTLINKS_H

#include <QString>

class OTLinks
{
public:
    inline static const QString showroom = "https://reboot.omsi-webdisk.de/community/thread/4783";
    inline static const QString support = "https://reboot.omsi-webdisk.de/community/thread/5683";
    inline static const QString feedbackForm = "https://forms.gle/5VGy7fwohHys18bf9";

    class GitHub
    {
    public:
        inline static const QString main = "https://github.com/pietr26/OMSI-Tools";
        inline static const QString releases = main + "/releases";
    };

    inline static const QString roadmap = "https://trello.com/b/wlId7JqM/roadmap-omsi-tools";

    inline static const QString apiPath = "https://backend.omsi-tools.de/api/v2/";
    inline static const QString latestVersion = apiPath + "?ref=currentVersion";
    inline static const QString releaseNotes = apiPath + "?ref=releaseNotes";
    inline static const QString versionBranches = apiPath + "?ref=versionBranches";
    inline static const QString inAppMessages = apiPath + "?ref=messages";
    inline static const QString download = apiPath + "?ref=downloadLink";

    inline static const QString csSuggestFiles = "https://backend.omsi-tools.de/api/csSuggestFiles/";
    inline static const QString csReportLink = "https://backend.omsi-tools.de/api/csReportLink/";
    inline static const QString empty = "https://backend.omsi-tools.de/api/empty/";

    inline static const QString bugDocUploads = "https://media.omsi-tools.de/bugDocUploads/";

    class Wiki
    {
    public:
        inline static const QString general = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Hauptseite";
        inline static const QString fonts = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Schriften";
        inline static const QString cleanup = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Aufräumer";
        inline static const QString contentSearch = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Inhaltssuche";
        inline static const QString verifyMap = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Kartenüberprüfung";
        inline static const QString preferences = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Einstellungen";
        inline static const QString maps = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Kartenbearbeitung";
        inline static const QString money = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Währungen";
        inline static const QString tickets = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Fahrkarten";
        inline static const QString lfClient = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Leistellen-Client";
    };

    class WDT
    {
    public:
        inline static const QString blackModList = "https://reboot.omsi-webdisk.de/wiki/entry/140-moderation-allgemeines/#3-Schwarze-Liste-der-Modifikationen";
        inline static const QString workflowFilebase = "https://reboot.omsi-webdisk.de/wiki/entry/143-moderation-arbeit-in-der-filebase/#2-Datei-Eintrge";
    };

    class lfClient
    {
    public:
        inline static const QString apiPath = "https://backend.omsi-tools.de/api/lfClient/v1/";
        inline static const QString getSpeakRequests = apiPath + "getAllSpeakRequests.php";
        inline static const QString getParticipant = apiPath + "getParticipantData.php";
        inline static const QString getParticipants = apiPath + "getAllParticipants.php";
        inline static const QString setSpeakRequest = apiPath + "requestSpeak.php";
    };
};

#endif // OTLINKS_H
