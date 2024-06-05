QT       += core widgets gui network concurrent sql printsupport

CONFIG += c++17

DEFINES += QT_MESSAGELOGCONTEXT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    OTBackend/DiscordGameSDK.cpp \
    OTBackend/DiscordGameSDK/achievement_manager.cpp \
    OTBackend/DiscordGameSDK/activity_manager.cpp \
    OTBackend/DiscordGameSDK/application_manager.cpp \
    OTBackend/DiscordGameSDK/core.cpp \
    OTBackend/DiscordGameSDK/image_manager.cpp \
    OTBackend/DiscordGameSDK/lobby_manager.cpp \
    OTBackend/DiscordGameSDK/network_manager.cpp \
    OTBackend/DiscordGameSDK/overlay_manager.cpp \
    OTBackend/DiscordGameSDK/relationship_manager.cpp \
    OTBackend/DiscordGameSDK/storage_manager.cpp \
    OTBackend/DiscordGameSDK/store_manager.cpp \
    OTBackend/DiscordGameSDK/types.cpp \
    OTBackend/DiscordGameSDK/user_manager.cpp \
    OTBackend/DiscordGameSDK/voice_manager.cpp \
    OTBackend/LFClientAPIInterface/lfclientapiinterface.cpp \
    OTBackend/OTLogger.cpp \
    OTModules/OTGeneric/wfeedback.cpp \
    OTModules/OTGeneric/wpreferences.cpp \
    OTModules/OTGeneric/wpromptselector.cpp \
    OTModules/OTGeneric/wselectencoding.cpp \
    OTModules/OTInternals/wregeditor.cpp \
    OTModules/wBugDoc/wbugdoc.cpp \
    OTModules/wContentSearch/wDBPanel.cpp \
    OTModules/wContentSearch/waddfiles.cpp \
    OTModules/wContentSearch/wcontentsearch.cpp \
    OTModules/wFbh/waddpath.cpp \
    OTModules/wFbh/waddterm.cpp \
    OTModules/wFbh/waddword.cpp \
    OTModules/wFbh/wdbcopyrights.cpp \
    OTModules/wFbh/wdbknownwords.cpp \
    OTModules/wLFClient/Widgets/wdgactions.cpp \
    OTModules/wLFClient/Widgets/wdginformation.cpp \
    OTModules/wLFClient/Widgets/wdgmanagetrips.cpp \
    OTModules/wLFClient/Widgets/wdgmanageusers.cpp \
    OTModules/wLFClient/Widgets/wdgnotification.cpp \
    OTModules/wLFClient/Widgets/wdgnotifications.cpp \
    OTModules/wLFClient/Widgets/wdgtrip.cpp \
    OTModules/wLFClient/Widgets/wdgtrips.cpp \
    OTModules/wLFClient/wlfclientdispatcher.cpp \
    OTModules/wLFClient/wlfclientparticipant.cpp \
    OTModules/wLFClient/wlogin.cpp \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.cpp \
    OTModules/wMaps/wGlobalProps/weditseason.cpp \
    OTModules/wMaps/wGlobalProps/wglobalprops.cpp \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.cpp \
    OTModules/wMaps/wmaps.cpp \
    OTModules/wcleanup.cpp \
    OTModules/wmapselection.cpp \
    OTModules/wmessageviewer.cpp \
    OTModules/OTInternals/wstyletest.cpp \
    OTWidgets/fadeinouttext.cpp \
    OTWidgets/firstsetup.cpp \
    OTWidgets/input2dcoordinates.cpp \
    OTWidgets/input3dcoordinates.cpp \
    OTWidgets/inputaddeditremove.cpp \
    OTWidgets/invalidmaindir.cpp \
    OTWidgets/message.cpp \
    OTWidgets/newupdate.cpp \
    OTWidgets/scrollingtext.cpp \
    OTWidgets/verifymaptools.cpp \
    main.cpp \
    OTModules/OTInternals/wdevtools.cpp \
    OTModules/wfonts.cpp \
    OTModules/wVerifyMap/wverifymap.cpp \
    OTModules/OTGeneric/wabout.cpp \
    OTModules/OTGeneric/wreleasenotes.cpp \
    OTModules/OTGeneric/wstart.cpp \
    OTModules/OTGeneric/wstartupscreen.cpp

