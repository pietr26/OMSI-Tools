QT       += core gui network concurrent sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_MESSAGELOGCONTEXT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    OTBackend/OTLogger.cpp \
    OTModules/wContentSearch/wDBPanel.cpp \
    OTModules/wContentSearch/wcontentsearch.cpp \
    OTModules/wVehicleList/wvehiclelist.cpp \
    OTModules/wbugrepot.cpp \
    OTWidgets/wTbnListWidgetTools.cpp \
    main.cpp \
    OTModules/wVerifyMap/dignorelist.cpp \
    OTModules/wdevtools.cpp \
    OTModules/wfonts.cpp \
    OTModules/wVerifyMap/wverifymap.cpp \
    wabout.cpp \
    wchangelog.cpp \
    wfirstsetup.cpp \
    wsettings.cpp \
    wstart.cpp \
    wstartupscreen.cpp

HEADERS += \
    OTBackend/OTDatabaseHandler.h \
    OTBackend/OTGlobal.h \
    OTBackend/OTLogger.h \
    OTBackend/OTOmsiFileHandler.h \
    OTModules/wContentSearch/wDBPanel.h \
    OTModules/wContentSearch/wcontentsearch.h \
    OTModules/wVehicleList/wvehiclelist.h \
    OTModules/wVerifyMap/dignorelist.h \
    OTModules/wbugrepot.h \
    OTModules/wdevtools.h \
    OTModules/wfonts.h \
    OTModules/wVerifyMap/wverifymap.h \
    OTWidgets/wTbnListWidgetTools.h \
    wabout.h \
    wchangelog.h \
    wfirstsetup.h \
    wsettings.h \
    wstart.h \
    wstartupscreen.h

FORMS += \
    OTModules/wContentSearch/wDBPanel.ui \
    OTModules/wContentSearch/wcontentsearch.ui \
    OTModules/wVehicleList/wvehiclelist.ui \
    OTModules/wVerifyMap/dignorelist.ui \
    OTModules/wbugrepot.ui \
    OTModules/wdevtools.ui \
    OTModules/wfonts.ui \
    OTModules/wVerifyMap/wverifymap.ui \
    OTWidgets/wTbnListWidgetTools.ui \
    wabout.ui \
    wchangelog.ui \
    wfirstsetup.ui \
    wsettings.ui \
    wstart.ui \
    wstartupscreen.ui

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
