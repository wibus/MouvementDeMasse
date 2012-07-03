#ifndef GROUNDCOMPONENT_H
#define GROUNDCOMPONENT_H

#include <MathsAndPhysics/Vector.h>

#include "MdMTypes.h"
#include "AbstractComponent.h"


class GroundComponent : public AbstractComponent
{
public:
    GroundComponent(DrawCityCommonData& common);
    ~GroundComponent();

    void setup();
    void draw();

protected:
    void setupLands();
    void setupRoads();
    void setupTriangles();

    bool isGrassRoad(int i, int j, CardinalDirection dir);

    unsigned int _groundTex;
    unsigned int _landsVao;
    int _landsNbElems;
    unsigned int _roadsVao;
    int _roadsNbElems;
    unsigned int _trianglesVao;
    int _trianglesNbElems;
};

#endif // GROUNDCOMPONENT_H
