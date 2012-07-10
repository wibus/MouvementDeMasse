#include "BridgesComponent.h"

#include <GL/glew.h>
using namespace std;

#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


BridgesComponent::BridgesComponent(City& city, cellar::GLShaderProgram& shader) :
    AbstractComponent(city, shader),
    _bridgeVao(0),
    _bridgeTex(0),
    _bridgeNbElems(0)
{
    glGenVertexArrays(1, &_bridgeVao);

    _bridgeTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/street.bmp", false)
    );
}

BridgesComponent::~BridgesComponent()
{
    glDeleteVertexArrays(1, &_bridgeVao);
    GLToolkit::deleteTextureId(_bridgeTex);
}

void BridgesComponent::setup()
{
    vector<Vec3f> positions;

    for(size_t b=0; b<_city.bridges().size(); ++b)
    {
        Vec2f endA = _city.bridges()[b].endA();
        Vec2f endB = _city.bridges()[b].endB();

        positions.push_back(Vec3f(endA.x(), endA.y(), _ground.heightAt(_city.bridges()[b].endA())));
        positions.push_back(Vec3f(endB.x(), endB.y(), _ground.heightAt(_city.bridges()[b].endB())));
    }

    _bridgeNbElems = positions.size();

    // Setup Vao
    glBindVertexArray( _bridgeVao );

    const int NB_BUFFS = 1;
    GLuint buffers[NB_BUFFS];
    glGenBuffers(NB_BUFFS, buffers);

    int position_loc = _shader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void BridgesComponent::draw()
{
    _shader.pushThisProgram();

    glBindVertexArray(_bridgeVao);
    glDrawArrays(GL_LINES, 0, _bridgeNbElems);
    glBindVertexArray(0);

    _shader.popProgram();
}
