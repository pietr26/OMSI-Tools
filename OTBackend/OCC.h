#ifndef OCC_H
#define OCC_H

#include "OTBackend/OTGlobal.h"

class OCBase
{
public:
    // TODO: Copy from OTGlobal[OTFileOperations] - move all OMSI-related stuff to here
    /// Returns an universal file header
    QString writeFileHeader() { return "File created with " + OTInformation::name + " " + OTInformation::versions::currentVersion.first + " on " + misc.getDate() + ", " + misc.getTime() + "\n\n"; }

private:
    OTMiscellaneous misc;
};

class OCNothing
{
    // just a placeholder for any unknown fields.
};

template<class T>
class OC2DCoordinates
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    OC2DCoordinates() { }
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
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    OC2DCoordinatesSide() { }
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
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    OC3DCoordinates() { }
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
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    OC3DBox() { }
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

class OCFile // Class for all classes with [optional] (directly) readable and/or writeable files - inherit as public!
{
public:
    enum FileIOResponse
    {
        errFunctionNotDefined = -3,
        errFileNotOpen = -2,
        errFileDoesntExist = -1,
        valid = 0,
        errMinor = 1,
        errCritical = 2
    };

    void processFileIOResponse(FileIOResponse response, QString filename = "", QString errorString = "")
    {
        switch (response)
        {
            default: qDebug() << "FileIO response: No response code given."; break;

            case -3: qWarning() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": Virtual function is not defined."; break;
            case -2: qWarning() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": Cannot open file."; break;
            case -1: qWarning() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": File does not exist."; break;
            case 0: qInfo() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + " successful."; break;
            case 1: qWarning() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": Minor error" + ((!errorString.isEmpty()) ? " (" + errorString + ")": ""); break;
            case 2: qCritical() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": Minor error" + ((!errorString.isEmpty()) ? " (" + errorString + ")": ""); break;
        }
    }

    QPair<FileIOResponse, QString> generateErrPair(FileIOResponse responseCode, QString errorString) { return QPair<FileIOResponse, QString>(responseCode, errorString); }

    virtual FileIOResponse read() { return FileIOResponse::errFunctionNotDefined; } // override (pls mark). For error code information take a look in source class
    virtual FileIOResponse write() { return FileIOResponse::errFunctionNotDefined; } // override (pls mark). For error code information take a look in source class

    virtual void clear() { } // override (pls mark).

    QList<QPair<FileIOResponse, QString>> errors; // TODO: error stack for minor errors while last reading / writing process - maybe use another template class for list
};

class OCTextureChangeMaster
{
public:
    QString filename;
    QString variable; // int variable!

    QList<QString> entries;
};

class OCModel : public OCFile // cfg
{
public:
    // If e.g. particle_emitter is used before all [mesh] entries, create an emtpy material - TODO: "OPTIONAL IN MESH" section in txt :-)
    // in Mesh[0] are general properties without any binding

    class Mesh
    {
    public:
        class Material
        {
        public:
            class Properties
            {
                // This props are changable with [matl_change] command. TODO: Check them!
            public:
                class Allcolor
                {
                public: // all: rgb
                    QColor diffuse; // also Alpha value here!
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

            QList<Properties> properties;
        };

        class Animation
        {
        public:
            class Method
            {
            public:
                QString variable;
                float factor;
            };

            OCOptionalValue<OC3DCoordinates<float>> originTransform;
            OCOptionalValue<OC3DCoordinates<int>> originRotation; // TODO: Does this makes sense? :')

            OCOptionalValue<Method> animationTransform;
            OCOptionalValue<Method> animationRotation;
            OCOptionalValue<float> maxSpeed;
            OCOptionalValue<float> delay;
            OCOptionalValue<float> offset;

            int internalIdent = -1;
        };

        class Visible
        {
        public:
            QString variable;
            QString value; // With this value, the object is visible

            int internalIdent = -1;
        };

        class Bone
        {
        public:
            QString name;
            int property;

            int internalIdent = -1;
        };

        class InteriorLight
        {
        public:
            QString variable;
            float distance; // from origin to 63% of power
            QColor color;
            OC3DCoordinates<float> position;
        };

        class Smoke  // TODO: Explore different values
        {
        public:
            OC3DCoordinates<QVariant> position;
            OC3DCoordinates<QVariant> outflowPosition;

            QVariant speed;
            QVariant speedVaration;
            QVariant frequence;
            QVariant lifeTime;
            QVariant brakeFactor;
            QVariant startSize;
            QVariant enlargementRate;
            QVariant alpha;
            QVariant alphaVariation;
            QColor color;
        };

        class ParticleEmitter // TODO: Explore different values
        {
        public: // [Might be a variable name!]
            class MethodMinMax
            {
            public:
                QVariant min;
                QVariant max;
            };

            class Color
            {
            public:
                QColor initial;
                QColor final; // TODO: ? - Test!
            };

            class Bitmap
            {
            public:
                QString bitmap;
                bool hasAlpha;
            };

            class InstantExplosionPartCount
            {
            public:
                QVariant frequency;
                QVariant directExplode;
            };

            class AttachTo
            {
            public:
                int attach;
                int attachGroup;
                int attachMulti;
            };

            OC3DCoordinates<QVariant> position;
            OC3DCoordinates<QVariant> outflowPosition;

            OCOptionalValue<MethodMinMax> velocity;
            OCOptionalValue<MethodMinMax> constantVelocity;
            OCOptionalValue<MethodMinMax> frequence;
            OCOptionalValue<MethodMinMax> liveTime;
            OCOptionalValue<QVariant> emitterLifeTime; // float
            OCOptionalValue<MethodMinMax> brakeFactor;
            OCOptionalValue<MethodMinMax> weightForce; // 0=no gravity
            OCOptionalValue<MethodMinMax> sizeStart;
            OCOptionalValue<MethodMinMax> sizeFinal;
            OCOptionalValue<MethodMinMax> alphaInitial;
            OCOptionalValue<MethodMinMax> alphaFinal;
            OCOptionalValue<Color> color; // rgb and hsv!
            OCOptionalValue<int> calculateDistance;
            OCOptionalValue<QVariant> isEmissive; // bool
            OCOptionalValue<Bitmap> bitmap;
            OCOptionalValue<QVariant> waitForParentDeath; // bool
            OCOptionalValue<InstantExplosionPartCount> instantExplosionPartCount;

