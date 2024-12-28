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
    OTBackend/OCC/OCBase.cpp \
    OTBackend/OCC/OCDriver.cpp \
    OTBackend/OCC/OCFS.cpp \
    OTBackend/OCC/OCFont.cpp \
    OTBackend/OCC/OCGameControler.cpp \
    OTBackend/OCC/OCHuman.cpp \
    OTBackend/OCC/OCKeyboard.cpp \
    OTBackend/OCC/OCLanguage.cpp \
    OTBackend/OCC/OCMap.cpp \
    OTBackend/OCC/OCModel.cpp \
    OTBackend/OCC/OCMoney.cpp \
    OTBackend/OCC/OCOptions.cpp \
    OTBackend/OCC/OCPassengercabin.cpp \
    OTBackend/OCC/OCPlugin.cpp \
    OTBackend/OCC/OCRail.cpp \
    OTBackend/OCC/OCSceneryobject.cpp \
    OTBackend/OCC/OCScript.cpp \
    OTBackend/OCC/OCSituation.cpp \
    OTBackend/OCC/OCSound.cpp \
    OTBackend/OCC/OCSpline.cpp \
    OTBackend/OCC/OCTextureChangeMaster.cpp \
    OTBackend/OCC/OCTextureProfile.cpp \
    OTBackend/OCC/OCTicketPack.cpp \
    OTBackend/OCC/OCTrain.cpp \
    OTBackend/OCC/OCTranslation.cpp \
    OTBackend/OCC/OCVehicle.cpp \
    OTBackend/OCC/OCViewable.cpp \
    OTBackend/OCC/OCWeather.cpp \
    OTBackend/OTLogger.cpp \
    OTModules/OTGeneric/wfeedback.cpp \
    OTModules/OTGeneric/wfirstsetup.cpp \
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
    OTModules/wFonts/Widgets/wdgeditor.cpp \
    OTModules/wFonts/Widgets/wdgpreview.cpp \
    OTModules/wFonts/wfonts.cpp \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.cpp \
    OTModules/wMaps/wGlobalProps/weditseason.cpp \
    OTModules/wMaps/wGlobalProps/wglobalprops.cpp \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.cpp \
    OTModules/wMaps/wmaps.cpp \
    OTModules/wVerifyMap/wdgoverviewtile.cpp \
    OTModules/wcleanup.cpp \
    OTModules/wmapselection.cpp \
    OTModules/wmessageviewer.cpp \
    OTWidgets/fadeinouttext.cpp \
    OTWidgets/graphicsview.cpp \
    OTWidgets/input2dcoordinates.cpp \
    OTWidgets/input3dcoordinates.cpp \
    OTWidgets/inputaddeditremove.cpp \
    OTWidgets/message.cpp \
    OTWidgets/scrollingtext.cpp \
    OTWidgets/verifymaptools.cpp \
    main.cpp \
    OTModules/OTInternals/wdevtools.cpp \
    OTModules/wVerifyMap/wverifymap.cpp \
    OTModules/OTGeneric/wabout.cpp \
    OTModules/OTGeneric/wreleasenotes.cpp \
    OTModules/OTGeneric/wstart.cpp

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
    OTBackend/OCC/OCBase.h \
    OTBackend/OCC/OCDriver.h \
    OTBackend/OCC/OCFS.h \
    OTBackend/OCC/OCFont.h \
    OTBackend/OCC/OCGameControler.h \
    OTBackend/OCC/OCHuman.h \
    OTBackend/OCC/OCKeyboard.h \
    OTBackend/OCC/OCLanguage.h \
    OTBackend/OCC/OCMap.h \
    OTBackend/OCC/OCModel.h \
    OTBackend/OCC/OCMoney.h \
    OTBackend/OCC/OCOptions.h \
    OTBackend/OCC/OCPassengercabin.h \
    OTBackend/OCC/OCPlugin.h \
    OTBackend/OCC/OCRail.h \
    OTBackend/OCC/OCSceneryobject.h \
    OTBackend/OCC/OCScript.h \
    OTBackend/OCC/OCSituation.h \
    OTBackend/OCC/OCSound.h \
    OTBackend/OCC/OCSpline.h \
    OTBackend/OCC/OCTextureChangeMaster.h \
    OTBackend/OCC/OCTextureProfile.h \
    OTBackend/OCC/OCTicketPack.h \
    OTBackend/OCC/OCTrain.h \
    OTBackend/OCC/OCTranslation.h \
    OTBackend/OCC/OCVehicle.h \
    OTBackend/OCC/OCViewable.h \
    OTBackend/OCC/OCWeather.h \
    OTBackend/OTExternal.h \
    OTBackend/OTGlobal.h \
    OTBackend/OTLogger.h \
    OTBackend/OTOmsiFileHandler.h \
    OTBackend/OTOmsiFileHandler_models.h \
    OTModules/OTGeneric/wfeedback.h \
    OTModules/OTGeneric/wfirstsetup.h \
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
    OTModules/wFonts/Widgets/wdgeditor.h \
    OTModules/wFonts/Widgets/wdgpreview.h \
    OTModules/wFonts/wfonts.h \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.h \
    OTModules/wMaps/wGlobalProps/weditseason.h \
    OTModules/wMaps/wGlobalProps/wglobalprops.h \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.h \
    OTModules/wMaps/wmaps.h \
    OTModules/wVerifyMap/wdgoverviewtile.h \
    OTModules/wcleanup.h \
    OTModules/wmapselection.h \
    OTModules/wmessageviewer.h \
    OTModules/OTInternals/wdevtools.h \
    OTModules/wVerifyMap/wverifymap.h \
    OTWidgets/fadeinouttext.h \
    OTWidgets/graphicsview.h \
    OTWidgets/input2dcoordinates.h \
    OTWidgets/input3dcoordinates.h \
    OTWidgets/inputaddeditremove.h \
    OTWidgets/message.h \
    OTWidgets/scrollingtext.h \
    OTWidgets/verifymaptools.h \
    OTModules/OTGeneric/wabout.h \
    OTModules/OTGeneric/wreleasenotes.h \
    OTModules/OTGeneric/wstart.h

