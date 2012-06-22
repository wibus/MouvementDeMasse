#include "ResidentialComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
#include <MathsAndPhysics/MatrixStack.h>
using namespace cellar;

ResidentialComponent::ResidentialComponent(DrawCityCommonData& common) :
    _common(common),
    _apartSidesVao(0),
    _apartSidesTex(0),
    _apartSidesNbElems(0),
    _apartTopVao(0),
    _apartTopTex(0),
    _apartTopNbElems(),
    _apartmentPos()
{
}

void ResidentialComponent::setup()
{
    setupApartSides();
    setupApartTop();

    for(int j=0; j<_common.cityMap.size().y(); ++j)
        for(int i=0; i<_common.cityMap.size().x(); ++i)
            if(_common.cityMap.lands().get(i,j)->type() == Land::RESIDENTIAL)
                _apartmentPos.push_back(Vec3f(i, j, landHeight(i, j)));
}

void ResidentialComponent::setupApartSides()
{
    _apartSidesTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/apartSides.bmp", false)
    );

    _apartSidesNbElems = 10;

    Vec3f positions[10];
    Vec2f texCoords[10];

    Vec2f dir(1-2*_common.roadWidth, 0);
    Vec2f pos(_common.roadWidth, _common.roadWidth);
    for(int i=0; i<5; ++i)
    {
        positions[i*2]   = Vec3f(pos.x(), pos.y(), 1);
        positions[i*2+1] = Vec3f(pos.x(), pos.y(), 0);

        texCoords[i*2]   = Vec2f(i, 1);
        texCoords[i*2+1] = Vec2f(i, 0);

        pos += dir;
        dir.rotateQuarterCCW();
    }

    // Ground VAO setup
    glGenVertexArrays(1, &_apartSidesVao);
    glBindVertexArray(_apartSidesVao);

    const int nbAttributes = 2;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.roadsShader.getAttributeLocation("position_att");
    int texCoord_loc = _common.roadsShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _apartSidesNbElems, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*texCoords) * _apartSidesNbElems, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void ResidentialComponent::setupApartTop()
{
    _apartTopTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/apartTop.bmp", false)
    );

    _apartTopNbElems = 4;

    Vec3f positions[4] =
    {
        Vec3f(_common.roadWidth,   _common.roadWidth,   1),
        Vec3f(1-_common.roadWidth, _common.roadWidth,   1),
        Vec3f(1-_common.roadWidth, 1-_common.roadWidth, 1),
        Vec3f(_common.roadWidth,   1-_common.roadWidth, 1)
    };

    Vec2f texCoords[4] =
    {
        Vec2f(0,0),
        Vec2f(1,0),
        Vec2f(1,1),
        Vec2f(0,1)
    };


    // Ground VAO setup
    glGenVertexArrays(1, &_apartTopVao);
    glBindVertexArray(_apartTopVao);

    const int nbAttributes = 2;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.roadsShader.getAttributeLocation("position_att");
    int texCoord_loc = _common.roadsShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _apartTopNbElems, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*texCoords) * _apartTopNbElems, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

float ResidentialComponent::landHeight(int i, int j)
{
    return cellar::min(cellar::min(cellar::min(
        _common.ground.heightAt(i,   j),
        _common.ground.heightAt(i+1, j)),
        _common.ground.heightAt(i+1, j+1)),
        _common.ground.heightAt(i,   j+1)
    );
}

void ResidentialComponent::draw()
{
    _common.roadsShader.pushThisProgram();
    glVertexAttrib3f(_common.roadsShader.getAttributeLocation("normal_att"), 0, 1, 1);

    MatrixStack< Matrix4x4<float> > matStack(_common.viewMat);
    for(size_t i=0; i<_apartmentPos.size(); ++i)
    {
        matStack.push();
        matStack.mult(Matrix4x4<float>().translate(_apartmentPos[i].x(), _apartmentPos[i].y(), _apartmentPos[i].z()));

        _common.roadsShader.setMatrix4x4("ModelViewMatrix", matStack.top() );
        glBindVertexArray(_apartSidesVao);
        glBindTexture(GL_TEXTURE_2D, _apartSidesTex);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _apartSidesNbElems);

        glBindVertexArray(_apartTopVao);
        glBindTexture(GL_TEXTURE_2D, _apartTopTex);
        glDrawArrays(GL_TRIANGLE_FAN, 0, _apartTopNbElems);

        matStack.pop();
    }

    _common.roadsShader.setMatrix4x4("ModelViewMatrix", matStack.top() );
    _common.roadsShader.popProgram();
}

void ResidentialComponent::update()
{
    _common.roadsShader.pushThisProgram();
    _common.roadsShader.setVec4f("sun.direction",   _common.viewedSunDirection);
    _common.roadsShader.popProgram();
}

void ResidentialComponent::updateProjectionMatrix()
{
    _common.roadsShader.pushThisProgram();
    _common.roadsShader.setMatrix4x4("ProjectionMatrix", _common.projMat);
    _common.roadsShader.popProgram();
}

void ResidentialComponent::updateModelViewMatrix()
{
    _common.roadsShader.pushThisProgram();
    _common.roadsShader.setMatrix4x4("ModelViewMatrix", _common.viewMat);
    _common.roadsShader.setMatrix3x3("NormalMatrix",    _common.normalMat);
    _common.roadsShader.popProgram();
}