            OCOptionalValue<AttachTo> attachTo; // see 'C 005F6DA4 TRauch.attachedto:Integer'
        };

        class Light
        {
        public:
            QString variable;
            float size;
            QColor color;
            OC3DCoordinates<float> position;
            /* [light]
             * <Variable>
             * <Size>
             * <RColor>
             * <GColor>
             * <BColor>
             * <xPos>
             * <yPos>
             * <zPos>
            */
        };

        class LightEnh
        {
        public:
            enum Rotation
            {
                onlyVector = 0,
                fromVectorToCamera = 1,
                allDirections = 2
            };

            OC3DCoordinates<float> position; // 0=0° 1=90° 2=180°
            OC3DCoordinates<float> direction; // 0=0° 1=90° 2=180°
            OC3DCoordinates<float> lightCone; // 0=0° 1=90° 2=180°
            bool lightInAllDirections;
            Rotation rotation;
        };

        class LightEnh2 : public LightEnh
        {
        public:
            QColor color;
            float size; // in m
            int innerlightConeAngle;
            int outerlightConeAngle;
            QString activiationVariable;
            QString brightnessVariable;
            int brightnessFactor;
            float playerOffset;
            int effect; // 0=none +1=stars +2=noFog +4=both
            bool hasLightCone;
            float delay; // in s
            QString effectTexture;
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

        int viewpoint = -1; // 0 equals 7   |   +1=userVehicleInside +2=userVehicleInside +4=aiVehicle

        QList<int> illuminationInterior; // min=4 max=4 - fill empty places with '-1'!

        QList<InteriorLight> interiorLights;

        QList<Smoke> smokes;
        QList<ParticleEmitter> particleEmitters;

        float lodFactor = -1;
        float detailFactor = -1;
        float texDetailFactor = -1;

        QList<Light> lights;
        QList<LightEnh> lightEnhs;
        QList<LightEnh2> lightEnh2s;
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

    class Spotlight
    {
    public:
        OC3DCoordinates<float> position;
        OC3DCoordinates<float> direction;
        QColor color;
        float distance;
        int innerlightConeAngle;
        int outerlightConeAngle;
    };

    QList<Mesh> meshes;

    QList<TextTexture> textTextures;
    QList<OC2DCoordinates<int>> scriptTextures;
    OCOptionalValue<OC3DBox<float>> viewbox; // [VFDmaxmin] - ATTENTION: Some calculations are necessary to fit values in the class! - values: 1=xMinPos 2=yMinPos 3=zMinPos 4=xMaxPos 5=yMaxPos 6=zMaxPos
    QList<ComplexTextureChange> complexTextureChanges;

    QList<Spotlight> spotlights;
};

class OCSound : public OCFile // in sound.cfg
{
public:
    class ThreeDimensionPosition
    {
    public:
        OC3DCoordinates<float> position;
        float maxIntensityRadius;
    };

    class Volcurve
    {
    public:
        QString variable;
        QList<OC2DCoordinates<float>> points;
    };

    template<class T>
    class Condition
    {
    public:
        enum Operation
        {
            notEqual = 0,
            equal = 1,
            lessThan = 2,
            greaterThan = 3,
            lessThanEqual = 4,
            greaterThanEqual = 5
        };

        static_assert(std::is_same_v<T, float> || std::is_same_v<T, int> || std::is_same_v<T, bool> || std::is_same_v<T, QVariant>, "The type T must be either float, int or bool.");
        QString variable;
        T value;
        Operation operation;
    };

    bool isLoopSound;
    float volume;
    // if (isLoopSound) {
    int sampleRate;
    QString pitchVariable;
    QString pitchVariableFactor; // TODO: Correct name?

    bool checkLoading;
    // }

    OCOptionalValue<ThreeDimensionPosition> position;

    int viewpoint = -1; // 0 equals 7   |   +1=userVehicleInside +2=userVehicleInside +4=aiVehicle

    // if (!isLoopSound) {
    bool noLoop;
    QString trigger;
    bool onlyOne; // TODO: ?
    // }

    // if (isLoopSound) {
    QList<Volcurve> volcurve;
    OCOptionalValue<Condition<QVariant>> condition;
    // }

    QVariant dir; // TODO: ? - direction?

    OCNothing nextRandom; // TODO: ?
};

class OCPassengerCabin : public OCFile // cfg
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

class OCScript : public OCFile // osc
{
public:
    class Action
    {
    public:

    };

    class Variable : public Action
    {
    public:

    };

    class Number : public Action
    {
    public:

    };

    class Container
    {
    public:
        QList<Action> actions;
    };

    // void test()
    // {
    //     Container tmp;
    //     Variable var;
    //     Number num;
    //     tmp.actions.append(var);
    // }

    QList<Container> script;
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

class OCViewable : public OCFile // Sceneryobjects, Vehicles, Humans
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

class OCTextureProfile : public OCFile // [textureName].[textureSuffix].cfg
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

class OCDriver : public OCFile // odr
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

class OCFont : public OCFile // oft
{
public:
    class SingleFont
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
        int maxHeightOfChars = -1; // TODO: '-1'?
        int distanceBetweenChars = -1; // TODO: '-1'?

        QList<Character> characters;
    };

    QList<SingleFont> fonts;
};

class OCHuman : public OCFile // hum
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

class OCGameControler : public OCFile // cfg
{
public:
        // TODO
};

class OCKeyboard : public OCFile // cfg
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

class OCTranslation : public OCFile // dsc
{
public:
    // TODO: Embed in other classes?
    QString name;
    QString description;
};

class OCLanguage : public OCFile // olf
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

/** @brief viewport position on a map
 * <hr>
 *   - Occurs in file(s): global.cfg, laststn.osn*/
class OCMapPosition
{
public:
    /** @brief tile where the view is located
 * <hr>
 *   - Occurs with other parameters: <code>true</code>
 *   - Multiple occurrences: <code>false</code>*/
    OC2DCoordinates<int> tilePosition;

