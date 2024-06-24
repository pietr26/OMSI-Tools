#ifndef OCSPLINE_H
#define OCSPLINE_H

#include "OCBase.h"

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
        std::optional<PatchworkChain> patchworkChain;
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

        std::optional<OCRail::RailEnh> rail;
        std::optional<OCRail::ThirdRail> thirdRail;

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

#endif // OCSPLINE_H
