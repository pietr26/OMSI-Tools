#ifndef OCC_H
#define OCC_H

#include "OTGlobal.h"

/* '//-' (hinter einer Variable): Schreibschutz / besondere Achtung bei Veränderung */

template<class T>
class OC2DCoordinates
{
public:
    static_assert(std::is_arithmetic<T>::value, "The type T must be numeric.");

    OC2DCoordinates(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    T x;
    T y;
};

template<class T>
class OC2DCoordinatesSide
{
public:
    static_assert(std::is_arithmetic<T>::value, "The type T must be numeric.");

    OC2DCoordinatesSide(T y, T z)
    {
        this->y = y;
        this->z = z;
    }

    T y;
    T z;
};

template<class T>
class OC3DCoordinates
{
public:
    static_assert(std::is_arithmetic<T>::value, "The type T must be numeric.");

    OC3DCoordinates(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    T x;
    T y;
    T z;
};

template<class T>
class OC3DBox
{
public:
    static_assert(std::is_arithmetic<T>::value, "The type T must be numeric.");

    OC3DBox(T xSize, T ySize, T zSize, T xPos, T yPos, T zPos)
    {
        this->xSize = xSize;
        this->ySize = ySize;
        this->zSize = zSize;
        this->xPos = xPos;
        this->yPos = yPos;
        this->zPos = zPos;
    }

    T xSize;
    T ySize;
    T zSize;
    T xPos;
    T yPos;
    T zPos;
};

template<class T>
class OCOptionalValue
{
public:
    OCOptionalValue() { exists = false; }
    OCOptionalValue(T value) { this->value = value; }

    bool exists;
    T value;
};

class OCTextureChangeMaster
{
public:
    QString filename;
    QString variable; // int variable!

    QList<QString> entries;
};

class OCModel // cfg
{
public:
    // If e.g. particle_emitter is used before all [mesh] entries, create an emtpy material - TODO: "OPTIONAL IN MESH" section in txt :-)

    class LOD
    {
    public:
        class Mesh
        {
        public:
            Mesh(bool isInofficial = false) { if (isInofficial) binded = OCOptionalValue<BindedProperties>(); }

            class BindedProperties
            {
            public:
                class Material
                {
                public:
                    class MaterialProperties
                    {
                        // This props are changable with [matl_change] command. TODO: Check them!
                    public:
                        class Allcolor
                        {
                        public: // all: rgb
                            QColor diffuse;
                            QColor ambient;
                            QColor specular;
                            QColor emissive;
                            float power;
                        };

                        enum TextureAdress
                        {
                            clamp = 1,
                            border = 2,
                            mirror = 3,
                            mirrorOnce = 4
                        };

                        class TextureMappingIntensity
                        {
                        public:
                            QString filename;
                            float intensity = -1;
                        };

                        class TextureMappingVariable
                        {
                        public:
                            QString filename;
                            QString variableName;
                        };

                        int changeIndex = -1;

                        OCOptionalValue<Allcolor> allcolor;
                        int alpha = -1;
                        OCOptionalValue<TextureMappingIntensity> bumpMap; // TODO: Binding to variable possible? TESTING
                        OCOptionalValue<TextureMappingIntensity> envMap; // TODO: see bumMap member
                        OCOptionalValue<TextureMappingIntensity> rainDropMap; // TODO: see bumMap member
                        QString envMapMask;
                        int envMapRealTimeFactor = -1;
                        QList<TextureMappingVariable> freeTextures;
                        QString nightMap;
                        QString transMap;
                        bool noZCheck;
                        bool noZWrite;
                        int zBias = -1;
                        OCOptionalValue<TextureAdress> textureAdress;

                        int nightMapMode = -1;

                        QString textureCoordinateTransformationX;
                        QString textureCoordinateTransformationY;

                        QString alphaScaleVariable;
                    };

                    QString filename;
                    int index;
                    int textTextureIndex = -1; // TODO: Also in MaterialProperty?
                    int scriptTextureIndex = -1; // TODO: see textTextureIndex member

                    QString changeVariable;
                };

                class Animation
                {
                public:
                    class AnimationMethod
                    {
                    public:
                        QString variable;
                        float factor;
                    };

                    OCOptionalValue<OC3DCoordinates<float>> originTransform;
                    OCOptionalValue<OC3DCoordinates<int>> originRotation; // TODO: Does this make sense? :')