    /** @brief position on the tile
 * <hr>
 *   - Occurs with other parameters: <code>true</code>
 *   - Multiple occurrences: <code>false</code>*/
    OC3DCoordinates<float> position;

    /** @brief rotation around global z axis
 * <hr>
 *
 * Attention: These values are inverted! Order: x, z
 *
 *   - Occurs with other parameters: <code>true</code>
 *   - Multiple occurrences: <code>false</code>*/
    float rotAroundZ;

    /** @brief rotation around real x axis
 * <hr>
 *   - Occurs with other parameters: <code>true</code>
 *   - Multiple occurrences: <code>false</code>*/
    float rotAroundX;

    /** @brief distance from position
 * <hr>
 *   - Occurs with other parameters: <code>true</code>
 *   - Multiple occurrences: <code>false</code>*/
    float distanceFromPosition;
};

class OCMap { // TODO: override clear function
public:
    /** @brief <code>global.cfg</code> - Defines global settings for a map.
     * <hr>
     *   - Occurs in file(s): <code>OMSI 2\maps\*\global.cfg</code>*/
    class Global : public OCFile // global.cfg
    {
    public:
        /** @brief <code>[groundtex]</code> - single ground texture
             * <hr>
             *   - Occurs in file(s): parent
             *   - Line: <code>0 - 5</code>
             *   - Occurs with other parameters: <code>false</code>
             *   - Multiple occurrences: <code>true</code>*/
        class Texture
        {
        public:
            /** @brief <code>[groundtex]</code> - main texture path (from main dir)
             * <hr>
             *   - Line: <code>1</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            QString mainTex;

            /** @brief <code>[groundtex]</code> - sub texture path
             * <hr>
             *   - Line: <code>2</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code> */
            QString subTex;

            /** @brief <code>[groundtex]</code> - exponent for squared texture size
             *
             * 2<sup>x</sup>
             *
             * Example: x = 3 --> 2<sup>3</sup>=8 --> texture size: 8x8px
             * <hr>
             *   - Line: <code>3</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            int texSizeExponent;

            /** @brief <code>[groundtex]</code> - main texture repeating count
             * <hr>
             *   - Line: <code>4</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            int mainTexRepeating;

            /** @brief <code>[groundtex]</code> - sub texture repeating count
             * <hr>
             *   - Line: <code>5</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            int subTexRepeating;
        };

        /** @brief <code>[addseason]</code> - single season
        * <hr>
        *   - Occurs in file(s): parent
        *   - Line: <code>0 - 3</code>
        *   - Occurs with other parameters: <code>false</code>
        *   - Multiple occurrences: <code>true</code>*/
        class Season
        {
        public:
            enum Type
            {
                summer = 0,
                spring = 1,
                autumn = 2,
                winter = 3,
                deepWinter = 4
            };

            QString name()
            {
                switch (type)
                {
                    case 1: return QObject::tr("Spring");
                    case 2: return QObject::tr("Autumn");
                    case 3: return QObject::tr("Winter");
                    case 4: return QObject::tr("Deep winter");
                    default: return QObject::tr("Summer");
                }
            }

            /** @brief <code>[addseason]</code> - defines a season
             *
             * <code>1</code>: Spring
             * <code>2</code>: Autumn
             * <code>3</code>: Winter
             * <code>4</code>: Deep winter (with snow)
             * <hr>
             *   - Line: <code>1</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            Type type;

            /** @brief <code>[addseason]</code> - defines first day of season (1-based to 1st January)
             * <hr>
             *   - Line: <code>2</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            QDate start;

            /** @brief <code>[addseason]</code> - defines last day of season (1-based to 1st January)
             * <hr>
             *   - Line: <code>3</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            QDate end;
        };

        QList<Season> sortSeasons(QList<Season> seasons)
        {
            QList<Season> list = seasons;

            std::sort(list.begin(), list.end(), [](const Season &a, const Season &b) {
                    return a.start.dayOfYear() < b.end.dayOfYear();
                });
            return list;
        }

        /** @brief <code>[trafficdensity_road]</code>, <code>[trafficdensity_passenger]</code> - single traffic density
             * <hr>
             *   - Occurs in file(s): parent
             *   - Line: <code>0 - 2</code>
             *   - Occurs with other parameters: <code>false</code>
             *   - Multiple occurrences: <code>true</code>*/
        class AiDensity
        {
        public:
            /** @brief <code>[trafficdensity_road]</code> - decimal time
             * <hr>
             *   - Line: <code>1</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            QTime time;

            /** @brief <code>[trafficdensity_road]</code> - density multiplier
             * <hr>
             *   - Line: <code>2</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            float factor;
        };

        /** @brief <code>[entrypoints]</code> - defines all entrypoints - TODO
             * <hr>
             *   - Occurs in file(s): parent
             *   - Line: <code>0 - ({1} * entrypointCount)</code>
             *   - Occurs with other parameters: <code>false</code>
             *   - Multiple occurrences: <code>false</code>*/

        // Wrap class for entrypoints
        class EntrypointCollection
        {
        public:
            class Entrypoint
            {
            public: // TODO
                /* [entrypoints]
             * entrypointCount      * (
             * objectID
             * ?
             * xpos
             * zpos            Attention! Inverted
             * ypos
             * ?        \
             * ?        |  maybe       | very awkward dependencies
             * ?        |  irrelevant  | with rot, pitch and bank
             * ?        /
             * tileID  // see order in global.cfg
             * name
             * )
             */

                // N: zero-based for iterator (to entrypointCount)