HEADERS += \
    OTBackend/DiscordGameSDK.h \
    OTBackend/DiscordGameSDK/achievement_manager.h \
    OTBackend/DiscordGameSDK/activity_manager.h \
    OTBackend/DiscordGameSDK/application_manager.h \
    OTBackend/DiscordGameSDK/core.h \
    OTBackend/DiscordGameSDK/discord.h \
    OTBackend/DiscordGameSDK/event.h \
    OTBackend/DiscordGameSDK/ffi.h \
    OTBackend/DiscordGameSDK/image_manager.h \
    OTBackend/DiscordGameSDK/lobby_manager.h \
    OTBackend/DiscordGameSDK/network_manager.h \
    OTBackend/DiscordGameSDK/overlay_manager.h \
    OTBackend/DiscordGameSDK/relationship_manager.h \
    OTBackend/DiscordGameSDK/storage_manager.h \
    OTBackend/DiscordGameSDK/store_manager.h \
    OTBackend/DiscordGameSDK/types.h \
    OTBackend/DiscordGameSDK/user_manager.h \
    OTBackend/DiscordGameSDK/voice_manager.h \
    OTBackend/LFClientAPIInterface/lfclientapiinterface.h \
    OTBackend/OCC.h \
    OTBackend/OTExternal.h \
    OTBackend/OTGlobal.h \
    OTBackend/OTLogger.h \
    OTBackend/OTOmsiFileHandler.h \
    OTBackend/OTOmsiFileHandler_models.h \
    OTModules/OTGeneric/wfeedback.h \
    OTModules/OTGeneric/wpreferences.h \
    OTModules/OTGeneric/wpromptselector.h \
    OTModules/OTGeneric/wselectencoding.h \
    OTModules/OTInternals/wregeditor.h \
    OTModules/wBugDoc/wbugdoc.h \
    OTModules/wContentSearch/wDBPanel.h \
    OTModules/wContentSearch/waddfiles.h \
    OTModules/wContentSearch/wcontentsearch.h \
    OTModules/wFbh/waddpath.h \
    OTModules/wFbh/waddterm.h \
    OTModules/wFbh/waddword.h \
    OTModules/wFbh/wdbcopyrights.h \
    OTModules/wFbh/wdbknownwords.h \
    OTModules/wLFClient/Widgets/wdgactions.h \
    OTModules/wLFClient/Widgets/wdginformation.h \
    OTModules/wLFClient/Widgets/wdgmanagetrips.h \
    OTModules/wLFClient/Widgets/wdgmanageusers.h \
    OTModules/wLFClient/Widgets/wdgnotification.h \
    OTModules/wLFClient/Widgets/wdgnotifications.h \
    OTModules/wLFClient/Widgets/wdgtrip.h \
    OTModules/wLFClient/Widgets/wdgtrips.h \
    OTModules/wLFClient/wlfclientdispatcher.h \
    OTModules/wLFClient/wlfclientparticipant.h \
    OTModules/wLFClient/wlogin.h \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.h \
    OTModules/wMaps/wGlobalProps/weditseason.h \
    OTModules/wMaps/wGlobalProps/wglobalprops.h \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.h \
    OTModules/wMaps/wmaps.h \
    OTModules/wcleanup.h \
    OTModules/wmapselection.h \
    OTModules/wmessageviewer.h \
    OTModules/OTInternals/wdevtools.h \
    OTModules/wfonts.h \
    OTModules/wVerifyMap/wverifymap.h \
    OTModules/OTInternals/wstyletest.h \
    OTWidgets/fadeinouttext.h \
    OTWidgets/firstsetup.h \
    OTWidgets/input2dcoordinates.h \
    OTWidgets/input3dcoordinates.h \
    OTWidgets/inputaddeditremove.h \
    OTWidgets/invalidmaindir.h \
    OTWidgets/message.h \
    OTWidgets/newupdate.h \
    OTWidgets/scrollingtext.h \
    OTWidgets/verifymaptools.h \
    OTModules/OTGeneric/wabout.h \
    OTModules/OTGeneric/wreleasenotes.h \
    OTModules/OTGeneric/wstart.h \
    OTModules/OTGeneric/wstartupscreen.h

