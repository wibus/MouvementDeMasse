#ifndef DRAWSUNANDGROUND_H
#define DRAWSUNANDGROUND_H

#include "DrawSun.h"


class DrawSunAndGround : public DrawSun
{
public:
    DrawSunAndGround();

    // DrawAlgorithm interface
    virtual void setup(CityMap& cityMap);
    virtual void draw();

    virtual void updateViewMatrix(const cellar::Matrix4x4<float>& view);
    virtual void updateProjectionMatrix(const cellar::Matrix4x4<float>& proj);    

protected:
    virtual void refresh();
    virtual void setupSpotLights();

    virtual void setupGround();
    virtual void genGroundTex();
    virtual void drawGround();

    virtual void setupWater();    
    virtual void genWaterTex();    
    virtual void drawWater();

    virtual void computeGroundVertex(int& idx,
                                     cellar::Vec3f* pos, cellar::Vec3f* norm,
                                     int i,     int j);

    // Ground
    cellar::GLShaderProgram _groundShader;
    unsigned int _groundVao;
    unsigned int _groundTex;
    int _groundNbElems;
    float _groundShininess;

    // Water
    unsigned int _waterVao;
    unsigned int _waterTex;
    const int _waterNbElems;
    cellar::Vec4f _waterColor;
    float _waterShininess;

    // Spot lights
    cellar::SpotLight3D _spots;
};

#endif // DRAWSUNANDGROUND_H
