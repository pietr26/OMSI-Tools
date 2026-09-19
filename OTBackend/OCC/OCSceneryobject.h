#ifndef OCSCENERYOBJECT_H
#define OCSCENERYOBJECT_H


#include <QColor>
#include <QList>
#include <QString>
#include "OCBase.h"
#include "OCRail.h"
#include "OCViewable.h"

class OCSceneryobject
{
public:
    class Tree
    {
    public:
        QString texture;
        float minHeight = 0.0f;
        float maxHeight = 0.0f;
        float minRatio = 0.0f;
        float maxRatio = 0.0f;
    };

    class Maplight
    {
    public:
        OCType::Coord3D<float> position;
        QColor color;
        float maxBrightnessRadius = -1;
    };

    class Triggerbox
    {
    public:
        OCType::Box<float> geometry;

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
                float duration = 0.0f;
            };

            QString name;
            QList<Phase> phases;
            float approachDistance = -1; // should not be 0!
        };

        class TrafficLightJumpStop
        {
        public:
            int checkOnApproachIdent = 0;
            float jumpTimePosition = 0.0f;
            bool jumpIfNoApproach = false;
            float jumpToTime = -1; // mandatory for [traffic_light_jump] {if!=-1} or [traffic_light_stop] {if=-1}!
        };

        float duration = 0.0f;
        QList<TrafficLight> trafficLights;
        QList<TrafficLightJumpStop> jumpStops;
    };

    class SplineHelper
    {
    public:
        OCType::Coord3D<float> position; // Attention! Inverted: xzy
        float rotation; // can be bigger as 360
        float splineHelper5thValue; // TODO: ?
        float splineHelper6thValue; // TODO: ?
    };

    class Path
    {
    public:
        OCType::Coord3D<float> position; // not inverted
        float rotation = 0.0f;
        float radius = 0.0f;
        float length = 10;
        float gradientStart = 0.0f;
        float gradientEnd = 0.0f;
        int type; // 0=Street 1=Sidewalk 2=Railroad
        float width = 0.0f;
        int direction; // 0=Forward 1=Reverse/Backward 2=Both
        int blinker; // 0=no 1=Straight 2=Left 3=Right

        int trafficLightIdent = -1;

        bool hasCrossingProblem = false;

        int blockPath1stValue; // TODO: ? |-> Probably used to keep this path clear, i.e. to stop AI traffic from halting there
        int blockPath2ndValue; // TODO: ? /   s. Sceneryobjects\ADDON_SimpleStreets\Kreuz_MC\Einm_001_001_1.sco - Pfad 8: sogar 4 blockpaths auf einmal.

        // if (path2) {
        int path2_13thValue; // TODO: ?
        int path2_14ndValue; // TODO: ?
        // }

        std::optional<OCRail::RailEnh> rail;
        std::optional<OCRail::ThirdRail> thirdRail;

        int switchdir = -1;
    };

    bool isDepot = false;
    bool isBusstop = false;
    bool isCarPark = false;
    bool isPetrolStation = false;
    bool isEntrypoint = false;
    bool isHelpArrow = false;
    bool isSignal = false;
    bool isTrafficLight = false;

    bool usesAbsoluteHeight = false;
    bool lightMapMapping = false;
    bool noCollision = false;
    QList<QString> collisionMeshes;
    bool noMapLighting = false;
    bool isJoinable = false;
    bool isFixed = false;
    QString rendertype;

    int complexity = -1;

    std::optional<OCType::Coord2D<float>> crashModePole;

    std::optional<OCType::Coord2D<float>> centerOfGravity;

    QString crossingHeightDeform;
    QString terrainHole;

    bool onlyVisibleInEditor = false;
    bool isSurface = false;

    std::optional<Tree> tree;

    QList<Triggerbox> triggerboxes;

    QList<TrafficLightGroup> trafficLightGroups;
    QList<SplineHelper> splineHelpers;
    QList<Path> paths; // TODO: see Template class class

    int switchDirectionCount = -1; // e.g.: single / 'simple' switch: 2  -  [switch]

    OCViewable::Viewable object;
};

#endif // OCSCENERYOBJECT_H
