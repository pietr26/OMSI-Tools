#ifndef OTMODELCONFIGVALIDATOR_H
#define OTMODELCONFIGVALIDATOR_H

#include "OTContentValidator.h"

class OTModelConfigValidator : public OTContentValidator
{
    Q_OBJECT
public:
    explicit OTModelConfigValidator(QObject *parent, const QString &filePath);

protected:
    void specificValidate() override;
    void validateLine() override;
    void finalizeValidation() override;

    void readVarlist(const QString &filePath, const bool &stringvarlist);
    void readMaterial(const bool &isSplineTexture = false);
    void checkLastMaterial();

    void initPredefinedVars();

    bool _meshFound = false, _matlFound = false, _matlItemFound = false, _matlIsTexttexture = false;
    QPair<int, int> _lastMatl = QPair<int, int>(0, 0); //<line number, type> (0 = invalid/unspecified, 1 = matl/texture (spline), 2 = matl_change)
    QString _lastMatlTexture;

    bool _splineScanning = false, _splineTextureFound = false;

    QHash<int, QString> _foundVariables, _foundStringVariables;
    QStringList _definedVariables, _definedStringVariables;

    int _texttextureCount = 0;
    QHash<int, int> _texttextureAssignments;

    const QStringList _matlProperties = {
        "[matl_allcolor]",
        "[matl_alpha]",
        "[matl_bumpmap]",
        "[matl_envmap_mask]",
        "[matl_envmap]",
        "[matl_freetex]",
        "[matl_item]",
        "[matl_lightmap]",
        "[matl_nightmap]",
        "[matl_noZcheck]",
        "[matl_noZwrite]",
        "[matl_texadress_border]",
        "[matl_texadress_clamp]",
        "[matl_transmap]"
    };

    const QStringList _scoPredefinedVars = {
        "NightLightA",
        "InUse",
        "TrafficLightPhase",
        "TrafficLightApproach",
        "Colorscheme",
        "Signal",
        "NextSignal",
        "Refresh_Strings",
        "Switch"
    };

