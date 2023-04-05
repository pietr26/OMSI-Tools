#ifndef OCMAIN_H
#define OCMAIN_H

#include "OTGlobal.h"

/* '//-' (hinter einer Variable): Schreibschutz / besondere Achtung bei Veränderung */

class OCModel // cfg
{
public:
    // TODO
};

////////////////////////////////////////

class OCDriver // odr
{
public:
    class BusInfo
    {
    public:
        QString friendlyname;
        int distance; // 100m (1 =^ 100m)
    };

    QString name;
    QString genderIdent;
    QDate birthDate;
    QDate hireDate;

    int totalBusstops;
    int tooLateBusstops;
    int tooEarlyBusstops;

    int distance; // km

    int crashes;
    int hitAndRun;
    int extremeCrashes;
    int injuredPeds;

    int tickets;
    float money;

    int passengerComfortTotal;
    int passengerComfortPositive;
    int ticketComfortTotal;
    int ticketComfortPositive;
    float driveComfort; // % decimal

    QList<BusInfo> busInfo;
};

class OCFont // oft
{
public:
    class Character
    {
    public:
        Character(QString ca = "", int lP = -1, int rP = -1, int hP = -1, QString co = "")
        {
            character = ca;
            leftPixel = lP;
            rightPixel = rP;
            highestPixelInFontRow = hP;
            comment = co;
        }

        QString character;
        int leftPixel;
        int rightPixel;
        int highestPixelInFontRow;
        QString comment;
    };

    QString path;
    QString name;
    QString colorTexture;
    QString alphaTexture;
    int maxHeightOfChars = -1;
    int distanceBetweenChars = -1;

    QList<Character> characters;
};

class OCHuman // hum
{
public:
    // TODO
};

class OCGameControler // cfg
{
public:
    // TODO
};

class OCKeyboard // cfg
{
public:
    class Entry
    {
    public:
        QString event;
        int keyIdent = -1;
        // bool ?;
    };


    QList<Entry> entries;
};

class OCTextTranslation // dsc
{
public:
    // TODO: Embed in other classes?
    QString name;
    QString description;
};

class OCLanguage // olf
{
public:
    class Part
    {
    public:
        QString ident;
        QString translation;
    };

    QString ident;
    QList<Part> parts;
};

class OCMap {
public:
    class Global // global.cfg
    {
    public:
        class Texture
        {
        public:
            QString mainTex;
            QString subTex;
            //OBJECT ???; // TODO
            //OBJECT ???; // TODO
            //OBJECT ???; // TODO
        };

        class Season
        {
        public:
            int season;
            int seasonStartDay;
            int seasonEndDay;
        };

        class AiDensity
        {
        public:
            float timeDecimal;
            float density; // can be bigger as 1?
        };

        class Tile
        {
        public:
            int x;
            int y;
        };

        class Entrypoint
        {
        public:

            // TODO
            /* [entrypoints]
         * entrypointCount      * (
         * objectID
         * ?
         * xpos
         * zpos            Attention! Inverted
         * ypos
         * ?        \
         * ?        |  maybe       | very special dependencies
         * ?        |  irrelevant  | with rot, pitch and bank
         * ?        /
         * tileID  // see oreder in global.cfg
         * name
         * )
        */
        };

        class BackgroundImage
        {
        public:
            bool isVisible;
            QString picturePath;
            float width;
            float height;
            float startWidth;
            float startHeight;

            /* [backgroundimage]
         * isActive (0=false 1=true)
         * picturePath (absolute path)
         * width
         * height
         * startwidth       decimal
         * startheight      decimal
        */
        };

        class Mapcam
        {
        public:
            int tileXPos;
            int tileYPos;
            float xPos;
            float yPos;
            float zPos;
            float rotAroundZ;
            float rotAroundX;
            float distanceFromZeroMapHeight;

            /* [mapcam]
         * tileXPos
         * tileYPos
         * xpos
         * zpos            Attention! Inverted
         * ypos
         * rotAroundZ
         * rotAroundX
         * distanceFromZeroMapHeight
        */
        };

        QString name;
        QString friendlyname;
        QString description;
        int version; //-
        int nextIDCode; //-
        bool worldCoodinates = false;
        bool dynHelpers = false;
        bool lht = false;
        bool realrail = false;
        BackgroundImage bgImage;
        Mapcam standardView;
        QString moneysystem;
        QString ticketpack;
        int repairTime = -1; // minimal time
        QString standardDepot;
        QList<Texture> groundTextures;
        int startYear;
        int endYear;
        int realYearOffset;
        QList<Season> seasons;
        QList<AiDensity> trafficDensities;
        QList<AiDensity> passengerDensities;
        QList<Entrypoint> entrypoints;
        QList<Tile> tiles;
    };

    class Tile
    {
    public:
        class Spline
        {
        public:
            class Rule
            {
            public:
                int pathIdent = -1;
                QString param; // TODO: params are const values, list them!
                int value = -1;
                int aiGroupIndex = -1;
            };

            bool h = false;

