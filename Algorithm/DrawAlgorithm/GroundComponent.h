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
    void genGroundTex();
    void computeGroundVertex(int& idx, cellar::Vec3f* pos, cellar::Vec3f* norm, int i, int j);


    DrawCityCommonData& _common;
    unsigned int _groundVao;
    unsigned int _groundTex;
    int _groundNbElems;
};

#endif // GROUNDCOMPONENT_H
