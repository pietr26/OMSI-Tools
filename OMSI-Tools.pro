QT       += core widgets gui network concurrent sql printsupport

CONFIG += c++17 #console

DEFINES += QT_MESSAGELOGCONTEXT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    OTBackend/DiscordGameSDK/include/achievement_manager.cpp \
#    OTBackend/DiscordGameSDK/include/activity_manager.cpp \
#    OTBackend/DiscordGameSDK/include/application_manager.cpp \
#    OTBackend/DiscordGameSDK/include/core.cpp \
#    OTBackend/DiscordGameSDK/include/image_manager.cpp \
#    OTBackend/DiscordGameSDK/include/lobby_manager.cpp \
#    OTBackend/DiscordGameSDK/include/network_manager.cpp \
#    OTBackend/DiscordGameSDK/include/overlay_manager.cpp \
#    OTBackend/DiscordGameSDK/include/relationship_manager.cpp \
#    OTBackend/DiscordGameSDK/include/storage_manager.cpp \
#    OTBackend/DiscordGameSDK/include/store_manager.cpp \
#    OTBackend/DiscordGameSDK/include/types.cpp \
#    OTBackend/DiscordGameSDK/include/user_manager.cpp \
#    OTBackend/DiscordGameSDK/include/voice_manager.cpp \
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
    OTModules/wLFClient/wlfclientdispatcher.cpp \
    OTModules/wLFClient/wlfclientparticipant.cpp \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.cpp \
    OTModules/wMaps/wGlobalProps/weditseason.cpp \
    OTModules/wMaps/wGlobalProps/wglobalprops.cpp \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.cpp \
    OTModules/wMaps/wmaps.cpp \
    OTModules/wProjectManagement/remote/wdownloadproject.cpp \
    OTModules/wProjectManagement/remote/wuploadproject.cpp \
    OTModules/wProjectManagement/wcreateproject.cpp \
    OTModules/wProjectManagement/wmanagefiles.cpp \
    OTModules/wProjectManagement/wprojectmanagement.cpp \
    OTModules/wProjectManagement/wprojectpreferences.cpp \
    OTModules/wcleanup.cpp \
    OTModules/wmapselection.cpp \
    OTModules/wmessageviewer.cpp \
    OTModules/wvehiclelist.cpp \
    OTModules/OTInternals/wstyletest.cpp \
    OTWidgets/fadeinouttext.cpp \
    OTWidgets/firstsetup.cpp \
    OTWidgets/input2dcoordinates.cpp \
    OTWidgets/input3dcoordinates.cpp \
    OTWidgets/inputaddeditremove.cpp \
    OTWidgets/invalidmaindir.cpp \
    OTWidgets/message.cpp \
    OTWidgets/newupdate.cpp \
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
#    OTBackend/DiscordGameSDK/include/achievement_manager.h \
#    OTBackend/DiscordGameSDK/include/activity_manager.h \
#    OTBackend/DiscordGameSDK/include/application_manager.h \
#    OTBackend/DiscordGameSDK/include/core.h \
#    OTBackend/DiscordGameSDK/include/discord.h \
#    OTBackend/DiscordGameSDK/include/event.h \
#    OTBackend/DiscordGameSDK/include/ffi.h \
#    OTBackend/DiscordGameSDK/include/image_manager.h \
#    OTBackend/DiscordGameSDK/include/lobby_manager.h \
#    OTBackend/DiscordGameSDK/include/network_manager.h \
#    OTBackend/DiscordGameSDK/include/overlay_manager.h \
#    OTBackend/DiscordGameSDK/include/relationship_manager.h \
#    OTBackend/DiscordGameSDK/include/storage_manager.h \
#    OTBackend/DiscordGameSDK/include/store_manager.h \
#    OTBackend/DiscordGameSDK/include/types.h \
#    OTBackend/DiscordGameSDK/include/user_manager.h \
#    OTBackend/DiscordGameSDK/include/voice_manager.h \
    OTBackend/OCC.h \
    OTBackend/OTExternal.h \
    OTBackend/discordRPC.h \
    OTBackend/OTGlobal.h \
    OTBackend/OTLogger.h \
    OTBackend/OTOmsiFileHandler.h \
    OTBackend/OTOmsiFileHandler_models.h \
    OTBackend/OTStyleDraw.h \
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
    OTModules/wLFClient/wlfclientdispatcher.h \
    OTModules/wLFClient/wlfclientparticipant.h \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.h \
    OTModules/wMaps/wGlobalProps/weditseason.h \
    OTModules/wMaps/wGlobalProps/wglobalprops.h \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.h \
    OTModules/wMaps/wmaps.h \
    OTModules/wProjectManagement/remote/wdownloadproject.h \
    OTModules/wProjectManagement/remote/wuploadproject.h \
    OTModules/wProjectManagement/wcreateproject.h \
    OTModules/wProjectManagement/wmanagefiles.h \
    OTModules/wProjectManagement/wprojectmanagement.h \
    OTModules/wProjectManagement/wprojectpreferences.h \
    OTModules/wcleanup.h \
    OTModules/wmapselection.h \
    OTModules/wmessageviewer.h \
    OTModules/wvehiclelist.h \
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
    OTModules/wLFClient/wlfclientdispatcher.ui \
    OTModules/wLFClient/wlfclientparticipant.ui \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.ui \
    OTModules/wMaps/wGlobalProps/weditseason.ui \
    OTModules/wMaps/wGlobalProps/wglobalprops.ui \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.ui \
    OTModules/wMaps/wmaps.ui \
    OTModules/wProjectManagement/remote/wdownloadproject.ui \
    OTModules/wProjectManagement/remote/wuploadproject.ui \
    OTModules/wProjectManagement/wcreateproject.ui \
    OTModules/wProjectManagement/wmanagefiles.ui \
    OTModules/wProjectManagement/wprojectmanagement.ui \
    OTModules/wProjectManagement/wprojectpreferences.ui \
    OTModules/wcleanup.ui \
    OTModules/wmapselection.ui \
    OTModules/wmessageviewer.ui \
    OTModules/wvehiclelist.ui \
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
    OTWidgets/verifymaptools.ui \
    OTModules/OTGeneric/wabout.ui \
    OTModules/OTGeneric/wreleasenotes.ui \
    OTModules/OTGeneric/wstart.ui \
    OTModules/OTGeneric/wstartupscreen.ui

TRANSLATIONS += \
    data/translations/tr_de.ts \
    data/translations/tr_fr.ts \
    data/translations/tr_it.ts \
    data/translations/tr_cs.ts \
    data/translations/tr_cy.ts \
    data/translations/tr_eo.ts \
    data/translations/tr_et.ts \
    data/translations/tr_fi.ts \
    data/translations/tr_is.ts \
    data/translations/tr_ja.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = data/icons/iconOMSI-Tools.ico

RESOURCES += \
    ressources.qrc

DISTFILES += \
    devNotes

#LIBS += -LD:/OMSI-Tools/OMSI-Tools/OTBackend/DiscordGameSDK/lib -ldiscord_game_sdk
#INCLUDEPATH += D:/OMSI-Tools/OMSI-Tools/OTBackend/DiscordGameSDK/include