            //?
            QString path;
            //?...

            QList<Rule> rules;

            // Different param count spline / splineH?
        };

        class Sceneryobject
        {
        public:
            // TODO
            // possible for objects: [object] [splineAttachement] [attachObj] [splineAttachement_repeater]

            //?
            QString path;
            //?...

            int parentObjectID = -1; // [varparent]

            // Attention: Different param count object / attachement!

            /* [object]
         * ?
         * path
         * objectID
         * xpos
         * ypos
         * zpos
         * rot
         * pitch
         * bank
         * labelCount
         * labelCount * labels... \./
        */
        };

        int version;
        bool terrain = false;
        bool variableTerrainLightmap = false;
        bool variableTerrain = false;

        QList<Spline> splines;

        QList<Sceneryobject> sceneryobjects;
    };

    class UnschedTrafficDensitiesGroup
    {
    public:
        class day
        {
        public:
            class density
            {
            public:
                float time;
                float density; // can be bigger as 1!
            };

            // 0 = all days - else: +1 = weekday, +2 = saturday, +4 = sunday
            int dayIdent = -1;
            QList<density> densities;

        };

        QString name;
        float density;
        QList<day> days;
    };

    class UnschedVehicleGroup
    {
    public:
        QString name;
        /*
         * 0 = no traffic by default,
         * 1 = use density of first UVG by default,
         * 2 = ...            second ...          ,
         * 3 = ...
         if this is the first UVG, then 1 = use density class Medium by default */
        int defaultDensity = -1;
    };

    class Parklist
    {
    public:
        int ident;
        QList<QString> objectList;
    };

    class Ailist
    {
    public:
        class Ailist1
        {
        public:
            class Group
            {
            public:
                QString name;
                int vehicleCount = 0;
                QList<int> vehicleIdents;
                QString hofFileName;
            };

            //?
            //?
            int vehicleCount = 0;
            QList<QString> vehiclePaths;
            QList<Group> groups;
        };

        class Ailist2
        {
        public:
            class Group
            {
            public:
                class Vehicle
                {
                public:
                    QString vehiclePath;
                    int density;
                };

                QString name;
                QString hofFileName; //?
                QList<Vehicle> vehicles;
            };

            class Groupdepot
            {
            public:
                class Typgroup2
                {
                public:
                    class Vehicle
                    {
                    public:
                        QString number;
                        QString repaintName;
                        QString regPlate;
                    };

                    QString vehiclePath;
                    QList<Vehicle> vehicles;
                };

                QString name;
                QString hofFileName;

                // TODO: Typgroup (Typgroup1)?
                QList<Typgroup2> typgroup2s;
            };

            QList<Group> groups;
            QList<Groupdepot> groupdepots;
        };

        int usedAilist = -1;
    };

    class Chrono
    {
    public:
        class Tile
        {
        public:
            class selectObject
            {
            public:
                int ident = -1;

                int relabelCount;
                QList<QString> relabels;

                bool isDeleted = false;
                QString type; // optional new path for an object
            };

            class selectSpline
            {
            public:
                int ident = -1;
                bool isDeleted = false;
                QString type; // optional new path for an spline

                QList<OCMap::Tile::Spline::Rule> rules;
            };

            int version;

            QList<OCMap::Tile::Spline> splines;
            QList<OCMap::Tile::Sceneryobject> sceneryobjects;
        };

        QString folderName;
        QString name;
        QString description;
        QList<QString> deactivatedLines;
        QDate start;
        QDate end;
        QString ticketpack;
        QString moneysystem;
        QList<Tile> changedTiles;
        OCMap::Ailist ailist; // ailists_#upd.txt
        Ailist ailistLow; // ailists_#low_#upd.txt -- ??

        // TODO: TTData!
        // TODO: humans, drivers etc.?
    };

    class Signalroute
    {
    public:
            // TODO
    };

    class Holidays
    {
        // TODO: This file exists multiple times with file (pre-)extension [language code].
    public:
        QDate start;
        QDate end;
        QString name;
    };

    class Holiday
    {
        // TODO: This file exists multiple times with file (pre-)extension [language code].
    public:
        QDate date;
        QString name;
    };

    Global global;
    QList<QString> humans; // humans.txt
    QList<QString> drivers; // drivers.txt
    QList<QString> registrations; // regstrations.txt
    QList<Parklist> parklists; // parklist.txt | parklist_n.txt
    QList<Chrono> chronos; // \Chrono\*[DIR]
    QList<UnschedVehicleGroup> unschedVehicleGroups; // unsched_vehgroups.txt
    QList<UnschedTrafficDensitiesGroup> unschedTrafficDensitiesGroups; // unsched_trafficdens.txt
    QList<Signalroute> signalroutes; // signalroutes.cfg
    QList<Holidays> holidaysMultiple; // holidays.txt | holidays_[LGC].txt // TODO: see list class
    QList<Holiday> holidaysSingle; // holidays.txt | holidays_[LGC].txt // TODO: see list class
    Ailist ailist; // ailists.txt
    Ailist ailistLow; // ailists_#low.txt
};