FORMS += \
    OTModules/OTGeneric/wfeedback.ui \
    OTModules/OTGeneric/wfirstsetup.ui \
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
    OTModules/wFonts/Widgets/wdgeditor.ui \
    OTModules/wFonts/Widgets/wdgpreview.ui \
    OTModules/wFonts/wfonts.ui \
    OTModules/wMaps/wGlobalProps/weditgroundtexture.ui \
    OTModules/wMaps/wGlobalProps/weditseason.ui \
    OTModules/wMaps/wGlobalProps/wglobalprops.ui \
    OTModules/wMaps/wPlaceObjects/wplaceobjects.ui \
    OTModules/wMaps/wmaps.ui \
    OTModules/wVerifyMap/wdgoverviewtile.ui \
    OTModules/wcleanup.ui \
    OTModules/wmapselection.ui \
    OTModules/wmessageviewer.ui \
    OTModules/OTInternals/wdevtools.ui \
    OTModules/wVerifyMap/wverifymap.ui \
    OTWidgets/fadeinouttext.ui \
    OTWidgets/input2dcoordinates.ui \
    OTWidgets/input3dcoordinates.ui \
    OTWidgets/inputaddeditremove.ui \
    OTWidgets/message.ui \
    OTWidgets/scrollingtext.ui \
    OTWidgets/verifymaptools.ui \
    OTModules/OTGeneric/wabout.ui \
    OTModules/OTGeneric/wreleasenotes.ui \
    OTModules/OTGeneric/wstart.ui

TRANSLATIONS += \
    data/translations/tr_de.ts \
    data/translations/tr_fr.ts \
    data/translations/tr_cs.ts \
    data/translations/tr_fi.ts \
    data/translations/tr_ja.ts \
    data/translations/tr_nl.ts


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