    const QStringList _busOvhPredefinedVars = {
        "Refresh_Strings",
        "Envir_Brightness",
        "StreetCond",
        "Spot_Select",
        "Colorscheme",
        "M_Wheel",
        "n_Wheel",
        "Throttle",
        "Brake",
        "Clutch",
        "Brakeforce",
        "Velocity",
        "Velocity_Ground",
        "tank_percent",
        "kmcounter_km",
        "kmcounter_m",
        "relrange",
        "Driver_Seat_VertTransl",
        "Wheel_Rotation_0_L",
        "Wheel_Rotation_1_L",
        "Wheel_Rotation_2_L",
        "Wheel_Rotation_3_L",
        "Wheel_Rotation_0_R",
        "Wheel_Rotation_1_R",
        "Wheel_Rotation_2_R",
        "Wheel_Rotation_3_R",
        "Wheel_RotationSpeed_0_L",
        "Wheel_RotationSpeed_1_L",
        "Wheel_RotationSpeed_2_L",
        "Wheel_RotationSpeed_3_L",
        "Wheel_RotationSpeed_0_R",
        "Wheel_RotationSpeed_1_R",
        "Wheel_RotationSpeed_2_R",
        "Wheel_RotationSpeed_3_R",
        "Axle_Suspension_0_L",
        "Axle_Suspension_1_L",
        "Axle_Suspension_2_L",
        "Axle_Suspension_3_L",
        "Axle_Suspension_0_R",
        "Axle_Suspension_1_R",
        "Axle_Suspension_2_R",
        "Axle_Suspension_3_R",
        "Axle_Steering_0_L",
        "Axle_Steering_1_L",
        "Axle_Steering_2_L",
        "Axle_Steering_3_L",
        "Axle_Steering_0_R",
        "Axle_Steering_1_R",
        "Axle_Steering_2_R",
        "Axle_Steering_3_R",
        "Axle_Springfactor_0_L",
        "Axle_Springfactor_1_L",
        "Axle_Springfactor_2_L",
        "Axle_Springfactor_3_L",
        "Axle_Springfactor_0_R",
        "Axle_Springfactor_1_R",
        "Axle_Springfactor_2_R",
        "Axle_Springfactor_3_R",
        "Axle_Brakeforce_0_L",
        "Axle_Brakeforce_1_L",
        "Axle_Brakeforce_2_L",
        "Axle_Brakeforce_3_L",
        "Axle_Brakeforce_0_R",
        "Axle_Brakeforce_1_R",
        "Axle_Brakeforce_2_R",
        "Axle_Brakeforce_3_R",
        "Axle_SurfaceID_0_L",
        "Axle_SurfaceID_1_L",
        "Axle_SurfaceID_2_L",
        "Axle_SurfaceID_3_L",
        "Axle_SurfaceID_0_R",
        "Axle_SurfaceID_1_R",
        "Axle_SurfaceID_2_R",
        "Axle_SurfaceID_3_R",
        "Debug_0",
        "Debug_1",
        "Debug_2",
        "Debug_3",
        "Debug_4",
        "Debug_5",
        "A_Trans_X",
        "A_Trans_Y",
        "A_Trans_Z",
        "AI_Blinker_L",
        "AI_Blinker_R",
        "AI_Light",
        "AI_Interiorlight",
        "AI_Brakelight",
        "AI_Engine",
        "AI_target_index",
        "target_index_int",
        "AI_Scheduled_AtStation",
        "AI_Scheduled_AtStation_Side",
        "AI",
        "PAX_Entry0_Open",
        "PAX_Entry1_Open",
        "PAX_Entry2_Open",
        "PAX_Entry3_Open",
        "PAX_Entry4_Open",
        "PAX_Entry5_Open",
        "PAX_Entry6_Open",
        "PAX_Entry7_Open",
        "PAX_Exit0_Open",
        "PAX_Exit1_Open",
        "PAX_Exit2_Open",
        "PAX_Exit3_Open",
        "PAX_Exit4_Open",
        "PAX_Exit5_Open",
        "PAX_Exit6_Open",
        "PAX_Exit7_Open",
        "PAX_Entry0_Req",
        "PAX_Entry1_Req",
        "PAX_Entry2_Req",
        "PAX_Entry3_Req",
        "PAX_Entry4_Req",
        "PAX_Entry5_Req",
        "PAX_Entry6_Req",
        "PAX_Entry7_Req",
        "PAX_Exit0_Req",
        "PAX_Exit1_Req",
        "PAX_Exit2_Req",
        "PAX_Exit3_Req",
        "PAX_Exit4_Req",
        "PAX_Exit5_Req",
        "PAX_Exit6_Req",
        "PAX_Exit7_Req",
        "GivenTicket",
        "humans_count",
        "FF_Vib_Period",
        "FF_Vib_Amp",
        "Snd_OutsideVol",
        "Snd_Microphone",
        "Snd_Radio",
        "Cabinair_Temp",
        "Cabinair_absHum",
        "Cabinair_relHum",
        "PrecipRate",
        "PrecipType",
        "Dirt_Norm",
        "DirtRate",
        "schedule_active",
        "train_frontcoupling",
        "train_backcoupling",
        "train_me_reverse",
        "TrafficPriority",
        "wearlifespan",
        "articulation_#_alpha",
        "articulation_#_beta",
        "boogie_#_wheel_at_limit",
        "boogie_#_invradius",
        "contactshoe_#_rail_pos_x",
        "contactshoe_#_rail_pos_y",
        "contactshoe_#_rail_index",
        "contactshoe_#_volt_rail",
        "contactshoe_#_volt_veh",
        "contactshoe_#_freq"
    };

    const QStringList _busOvhPredefinedStringVars = {
        "ident",
        "number",
        "act_route",
        "act_busstop",
        "SetLineTo",
        "yard",
        "file_schedule"
    };

    const QStringList _humPredefinedVars = {
        "LastMovedDist",
        "PAX_State",
        "HeightOfSeat",
        "Colorscheme"
    };
};

#endif // OTMODELCONFIGVALIDATOR_H