                    OCOptionalValue<AnimationMethod> animationTransform;
                    OCOptionalValue<AnimationMethod> animationRotation;
                    OCOptionalValue<float> maxSpeed;
                    OCOptionalValue<float> delay;
                    OCOptionalValue<float> offset;
                };

                class Visible
                {
                public:
                    QString variable;
                    QString value; // With this value, the object is visible
                };

                class Bone
                {
                public:
                    QString name;
                    int property;
                };

                QString filename;

                QList<Material> materials;
                QList<Animation> animations;
                QString ownAnimationGroup; // [mesh_ident] - above [animparent]!
                QString parentToAnimationGroup; // [animparent] - above [mesh_ident]!

                QString mouseEvent; // trigger name

                bool hasShadow;
                bool isShadowMesh;

                Visible visible;

                QList<Bone> bones;

                QString texchangeFile;

                bool hasSmoothSkin;

                int viewpoint = -1; // 0=^7   |   +1=userVehicleInside +2=userVehicleInside +4=aiVehicle

                QList<int> illuminationInterior; // min=4 max=4 - fill empty places with '-1'!
            };

            OCOptionalValue<BindedProperties> binded;

            class InteriorLight
            {
            public:
                QString variable;
                float distance; // from origin to 63% of power
                QColor color;
                OC3DCoordinates<float> position;
            };

            class ParticleEmitter
            {
            public:
                enum Methods
                {

                };
            };

            QList<InteriorLight> interiorLights;

        };

        float factor = -1;
        Mesh mesh;
    };

    class TextTexture
    {
    public:
        bool isEnh;

        QString variable;
        QString fontName;
        int width = -1;
        int height = -1;
        bool useColorTexture;
        QColor color; // if useColorTexture == true

        // if (isEnh) {
        enum Orientation
        {
            middle = 0,
            left = 1,
            right = 2,
            middlePerfect = 3,
            middleFromLeft = 4,
            middleFRomRight = 5
        };

        Orientation orientation;
        bool snapFontToTop;
        // }
    };

    class ComplexTextureChange
    {
    public:
        class Texture
        {
        public:
            QString textureChangeIdent;
            QString newTexture;
        };

        QString variable = "Colorscheme"; // from OMSI
        QString ctiPath; // from OCViewable folder (bus / sco folder)
        int textureLevel = 0;

        QList<Texture> textures;
    };



    QList<LOD> lods;

    QList<TextTexture> textTextures;
    QList<OC2DCoordinates<int>> scriptTextures;
    OCOptionalValue<OC3DBox<float>> viewbox; // [VFDmaxmin] - ATTENTION: Some calculations are necessary to fit values in the class! - values: 1=xMinPos 2=yMinPos 3=zMinPos 4=xMaxPos 5=yMaxPos 6=zMaxPos
    QList<ComplexTextureChange> complexTextureChanges;

};

class OCSound // cfg
{
public:
    // TODO
};

class OCPassengerCabin // cfg
{
public:
    // TODO
};

template<class T>
class OCVariableDeclaration
{
public:
    QString varname;
    T value;

    /*
        QVariant value:
        if (value.type() == QVariant::Int) // int
        else if (value.type() == QVariant::Double || value.type() == QVariant::Float) // float
    */
};

class OCScript
{
public:
    // TODO
};

class OCRail
{
public:
    virtual void abstract() = 0;

    class RailEnh
    {
    public:
        float singleRailLength;
        bool pushes;
        float waveLengthY;
        float maxAmplitudeY;
        float errorExponentY;

        float waveLengthZ;
        float maxAmplitudeZ;
        float errorExponentZ;
    };

    class ThirdRail
    {
    public:
        int thirdRail1stValue; // TODO: ?
        int thirdRail2ndValue; // TODO: ?
        int thirdRail3rdValue; // TODO: ?
        int thirdRail4thValue; // TODO: ?
        int thirdRail5thValue; // TODO: ?
        int thirdRail6thValue; // TODO: ?
    };
};

class OCViewable // Sceneryobjects, Vehicles, Humans
{
public:
    class Attachment
    {
    public:
        class Method
        {
        public:
            enum MethodIdent
            {
                xRotation = 1,
                yRotation = 2,
                zRotation = 3,
                transform = 4
            };

