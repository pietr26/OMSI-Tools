cmake_minimum_required(VERSION 3.21)

project(OMSI-Tools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# moc, uic and rcc are run on demand for everything listed below.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 6.7 is the minimum the sources need: QIcon::ThemeIcon was introduced there.
find_package(Qt6 6.7 REQUIRED COMPONENTS Core Gui Widgets Network Concurrent Sql PrintSupport)
find_package(Qt6 QUIET COMPONENTS LinguistTools)

# ------------------------------------------------------------------------------
# Sources

set(OT_SOURCES
    OTBackend/DiscordGameSDK.cpp
    OTBackend/OCC/OCBase.cpp
    OTBackend/OCC/OCDriver.cpp
    OTBackend/OCC/OCFS.cpp
    OTBackend/OCC/OCFont.cpp
    OTBackend/OCC/OCGameControler.cpp
    OTBackend/OCC/OCHuman.cpp
    OTBackend/OCC/OCKeyboard.cpp
    OTBackend/OCC/OCLanguage.cpp
    OTBackend/OCC/OCMap.cpp
    OTBackend/OCC/OCModel.cpp
    OTBackend/OCC/OCMoney.cpp
    OTBackend/OCC/OCOptions.cpp
    OTBackend/OCC/OCPassengercabin.cpp
    OTBackend/OCC/OCPlugin.cpp
    OTBackend/OCC/OCRail.cpp
    OTBackend/OCC/OCSceneryobject.cpp
    OTBackend/OCC/OCScript.cpp
    OTBackend/OCC/OCSituation.cpp
    OTBackend/OCC/OCSound.cpp
    OTBackend/OCC/OCSpline.cpp
    OTBackend/OCC/OCTextureChangeMaster.cpp
    OTBackend/OCC/OCTextureProfile.cpp
    OTBackend/OCC/OCTicketPack.cpp
    OTBackend/OCC/OCTrain.cpp
    OTBackend/OCC/OCTranslation.cpp
    OTBackend/OCC/OCVehicle.cpp
    OTBackend/OCC/OCViewable.cpp
    OTBackend/OCC/OCWeather.cpp
    OTBackend/OTContentValidator/OTContentValidator.cpp
    OTBackend/OTContentValidator/OTModelConfigValidator.cpp
    OTBackend/OTContentValidator/OTSceneryobjectValidator.cpp
    OTBackend/OTContentValidator/OTSplineValidator.cpp
    OTBackend/OTContentValidator/OTVehicleValidator.cpp
    OTBackend/OTFileSource.cpp
    OTBackend/OTLogger.cpp
    OTBackend/OTDdsImage.cpp
    OTBackend/OTPath.cpp
    OTBackend/OTMapScanner.cpp
    OTModules/OTGeneric/wfeedback.cpp
    OTModules/OTGeneric/wfirstsetup.cpp
    OTModules/OTGeneric/wpreferences.cpp
    OTModules/OTGeneric/wpromptselector.cpp
    OTModules/OTGeneric/wselectencoding.cpp
    OTModules/OTInternals/wregeditor.cpp
    OTModules/wBugDoc/wbugdoc.cpp
    OTModules/wContentSearch/wDBPanel.cpp
    OTModules/wContentSearch/waddfiles.cpp
    OTModules/wContentSearch/wcontentsearch.cpp
    OTModules/wFbh/waddpath.cpp
    OTModules/wFbh/waddterm.cpp
    OTModules/wFbh/waddword.cpp
    OTModules/wFbh/wdbcopyrights.cpp
    OTModules/wFbh/wdbknownwords.cpp
    OTModules/wFonts/Widgets/wdgeditor.cpp
    OTModules/wFonts/Widgets/wdggraphicsview.cpp
    OTModules/wFonts/Widgets/wdgpreview.cpp
    OTModules/wFonts/wfonts.cpp
    OTModules/wMaps/wGlobalProps/weditgroundtexture.cpp
    OTModules/wMaps/wGlobalProps/weditseason.cpp
    OTModules/wMaps/wGlobalProps/wglobalprops.cpp
    OTModules/wMaps/wPlaceObjects/wplaceobjects.cpp
    OTModules/wMaps/wmaps.cpp
    OTModules/wVerifyMap/Widgets/dlgfilepreview.cpp
    OTModules/wVerifyMap/Widgets/dlgsourceslist.cpp
    OTModules/wVerifyMap/Widgets/wdgoverviewtile.cpp
    OTModules/wVerifyMap/Widgets/wdgtab.cpp
    OTModules/wCleanup/wcleanup.cpp
    OTModules/wmapselection.cpp
    OTModules/wmessageviewer.cpp
    OTWidgets/fadeinouttext.cpp
    OTWidgets/input2dcoordinates.cpp
    OTWidgets/input3dcoordinates.cpp
    OTWidgets/inputaddeditremove.cpp
    OTWidgets/message.cpp
    OTWidgets/scrollingtext.cpp
    main.cpp
    OTModules/OTInternals/wdevtools.cpp
    OTModules/wVerifyMap/wverifymap.cpp
    OTModules/OTGeneric/wabout.cpp
    OTModules/OTGeneric/wreleasenotes.cpp
    OTModules/OTGeneric/wstart.cpp
)

set(OT_HEADERS
    OTBackend/DiscordGameSDK.h
    OTBackend/DiscordGameSDK/achievement_manager.h
    OTBackend/DiscordGameSDK/activity_manager.h
    OTBackend/DiscordGameSDK/application_manager.h
    OTBackend/DiscordGameSDK/core.h
    OTBackend/DiscordGameSDK/discord.h
    OTBackend/DiscordGameSDK/event.h
    OTBackend/DiscordGameSDK/ffi.h
    OTBackend/DiscordGameSDK/image_manager.h
    OTBackend/DiscordGameSDK/lobby_manager.h
    OTBackend/DiscordGameSDK/network_manager.h
    OTBackend/DiscordGameSDK/overlay_manager.h
    OTBackend/DiscordGameSDK/relationship_manager.h
    OTBackend/DiscordGameSDK/storage_manager.h
    OTBackend/DiscordGameSDK/store_manager.h
    OTBackend/DiscordGameSDK/types.h
    OTBackend/DiscordGameSDK/user_manager.h
    OTBackend/DiscordGameSDK/voice_manager.h
    OTBackend/OCC/OCBase.h
    OTBackend/OCC/OCDriver.h
    OTBackend/OCC/OCFS.h
    OTBackend/OCC/OCFont.h
    OTBackend/OCC/OCGameControler.h
    OTBackend/OCC/OCHuman.h
    OTBackend/OCC/OCKeyboard.h
    OTBackend/OCC/OCLanguage.h
    OTBackend/OCC/OCMap.h
    OTBackend/OCC/OCModel.h
    OTBackend/OCC/OCMoney.h
    OTBackend/OCC/OCOptions.h
    OTBackend/OCC/OCPassengercabin.h
    OTBackend/OCC/OCPlugin.h
    OTBackend/OCC/OCRail.h
    OTBackend/OCC/OCSceneryobject.h
    OTBackend/OCC/OCScript.h
    OTBackend/OCC/OCSituation.h
    OTBackend/OCC/OCSound.h
    OTBackend/OCC/OCSpline.h
    OTBackend/OCC/OCTextureChangeMaster.h
    OTBackend/OCC/OCTextureProfile.h
    OTBackend/OCC/OCTicketPack.h
    OTBackend/OCC/OCTrain.h
    OTBackend/OCC/OCTranslation.h
    OTBackend/OCC/OCVehicle.h
    OTBackend/OCC/OCViewable.h
    OTBackend/OCC/OCWeather.h
    OTBackend/OTContentValidator/OTContentValidator.h
    OTBackend/OTContentValidator/OTModelConfigValidator.h
    OTBackend/OTContentValidator/OTSceneryobjectValidator.h
    OTBackend/OTContentValidator/OTSplineValidator.h
    OTBackend/OTContentValidator/OTVehicleValidator.h
    OTBackend/OTExternal.h
    OTBackend/OTFileSource.h
    OTBackend/OTGlobal.h
    OTBackend/OTFileMethods.h
    OTBackend/OTBuildOptions.h
    OTBackend/OTInformation.h
    OTBackend/OTLinks.h
    OTBackend/OTNetworkConnection.h
    OTBackend/OTMiscellaneous.h
    OTBackend/OTUpdater.h
    OTBackend/OTFileOperations.h
    OTBackend/OTSettings.h
    OTBackend/OTMessage.h
    OTBackend/OTDiskUsage.h
    OTBackend/OTStrings.h
    OTBackend/OTInAppMessage.h
    OTBackend/OTLogger.h
    OTBackend/OTMapScanner.h
    OTBackend/OTOmsiFileHandler.h
    OTBackend/OTDdsImage.h
    OTBackend/OTPath.h
    OTBackend/OTPlatform.h
    OTBackend/OTOmsiFileHandler_models.h
    OTModules/OTGeneric/wfeedback.h
    OTModules/OTGeneric/wfirstsetup.h
    OTModules/OTGeneric/wpreferences.h
    OTModules/OTGeneric/wpromptselector.h
    OTModules/OTGeneric/wselectencoding.h
    OTModules/OTInternals/wregeditor.h
    OTModules/wBugDoc/wbugdoc.h
    OTModules/wContentSearch/wDBPanel.h
    OTModules/wContentSearch/waddfiles.h
    OTModules/wContentSearch/wcontentsearch.h
    OTModules/wFbh/waddpath.h
    OTModules/wFbh/waddterm.h
    OTModules/wFbh/waddword.h
    OTModules/wFbh/wdbcopyrights.h
    OTModules/wFbh/wdbknownwords.h
    OTModules/wFonts/Widgets/wdgeditor.h
    OTModules/wFonts/Widgets/wdggraphicsview.h
    OTModules/wFonts/Widgets/wdgpreview.h
    OTModules/wFonts/wfonts.h
    OTModules/wMaps/wGlobalProps/weditgroundtexture.h
    OTModules/wMaps/wGlobalProps/weditseason.h
    OTModules/wMaps/wGlobalProps/wglobalprops.h
    OTModules/wMaps/wPlaceObjects/wplaceobjects.h
    OTModules/wMaps/wmaps.h
    OTModules/wVerifyMap/Widgets/dlgfilepreview.h
    OTModules/wVerifyMap/Widgets/dlgsourceslist.h
    OTModules/wVerifyMap/Widgets/wdgoverviewtile.h
    OTModules/wVerifyMap/Widgets/wdgtab.h
    OTModules/wCleanup/wcleanup.h
    OTModules/wmapselection.h
    OTModules/wmessageviewer.h
    OTModules/OTInternals/wdevtools.h
    OTModules/wVerifyMap/wverifymap.h
    OTWidgets/fadeinouttext.h
    OTWidgets/input2dcoordinates.h
    OTWidgets/input3dcoordinates.h
    OTWidgets/inputaddeditremove.h
    OTWidgets/message.h
    OTWidgets/scrollingtext.h
    OTModules/OTGeneric/wabout.h
    OTModules/OTGeneric/wreleasenotes.h
    OTModules/OTGeneric/wstart.h
)

set(OT_FORMS
    OTModules/OTGeneric/wfeedback.ui
    OTModules/OTGeneric/wfirstsetup.ui
    OTModules/OTGeneric/wpreferences.ui
    OTModules/OTGeneric/wpromptselector.ui
    OTModules/OTGeneric/wselectencoding.ui
    OTModules/OTInternals/wregeditor.ui
    OTModules/wBugDoc/wbugdoc.ui
    OTModules/wContentSearch/wDBPanel.ui
    OTModules/wContentSearch/waddfiles.ui
    OTModules/wContentSearch/wcontentsearch.ui
    OTModules/wFbh/waddpath.ui
    OTModules/wFbh/waddterm.ui
    OTModules/wFbh/waddword.ui
    OTModules/wFbh/wdbcopyrights.ui
    OTModules/wFbh/wdbknownwords.ui
    OTModules/wFonts/Widgets/wdgeditor.ui
    OTModules/wFonts/Widgets/wdgpreview.ui
    OTModules/wFonts/wfonts.ui
    OTModules/wMaps/wGlobalProps/weditgroundtexture.ui
    OTModules/wMaps/wGlobalProps/weditseason.ui
    OTModules/wMaps/wGlobalProps/wglobalprops.ui
    OTModules/wMaps/wPlaceObjects/wplaceobjects.ui
    OTModules/wMaps/wmaps.ui
    OTModules/wVerifyMap/Widgets/dlgfilepreview.ui
    OTModules/wVerifyMap/Widgets/dlgsourceslist.ui
    OTModules/wVerifyMap/Widgets/wdgoverviewtile.ui
    OTModules/wVerifyMap/Widgets/wdgtab.ui
    OTModules/wCleanup/wcleanup.ui
    OTModules/wmapselection.ui
    OTModules/wmessageviewer.ui
    OTModules/OTInternals/wdevtools.ui
    OTModules/wVerifyMap/wverifymap.ui
    OTWidgets/fadeinouttext.ui
    OTWidgets/input2dcoordinates.ui
    OTWidgets/input3dcoordinates.ui
    OTWidgets/inputaddeditremove.ui
    OTWidgets/message.ui
    OTWidgets/scrollingtext.ui
    OTModules/OTGeneric/wabout.ui
    OTModules/OTGeneric/wreleasenotes.ui
    OTModules/OTGeneric/wstart.ui
)

set(OT_RESOURCES
    resources.qrc
)

# The compiled .qm files are checked in and embedded through resources.qrc, so the
# .ts files are listed for the IDE and for the translation targets further down only.
set(OT_TRANSLATIONS
    data/translations/tr_en.ts
    data/translations/tr_de.ts
    data/translations/tr_fr.ts
    data/translations/tr_cs.ts
    data/translations/tr_fi.ts
    data/translations/tr_ja.ts
    data/translations/tr_nl.ts
)

set_source_files_properties(${OT_TRANSLATIONS} PROPERTIES HEADER_FILE_ONLY ON SKIP_AUTOGEN ON)

# ------------------------------------------------------------------------------
# Target

qt_add_executable(OMSI-Tools WIN32 MACOSX_BUNDLE
    ${OT_SOURCES}
    ${OT_HEADERS}
    ${OT_FORMS}
    ${OT_RESOURCES}
    ${OT_TRANSLATIONS}
)

target_include_directories(OMSI-Tools PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(OMSI-Tools PRIVATE QT_MESSAGELOGCONTEXT)

# The warning level qmake's mkspecs used to switch on by default.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(OMSI-Tools PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(OMSI-Tools PRIVATE /W3)
endif()

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#target_compile_definitions(OMSI-Tools PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x060000)

target_link_libraries(OMSI-Tools PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
    Qt6::Sql
    Qt6::PrintSupport
)

if(WIN32)
    target_link_libraries(OMSI-Tools PRIVATE user32)

    # The executable icon, which RC_ICONS used to provide. Qt only generates the
    # resource once a version is set - "0.0.0.0" is what qmake wrote as well, because
    # the project never defined a VERSION.
    set_target_properties(OMSI-Tools PROPERTIES
        QT_TARGET_VERSION "0.0.0.0"
        QT_TARGET_RC_ICONS "${CMAKE_CURRENT_SOURCE_DIR}/data/icons/logo.ico"
    )
endif()

# ------------------------------------------------------------------------------
# Discord Game SDK
#
# The SDK is only shipped with this repository as a Windows library. On every other
# platform it is used as soon as the matching shared library is placed next to it,
# otherwise the rich presence is compiled out (see OT_NO_DISCORD).

set(DISCORD_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/OTBackend/DiscordGameSDK")

if(WIN32)
    set(DISCORD_SDK_LIB "${DISCORD_SDK_DIR}/discord_game_sdk.dll.lib")
    set(DISCORD_SDK_RUNTIME "${DISCORD_SDK_DIR}/discord_game_sdk.dll")
elseif(APPLE)
    set(DISCORD_SDK_LIB "${DISCORD_SDK_DIR}/discord_game_sdk.dylib")
    set(DISCORD_SDK_RUNTIME "${DISCORD_SDK_LIB}")
else()
    set(DISCORD_SDK_LIB "${DISCORD_SDK_DIR}/discord_game_sdk.so")
    set(DISCORD_SDK_RUNTIME "${DISCORD_SDK_LIB}")
endif()

if(EXISTS "${DISCORD_SDK_LIB}")
    target_sources(OMSI-Tools PRIVATE
        "${DISCORD_SDK_DIR}/achievement_manager.cpp"
        "${DISCORD_SDK_DIR}/activity_manager.cpp"
        "${DISCORD_SDK_DIR}/application_manager.cpp"
        "${DISCORD_SDK_DIR}/core.cpp"
        "${DISCORD_SDK_DIR}/image_manager.cpp"
        "${DISCORD_SDK_DIR}/lobby_manager.cpp"
        "${DISCORD_SDK_DIR}/network_manager.cpp"
        "${DISCORD_SDK_DIR}/overlay_manager.cpp"
        "${DISCORD_SDK_DIR}/relationship_manager.cpp"
        "${DISCORD_SDK_DIR}/storage_manager.cpp"
        "${DISCORD_SDK_DIR}/store_manager.cpp"
        "${DISCORD_SDK_DIR}/types.cpp"
        "${DISCORD_SDK_DIR}/user_manager.cpp"
        "${DISCORD_SDK_DIR}/voice_manager.cpp"
    )

    target_link_libraries(OMSI-Tools PRIVATE "${DISCORD_SDK_LIB}")

    # Put the library next to the binary => no runtime error :)
    add_custom_command(TARGET OMSI-Tools POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DISCORD_SDK_RUNTIME}" "$<TARGET_FILE_DIR:OMSI-Tools>"
        VERBATIM
    )

    if(APPLE)
        set_target_properties(OMSI-Tools PROPERTIES BUILD_RPATH "@loader_path" INSTALL_RPATH "@loader_path")
    elseif(NOT WIN32)
        set_target_properties(OMSI-Tools PROPERTIES BUILD_RPATH "$ORIGIN" INSTALL_RPATH "$ORIGIN")
    endif()
else()
    message(STATUS "Discord Game SDK library not found (${DISCORD_SDK_LIB}) - building without rich presence.")
    target_compile_definitions(OMSI-Tools PRIVATE OT_NO_DISCORD)
endif()

# ------------------------------------------------------------------------------
# Translations
#
# Both targets are built on request only and write their result next to the .ts
# files, which is exactly what the Linguist tools of the IDE did before.

if(Qt6LinguistTools_FOUND)
    add_custom_target(update_translations
        COMMAND Qt6::lupdate "${CMAKE_CURRENT_SOURCE_DIR}" -ts ${OT_TRANSLATIONS} -locations absolute -no-obsolete
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Collecting translatable strings into the .ts files"
        VERBATIM
    )

    add_custom_target(release_translations
        COMMAND Qt6::lrelease ${OT_TRANSLATIONS}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Compiling the .ts files into .qm files"
        VERBATIM
    )
endif()

# ------------------------------------------------------------------------------
# Installation - the destination qmake used by default.

if(UNIX AND NOT APPLE AND NOT ANDROID)
    install(TARGETS OMSI-Tools RUNTIME DESTINATION "/opt/${PROJECT_NAME}/bin")
endif()
