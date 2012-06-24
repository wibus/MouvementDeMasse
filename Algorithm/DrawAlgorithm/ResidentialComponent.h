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
    void setupPositions();
    void setupBuidlindSides();
    void setupRoofTop();

    DrawCityCommonData& _common;
    unsigned int _buildingWallsVao;
    int          _buildingNbElems;

    unsigned int _roofVao;
    int          _roofNbElems;
    unsigned int _roofTex;
    std::vector< cellar::Vec4f > _roofPos;

    unsigned int _apartmentTex;
    std::vector< cellar::Vec4f > _apartmentsPos;

    unsigned int _commercesTex;
    std::vector< cellar::Vec4f > _commercesPos;
};

#endif // RESIDENTIALCOMPONENT_H