            MethodIdent type;
            float rotation;
            OC3DCoordinates<float> transformation;
        };
        QList<Method> methods;
    };

    QString friendlyname;
    QList<QString> groups;
    QString description;

    QList<Attachment> attachments;

    OCOptionalValue<OC3DBox<float>> boundingBox;

    QList<QString> varNameLists;
    QList<QString> stringVarNameLists;
    QList<QString> scripts;
    QList<QString> consfiles;

    QString modelPath;
    QString passengerCabinPath;
    QString pathPath;
    QString soundPath;
    QString aiSoundPath;

    OCOptionalValue<int> mass;

    OCOptionalValue<OC3DCoordinates<int>> momentOfIntertia;

    bool noDistanceCheck;
};

class OCTextureProfile
{
public:
    bool hasMoisture;
    bool hasPuddles;
    bool hasTerrainMapping;
    int surface = -1; // TODO: enum? - OCOptionalValue?
    /*  0	asphalt (standard) - normaler Asphalt
        1	concrete - Beton(-platten)
        2	cobllestone - Kopfsteinpflaster
        3	dirt - Schmutz/Erde
        4	grass - Gras
        5	gravel - Kies
        6	snow - Schnee
        7	deep snow - tiefer Schnee
        8	slush - Matsch
        9	ice - Eis
        10	water - Wasser
        11	mud - Schlamm
        12	sand - Sand
        13	metal - Metall
        14	wood - Holz
        15	small cobblestone - kleine Kopfsteinpflaster
        16	interlocking paver - Verbundpflaster
    */
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
    int maxHeightOfChars = -1; // TODO: '1-'?
    int distanceBetweenChars = -1; // TODO: '-1'?

    QList<Character> characters;
};

class OCHuman // hum
{
public:
    int age = -1;
    float seatHeight;
    float feetDistance;
    float height;

    OC3DCoordinates<float> hip; // xyz
    OC3DCoordinates<float> knee; // xyz
    OC2DCoordinatesSide<float> waist; // yz
    OC3DCoordinates<float> shoulder; // xyz
    OC3DCoordinates<float> elbow; // xyz
    OC2DCoordinatesSide<float> neck; // yz
    OC3DCoordinates<float> hand; // xyz
    OC3DCoordinates<float> finger; // xyz

    float stepWidth = 1.4;
    int upperArm = 66;
    float armSwingMultiplicator = 1;
    float walkHip = 1;
    float animWalkWaist = 0;
    QString ticketPackVoiceFolderName;

    OCViewable model;
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
        int keyIdent = -1; // TODO: OCOptionalValue?
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

class OCMapPosition
{
public:
    OC2DCoordinates<int> tilePosition;
    OC3DCoordinates<float> position;
    float rotAroundZ;
    float rotAroundX;
    float distanceFromZeroMapHeight;

