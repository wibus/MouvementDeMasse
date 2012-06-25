#include "StreetsComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


StreetsComponent::StreetsComponent(DrawCityCommonData& common) :
    _common(common),
    _streetsVao(0),
    _streetsTex(0),
    _streetsNbElems(0)
{
    // Texture
    _streetsTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/street.bmp", false)
    );
}

void StreetsComponent::setup()
{
    // Compute number of street vertices
    _streetsNbElems = 0;

    for(int j=0; j<_common.cityMap.size().y()+1; ++j)
    {
        for(int i=0; i<_common.cityMap.size().x()+1; ++i)
        {
            if(_common.cityMap.junctions().get(i , j)->getStreet(EAST)!= 0x0)
                _streetsNbElems += 4;
            if(_common.cityMap.junctions().get(i , j)->getStreet(NORTH)!= 0x0)
                _streetsNbElems += 4;
        }
    }

    // Collect streets to draw
    Vec3f* streetsPos = new Vec3f[_streetsNbElems];
    Vec3f* streetsNorm = new Vec3f[_streetsNbElems];
    Vec2f* streetsTex = new Vec2f[_streetsNbElems];
    int idx = -1;

    float lengthRatio = (1-2*_common.roadWidth) / _common.roadWidth;

    for(int j=0; j<_common.cityMap.size().y()+1; ++j)
    {
        for(int i=0; i<_common.cityMap.size().x()+1; ++i)
        {
            if(_common.cityMap.junctions().get(i , j)->getStreet(EAST) != 0x0)
            {
                float startHeight = _common.ground.heightAt(i, j);
                float endHeight   = _common.ground.heightAt(i+1, j);
                Vec3f dzdx = Vec3f(_common.ground.dzdx(i+0.5, j), 0, 1).normalize();

                streetsPos[++idx] = Vec3f(i + _common.roadWidth, j-_common.roadWidth, startHeight);
                streetsNorm[idx] =  dzdx;
                streetsTex[idx] = Vec2f(0, 0);

                streetsPos[++idx] = Vec3f(i+1-_common.roadWidth, j-_common.roadWidth, endHeight);
                streetsNorm[idx] =  dzdx;
                streetsTex[idx] = Vec2f(lengthRatio, 0);

                streetsPos[++idx] = Vec3f(i+1-_common.roadWidth, j+_common.roadWidth, endHeight);
                streetsNorm[idx] =  dzdx;
                streetsTex[idx] = Vec2f(lengthRatio, 1);

                streetsPos[++idx] = Vec3f(i + _common.roadWidth, j+_common.roadWidth, startHeight);
                streetsNorm[idx] =  dzdx;
                streetsTex[idx] = Vec2f(0, 1);
            }
            if(_common.cityMap.junctions().get(i , j)->getStreet(NORTH) != 0x0)
            {
                float startHeight = _common.ground.heightAt(i, j);
                float endHeight = _common.ground.heightAt(i, j+1);
                Vec3f dzdy = Vec3f(_common.ground.dzdy(i, j+0.5), 0, 1).normalize();

                streetsPos[++idx] = Vec3f(i-_common.roadWidth, j + _common.roadWidth, startHeight);
                streetsNorm[idx] =  dzdy;
                streetsTex[idx] = Vec2f(0, 1);

                streetsPos[++idx] = Vec3f(i+_common.roadWidth, j + _common.roadWidth, startHeight);
                streetsNorm[idx] =  dzdy;
                streetsTex[idx] = Vec2f(0, 0);

                streetsPos[++idx] = Vec3f(i+_common.roadWidth, j+1-_common.roadWidth, endHeight);
                streetsNorm[idx] =  dzdy;
                streetsTex[idx] = Vec2f(lengthRatio, 0);

                streetsPos[++idx] = Vec3f(i-_common.roadWidth, j+1-_common.roadWidth, endHeight);
                streetsNorm[idx] =  dzdy;
                streetsTex[idx] = Vec2f(lengthRatio, 1);
            }
        }
    }

    // Setup Vao
    glGenVertexArrays(1, &_streetsVao);
    glBindVertexArray( _streetsVao );

    const int NB_BUFFS = 3;
    GLuint buffers[NB_BUFFS];
    glGenBuffers(NB_BUFFS, buffers);

    int position_loc = _common.infrastructShader.getAttributeLocation("position_att");
    int normal_loc   = _common.infrastructShader.getAttributeLocation("normal_att");
    int texCoord_loc = _common.infrastructShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*streetsPos) * _streetsNbElems, streetsPos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*streetsNorm) * _streetsNbElems, streetsNorm, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*streetsTex) * _streetsNbElems, streetsTex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    // Arrays sweaping
    delete [] streetsPos;
    delete [] streetsTex;
}

void StreetsComponent::draw()
{
    _common.infrastructShader.pushThisProgram();
    _common.infrastructShader.setVec3f("Translation", Vec3f());
    _common.infrastructShader.setFloat("StructureHeight", 1.0f);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _streetsTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _streetsTex);

    glBindVertexArray(_streetsVao);
    glDrawArrays(GL_QUADS, 0, _streetsNbElems);

    _common.infrastructShader.popProgram();
}

void StreetsComponent::update()
{
}

void StreetsComponent::updateProjectionMatrix()
{
}

void StreetsComponent::updateModelViewMatrix()
{
}
