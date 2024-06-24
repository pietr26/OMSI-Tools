#ifndef OCOPTIONS_H
#define OCOPTIONS_H

#include "OCBase.h"

class OCOptions // *.oop | options.cfg
{
public:
    QString lastMap; // from mainDir
    QString lastDriver; // from mainDir
    QString ICAOWeather; // curr. ICAO code // TODO: In which file?
    bool driverViewSmooth;
    bool driverViewMoving; // TODO: ?

    bool noTerrainCollision;
    bool noCollision;
    bool noVehicleCollision;
    bool noPedestrianCollision;

    bool noAutoTimetableView;

    bool noAutomaticClutch;
    bool showErrorMessages;
    bool wearLifespan; // general vehicle maintenance status: 0=endless 1=vbad 2=bad 3=normal 4=good
    bool autoSteeringCenter; // automatically center steering in keyboard mode
    bool reducedSteeringSpeed; // reduces steering speed on high speed
    int ticketselling; // mode: 0=none 1=easy 2=advanced
    bool useActTime;
    bool useActDate;
    bool useActYear;
    bool alternativeViews; // alternative view keys [altView]
    bool seeOwnDriver;
    QString radioLink;
    QString font; // system font name, e.g. 'Courier New'
    QString languageIdent;
    bool noVehiclePreview;

    bool noTicketInfoVisible;

    bool disableMultithreadingCalculations;
    bool disableMultithreadingTextureLoad;

    bool loadAllTiles;
    int reflexions = -1; // [performance_realreflexions] - none[0] | economy[1] | full[2]

    int realtimeTexRelfexionSize; // [performance_reflTexSize] - in pixels: 0=1; 1=2; 2=4; 3=8; ... [increases 2^n] ...; 12=4096

    bool sunglow;

    bool restrictTexturesTo256px;

    bool noStencilBuffer;
    bool stencilShadows; // 'on'=true; 'off'=false
    bool noRainReflexions;
    bool noHumanRainReflexions;

    int maxNeighbourTiles = 1; // min: 1
    float objectDistance = 20; // min: 20
    float minObjectSize; // %, decimal, 0% - 10%
    float minObjectRelfexionSize; // %, decimal, 0% - 50%
    int objectComplexity = 3; // 0 - 3
    int mapComplexity = 2; // 0 - 2

    float switchToEconomyReflexionsMinimum = -1; // [performance_dyn_redrefl], 1st
    float switchToEconomyReflexionsMaximum = -1; // [performance_dyn_redrefl], 2nd

    float reduceNeighbourTilesMinimum = -1; // [performance_dyn_tile_red], 1st
    float reduceNeighbourTilesMaximum = -1; // [performance_dyn_tile_red], 2nd

    int maxFPS = 30;

    int texFiler1stValue = -1; // [texFilter] // TODO: ?
    int texFiler2ndValue = -1;

    int texture1stValue = -1; // [texture] // TODO: ?
    int texture2ndValue = -1;

    bool useLowTextures;

    float maxTextureMemory = 600;

    bool smokesystemsEnabled; // 0 | 1
    int particlesPerSender = 50;
    bool onlyOwnVehicle; // 0 | 1
    bool noSmokesystemsInRelfexions; // 0 | 1

    bool noTerrainLightmap;
    bool noLightmap;
    bool noNightmap;
    bool noRelfexionmap;
    bool noBumpmap;

    int maxSounds = 350; // 5 - 1000
    float masterVolume = 1; // 0.0 - 1.0
    int stereoEffect = 50; // 0 - 100
    bool dopplerSound; // 'on'=true; 'off'=false
    bool aiHasSound;
    bool sceneryHasSound;
    bool noReverbSound;

    int aiVehicleCount = 100; // 0 - 1000
    int aiHumanCount = 250; // 0 - 1000
    int aiMaxCountRandom3rdValue = -1; // TODO: ?
    int aiMaxCountRandom4thValue = -1; // TODO: ?
    int aiMaxCountRandom5thValue = -1; // TODO: ?
    int aiMaxCountRandom6thValue = -1; // TODO: ?
    int aiMaxCountRandom7thValue = -1; // TODO: ?
    int aiMaxCountRandom8thValue = -1; // TODO: ?
    int aiMaxCountRandom9thValue = -1; // TODO: ?

    int aiVehicleFactor = 100; // % (no decimal! 100 = '100%')
    int parkedVehicleFactor = 100; // % (no decimal! 100 = '100%')

    int schedAiVehicleCount = 30; // 0 - 1000
    int schedAiVehiclePriority = 4; // 1 - 4

    bool useLowAilist;

    QString editorAerialLink;

    std::optional<int> screenRatio;

    bool noTextLogHighSwitch; // e.g. in 'PC 2009.oop' as '[no_tex_low_high_switch]' // TODO: ?

    bool trackIRActive;
    bool gameControlerActive;
    bool noAutoSave;
};

#endif // OCOPTIONS_H