    /* [mapcam] - from global.cfg
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

        class TileInformation
        {
        public:
            OC2DCoordinates<int> position;
            QString name; // should be auto-generated by OMSI Editor in most cases
        };

        QString name;
        QString friendlyname;
        QString description;
        int version; //-
        int nextIDCode; //-
        bool worldCoodinates;
        bool dynHelpers;
        bool lht;
        bool realrail;
        BackgroundImage bgImage;
        OCMapPosition standardView;
        QString moneysystem;
        QString ticketpack;
        int repairTime; // minimal time
        QString standardDepot;
        QList<Texture> groundTextures;
        int startYear;
        int endYear;
        OCOptionalValue<int> realYearOffset;
        QList<Season> seasons;
        QList<AiDensity> trafficDensities;
        QList<AiDensity> passengerDensities;
        QList<Entrypoint> entrypoints;
        QList<TileInformation> tiles;
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
                int pathIdent = -1; // TODO: OCOptionalValue?
                QString param; // TODO: params are const values, list them!
                int value = -1; // TODO: OCOptionalValue?
                int aiGroupIndex = -1; // TODO: OCOptionalValue?
            };

            bool h;

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

            OCOptionalValue<int> parentObjectID; // [varparent]

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
        bool terrain;
        bool variableTerrainLightmap;
        bool variableTerrain;

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
            int dayIdent = -1; // TODO: OCOptionalValue?
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
        int defaultDensity = -1; // TODO: OCOptionalValue?
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
                int vehicleCount;
                QList<int> vehicleIdents;
                QString hofFileName;
            };

            int vehicleClass;
            int baseClassForUnschedAI = -1; // -1: All vehicles that weren't mentioned in a group
            int vehicleCount;
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
                QString hofFileName; // TODO: ?
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

        bool useAilist2; // TODO: (?), correct?
    };

    class Chrono
    {
    public:
        class Tile
        {
        public:
            class SelectObject
            {
            public:
                int ident = -1;

                QList<QString> relabels;

                bool isDeleted;
                QString type; // optional new path for an object
            };

            class SelectSpline
            {
            public:
                int ident = -1;

                QList<OCMap::Tile::Spline::Rule> rules;

                bool isDeleted;
                QString type; // optional new path for an spline
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
        QList<Tile> tiles;
        bool hasAilist; // ailists_#upd.txt
        bool hasAilistLow; // ailists_#low_#upd.txt

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

    class Timezone
    {
    public:
        class DaylightSavingTime
        {
        public:
            QDate start; // with hours!
            QDate end; // with hours!
            int changeHours;
        };

        int timezone;
        OC2DCoordinates<float> location;

        QList<DaylightSavingTime> daylistSavingTimes;
    };

    class CarUse
    {
    public:
        class TypeTour
        {
        public:
            QString vehicleIdent; // number or path to vehicle file
            QString tour;

            // if (typesPrefered) {
            float probability;
            // }
        };

        QString filename;

        QDate start;
        QDate end;

        QString line;
        QList<TypeTour> numberTours;
        QList<TypeTour> typeTours;
        QList<TypeTour> typesPrefered;
        QList<QString> onlyTypes;

    };

    Global global;
    Timezone timezone;
    QList<CarUse> carUses;
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
    bool hasAilist; // ailists.txt
    bool hasAilistLow; // ailists_#low.txt
};

class OCMoney { // *.cfg
public:
    class moneyPart
    {
    public:
        QString modelFilename; // relative to CURRENT folder (without model folder!)
        float value;
    };

    QString name;
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

    OCOptionalValue<int> screenRatio;

    bool noTextLogHighSwitch; // e.g. in 'PC 2009.oop' as '[no_tex_low_high_switch]' // TODO: ?

    bool trackIRActive;
    bool gameControlerActive;
    bool noAutoSave;
};

class OCPlugin // *.opl
{
public:
    QString dllName;
    QList<QString> varnames;
    QList<QString> stringvarnames;
    QList<QString> systemvarnames;
    QList<QString> triggers;

};

class OCSceneryobject
{
public:
    class Tree
    {
    public:
        QString texture;
        float minHeight;
        float maxHeight;
        float minRatio;
        float maxRatio;
    };

    class Maplight
    {
    public:
        OC3DCoordinates<float> position;
        QColor color;
        float maxBrightnessRadius = -1;
    };

    class Triggerbox
    {
    public:
        OC3DBox<float> geometry;

        float reverbTime = -1; // in s; max: 3
        float transition = -1; // in m, means "softness" when a vehicle goes in / out of the triggerbox
    };

    class TrafficLightGroup
    {
    public:
        class TrafficLight
        {
        public:
            class Phase
            {
            public:
                int currentPhaseIdent = 12; // 12: off
                float duration;
            };

            QString name;
            QList<Phase> phases;
            float approachDistance = -1; // should not be 0!
        };

        class TrafficLightJumpStop
        {
        public:
            int checkOnApproachIdent;
            float jumpTimePosition;
            bool jumpIfNoApproach;
            float jumpToTime = -1; // mandatory for [traffic_light_jump] {if!=-1} or [traffic_light_stop] {if=-1}!
        };

        float duration;
        QList<TrafficLight> trafficLights;
        QList<TrafficLightJumpStop> jumpStops;
    };

    class SplineHelper
    {
    public:
        OC3DCoordinates<float> position; // Attention! Inverted: xzy
        float rotation; // can be bigger as 360
        float splineHelper5thValue; // TODO: ?
        float splineHelper6thValue; // TODO: ?
    };

    class Path
    {
    public:
        OC3DCoordinates<float> position; // not inverted
        float rotation;
        float radius;
        float length = 10;
        float gradientStart;
        float gradientEnd;
        int type; // 0=Street 1=Sidewalk 2=Railroad
        float width;
        int direction; // 0=Forward 1=Reverse/Backward 2=Both
        int blinker; // 0=no 1=Straight 2=Left 3=Right

        int trafficLightIdent = -1;

        bool hasCrossingProblem = false;

        int blockPath1stValue; // TODO: ? |-> Wird vmtl. dafür genutzt, diesen Pfad freizuhalten bzw., dass dort keine KI zum Stehen kommt
        int blockPath2ndValue; // TODO: ? /   s. Sceneryobjects\ADDON_SimpleStreets\Kreuz_MC\Einm_001_001_1.sco - Pfad 8: sogar 4 blockpaths auf einmal.

        // if (path2) {
        int path2_13thValue; // TODO: ?
        int path2_14ndValue; // TODO: ?
        // }

        OCOptionalValue<OCRail::RailEnh> rail;
        OCOptionalValue<OCRail::ThirdRail> thirdRail;

        int switchdir = -1;
    };

    bool isDepot;
    bool isBusstop;
    bool isCarPark;
    bool isPetrolStation;
    bool isEntrypoint;
    bool isHelpArrow;
    bool isSignal;
    bool isTrafficLight;

    bool usesAbsoluteHeight;
    bool lightMapMapping;
    bool noCollision;
    QList<QString> collisionMeshes;
    bool noMapLighting;
    bool isJoinable;
    bool isFixed;
    QString rendertype;

    int complexity = -1;

    OCOptionalValue<OC2DCoordinates<float>> crashModePole;

    OCOptionalValue<OC3DCoordinates<float>> centerOfGravity;

    QString crossingHeightDeform;
    QString terrainHole;

    bool onlyVisibleInEditor;
    bool isSurface;

    OCOptionalValue<Tree> tree;

    QList<Triggerbox> triggerboxes;

    QList<TrafficLightGroup> trafficLightGroups;
    QList<SplineHelper> splineHelpers;
    QList<Path> paths; // TODO: see Template class class

    int switchDirectionCount = -1; // e.g.: single / 'simple' switch: 2  -  [switch]

    OCViewable object;
};

class OCSituation { // .osn
public:
    // For some situations there may be a weather file: [situationFileNameWithSuffix].owt

    class VehicleInformation
    {
    public:
        QString vehiclePath;
        OC3DCoordinates<float> position;
        float vehicle5thValue; // TODO: ?
        float vehicle6thValue; // TODO: ?
        float vehicle7thValue; // TODO: ?
        float vehicle8thValue; // TODO: ?
        float vehicle9thValue; // TODO: ?
        float vehicle10thValue; // TODO: ?
        float vehicle11thValue; // TODO: ?
        float vehicle12thValue; // TODO: ?
        int vehicle13thValue; // TODO: ?
        int vehicle14thValue; // TODO: ?
        QString hofFileName;

        bool isMyVehicle;

        QList<OCVariableDeclaration<float>> variables; // There are also variables with '' as name. - do not save int values (e.g. 1) as float (e.g. 1.0)! - [vars]
        QList<OCVariableDeclaration<QString>> stringvariables; // see 1st on member 'variables' - [stringvars]

        int coupledWith; // TODO: -1 actually in use?
    };

    class TimeTableInformation
    {
    public:
        QString line;
        QString tour;
        int settimetable3rdValue; // TODO: ?
        int settimetable4thValue; // TODO: ?
        int settimetable5thValue; // TODO: ?
        int settimetable6thValue; // TODO: ?
    };

    QString name;
    QString description;

    QString globalPath;
    QDateTime time; // year[int], daysFrom01Jan[int], hoursFromMidnight[int], minutesFromFullHour[int], secondsFromFullMinute[float]

    OC2DCoordinates<int> centerTile;
    OCMapPosition mapcam; // tilePos not used here! See 'centerTile' member - Attention: Inverted! xzy
    OCMapPosition egoPosition; // tilePos not used here! See 'centerTile' member - std: x=10;y=10 - attention: Inverted! xzy

    bool icaoWeatherActive;

    QList<VehicleInformation> vehicles;

    bool ttActive; // TODO: ?
    TimeTableInformation timeTableInformation;

    int myVehicle; // for OMSI itself, this variable has not any real sense
    int view; // TODO: / values maybe: 0=driver 1=passengers 2=outsideVehicle 3=outsideFull
};



class OCSpline
{
public:
    class HeightProfile
    {
    public:
        float leftXPosition;
        float rightXPosition;
        float leftZPosition;
        float rightZPosition;
    };

    class Texture
    {
    public:
        class PatchworkChain
        {
        public:
            float length;
            QString transitions;
            QString frequencies;
            QString mirroring;

            /* [patchwork_chain]
             * 10
             * AABAABAAA            (segmentCount+1 is correct here. see wiki entry)
             * 11211114
             * 11011011
            */
        };

