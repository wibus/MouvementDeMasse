#ifndef DRAWSUNGROUNDANDROADS_H
#define DRAWSUNGROUNDANDROADS_H

#include "DrawSunAndGround.h"


class DrawSunGroundAndRoads : public DrawSunAndGround
{
public:
    DrawSunGroundAndRoads();

    // DrawAlgorithm interface
    virtual void setup(CityMap& cityMap);
    virtual void draw();

    virtual void updateViewMatrix(const cellar::Matrix4x4<float>& view);
    virtual void updateProjectionMatrix(const cellar::Matrix4x4<float>& proj);

protected:
    virtual void setupJunctions();
    virtual void drawJunctions();

    virtual void setupStreets();
    virtual void drawStreets();

    virtual float junctionHeight(unsigned int x, unsigned int y);


    // Attributes
    float _roadWidth;
    cellar::GLShaderProgram _roadShader;

    unsigned int _junctionsVao;
    unsigned int _junctionsTex;
    int _junctionsNbElems;

    unsigned int _streetsVao;
    unsigned int _streetsTex;
    int _streetsNbElems;
};

#endif // DRAWSUNGROUNDANDROADS_H
