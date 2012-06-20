#include "GroundComponent.h"
#include "DrawCityModule.h"
#include "Land/Land.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


GroundComponent::GroundComponent(DrawCityCommonData& common) :
    _common(common),
    _groundVao(0),
    _groundNbElems(0),
    _landsVao(0),
    _landsNbElems(0),
    _roadsVao(0),
    _roadsNbElems(0),
    _trianglesVao(0),
    _trianglesNbElems(0)
{
    _common.groundShader.pushThisProgram();
    _common.groundShader.setVec4f("sun.direction", _common.sunLight.direction);
    _common.groundShader.setVec4f("sun.ambient",   _common.sunLight.ambient);
    _common.groundShader.setVec4f("sun.diffuse",   _common.sunLight.diffuse);
    _common.groundShader.setVec4f("sun.specular",  _common.sunLight.specular);
    _common.groundShader.setFloat("Shininess",     _common.groundShininess);
    _common.groundShader.setFloat("WaterHeight",   _common.ground.waterHeight());
    _common.groundShader.setVec4f("GrassColor",    _common.grassColor);
    _common.groundShader.setVec4f("MudColor",      _common.mudColor);
    _common.groundShader.setVec4f("WaterColor",    _common.waterColor);    
    _common.groundShader.popProgram();
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
    Vec2f* gtexCoords = new Vec2f[_groundNbElems];
    Vec3f* gnormals   = new Vec3f[_groundNbElems];
    Vec3f* gpositions = new Vec3f[_groundNbElems];


    int idx = -1;
    for(int j=0; j < gNbRows; ++j)
    {
        computeGroundVertex(idx, gpositions, gnormals, gtexCoords, 0, j);

        for(int i=0; i <= _common.cityMap.size().x(); ++i)
        {
            computeGroundVertex(idx, gpositions, gnormals, gtexCoords, i, j);
            computeGroundVertex(idx, gpositions, gnormals, gtexCoords, i, j+1);
        }

        int lastx = _common.cityMap.size().x();
        computeGroundVertex(idx, gpositions, gnormals, gtexCoords, lastx, j+1);
    }


    // Ground VAO setup
    glGenVertexArrays(1, &_groundVao);
    glBindVertexArray(_groundVao);

    const int nbAttributes = 3;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.groundShader.getAttributeLocation("position_att");
    int normal_loc = _common.groundShader.getAttributeLocation("normal_att");
    int texCoord_loc = _common.groundShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gnormals) * _groundNbElems, gnormals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gpositions) * _groundNbElems, gpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gtexCoords) * _groundNbElems, gtexCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );


    // Arrays sweaping
    delete [] gtexCoords;
    delete [] gnormals;
    delete [] gpositions;

    _groundTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/grass.bmp", false)
    );
}

void GroundComponent::computeGroundVertex(int& idx, cellar::Vec3f* pos, cellar::Vec3f* norm,
                                          cellar::Vec2f* tex, int i, int j)
{
    ++idx;
    norm[idx] = _common.ground.normalAt(i, j);
    pos[idx]  = Vec3f(i, j, _common.ground.heightAt(i, j));
    tex[idx]  = Vec2f(i, j);
}

void GroundComponent::setupLands()
{
    /*
    _landsNbElems = 0;
    for(int j=0; j<_common.cityMap.size().y(); ++j)
        for(int i=0; i<_common.cityMap.size().x(); ++i)
            if(_common.cityMap.lands().get(i, j)->type() == )
            */
}

void GroundComponent::setupRoads()
{
}

void GroundComponent::setupTriangles()
{
}

void GroundComponent::draw()
{
    _common.groundShader.pushThisProgram();

    glBindVertexArray(_groundVao);
    glBindTexture(GL_TEXTURE_2D, _groundTex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _groundNbElems);

    _common.groundShader.popProgram();
}

void GroundComponent::update()
{
    _common.groundShader.pushThisProgram();
    _common.groundShader.setVec4f("sun.direction",   _common.viewedSunDirection);
    _common.groundShader.popProgram();
}

void GroundComponent::updateProjectionMatrix()
{
    _common.groundShader.pushThisProgram();
    _common.groundShader.setMatrix4x4("ProjectionMatrix", _common.projMat);
    _common.groundShader.popProgram();
}

void GroundComponent::updateModelViewMatrix()
{
    _common.groundShader.pushThisProgram();
    _common.groundShader.setMatrix4x4("ModelViewMatrix", _common.viewMat);
    _common.groundShader.setMatrix3x3("NormalMatrix",    _common.normalMat);
    _common.groundShader.popProgram();
}