        QString textureName;
        QString materials = ""; // TODO: Material object - TODO: ALL matl operations possible?
        OCOptionalValue<PatchworkChain> patchworkChain;
        bool scaleTextureByLength;
    };

    class Profile
    {
    public:
        class ProfilePoint
        {
        public:
            float xPosition;
            float zPosition;
            float xPositiononTexture;
            float stretchFactor;
        };

        int textureIdent = -1;
        QList<ProfilePoint> profilePoints;
    };

    class TerrainHoleProfile
    {
    public:
        // TODO: maximum of 2 profilepoints?
        QList<OC3DCoordinates<float>> points;
    };

    class Path
    {
    public:
        int type = -1; // 0=Street 1=Sidewalk 2=Railroad
        float xPosition;
        float zPosition;
        float width;
        int direction = -1; // 0=Forward 1=Reverse/Backward 2=Both

        OCOptionalValue<OCRail::RailEnh> rail;
        OCOptionalValue<OCRail::ThirdRail> thirdRail;

        float path6thValue; // TODO: ?
    };

    int halfCantWidth = -1;

    bool onlyVisibleInEditor;
    QString rendertype; // see OCSceneryobject

    QList<HeightProfile> heightProfiles;
    QList<Texture> textures;
    QList<Profile> profiles;
    QList<TerrainHoleProfile> terrainHoleProfiles; // Attention! Inverted: xzy
};

class OCTicketPack
{
public:
    class SingleTicketPack
    {
    public:
        class Ticket
        {
        public:
            bool isTicket2;
            QString nameDeu;
            QString nameEng;
            int maxStops;
            int minimumAge;
            int maximumAge;
            float price;
            QString displayText;

