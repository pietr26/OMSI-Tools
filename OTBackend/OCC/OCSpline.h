#ifndef OCSPLINE_H
#define OCSPLINE_H


#include <QList>
#include <QString>
#include "OCBase.h"
#include "OCRail.h"

class OCSpline
{
public:
    class HeightProfile
    {
    public:
        float leftXPosition = 0.0f;
        float rightXPosition = 0.0f;
        float leftZPosition = 0.0f;
        float rightZPosition = 0.0f;
    };

    class Texture
    {
    public:
        class PatchworkChain
        {
        public:
            float length = 0.0f;
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
        std::optional<PatchworkChain> patchworkChain;
        bool scaleTextureByLength = false;
    };

    class Profile
    {
    public:
        class ProfilePoint
        {
        public:
            float xPosition = 0.0f;
            float zPosition = 0.0f;
            float xPositiononTexture = 0.0f;
            float stretchFactor = 0.0f;
        };

        int textureIdent = -1;
        QList<ProfilePoint> profilePoints;
    };

    class TerrainHoleProfile
    {
    public:
        // TODO: maximum of 2 profilepoints?
        QList<OCType::Coord3D<float>> points;
    };

    class Path
    {
    public:
        int type = -1; // 0=Street 1=Sidewalk 2=Railroad
        float xPosition = 0.0f;
        float zPosition = 0.0f;
        float width = 0.0f;
        int direction = -1; // 0=Forward 1=Reverse/Backward 2=Both

        std::optional<OCRail::RailEnh> rail;
        std::optional<OCRail::ThirdRail> thirdRail;

        float path6thValue; // TODO: ?
    };

    int halfCantWidth = -1;

    bool onlyVisibleInEditor = false;
    QString rendertype; // see OCSceneryobject

    QList<HeightProfile> heightProfiles;
    QList<Texture> textures;
    QList<Profile> profiles;
    QList<TerrainHoleProfile> terrainHoleProfiles; // Attention! Inverted: xzy
};

#endif // OCSPLINE_H