                /** @brief <code>[entrypoints]</code> - tile-local objectID (based on position of entry in map file)
             * <hr>
             *   - Line: <code>(2 + 11 * n)</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                int objectID;

                /** @brief <code>[entrypoints]</code> - global ID of all sco / sli
             * <hr>
             *   - Line: <code>(3 + 11 * n)</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                int globalThingID;

                /** @brief <code>[entrypoint]</code> - awkwardValue1
             * <hr>
             *   - Line: <code>(4 + 11 * n)</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                int awkwardValue1;

                /** @brief <code>[entrypoints]</code> - defines position of entrypoint
             * <hr>
             *
             * Attention: These values are inverted! Order: x, z, y
             *
             *   - Line: <code>(5 + 11 * n), (6 + 11 * n), (7 + 11 * n)</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                OC3DCoordinates<float> position;

                /** @brief <code>[entrypoint]</code> - awkwardValue2
             * <hr>
             *   - Line: <code>(8 + 11 * n)</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                float awkwardValue2;

                /** @brief <code>[entrypoint]</code> - awkwardValue3
             * <hr>
             *   - Line: <code>(9 + 11 * n)</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                float awkwardValue3;

                /** @brief <code>[entrypoint]</code> - awkwardValue4
             * <hr>
             *   - Line: <code>(10 + 11 * n)</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                float awkwardValue4;

                /** @brief <code>[entrypoint]</code> - awkwardValue5
             * <hr>
             *   - Line: <code>(11 + 11 * n)</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                float awkwardValue5;

                /** @brief <code>[entrypoints]</code> - tile ID - zero-based on [map] ordner in global.cfg
             * <hr>
             *   - Line: <code>(12 + 11 * n))</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
                int tileID;
            };
            /** @brief <code>[entrypoints]</code> - entrypoint name
             * <hr>
             *   - Line: <code>(13 + 11 * n))</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>true</code>*/
            QString name;

            QList<Entrypoint> entrypoints;
        };


        /** @brief <code>[backgroundimage]</code> - sets background image for tiles in editor
         * <hr>
         *   - Occurs in file(s): parent
         *   - Line: <code>0 - 6</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        class BackgroundImage
        {
        public:
            /** @brief <code>[backgroundimage]</code> - sets background image visible
             *
             *  save as int
             * <hr>
             *   - Line: <code>1</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            bool isVisible;

            /** @brief <code>[backgroundimage]</code> - picture path (absolute)
             * <hr>
             *   - Line: <code>2</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            QString picturePath;

            /** @brief <code>[backgroundimage]</code> - picture width
             * <hr>
             *   - Line: <code>3</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            float width;

            /** @brief <code>[backgroundimage]</code> - picture height
             * <hr>
             *   - Line: <code>4</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            float height;

            /** @brief <code>[backgroundimage]</code> - start width on picture (0.0 to 1.0)
             * <hr>
             *   - Line: <code>5</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            float startWidth;

            /** @brief <code>[backgroundimage]</code> - start height on picture (0.0 to 1.0)
             * <hr>
             *   - Line: <code>6</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            float startHeight;
        };

        /** @brief <code>[map]</code> - single tile information
         *
         *  Attention: Values inverted! 1=y | 2=x
         *
         * <hr>
         *   - Occurs in file(s): parent
         *   - Line: <code>0 - 3</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>true</code>*/
        class TileInformation
        {
        public:
            /** @brief <code>[map]</code> - location
             * <hr>
             *   - Line: <code>1 + 2</code>(x + y)
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            OC2DCoordinates<int> position;

            /** @brief <code>[map]</code> - file name of tile (auto-generated by editor, so pseudo case-sensitive)
             * <hr>
             *   - Line: <code>3</code>
             *   - Occurs with other parameters: <code>true</code>
             *   - Multiple occurrences: <code>false</code>*/
            QString filename;
        };

        /** @brief <code>[name]</code> - name of map (not in use)
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        QString name;

        /** @brief <code>[friendlyname]</code> - friendlyname of map (usually in use)
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        QString friendlyname;

        /** @brief <code>[description]</code> - description of map
         * <hr>
         *   - Line: <code>1</code> to arg <code>[end]</code
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        QString description;

        /** @brief <code>[version]</code> - version identifier (case sensitive)
         *
         *  value for latest OMSI version: 14
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        int version;

        /** @brief <code>[NextIDCode]</code> - next ID code for sceneryobjects / splines in editor (case-sensitive)
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        unsigned int nextIDCode;

        /** @brief <code>[worldcoordinates]</code> - enables aerials with world coordinates
         * <hr>
         *   - Line: <code>0</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        bool worldCoodinates;

        /** @brief <code>[dynhelperactive]</code> - enables dynamich help arrows
         * <hr>
         *   - Line: <code>0</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        bool dynHelpers;

        /** @brief <code>[LHT]</code> - enables semi left-side driving
         * <hr>
         *   - Line: <code>0</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        bool lht;

        /** @brief <code>[realrail]</code> - better support for rail vehicles
         * <hr>
         *   - Line: <code>0</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        bool realrail;

        /** @brief <code>[backgroundimage]</code> - sets background image for editor
         * <hr>
         *   - Line: <code>0+</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        BackgroundImage bgImage;

        /** @brief <code>[mapcam]</code> - set default view (no entrypoint selection on map load)
         * <hr>
         *   - Line: <code>0+</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        OCMapPosition standardView;

        /** @brief <code>[moneysystem]</code> - sets money system (from main dir)
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        QString currency;

        /** @brief <code>[ticketpack]</code> - sets ticket pack (from main dir)
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        QString ticketpack;

        /** @brief <code>[repair_time_min]</code> - sets minimal repair time
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        QTime repairTime;

        /** @brief <code>[years]</code> - sets first year for random date generation on load
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>true</code>
         *   - Multiple occurrences: <code>false</code>*/
        int startYear;

        /** @brief <code>[years]</code> - sets last year for random date generation on load
         * <hr>
         *   - Line: <code>2</code>
         *   - Occurs with other parameters: <code>true</code>
         *   - Multiple occurrences: <code>false</code>*/
        int endYear;

        /** @brief <code>[realyearoffset]</code> - used as offset when pressing "real year" button on map load
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        int realYearOffset;

        /** @brief <code>[standarddepot]</code> - string of AI group name to select the corrent HOF file on use vehicle load
         * <hr>
         *   - Line: <code>1</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        QString standardDepot;

        /** @brief <code>[groundtex]</code> - ground textures for terrain painting
         *
         *  The order of the entries is <b>very important</b>.
         * <hr>
         *   - Line: <code>0+</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>true</code>*/
        QList<Texture> groundTextures;

