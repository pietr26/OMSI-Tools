#ifndef OCMODEL_H
#define OCMODEL_H

#include "OCBase.h"

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

                std::optional<Allcolor> allcolor;
                int alpha = -1;
                std::optional<TextureMappingIntensity> bumpMap; // TODO: Binding to variable possible? TESTING
                std::optional<TextureMappingIntensity> envMap; // TODO: see bumMap member
                std::optional<TextureMappingIntensity> rainDropMap; // TODO: see bumMap member
                QString envMapMask;
                int envMapRealTimeFactor = -1;
                QList<TextureMappingVariable> freeTextures;
                QString nightMap;
                QString transMap;
                bool noZCheck;
                bool noZWrite;
                int zBias = -1;
                std::optional<TextureAdress> textureAdress;

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

            std::optional<OC3DCoordinates<float>> originTransform;
            std::optional<OC3DCoordinates<int>> originRotation; // TODO: Does this makes sense? :')

            std::optional<Method> animationTransform;
            std::optional<Method> animationRotation;
            std::optional<float> maxSpeed;
            std::optional<float> delay;
            std::optional<float> offset;

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

            std::optional<MethodMinMax> velocity;
            std::optional<MethodMinMax> constantVelocity;
            std::optional<MethodMinMax> frequence;
            std::optional<MethodMinMax> liveTime;
            std::optional<QVariant> emitterLifeTime; // float
            std::optional<MethodMinMax> brakeFactor;
            std::optional<MethodMinMax> weightForce; // 0=no gravity
            std::optional<MethodMinMax> sizeStart;
            std::optional<MethodMinMax> sizeFinal;
            std::optional<MethodMinMax> alphaInitial;
            std::optional<MethodMinMax> alphaFinal;
            std::optional<Color> color; // rgb and hsv!
            std::optional<int> calculateDistance;
            std::optional<QVariant> isEmissive; // bool
            std::optional<Bitmap> bitmap;
            std::optional<QVariant> waitForParentDeath; // bool
            std::optional<InstantExplosionPartCount> instantExplosionPartCount;

            std::optional<AttachTo> attachTo; // see 'C 005F6DA4 TRauch.attachedto:Integer'
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
    std::optional<OC3DBox<float>> viewbox; // [VFDmaxmin] - ATTENTION: Some calculations are necessary to fit values in the class! - values: 1=xMinPos 2=yMinPos 3=zMinPos 4=xMaxPos 5=yMaxPos 6=zMaxPos
    QList<ComplexTextureChange> complexTextureChanges;

    QList<Spotlight> spotlights;
};

#endif // OCMODEL_H
