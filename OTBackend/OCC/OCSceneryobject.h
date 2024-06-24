#ifndef OCSCENERYOBJECT_H
#define OCSCENERYOBJECT_H

#include "OCBase.h"

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

        std::optional<OCRail::RailEnh> rail;
        std::optional<OCRail::ThirdRail> thirdRail;

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

    std::optional<OC2DCoordinates<float>> crashModePole;

    std::optional<OC3DCoordinates<float>> centerOfGravity;

    QString crossingHeightDeform;
    QString terrainHole;

    bool onlyVisibleInEditor;
    bool isSurface;

    std::optional<Tree> tree;

    QList<Triggerbox> triggerboxes;

    QList<TrafficLightGroup> trafficLightGroups;
    QList<SplineHelper> splineHelpers;
    QList<Path> paths; // TODO: see Template class class

    int switchDirectionCount = -1; // e.g.: single / 'simple' switch: 2  -  [switch]

    OCViewable object;
};

#endif // OCSCENERYOBJECT_H