        /** @brief <code>[addseason]</code> - defines season start and end dates
         * <hr>
         *   - Line: <code>0+</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>true</code>*/
        QList<Season> seasons;

        /** @brief <code>[trafficdensity_road]</code> - defines traffic density for vehicles (ascending by time)
         * <hr>
         *   - Line: <code>0+</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>true</code>*/
        QList<AiDensity> trafficDensities;

        /** @brief <code>[trafficdensity_road]</code> - defines traffic density for humans (ascending by time)
         * <hr>
         *   - Line: <code>0+</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>true</code>*/
        QList<AiDensity> passengerDensities;

        /** @brief <code>[entrypoints]</code> - defines entrypoints (all-in-one param)
         * <hr>
         *   - Line: <code>0+</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>false</code>*/
        QList<EntrypointCollection> entrypoints;

        /** @brief <code>[map]</code> - defines tiles and its locations
         * <hr>
         *   - Line: <code>0+</code>
         *   - Occurs with other parameters: <code>false</code>
         *   - Multiple occurrences: <code>true</code>*/
        QList<TileInformation> tiles;

        FileIOResponse read() override
        {
            if (dir.isEmpty())
            {
                qWarning() << "Reading process of Global stopped: No file path given.";
                return FileIOResponse::errFileDoesntExist;
            }

            QFile global(dir + "/global.cfg");

            if (!global.open(QFile::ReadOnly | QFile::Text))
            {
                // msg.fileOpenErrorCloseOMSI(parent, mapFolderPath); TODO
                qDebug().noquote() << "Cannot open file: Full path: '" + QFileInfo(global).absoluteFilePath() + "'";
                return FileIOResponse::errFileNotOpen;
            }

            QTextStream in(&global);
            in.setEncoding(QStringConverter::Latin1);
            QString line = "";

            clear();

            try
            {
                while (!in.atEnd())
                {
                    line = in.readLine();

                    if (line == "[name]") name = in.readLine();
                    else if (line == "[friendlyname]") friendlyname = in.readLine();
                    else if (line == "[description]")
                    {
                        line = in.readLine();
                        description.clear();

                        while (line != "[end]")
                        {
                            description += line +  "\n";
                            line = in.readLine();
                        }

                        description.replace("Ä", "Ae", Qt::CaseSensitive);
                        description.replace("Ö", "Oe", Qt::CaseSensitive);
                        description.replace("Ü", "Ue", Qt::CaseSensitive);
                        description.replace("ä", "ae", Qt::CaseSensitive);
                        description.replace("ö", "oe", Qt::CaseSensitive);
                        description.replace("ü", "ue", Qt::CaseSensitive);
                        description.replace("ß", "ss", Qt::CaseSensitive);

                        description = description.trimmed();
                    }
                    else if (line == "[version]") version = in.readLine().toInt();
                    else if (line == "[NextIDCode]") nextIDCode = in.readLine().toUInt();
                    else if (line == "[worldcoordinates]") worldCoodinates = true;
                    else if (line == "[dynhelperactive]") dynHelpers = true;
                    else if (line == "[LHT]") lht = true;
                    else if (line == "[realrail]") realrail = true;
                    else if (line == "[backgroundimage]")
                    {
                        bgImage.isVisible = in.readLine().toInt(); // 1
                        bgImage.picturePath = in.readLine(); // 2
                        bgImage.width = in.readLine().toFloat(); // 3
                        bgImage.height = in.readLine().toFloat(); // 4
                        bgImage.startWidth = in.readLine().toFloat(); // 5
                        bgImage.startHeight = in.readLine().toFloat(); // 6
                    }
                    else if (line == "[mapcam]")
                    {
                        // Attention: Inverted values
                        int yTilePos = in.readLine().toInt();
                        int xTilePos = in.readLine().toInt();

                        standardView.tilePosition = OC2DCoordinates<int>(xTilePos, yTilePos); // 1, 2

                        // Attention: Inverted values
                        float xPos = in.readLine().toFloat(); // 3
                        float zPos = in.readLine().toFloat(); // 4
                        float yPos = in.readLine().toFloat(); // 5
                        standardView.position = OC3DCoordinates<float>(xPos, yPos, zPos); // 3, 4, 5

                        standardView.rotAroundZ = in.readLine().toFloat();
                        standardView.rotAroundX = in.readLine().toFloat();
                        standardView.distanceFromPosition = in.readLine().toFloat();
                    }
                    else if (line == "[moneysystem]") currency = in.readLine();
                    else if (line == "[ticketpack]") ticketpack = in.readLine();
                    else if (line == "[repair_time_min]")
                    {
                        float decimalTime = in.readLine().toFloat(); int hours = 0;
                        while (decimalTime >= 60) { hours++; decimalTime -= 60; }
                        repairTime = QTime(hours, decimalTime * 60);
                    }
                    else if (line == "[years]")
                    {
                        startYear = in.readLine().toInt();
                        endYear = in.readLine().toInt();
                    }
                    else if (line == "[standarddepot]") standardDepot = in.readLine();
                    else if (line == "[realyearoffset]") realYearOffset = in.readLine().toInt();
                    else if (line == "[groundtex]")
                    {
                        Texture groundtex;
                        groundtex.mainTex = in.readLine();
                        groundtex.subTex = in.readLine();
                        groundtex.texSizeExponent = in.readLine().toInt();
                        groundtex.mainTexRepeating = in.readLine().toInt();
                        groundtex.subTexRepeating = in.readLine().toInt();
                        groundTextures.append(groundtex);
                    }
                    else if (line == "[addseason]")
                    {
                        Season season;

                        int type = in.readLine().toInt();
                        switch (type)
                        {
                            case 1: season.type = Season::Type::spring; break;
                            case 2: season.type = Season::Type::autumn; break;
                            case 3: season.type = Season::Type::winter; break;
                            case 4: season.type = Season::Type::deepWinter; break;
                            default: season.type = Season::Type::summer; break;
                        };

                        season.start = QDate(QDate::currentDate().year(), 1, 1).addDays(in.readLine().toInt());
                        season.end = QDate(QDate::currentDate().year(), 1, 1).addDays(in.readLine().toInt());

                        seasons.append(season);
                    }
                    else if (line == "[trafficdensity_road]")
                    {
                        AiDensity density;
                        float decimalTime = in.readLine().toFloat(); int hours = static_cast<int>(decimalTime);
                        // TODO: Workaround
                        density.time = (hours != 24) ? QTime(hours, (decimalTime - hours) * 60) : QTime(23, 59);
                        density.factor = in.readLine().toFloat();
                        trafficDensities.append(density);
                    }
                    else if (line == "[trafficdensity_passenger]")
                    {
                        AiDensity density;
                        float decimalTime = in.readLine().toFloat(); int hours = static_cast<int>(decimalTime);
                        // TODO: Workaround
                        density.time = (hours != 24) ? QTime(hours, (decimalTime - hours) * 60) : QTime(23, 59);
                        density.factor = in.readLine().toFloat();
                        passengerDensities.append(density);
                    }
                    else if (line == "[entrypoints]")
                    {
                        int entrypointCount = in.readLine().toInt();

                        for (int i = 0; i < entrypointCount; i++)
                        {
                            EntrypointCollection::Entrypoint entrypoint;
                            entrypoint.objectID = in.readLine().toInt();
                            entrypoint.globalThingID = in.readLine().toInt();
                            entrypoint.awkwardValue1 = in.readLine().toInt();

                            // Attention: Inverted values
                            float xPos = in.readLine().toFloat();
                            float zPos = in.readLine().toFloat();
                            float yPos = in.readLine().toFloat();
                            entrypoint.position = OC3DCoordinates<float>(xPos, yPos, zPos);

                            entrypoint.awkwardValue2 = in.readLine().toFloat();
                            entrypoint.awkwardValue3 = in.readLine().toFloat();
                            entrypoint.awkwardValue4 = in.readLine().toFloat();
                            entrypoint.awkwardValue5 = in.readLine().toFloat();
                            entrypoint.tileID = in.readLine().toInt();

                            EntrypointCollection collection;
                            collection.name = in.readLine();
                            collection.entrypoints.append(entrypoint);

                            int collectionIndex = -1;
                            for (int j = 0; j < entrypoints.count(); j++) { if (entrypoints[j].name == collection.name) { collectionIndex = j; break; } }

                            if (collectionIndex != -1)
                                entrypoints[collectionIndex].entrypoints.append(entrypoint);
                            else entrypoints.append(collection);
                        }
                    }
                    else if (line == "[map]")
                    {
                        TileInformation tile;

                        int x = in.readLine().toInt();
                        int y = in.readLine().toInt();

                        tile.position = OC2DCoordinates<int>(x, y);
                        tile.filename = in.readLine();

                        tiles.append(tile);
                    }
                }

                global.close();
            }
            catch (...)
            {
                global.close();
                return FileIOResponse::errCritical;
            }

            seasons = sortSeasons(seasons);

            return FileIOResponse::valid;
        }

