#ifndef GROUNDCOMPONENT_H
#define GROUNDCOMPONENT_H

#include <DataStructure/Vector.h>

#include "MdMTypes.h"
#include "AbstractComponent.h"


class GroundComponent : public AbstractComponent
{
public:
    GroundComponent(City& city, cellar::GlProgram& shader);
    ~GroundComponent();

    void setup();
    void draw();

protected:
    void setupLands();
    void setupRoads();
    void setupTriangles();

    bool isGrassRoad(int i, int j, CardinalDirection dir);

    unsigned int _groundTex;

    static const int _LANDS_NB_BUFFS = 3;
    unsigned int _landsBuffs[_LANDS_NB_BUFFS];
    unsigned int _landsVao;
    int _landsNbElems;

    static const int _ROADS_NB_BUFFS = 3;
    unsigned int _roadsBuffs[_ROADS_NB_BUFFS];
    unsigned int _roadsVao;
    int _roadsNbElems;

    static const int _TRIANGLES_NB_BUFFS = 3;
    unsigned int _trianglesBuffs[_TRIANGLES_NB_BUFFS];
    unsigned int _trianglesVao;
    int _trianglesNbElems;
};

#endif // GROUNDCOMPONENT_H
