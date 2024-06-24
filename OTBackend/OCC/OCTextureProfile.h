#ifndef OCTEXTUREPROFILE_H
#define OCTEXTUREPROFILE_H

#include "OCBase.h"

class OCTextureProfile : public OCFile // [textureName].[textureSuffix].cfg
{
public:
    bool hasMoisture;
    bool hasPuddles;
    bool hasTerrainMapping;
    int surface = -1; // TODO: enum? - std::optional?
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

#endif // OCTEXTUREPROFILE_H