        FileIOResponse write() override
        {
            if (dir.isEmpty())
            {
                qWarning() << "Writing process of Global stopped: No file path given.";
                return FileIOResponse::errFileDoesntExist;
            }

            QFile global(dir + "/global.cfg");

            // Backup
            if (!QDir().exists(dir + "/backup")) qDebug() << "Backup dir create:" << QDir().mkdir(dir + "/backup");
            if (QFile(dir + "/backup/global.cfg").exists()) QFile(dir + "/backup/global.cfg").remove();
            global.copy(dir + "/backup/global.cfg");

            if (!global.open(QFile::WriteOnly | QFile::Text))
            {
                // msg.fileOpenErrorCloseOMSI(parent, mapFolderPath); TODO
                qDebug().noquote() << "Cannot write file: Full path: '" + QFileInfo(global).absoluteFilePath() + "'";
                return FileIOResponse::errFileNotOpen;
            }

            QTextStream out(&global);
            // Notepad++ -> UTF-8 - readable by OMSI!
            out.setEncoding(QStringConverter::Latin1);

            try {
                OCBase base;
                out << base.writeFileHeader() << "\n\n";

                out << "[name]" << "\n";
                out << name << "\n\n";

                out << "[friendlyname]" << "\n";
                out << friendlyname << "\n\n";

                out << "[description]" << "\n";
                out << description << "\n";
                out << "[end]" << "\n\n";

                out << "[version]" << "\n";
                out << version << "\n\n";

                out << "[NextIDCode]" << "\n";
                out << nextIDCode << "\n\n";

                if (worldCoodinates) out << "[worldcoordinates]" << "\n\n";

                if (dynHelpers) out << "[dynhelperactive]" << "\n\n";

                if (lht) out << "[LHT]" << "\n\n";

                if (realrail) out << "[realrail]" << "\n\n";

                out << "[backgroundimage]" << "\n";
                out << bgImage.isVisible << "\n";
                out << bgImage.picturePath.replace("/", "\\") << "\n";
                out << bgImage.width << "\n";
                out << bgImage.height << "\n";
                out << bgImage.startWidth << "\n";
                out << bgImage.startHeight << "\n\n";

                out << "[mapcam]" << "\n";
                out << standardView.tilePosition.y << "\n";
                out << standardView.tilePosition.x << "\n";
                out << standardView.position.x << "\n";
                out << standardView.position.z << "\n";
                out << standardView.position.y << "\n";
                out << standardView.rotAroundZ << "\n";
                out << standardView.rotAroundX << "\n";
                out << standardView.distanceFromPosition << "\n\n";

                out << "[moneysystem]" << "\n";
                out << currency.replace("/", "\\") << "\n\n";

                out << "[ticketpack]" << "\n";
                out << ticketpack.replace("/", "\\") << "\n\n";

                out << "[repair_time_min]" << "\n";
                float decimalRepairTime = 0; int hours = repairTime.hour();
                while (hours != 0) { hours--; decimalRepairTime += 60; }
                decimalRepairTime += repairTime.minute() / 60;
                out << decimalRepairTime << "\n\n";

                out << "[years]" << "\n";
                out << startYear << "\n";
                out << endYear << "\n\n";

                out << "[standarddepot]" << "\n";
                out << standardDepot << "\n\n";

                if (realYearOffset != 0)
                {
                    out << "[realyearoffset]" << "\n";
                    out << realYearOffset << "\n\n";
                }

                for (int i = 0; i < groundTextures.count(); i++)
                {
                    out << "[groundtex]" << "\n";
                    out << groundTextures[i].mainTex.replace("/", "\\") << "\n";
                    out << groundTextures[i].subTex.replace("/", "\\") << "\n";
                    out << groundTextures[i].texSizeExponent << "\n";
                    out << groundTextures[i].mainTexRepeating << "\n";
                    out << groundTextures[i].subTexRepeating << "\n\n";
                }

                seasons = sortSeasons(seasons);
                for (int i = 0; i < seasons.count(); i++)
                {
                    out << "[addseason]" << "\n";
                    out << seasons[i].type << "\n";
                    out << (QDate(QDate::currentDate().year(), 1, 1).daysTo(seasons[i].start)) << "\n";
                    out << (QDate(QDate::currentDate().year(), 1, 1).daysTo(seasons[i].end)) << "\n\n";
                }

                std::sort(trafficDensities.begin(), trafficDensities.end(), [](const AiDensity &a, const AiDensity &b) {
                    return a.time < b.time;
                });

                for (int i = 0; i < trafficDensities.count(); i++)
                {
                    out << "[trafficdensity_road]" << "\n";
                    // TODO: Workaround
                    if (trafficDensities[i].time == QTime(23, 59)) out << "24.000\n";
                    else out << QString::number(trafficDensities[i].time.hour()) + "." + QString::number(trafficDensities[i].time.minute() / 60) << "\n";
                    out << trafficDensities[i].factor << "\n\n";
                }

                std::sort(passengerDensities.begin(), passengerDensities.end(), [](const AiDensity &a, const AiDensity &b) {
                    return a.time < b.time;
                });

                for (int i = 0; i < passengerDensities.count(); i++)
                {
                    out << "[trafficdensity_passenger]" << "\n";
                    // TODO: Workaround
                    if (passengerDensities[i].time == QTime(23, 59)) out << "24.000\n";
                    else out << QString::number(passengerDensities[i].time.hour()) + "." + QString::number(passengerDensities[i].time.minute() / 60) << "\n";
                    out << passengerDensities[i].factor << "\n\n";
                }

                out << "[entrypoints]" << "\n";
                int count = 0;
                foreach (EntrypointCollection current, entrypoints) count += current.entrypoints.count();
                out << count << "\n";
                for (int i = 0; i < entrypoints.count(); i++)
                {
                    for (int j = 0; j < entrypoints[i].entrypoints.count(); j++)
                    {
                        out << entrypoints[i].entrypoints[j].objectID << "\n";
                        out << entrypoints[i].entrypoints[j].globalThingID << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue1 << "\n";
                        out << entrypoints[i].entrypoints[j].position.x << "\n";
                        out << entrypoints[i].entrypoints[j].position.z << "\n";
                        out << entrypoints[i].entrypoints[j].position.y << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue2 << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue3 << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue4 << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue5 << "\n";
                        out << entrypoints[i].entrypoints[j].tileID << "\n";
                        out << entrypoints[i].name << "\n";
                    }
                }

                out << "\n";

                for (int i = 0; i < tiles.count(); i++)
                {
                    out << "[map]" << "\n";
                    out << tiles[i].position.x << "\n";
                    out << tiles[i].position.y << "\n";
                    out << tiles[i].filename.replace("/", "\\") << "\n\n";
                }

                out << "\n";

                global.close();
            }
            catch (...)
            {
                global.close();
                return FileIOResponse::errCritical;
            }

            return FileIOResponse::valid;
        }
    };