            // if(isTicket2) {
            bool isDayTicket;
            float maxTicketPurchaseFactor = -1;
            // }
        };

        float devaluationFactor = -1;
        float PurchaseFactor = -1;
        float talkFactor = -1;
        float complainFactor = 1;

        QList<Ticket> tickets;
    };

    QString voicePath;
    QList<SingleTicketPack> ticketPacks;
};

class OCTrain
{
public:
    class TrainPart
    {
    public:
        QString trainPartPath;
        int zugFile2thValue; // TODO: ?
    };

    QList<TrainPart> trainParts;
};

class OCVehicle // bus, ovh
{
public:
    class View
    {
    public:
        enum Type
        {
            driver = 1,
            passenger = 2,
            reflexion = 3
        };

        Type type;
        OC3DCoordinates<float> position;
        float distanceFromOrigin;
        int perspective;
        float xRotation;
        float zRotation;

        // if (driverView) {
        bool isScheduleView;
        bool isTicketSellingView;
        // }

        // if (isReflexion) {
        bool isExtendedReflexion;
        // }
    };

    class Axis
    {
    public:
        class Method
        {
        public:
            enum AxisIdent
            {
                achse_long = 1,
                achse_maxwidth = 2,
                achse_minwidth = 3,
                achse_raddurchmesser = 4,
                achse_feder = 5,
                achse_maxforce = 6,
                achse_daempfer = 7,
                achse_antrieb = 8
            };

            int type;
            QVariant value;
        };

        QList<Method> methods;
    };

    class CouplingDefintion
    {
    public:
        class FrontCharacter
        {
        public:
            float alpha = -1; // Kickwinkel rechts-links
            float betaMin; // Knickwinkel Ri. unten
            float betaMax; // Knickwinkel Ri. oben
            float type; // 0=Truck (three degrees of freedom); 1=Bus (two degrees of freedom)
        };

        OCOptionalValue<OC3DCoordinates<float>> couplingFront; // coupling_front
        QString couplingFrontFile;  // couple_front
        bool couplingFrontIsReversed; // ...

        OCOptionalValue<FrontCharacter> frontCharacter;

        bool couplingFrontIsOpenForSound; // couple_front_open_for_sound

