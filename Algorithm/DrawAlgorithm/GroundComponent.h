#ifndef GROUNDCOMPONENENT_H
#define GROUNDCOMPONENENT_H

#include "Graphics/GL/GLShaderProgram.h"

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
    cellar::GLShaderProgram _groundShader;
    unsigned int _groundVao;
    unsigned int _groundTex;
    int _groundNbElems;
};

#endif // GROUNDCOMPONENENT_H
