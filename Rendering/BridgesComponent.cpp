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


}

void BridgesComponent::draw()
{
}
