#include "StreetsComponent.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


StreetsComponent::StreetsComponent(City &city, GLShaderProgram &shader) :
    AbstractComponent(city, shader),
    _streetsVao(0),
    _streetsTex(0),
    _streetsNbElems(0)
{
    glGenVertexArrays(1, &_streetsVao);

    _streetsTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/street.bmp", false)
    );
}

StreetsComponent::~StreetsComponent()
{
    glDeleteVertexArrays(1, &_streetsVao);
    GLToolkit::deleteTextureId(_streetsTex);
}

void StreetsComponent::setup()
{
    // Compute number of street vertices
    _streetsNbElems = 0;

    for(int j=0; j<_city.size().y()+1; ++j)
    {
        for(int i=0; i<_city.size().x()+1; ++i)
        {
            if(_city.junctions().get(i , j)->getStreet(EAST)!= 0x0)
                _streetsNbElems += 4;
            if(_city.junctions().get(i , j)->getStreet(NORTH)!= 0x0)
                _streetsNbElems += 4;
        }
    }

    // Collect streets to draw
    Vec3f* streetsPos = new Vec3f[_streetsNbElems];
    Vec3f* streetsNorm = new Vec3f[_streetsNbElems];
    Vec2f* streetsTex = new Vec2f[_streetsNbElems];
    int idx = -1;

    float lengthRatio = (1-2*_visual.roadWidth) / _visual.roadWidth;

    for(int j=0; j<_city.size().y()+1; ++j)
    {
        for(int i=0; i<_city.size().x()+1; ++i)
        {
            if(_city.junctions().get(i , j)->getStreet(EAST) != 0x0)
            {
                float startHeight = _ground.heightAt(i, j);
                float endHeight   = _ground.heightAt(i+1, j);
                Vec3f dzdx = Vec3f(_ground.dzdx(i+0.5, j), 0, 1).normalize();

                streetsPos[++idx] = Vec3f(i + _visual.roadWidth, j-_visual.roadWidth, startHeight);
                streetsNorm[idx] =  dzdx;
                streetsTex[idx] = Vec2f(0, 0);

                streetsPos[++idx] = Vec3f(i+1-_visual.roadWidth, j-_visual.roadWidth, endHeight);
                streetsNorm[idx] =  dzdx;
                streetsTex[idx] = Vec2f(lengthRatio, 0);

                streetsPos[++idx] = Vec3f(i+1-_visual.roadWidth, j+_visual.roadWidth, endHeight);
                streetsNorm[idx] =  dzdx;
                streetsTex[idx] = Vec2f(lengthRatio, 1);

                streetsPos[++idx] = Vec3f(i + _visual.roadWidth, j+_visual.roadWidth, startHeight);
                streetsNorm[idx] =  dzdx;
                streetsTex[idx] = Vec2f(0, 1);
            }
            if(_city.junctions().get(i , j)->getStreet(NORTH) != 0x0)
            {
                float startHeight = _ground.heightAt(i, j);
                float endHeight = _ground.heightAt(i, j+1);
                Vec3f dzdy = Vec3f(_ground.dzdy(i, j+0.5), 0, 1).normalize();

                streetsPos[++idx] = Vec3f(i-_visual.roadWidth, j + _visual.roadWidth, startHeight);
                streetsNorm[idx] =  dzdy;
                streetsTex[idx] = Vec2f(0, 1);

                streetsPos[++idx] = Vec3f(i+_visual.roadWidth, j + _visual.roadWidth, startHeight);
                streetsNorm[idx] =  dzdy;
                streetsTex[idx] = Vec2f(0, 0);

                streetsPos[++idx] = Vec3f(i+_visual.roadWidth, j+1-_visual.roadWidth, endHeight);
                streetsNorm[idx] =  dzdy;
                streetsTex[idx] = Vec2f(lengthRatio, 0);

                streetsPos[++idx] = Vec3f(i-_visual.roadWidth, j+1-_visual.roadWidth, endHeight);
                streetsNorm[idx] =  dzdy;
                streetsTex[idx] = Vec2f(lengthRatio, 1);
            }
        }
    }

    // Setup Vao
    glBindVertexArray( _streetsVao );

    const int NB_BUFFS = 3;
    GLuint buffers[NB_BUFFS];
    glGenBuffers(NB_BUFFS, buffers);

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

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
    _shader.pushThisProgram();
    _shader.setVec3f("Translation", Vec3f());
    _shader.setFloat("StructureHeight", 1.0f);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _streetsTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _streetsTex);

    glBindVertexArray(_streetsVao);
    glDrawArrays(GL_QUADS, 0, _streetsNbElems);

    _shader.popProgram();
}
