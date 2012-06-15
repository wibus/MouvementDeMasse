#include "GroundComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
using namespace cellar;


GroundComponent::GroundComponent(DrawCityCommonData& common) :
    _common(common),
    _groundVao(0),
    _groundNbElems(0)
{
    GLInOutProgramLocation groundLocations;
    groundLocations.setInput(0, "position_att");
    groundLocations.setInput(1, "normal_att");
    _groundShader.setInAndOutLocations(groundLocations);
    _groundShader.loadShadersFromFile("resources/shaders/ground.vert",
                                      "resources/shaders/ground.frag");
    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction", _common.sunLight.direction);
    _groundShader.setVec3f("sun.ambient",   _common.sunLight.ambient);
    _groundShader.setVec3f("sun.diffuse",   _common.sunLight.diffuse);
    _groundShader.setVec3f("sun.specular",  _common.sunLight.specular);
    _groundShader.setFloat("Shininess",     _common.groundShininess);
    _groundShader.setFloat("WaterHeight",   _common.ground.waterHeight());
    _groundShader.setVec4f("WaterColor",    _common.waterColor);
    _groundShader.setVec4f("GrassColor",    _common.grassColor);
    _groundShader.popProgram();
}

void GroundComponent::setup()
{
    /// Algorithm (pseudo code)
    /// ->
    /// idx = -1
    /// for j=0 to height-1
    /// {
    ///     vertex[++idx] = get(0, j)
    ///     for i=0 to width
    ///     {
    ///         vertex[++idx] = get(i, j)
    ///         vertex[++idx] = get(i, j+1)
    ///     }
    ///     vertex[++idx] = get(i, j+1)
    /// }
    /// <-
    /// nbElems = ((width + 1) * 2) * (height - 1)

    int gNbRows = _common.cityMap.size().y();
    int gElemByRow = (_common.cityMap.size().x() + 2) * 2;
    _groundNbElems = gElemByRow * gNbRows;
    Vec3f* gnormals   = new Vec3f[_groundNbElems];
    Vec3f* gpositions = new Vec3f[_groundNbElems];


    int idx = -1;
    for(int j=0; j < gNbRows; ++j)
    {
        computeGroundVertex(idx, gpositions, gnormals, 0, j);

        for(int i=0; i <= _common.cityMap.size().x(); ++i)
        {
            computeGroundVertex(idx, gpositions, gnormals, i, j);
            computeGroundVertex(idx, gpositions, gnormals, i, j+1);
        }

        int lastx = _common.cityMap.size().x();
        computeGroundVertex(idx, gpositions, gnormals, lastx, j+1);
    }


    // Ground VAO setup
    glGenVertexArrays(1, &_groundVao);
    glBindVertexArray(_groundVao);

    const int nbAttributes = 2;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _groundShader.getAttributeLocation("position_att");
    int normal_loc = _groundShader.getAttributeLocation("normal_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gnormals) * _groundNbElems, gnormals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gnormals) * _groundNbElems, gpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );


    // Arrays sweaping
    delete [] gnormals;
    delete [] gpositions;

    _groundShader.pushThisProgram();
    float amplitude = cellar::max(abs(_common.ground.minHeight()),
                                  abs(_common.ground.maxHeight()));
    _groundShader.setFloat("HillsAmplitude", amplitude);
    _groundShader.popProgram();
}

void GroundComponent::computeGroundVertex(int& idx, cellar::Vec3f* pos, cellar::Vec3f* norm, int i, int j)
{
    ++idx;
    norm[idx] = _common.ground.normalAt(i, j);
    pos[idx]  = Vec3f(i, j, _common.ground.heightAt(i, j));
}

void GroundComponent::draw()
{
    _groundShader.pushThisProgram();

    glBindVertexArray(_groundVao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _groundNbElems);

    _groundShader.popProgram();
}

void GroundComponent::update()
{
    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction",   _common.viewedSunDirection);
    _groundShader.popProgram();
}

void GroundComponent::updateProjectionMatrix()
{
    _groundShader.pushThisProgram();
    _groundShader.setMatrix4x4("ProjectionMatrix", _common.projMat);
    _groundShader.popProgram();
}

void GroundComponent::updateModelViewMatrix()
{
    _groundShader.pushThisProgram();
    _groundShader.setMatrix4x4("ModelViewMatrix", _common.viewMat);
    _groundShader.setMatrix3x3("NormalMatrix",    _common.normalMat);
    _groundShader.popProgram();
}