    class Tile : public OCFile // map
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

    class UnschedTrafficDensitiesGroup : public OCFile // unsched_trafficdens.txt
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

    class UnschedVehicleGroup : public OCFile // unsched_vehgroups.txt
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
                        OCOptionalValue<QString> number;
                        OCOptionalValue<QString> repaintName;
                        OCOptionalValue<QString> regPlate;
                        OCOptionalValue<QDate> startDate;
                        OCOptionalValue<QDate> endDate;
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

    class TTData : public OCFile
    {
    public:
        class Trip
        {
        public:
            class Station
            {
            public:
                int objectID;
                int trackIndexParent;
                QString name;
                int tileID;
                float awkwardValue1;
                float awkwardValue2;
                float awkwardValue3;
                float awkwardValue4;
            };

            class Profile
            {
            public:
                QString name;
                QTime duration; // TODO: good type for use-cases?
            };

            QString name;
            QString trackName;
            QString terminus;
            QString line;

            QList<Station> stations;
            QList<Profile> profiles;
        };

        class Tour
        {
        public:
            class TripInformation
            {
            public:
                QString name;
                int profileIndex;
                QTime start;
            };

            enum Day
            {
                Monday = 1,
                Tuesday = 2,
                Wednesday = 4,
                Thursday = 8,
                Friday = 16,
                Saturday = 32,
                Sunday = 64,
                Holiday = 128,
                Hols = 256,
                NoHols = 512
            };

            Q_DECLARE_FLAGS(Days, Day)

            QString name;
            QString aiGroupName;
            Days days;
            QList<TripInformation> trips;
        };

        class Line
        {
        public:
            QString name;
            bool userAllowed = false;
            unsigned int priority;

            QList<Tour> tours;
        };

        QList<Trip> trips;
        QList<Line> lines;

