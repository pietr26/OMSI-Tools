QT       += core widgets gui network concurrent sql

CONFIG += c++17

DEFINES += QT_MESSAGELOGCONTEXT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    OTBackend/OTLogger.cpp \
    OTModules/OTGeneric/wfeedback.cpp \
    OTModules/wContentSearch/wDBPanel.cpp \
    OTModules/wContentSearch/wcontentsearch.cpp \
    OTModules/wFbh/wfbh.cpp \
    OTModules/wcleanup.cpp \
    OTModules/wmapselection.cpp \
    OTModules/wvehiclelist.cpp \
    OTModules/OTInternals/wstyletest.cpp \
    OTWidgets/fadeinouttext.cpp \
    OTWidgets/firstsetup.cpp \
    OTWidgets/verifymaptools.cpp \
    main.cpp \
    OTModules/wVerifyMap/dignorelist.cpp \
    OTModules/OTInternals/wdevtools.cpp \
    OTModules/wfonts.cpp \
    OTModules/wVerifyMap/wverifymap.cpp \
    OTModules/OTGeneric/wabout.cpp \
    OTModules/OTGeneric/wreleasenotes.cpp \
    OTModules/OTGeneric/wsettings.cpp \
    OTModules/OTGeneric/wstart.cpp \
    OTModules/OTGeneric/wstartupscreen.cpp

HEADERS += \
    OTBackend/OTDatabaseHandler.h \
    OTBackend/OTGlobal.h \
    OTBackend/OTLogger.h \
    OTBackend/OTOmsiFileHandler.h \
    OTBackend/OTStyleDraw.h \
    OTModules/OTGeneric/wfeedback.h \
    OTModules/wContentSearch/wDBPanel.h \
    OTModules/wContentSearch/wcontentsearch.h \
    OTModules/wFbh/FbhFunctions.h \
    OTModules/wFbh/wfbh.h \
    OTModules/wcleanup.h \
    OTModules/wmapselection.h \
    OTModules/wvehiclelist.h \
    OTModules/wVerifyMap/dignorelist.h \
    OTModules/wVerifyMap/ignoreListFunctions.h \
    OTModules/OTInternals/wdevtools.h \
    OTModules/wfonts.h \
    OTModules/wVerifyMap/wverifymap.h \
    OTModules/OTInternals/wstyletest.h \
    OTWidgets/fadeinouttext.h \
    OTWidgets/firstsetup.h \
    OTWidgets/verifymaptools.h \
    OTModules/OTGeneric/wabout.h \
    OTModules/OTGeneric/wreleasenotes.h \
    OTModules/OTGeneric/wsettings.h \
    OTModules/OTGeneric/wstart.h \
    OTModules/OTGeneric/wstartupscreen.h

FORMS += \
    OTModules/OTGeneric/wfeedback.ui \
    OTModules/wContentSearch/wDBPanel.ui \
    OTModules/wContentSearch/wcontentsearch.ui \
    OTModules/wFbh/wfbh.ui \
    OTModules/wcleanup.ui \
    OTModules/wmapselection.ui \
    OTModules/wvehiclelist.ui \
    OTModules/wVerifyMap/dignorelist.ui \
    OTModules/OTInternals/wdevtools.ui \
    OTModules/wfonts.ui \
    OTModules/wVerifyMap/wverifymap.ui \
    OTModules/OTInternals/wstyletest.ui \
    OTWidgets/fadeinouttext.ui \
    OTWidgets/firstsetup.ui \
    OTWidgets/verifymaptools.ui \
    OTModules/OTGeneric/wabout.ui \
    OTModules/OTGeneric/wreleasenotes.ui \
    OTModules/OTGeneric/wsettings.ui \
    OTModules/OTGeneric/wstart.ui \
    OTModules/OTGeneric/wstartupscreen.ui

TRANSLATIONS += \
    data/translations/tr_de.ts \
    data/translations/tr_fr.ts \
    data/translations/tr_it.ts \
    data/translations/tr_cz.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = data/icons/iconOMSI-Tools.ico

RESOURCES += \
    ressources.qrc

DISTFILES += \
    devNotes