        OCOptionalValue<OC3DCoordinates<float>> couplingBack; // coupling_back
        QString couplingBackFile;   // couple_back
        bool couplingBackIsReversed; // ...
    };

    class Distance
    {
    public:
        int constructionYear;
        int kilometersPerYear;
    };

    class VehicleOscillating
    {
    public:
        float distanceBetweenBoogieAndCog; // z axis
        OC3DCoordinates<int> frequencies; // values: 2nd, 4th, 6th
        OC3DCoordinates<int> damping; // values: 3rd, 5th, 7th
    };

    class Registration
    {
    public:
        enum Method
        {
            automatic = 1,
            list = 2,
            free = 3
        };

        Method method;

        QString file;
        QString prefix;
        QString postfix;
    };

    class Sinus
    {
    public:
        float wheelRadius;
        float wheelDistance = 1.5;
        float wheelTaping;
        float damping;
    };

    class AiBrakePerformance
    {
    public:
        float meanBrakeDeceleration = 1.0;
        float rowdyFactorVariation = 0.2;
        float frictionUse = 0.3;
        float maxBreake = 0.2;
        float holingPointOffset = 0;
    };

    class ContactShoe // see Vehicles\A3\A3_64-66_K.ovh
    {
    public:
        enum Position
        {
            front = 0,
            rear = 1
        };

        Position position;
        float xMin;
        float xMax;
        float zMin;
        float zMax;
        int type; // +1=topContactRail +2=BottomContactRail +4=SideContactRail
    };

    class ControlCable // see Vehicles\A3\A3_64-66_K.ovh
    {
    public:
        enum Position
        {
            front = 1,
            back = 0
        };

        Position position;
        QString couplingPosition;
        int cablePosition;
        QString variabelRead;
        QString variableWrite;
        QString brokeCableVariable;
    };

    QList<View> driverViews;
    QList<View> passengerViews;

    int standardDriverView = -1;
    OCOptionalValue<OC3DCoordinates<float>> outsideCameraPosition;

    QList<View> reflexions;

    QList<OCViewable::Attachment> ticketAttachments;

    OCOptionalValue<float> gravityZPosition;
    OCOptionalValue<int> rollingResistance;
    OCOptionalValue<float> yRotationPoint;
    OCOptionalValue<float> steeringRadius; // [inv_min_turnradius] OR (trains) [boogie]?
    OCOptionalValue<float> aiDeltaHeight;
    OCOptionalValue<AiBrakePerformance> aiBrakePerformance;
    OCOptionalValue<int> aiVehicleType; // 0=passengerCar 1=cab 2=bus 3=truck

    QList<Axis> axes;

    CouplingDefintion coupling;

    OCOptionalValue<Distance> Distance;

    OCOptionalValue<OC2DCoordinates<float>> rowdyFactor;

    OCOptionalValue<VehicleOscillating> railBodyOscillating;

    OCViewable object;
    bool scriptsharing;

    int type = -1; // TODO: ? - maybe see aiVehicleType (this class)

    QString numberFile;
    OCOptionalValue<Registration> registration;

    OCOptionalValue<Sinus> sinus;

    QList<ContactShoe> contactShoes;
};

class OCWeather
{
public:
    class Fog
    {
    public:
        int range;
        int viewDistance; // 50000=unlimited
    };

    class Wind
    {
    public:
        float direction;
        float force;
    };

    class Temperature
    {
    public:
        float temperature;
        float dewPoint;
    };

    class Cloud
    {
    public:
        QString name;
        float height;
    };

    class Precip
    {
    public:
        enum Type
        {
            none = 0,
            rain = 1,
            snow = 2
        };

        Type type;
        int intensity;
        int Precip3rdValue; // TODO: ?
        float Precip4thValue; // TODO: ?
        int Precip5thValue; // TODO: ?
    };

    class Groundwet
    {
    public:
        // TODO: ?
    };

    QString name;
    QString description;

    OCOptionalValue<Fog> fog;
    OCOptionalValue<Wind> wind;
    OCOptionalValue<Temperature> temperature;
    OCOptionalValue<float> pressure;
    OCOptionalValue<Cloud> clouds;
    OCOptionalValue<Precip> precip;

    OCOptionalValue<Groundwet> groundwet;

    bool snowOnRoad;
    bool snow;
};

// car_use (Spandau)
// TTData

#endif // OCC_H