        FileIOResponse read() override
        {
            clear();

            QDirIterator dirIteratorTtp(dir + "/TTData", QStringList() << "*.ttp", QDir::Files, QDirIterator::Subdirectories);

            // Trips
            while (dirIteratorTtp.hasNext())
            {
                QFile ttp(dirIteratorTtp.next());
                if (!ttp.open(QFile::ReadOnly | QFile::Text))
                {
                    // msg.fileOpenErrorCloseOMSI(parent, dir); TODO
                    qDebug().noquote() << "Cannot open file: Full path: '" + QFileInfo(ttp).absoluteFilePath() + "'";
                    return FileIOResponse::errFileNotOpen;
                }

                Trip trip;
                trip.name = QFileInfo(ttp).baseName();

                QTextStream in(&ttp);
                in.setEncoding(QStringConverter::Latin1);
                QString line = "";

                try
                {
                    while (!in.atEnd())
                    {
                        line = in.readLine();

                        if (line == "[trip]")
                        {
                            trip.trackName = in.readLine();
                            trip.terminus = in.readLine();
                            trip.line = in.readLine();
                        }
                        else if (line == "[station]")
                        {
                            Trip::Station station;
                            station.objectID = in.readLine().toInt();
                            station.trackIndexParent = in.readLine().toInt();
                            station.name = in.readLine();
                            station.tileID = in.readLine().toInt();
                            station.awkwardValue1 = in.readLine().toFloat();
                            station.awkwardValue2 = in.readLine().toFloat();
                            station.awkwardValue3 = in.readLine().toFloat();
                            station.awkwardValue4 = in.readLine().toFloat();

                            trip.stations << station;
                        }
                        else if (line == "[profile]")
                        {
                            Trip::Profile profile;
                            profile.name = in.readLine();
                            profile.duration = QTime::fromMSecsSinceStartOfDay(in.readLine().toFloat() * 60 * 1000);

                            trip.profiles << profile;
                        }
                    }

                    ttp.close();

                    trips << trip;
                }
                catch (...)
                {
                    ttp.close();
                    return FileIOResponse::errCritical;
                }
            }

            QDirIterator dirIteratorTtl(dir + "/TTData", QStringList() << "*.ttl", QDir::Files, QDirIterator::Subdirectories);

            // Lines & Tours
            while (dirIteratorTtl.hasNext())
            {
                QFile ttl(dirIteratorTtl.next());
                if (!ttl.open(QFile::ReadOnly | QFile::Text))
                {
                    // msg.fileOpenErrorCloseOMSI(parent, dir); TODO
                    qDebug().noquote() << "Cannot open file: Full path: '" + QFileInfo(ttl).absoluteFilePath() + "'";
                    return FileIOResponse::errFileNotOpen;
                }

                Line lineTT;
                lineTT.name = QFileInfo(ttl).baseName();

                QTextStream in(&ttl);
                in.setEncoding(QStringConverter::Latin1);
                QString line = "";

                try
                {
                    while (!in.atEnd())
                    {
                        line = in.readLine();

                        if (line == "[userallowed]") lineTT.userAllowed = true;
                        else if (line == "[priority]") lineTT.priority = in.readLine().toInt();
                        else if (line == "[newtour]")
                        {
                            Tour tour;
                            tour.name = in.readLine();
                            tour.aiGroupName = in.readLine();

                            int dayValue = in.readLine().toInt();

                            while (true)
                            {
                                if      (dayValue >= 512) { tour.days |= Tour::Hols;      dayValue -= 512; }
                                else if (dayValue >= 256) { tour.days |= Tour::NoHols;    dayValue -= 256; }
                                else if (dayValue >= 128) { tour.days |= Tour::Holiday;   dayValue -= 128; }
                                else if (dayValue >= 64)  { tour.days |= Tour::Sunday;    dayValue -= 64; }
                                else if (dayValue >= 32)  { tour.days |= Tour::Saturday;  dayValue -= 32; }
                                else if (dayValue >= 16)  { tour.days |= Tour::Friday;    dayValue -= 16; }
                                else if (dayValue >= 8)   { tour.days |= Tour::Thursday;  dayValue -= 8; }
                                else if (dayValue >= 4)   { tour.days |= Tour::Wednesday; dayValue -= 4; }
                                else if (dayValue >= 2)   { tour.days |= Tour::Tuesday;   dayValue -= 2; }
                                else if (dayValue >= 1)   { tour.days |= Tour::Monday;    break; }
                                else break;
                            }

                            lineTT.tours << tour;
                        }
                        else if (line == "[addtrip]")
                        {
                            Tour::TripInformation tripInformation;

                            tripInformation.name = in.readLine();
                            tripInformation.profileIndex = in.readLine().toInt();
                            tripInformation.start = QTime::fromMSecsSinceStartOfDay(in.readLine().toFloat() * 60 * 1000);

                            lineTT.tours.last().trips << tripInformation;
                        }
                    }

                    ttl.close();

                    lines << lineTT;
                }
                catch (...)
                {
                    ttl.close();
                    return FileIOResponse::errCritical;
                }
            }

            return FileIOResponse::valid;
        }
    };

    Global global;
    Timezone timezone;
    TTData ttData;
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

    inline static QString dir = "";
};

Q_DECLARE_OPERATORS_FOR_FLAGS(OCMap::TTData::Tour::Days);

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
    OCMapPosition mapcam; // tilePos not used here! See 'centerTile' member
    OCMapPosition egoPosition; // tilePos not used here! See 'centerTile' member - std: x=10;y=10

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

class OCEnvir // envir.cfg
{
public:
    class SkyTextures
    {
    public:
        QString day;
        QString twilight;
        QString night;
    };

    class TwilightAngle
    {
    public:
        int start; // should be < 0
        int end; // should bei > 0
    };

    class Lightcolor // all RGB values!
    {
        QColor nadir;
        QColor twilightStart;
        QColor sunrise;
        QColor twilightEnd;
        QColor zenith;
    };

    SkyTextures skyTextures;
    TwilightAngle twilightAngle;

    Lightcolor lightcolorPrimary;
    Lightcolor lightcolorSecondary;
    Lightcolor lightcolorAmbient;
};

class OCClouds
{
public:
    class Cloudtype
    {
    public:
        enum Coverage
        {
            ovc = 0, // high coverage (overcast)
            sct = 1 // low coverage (scattered)
        };

        QString name;
        QString texture;
        int textureSize;
        Coverage coverage;
    };

    QList<Cloudtype> clouds;
};

#endif // OCC_H