FORMS += \
    OTModules/OTGeneric/wfeedback.ui \
    OTModules/OTGeneric/wpreferences.ui \
    OTModules/OTGeneric/wpromptselector.ui \
    OTModules/OTGeneric/wselectencoding.ui \
    OTModules/OTInternals/wregeditor.ui \
    OTModules/wBugDoc/wbugdoc.ui \
    OTModules/wContentSearch/wDBPanel.ui \
    OTModules/wContentSearch/waddfiles.ui \
    OTModules/wContentSearch/wcontentsearch.ui \
    OTModules/wFbh/waddpath.ui \
    OTModules/wFbh/waddterm.ui \
    OTModules/wFbh/waddword.ui \
    OTModules/wFbh/wdbcopyrights.ui \
    OTModules/wFbh/wdbknownwords.ui \
    OTModules/wLFClient/Widgets/wdgactions.ui \
    OTModules/wLFClient/Widgets/wdginformation.ui \
    OTModules/wLFClient/Widgets/wdgmanagetrips.ui \
    OTModules/wLFClient/Widgets/wdgmanageusers.ui \
    OTModules/wLFClient/Widgets/wdgnotification.ui \
    OTModules/wLFClient/Widgets/wdgnotifications.ui \
    OTModules/wLFClient/Widgets/wdgtrip.ui \
    OTModules/wLFClient/Widgets/wdgtrips.ui \
    OTModules/wLFClient/wlfclientdispatcher.ui \
    OTModules/wLFClient/wlfclientparticipant.ui \
    OTModules/wLFClient/wlogin.ui \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.ui \
    OTModules/wMaps/wGlobalProps/weditseason.ui \
    OTModules/wMaps/wGlobalProps/wglobalprops.ui \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.ui \
    OTModules/wMaps/wmaps.ui \
    OTModules/wcleanup.ui \
    OTModules/wmapselection.ui \
    OTModules/wmessageviewer.ui \
    OTModules/OTInternals/wdevtools.ui \
    OTModules/wfonts.ui \
    OTModules/wVerifyMap/wverifymap.ui \
    OTModules/OTInternals/wstyletest.ui \
    OTWidgets/fadeinouttext.ui \
    OTWidgets/firstsetup.ui \
    OTWidgets/input2dcoordinates.ui \
    OTWidgets/input3dcoordinates.ui \
    OTWidgets/inputaddeditremove.ui \
    OTWidgets/invalidmaindir.ui \
    OTWidgets/message.ui \
    OTWidgets/newupdate.ui \
    OTWidgets/scrollingtext.ui \
    OTWidgets/verifymaptools.ui \
    OTModules/OTGeneric/wabout.ui \
    OTModules/OTGeneric/wreleasenotes.ui \
    OTModules/OTGeneric/wstart.ui \
    OTModules/OTGeneric/wstartupscreen.ui

TRANSLATIONS += \
    data/translations/tr_de.ts \
    data/translations/tr_fr.ts \
    data/translations/tr_cs.ts \
    data/translations/tr_fi.ts \
    data/translations/tr_ja.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = data/icons/logo.ico

RESOURCES += \
    ressources.qrc

DISTFILES += \
    devNotes

LIBS += $$PWD/OTBackend/DiscordGameSDK/discord_game_sdk.dll
