#ifndef RESIDENTIALCOMPONENT_H
#define RESIDENTIALCOMPONENT_H

#include <vector>
#include <MathsAndPhysics/Vector.h>
struct DrawCityCommonData;


class ResidentialComponent
{
public:
    ResidentialComponent(DrawCityCommonData& common);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix();
    void updateModelViewMatrix();

protected:
    float landHeight(int i, int j);
    void setupApartSides();
    void setupApartTop();

    DrawCityCommonData& _common;
    unsigned int _apartSidesVao;
    unsigned int _apartSidesTex;
    int          _apartSidesNbElems;

    unsigned int _apartTopVao;
    unsigned int _apartTopTex;
    int          _apartTopNbElems;

    std::vector<cellar::Vec3f> _apartmentPos;
};

#endif // RESIDENTIALCOMPONENT_H
