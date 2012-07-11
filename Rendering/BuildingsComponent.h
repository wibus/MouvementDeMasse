#ifndef BUILDINGSCOMPONENT_H
#define BUILDINGSCOMPONENT_H

#include <vector>
#include <MathsAndPhysics/Vector.h>

#include "AbstractComponent.h"


class BuildingsComponent : public AbstractComponent
{
public:
    BuildingsComponent(City& city, cellar::GLShaderProgram& shader);
    ~BuildingsComponent();

    void setup();
    void draw();

protected:
    float landHeight(int i, int j);
    void setupPositions();
    void setupBuidlindSides();
    void setupRoofTop();

    unsigned int _buildingWallsVao;
    int          _buildingNbElems;

    unsigned int _roofVao;
    int          _roofNbElems;
    unsigned int _roofTex;
    unsigned int _roofSpec;
    std::vector< cellar::Vec3f > _roofPos;

    unsigned int _apartmentTex;
    unsigned int _apartmentSpec;
    std::vector< cellar::Vec3f > _apartmentsPos;
    std::vector< cellar::Vec2f > _apartmentTexScaleCoeff;

    unsigned int _commerceTex;
    unsigned int _commerceSpec;
    std::vector< cellar::Vec3f > _commercePos;
    std::vector< cellar::Vec2f > _commerceTexScaleCoeff;
};

#endif // BUILDINGSCOMPONENT_H
