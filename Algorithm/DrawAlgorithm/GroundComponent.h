#ifndef GROUNDCOMPONENT_H
#define GROUNDCOMPONENT_H

#include <MathsAndPhysics/Vector.h>
struct DrawCityCommonData;


class GroundComponent
{
public:
    GroundComponent(DrawCityCommonData& common);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix();
    void updateModelViewMatrix();

protected:
    void setupLands();
    void setupRoads();
    void setupTriangles();

    //void computeGroundVertex(int& idx, cellar::Vec3f* pos, cellar::Vec3f* norm, cellar::Vec2f *tex, int i, int j);


    DrawCityCommonData& _common;
    unsigned int _groundVao;
    unsigned int _groundTex;
    int _groundNbElems;

    unsigned int _landsVao;
    int _landsNbElems;

    unsigned int _roadsVao;
    int _roadsNbElems;

    unsigned int _trianglesVao;
    int _trianglesNbElems;
};

#endif // GROUNDCOMPONENT_H
