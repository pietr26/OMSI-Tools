#ifndef OCMAIN_H
#define OCMAIN_H

#include "OTGlobal.h"
#include "OCHelp.h"

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
    class busInfo
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

    QList<busInfo> busInfo;
};

class OCFont // oft
{
public:
    class character
    {
    public:
        character(QString ca = "", int lP = -1, int rP = -1, int hP = -1, QString co = "")
        {
            chartr = ca;
            leftPixel = lP;
            rightPixel = rP;
            highestPixelInFontRow = hP;
            comment = co;
        }

        QString chartr;
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

    QList<character> charList;
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
    class entry
    {
    public:
        QString event;
        int keyIdent = -1;
        // bool ?;
    };


    QList<entry> entries;
};

class OCLanguage // olf
{
public:
    class part
    {
    public:
        QString textIdent;
        QString translation;
    };

    QString languageIdent;
    QList<part> parts;
};

class OCTile
{
public:
    class spline
    {
    public:
        class rule
        {
        public:
            int pathIdent = -1;
            QString param;
            int value = -1;
            int aiGroupIndex = -1;
        };

        bool h = false;

        //?
        QString path;
        //?...

        QList<rule> rules;

        // Different param count spline / splineH?
    };

    class sceneryobject
    {
    public:
        // TODO
        // possible for objects: [object] [splineAttachement] [attachObj] [splineAttachement_repeater]

        //?
        QString path;
        //?...

        // Attention: Different param count object / attachement!
    };

    int version;
    bool terrain = false;
    bool variableTerrainLightmap = false;
    bool variableTerrain = false;

    QList<spline> splines;

    QList<sceneryobject> sceneryobjects;
};

class OCMapGlobal // global.cfg
{
public:
    class texture
    {
    public:
        QString mainTex;
        QString subTex;
        //OBJECT ???; // TODO
        //OBJECT ???; // TODO
        //OBJECT ???; // TODO
    };

    class season
    {
    public:
        int season;
        int seasonStartDay;
        int seasonEndDay;
    };

    class aiDensity
    {
    public:
        float timeDecimal;
        float density; // can be bigger as 1?
    };

    class tile
    {
    public:
        int x;
        int y;
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
    //OBJECT backgroundImage; // TODO
    //OBJECT mapcam; // TODO
    QString moneysystem;
    QString ticketpack;
    int repairTime = -1; // min
    QString standardDepot;
    QList<texture> groundTextures;
    int startYear;
    int endYear;
    int realYearOffset;
    QList<season> seasons;
    QList<aiDensity> trafficDensities;
    QList<aiDensity> passengerDensities;
    //QList<OBJECT> entrypoints; // TODO
    QList<tile> tiles;
};

class OCVariableContentTranslation // dsc
{
public:
    // TODO
};

class OCMapAilist
{
public:
    class group
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
    QList<group> groups;
};

class OCMapAilist2
{
public:
    class group
    {
    public:
        class vehicle
        {
        public:
            QString vehiclePath;
            int density;
        };

        QString name;
        QString hofFileName; //?
        QList<vehicle> vehicles;
    };

    class groupdepot
    {
    public:
        class typgroup2
        {
        public:
            class vehicle
            {
            public:
                QString number;
                QString repaintName;
                QString regPlate;
            };

            QString vehiclePath;
            QList<vehicle> vehicles;
        };

        QString name;
        QString hofFileName;

        // TODO: Typgroup (Typgroup1)?
        QList<typgroup2> typgroup2s;
    };

    QList<group> groups;
    QList<groupdepot> groupdepots;
};

class OCMapHolidays // txt
{
    // This file exists multiple times with file (pre-)extension [language code].
public:
    class holidays
    {
    public:
        QDate start;
        QDate end;
        QString name;
    };

    class holiday
    {
    public:
        QDate date;
        QString name;
    };

    QList<holidays> holidaysMultiple;
    QList<holiday> holidaysSingle;
};

class OCMapSignalroutes
{
public:
    // TODO
};

class OCMapUnschedTrafficDensities
{
public:
    class OCMapUnschedTrafficDensitiesGroup
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

    QList<OCMapUnschedTrafficDensitiesGroup> groups;
};

class OCMapUnschedVehicleGroups
{
public:
    class group
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

    QList<group> groups;
};

// TTData

#endif // OCMAIN_H

// TODO: OCMap object (including ailist, global etc.)