class OCMoney { // *.cfg
public:
    class moneyPart
    {
    public:
        QString modelFilename; // relative to CURRENT folder (without model folder!)
        float value;
    };

    QString name; // TODO: Useless?
    int decimalCount;

    QList<moneyPart> coins;
    QList<moneyPart> bills;
};

class OCOptions // *.oop | options.cfg
{
public:
    QString lastMap; // from mainDir
    QString lastDriver; // from mainDir
    QString ICAOWeather; // curr. ICAO code // TODO: In which file?
    bool driverViewSmooth = false;
    bool driverViewMoving = false; // Todo: ?
    bool noTerrainCollision = false;
    bool wearLifespan = -1; // general vehicle maintenance status: 0=endless 1=vbad 2=bad 3=normal 4=good
    bool autoSteeringCenter = false; // automatically center steering in keyboard mode
    bool reducedSteeringSpeed = false; // reduces steering speed on high speed
    int ticketselling = -1; // mode: 0=none 1=easy 2=advanced
    bool useActTime = false;
    bool useActDate = false;
    bool useActYear = false;
    bool alternativeViews = false; // alternative view keys [altView]
    bool seeOwnDriver = false;
    QString radioLink;
    QString font; // system font name, e.g. 'Courier New'
    QString languageIdent;
    bool noVehiclePreview = false;

    bool disableMultithreadingCalculations = false;
    bool disableMultithreadingTextureLoad = false;

    bool loadAllTiles = false;
    QString Reflexions; // [performance_realreflexions] - none | economy | full

    int realtimeTexRelfexionSize = -1; // [performance_reflTexSize] - in pixels: 0=1; 1=2; 2=4; 3=8; ... [increases 2^n] ...; 12=4096

    bool sunglow = false;

    bool restrictTexturesTo256px = false;

    bool noStencilBuffer = false;
    bool stencilShadows; // 'on'=true; 'off'=false
    bool noRainReflexions = false;
    bool noHumanRainReflexions = false;

    int maxNeighbourTiles = -1; // min: 1
    float objectDistance = -1; // min: 20
    float minObjectSize = -1; // %, decimal, 0% - 10%
    float minObjectRelfexionSize = -1; // %, decimal, 0% - 50%
    int objectComplexity = -1; // 0 - 3
    int mapComplexity = -1; // 0 - 2

    float switchToEconomyReflexionsMinimum = -1; // [performance_dyn_redrefl], 1st
    float switchToEconomyReflexionsMaximum = -1; // [performance_dyn_redrefl], 2nd

    float reduceNeighbourTilesMinimum = -1; // [performance_dyn_tile_red], 1st
    float reduceNeighbourTilesMaximum = -1; // [performance_dyn_tile_red], 2nd

    int maxFPS = -1;

    int texFiler1stValue = -1; // [texFilter] // TODO: ?
    int texFiler2ndValue = -1;

    int texture1stValue = -1; // [texture] // TODO: ?
    int texture2ndValue = -1;

    bool useLowTextures = false;

    float maxTextureMemory = -1;

    bool smokesystemsEnabled; // 0 | 1
    int particlesPerSender = -1;
    bool onlyOwnVehicle; // 0 | 1
    bool noSmokesystemsInRelfexions; // 0 | 1

    bool noTerrainLightmap = false;
    bool noLightmap = false;
    bool noNightmap = false;
    bool noRelfexionmap = false;
    bool noBumpmap = false;

    int maxSounds = -1; // 5 - 1000
    float masterVolume = -1; // 0.0 - 1.0
    int stereoEffect = -1; // 0 - 100
    bool dopplerSound; // 'on'=true; 'off'=false
    bool aiHasSound = false;
    bool sceneryHasSound = false;

    int aiVehicleCount = -1; // 0 - 1000
    int aiHumanCount = -1; // 0 - 1000
    int aiMaxCountRandom3rdValue = -1; // TODO: ?
    int aiMaxCountRandom4thValue = -1; // TODO: ?
    int aiMaxCountRandom5thValue = -1; // TODO: ?
    int aiMaxCountRandom6thValue = -1; // TODO: ?
    int aiMaxCountRandom7thValue = -1; // TODO: ?
    int aiMaxCountRandom8thValue = -1; // TODO: ?
    int aiMaxCountRandom9thValue = -1; // TODO: ?

    int aiVehicleFactor = -1; // % (no decimal! 100 = '100%')
    int parkedVehicleFactor = -1; // % (no decimal! 100 = '100%')

    int schedAiVehicleCount = -1; // 0 - 1000
    int schedAiVehiclePriority = -1; // 1 - 4

    bool useLowAilist = false;

    QString editorAerialLink;
};

class OCSituation {
public:
    QString name;
    QString description;
    // TODO... - eins nach dem anderen, Piet! ;-)
};

// car_use (Spandau)
// TTData

#endif // OCMAIN_H

/* TODO:
 * set unsigned int, float etc. if useful
*/
