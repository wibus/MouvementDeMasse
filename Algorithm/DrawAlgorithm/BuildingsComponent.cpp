#include "BuildingsComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;

BuildingsComponent::BuildingsComponent(DrawCityCommonData& common) :
    _common(common),
    _buildingWallsVao(0),
    _buildingNbElems(0),
    _roofVao(0),
    _roofNbElems(),
    _roofTex(0),
    _roofPos(),
    _apartmentTex(0),
    _apartmentsPos(),
    _apartmentTexScaleCoeff(),
    _commercesTex(0),
    _commercesPos(),
    _commercesTexScaleCoeff()
{
}

void BuildingsComponent::setup()
{
    _roofTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/roof.bmp", false)
    );

    _apartmentTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/apartSides.bmp", false)
    );

    _commercesTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/commerceSides.bmp", false)
    );

    setupPositions();
    setupBuidlindSides();
    setupRoofTop();
}

void BuildingsComponent::setupPositions()
{
    _roofPos.clear();
    _apartmentsPos.clear();
    _commercesPos.clear();


    for(int j=0; j<_common.cityMap.size().y(); ++j)
    {
        for(int i=0; i<_common.cityMap.size().x(); ++i)
        {
            if(_common.cityMap.lands().get(i,j)->type() == Land::GRASS)
                continue;

            Vec3f pos = Vec3f(i, j, landHeight(i, j));
            float height = _common.cityMap.lands().get(i,j)->nbStories() * 0.25f;

            _roofPos.push_back(Vec3f(pos.x(), pos.y(), pos.z() + height));

            if(_common.cityMap.lands().get(i,j)->type() == Land::RESIDENTIAL)
            {
                _apartmentsPos.push_back( pos );
                _apartmentTexScaleCoeff.push_back( height );
            }

            if(_common.cityMap.lands().get(i,j)->type() == Land::COMMERCIAL)
            {
                _commercesPos.push_back( pos );
                _commercesTexScaleCoeff.push_back( height );
            }
        }
    }
}

void BuildingsComponent::setupBuidlindSides()
{
    _buildingNbElems = 10;

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
    glGenVertexArrays(1, &_buildingWallsVao);
    glBindVertexArray(_buildingWallsVao);

    const int nbAttributes = 2;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.infrastructShader.getAttributeLocation("position_att");
    int texCoord_loc = _common.infrastructShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _buildingNbElems, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*texCoords) * _buildingNbElems, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void BuildingsComponent::setupRoofTop()
{
    _roofNbElems = 4;

    Vec3f positions[4] =
    {
        Vec3f(_common.roadWidth,   _common.roadWidth,   0),
        Vec3f(1-_common.roadWidth, _common.roadWidth,   0),
        Vec3f(1-_common.roadWidth, 1-_common.roadWidth, 0),
        Vec3f(_common.roadWidth,   1-_common.roadWidth, 0)
    };

    Vec2f texCoords[4] =
    {
        Vec2f(0,0),
        Vec2f(1,0),
        Vec2f(1,1),
        Vec2f(0,1)
    };


    // Ground VAO setup
    glGenVertexArrays(1, &_roofVao);
    glBindVertexArray(_roofVao);

    const int nbAttributes = 2;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.infrastructShader.getAttributeLocation("position_att");
    int texCoord_loc = _common.infrastructShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _roofNbElems, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*texCoords) * _roofNbElems, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

float BuildingsComponent::landHeight(int i, int j)
{
    return cellar::min(cellar::min(cellar::min(
        _common.ground.heightAt(i,   j),
        _common.ground.heightAt(i+1, j)),
        _common.ground.heightAt(i+1, j+1)),
        _common.ground.heightAt(i,   j+1)
    );
}

void BuildingsComponent::draw()
{
    _common.infrastructShader.pushThisProgram();

    // Roof tops
    glBindVertexArray(_roofVao);
    glBindTexture(GL_TEXTURE_2D, _roofTex);
    _common.infrastructShader.setFloat("StructureHeight", 1.0f);
    for(size_t i=0; i<_roofPos.size(); ++i)
    {
        _common.infrastructShader.setVec3f("Translation", _roofPos[i] );
        glDrawArrays(GL_TRIANGLE_FAN , 0, _roofNbElems);
    }

    // Residential
    glBindVertexArray(_buildingWallsVao);
    glBindTexture(GL_TEXTURE_2D, _apartmentTex);
    for(size_t i=0; i<_apartmentsPos.size(); ++i)
    {
        _common.infrastructShader.setVec3f("Translation", _apartmentsPos[i]);
        _common.infrastructShader.setFloat("StructureHeight", _apartmentTexScaleCoeff[i]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _buildingNbElems);
    }

    // Commercial
    glBindVertexArray(_buildingWallsVao);
    glBindTexture(GL_TEXTURE_2D, _commercesTex);
    for(size_t i=0; i<_commercesPos.size(); ++i)
    {
        _common.infrastructShader.setVec3f("Translation", _commercesPos[i]);
        _common.infrastructShader.setFloat("StructureHeight", _commercesTexScaleCoeff[i]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _buildingNbElems);
    }

    _common.infrastructShader.popProgram();
}

void BuildingsComponent::update()
{
}

void BuildingsComponent::updateProjectionMatrix()
{
}

void BuildingsComponent::updateModelViewMatrix()
{
}
