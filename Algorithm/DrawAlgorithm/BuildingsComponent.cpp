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
    _roofSpec(0),
    _roofPos(),
    _apartmentTex(0),
    _apartmentSpec(0),
    _apartmentsPos(),
    _apartmentTexScaleCoeff(),
    _commerceTex(0),
    _commerceSpec(0),
    _commercePos(),
    _commerceTexScaleCoeff()
{
    _roofTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/roofTex.bmp", false)
    );

    _roofSpec = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/roofSpec.bmp", false)
    );

    _apartmentTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/apartTex.bmp", false)
    );

    _apartmentSpec = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/apartSpec.bmp", false)
    );

    _commerceTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/commerceTex.bmp", false)
    );

    _commerceSpec = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/commerceSpec.bmp", false)
    );
}

void BuildingsComponent::setup()
{
    setupPositions();
    setupBuidlindSides();
    setupRoofTop();
}

void BuildingsComponent::setupPositions()
{
    _roofPos.clear();
    _apartmentsPos.clear();
    _apartmentTexScaleCoeff.clear();
    _commercePos.clear();
    _commerceTexScaleCoeff.clear();


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
                _apartmentsPos.push_back( pos + Vec3f(0.5f, 0.5f, 0.0f) );
                _apartmentTexScaleCoeff.push_back( height );
            }

            if(_common.cityMap.lands().get(i,j)->type() == Land::COMMERCIAL)
            {
                _commercePos.push_back( pos + Vec3f(0.5f, 0.5f, 0.0f) );
                _commerceTexScaleCoeff.push_back( height );
            }
        }
    }
}

void BuildingsComponent::setupBuidlindSides()
{
    _buildingNbElems = 16;
    float halfWidth = 0.5f-_common.roadWidth;

    Vec3f positions[16];
    Vec3f normals[16];
    Vec2f texCoords[16];


    // South
    positions[0] = Vec3f(-halfWidth, -halfWidth, 0.0f);
    normals[0]   = Vec3f(0.0f,       -1.0f,      0.0f);
    texCoords[0] = Vec2f(0.0f,       0.0f);

    positions[1] = Vec3f(+halfWidth, -halfWidth, 0.0f);
    normals[1]   = Vec3f(0.0f,       -1.0f,      0.0f);
    texCoords[1] = Vec2f(1.0f,       0.0f);

    positions[2] = Vec3f(+halfWidth, -halfWidth, 1.0f);
    normals[2]   = Vec3f(0.0f,       -1.0f,      0.0f);
    texCoords[2] = Vec2f(1.0f,       1.0f);

    positions[3] = Vec3f(-halfWidth, -halfWidth, 1.0f);
    normals[3]   = Vec3f(0.0f,       -1.0f,      0.0f);
    texCoords[3] = Vec2f(0.0f,       1.0f);


    // East
    positions[4] = Vec3f(+halfWidth, -halfWidth, 0.0f);
    normals[4]   = Vec3f(1.0f,       0.0f,      0.0f);
    texCoords[4] = Vec2f(0.0f,       0.0f);

    positions[5] = Vec3f(+halfWidth, +halfWidth, 0.0f);
    normals[5]   = Vec3f(1.0f,       0.0f,      0.0f);
    texCoords[5] = Vec2f(1.0f,       0.0f);

    positions[6] = Vec3f(+halfWidth, +halfWidth, 1.0f);
    normals[6]   = Vec3f(1.0f,       0.0f,      0.0f);
    texCoords[6] = Vec2f(1.0f,       1.0f);

    positions[7] = Vec3f(+halfWidth, -halfWidth, 1.0f);
    normals[7]   = Vec3f(1.0f,       0.0f,      0.0f);
    texCoords[7] = Vec2f(0.0f,       1.0f);


    // North
    positions[8] = Vec3f(+halfWidth, +halfWidth, 0.0f);
    normals[8]   = Vec3f(0.0f,       +1.0f,      0.0f);
    texCoords[8] = Vec2f(0.0f,       0.0f);

    positions[9] = Vec3f(-halfWidth, +halfWidth, 0.0f);
    normals[9]   = Vec3f(0.0f,       +1.0f,      0.0f);
    texCoords[9] = Vec2f(1.0f,       0.0f);

    positions[10] = Vec3f(-halfWidth, +halfWidth, 1.0f);
    normals[10]   = Vec3f(0.0f,       +1.0f,      0.0f);
    texCoords[10] = Vec2f(1.0f,       1.0f);

    positions[11] = Vec3f(+halfWidth, +halfWidth, 1.0f);
    normals[11]   = Vec3f(0.0f,       +1.0f,      0.0f);
    texCoords[11] = Vec2f(0.0f,       1.0f);


    // West
    positions[12] = Vec3f(-halfWidth, +halfWidth, 0.0f);
    normals[12]   = Vec3f(-1.0f,      0.0f,      0.0f);
    texCoords[12] = Vec2f(0.0f,       0.0f);

    positions[13] = Vec3f(-halfWidth, -halfWidth, 0.0f);
    normals[13]   = Vec3f(-1.0f,      0.0f,      0.0f);
    texCoords[13] = Vec2f(1.0f,       0.0f);

    positions[14] = Vec3f(-halfWidth, -halfWidth, 1.0f);
    normals[14]   = Vec3f(-1.0f,      0.0f,      0.0f);
    texCoords[14] = Vec2f(1.0f,       1.0f);

    positions[15] = Vec3f(-halfWidth, +halfWidth, 1.0f);
    normals[15]   = Vec3f(-1.0f,      0.0f,      0.0f);
    texCoords[15] = Vec2f(0.0f,       1.0f);


    // Ground VAO setup
    glGenVertexArrays(1, &_buildingWallsVao);
    glBindVertexArray(_buildingWallsVao);

    const int nbAttributes = 3;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.infrastructShader.getAttributeLocation("position_att");
    int normal_loc   = _common.infrastructShader.getAttributeLocation("normal_att");
    int texCoord_loc = _common.infrastructShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _buildingNbElems, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*normals) * _buildingNbElems, normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[2]);
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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _roofSpec);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _roofTex);

    glBindVertexArray(_roofVao);
    glVertexAttrib3f(_common.infrastructShader.getAttributeLocation("normal_att"), 0,0,1);
    _common.infrastructShader.setFloat("StructureHeight", 1.0f);
    for(size_t i=0; i<_roofPos.size(); ++i)
    {
        _common.infrastructShader.setVec3f("Translation", _roofPos[i] );
        glDrawArrays(GL_TRIANGLE_FAN , 0, _roofNbElems);
    }


    // Residential
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _apartmentSpec);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _apartmentTex);

    glBindVertexArray(_buildingWallsVao);
    for(size_t i=0; i<_apartmentsPos.size(); ++i)
    {
        _common.infrastructShader.setVec3f("Translation", _apartmentsPos[i]);
        _common.infrastructShader.setFloat("StructureHeight", _apartmentTexScaleCoeff[i]);
        glDrawArrays(GL_QUADS, 0, _buildingNbElems);
    }


    // Commercial
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _commerceSpec);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _commerceTex);

    glBindVertexArray(_buildingWallsVao);
    for(size_t i=0; i<_commercePos.size(); ++i)
    {
        _common.infrastructShader.setVec3f("Translation", _commercePos[i]);
        _common.infrastructShader.setFloat("StructureHeight", _commerceTexScaleCoeff[i]);
        glDrawArrays(GL_QUADS, 0, _buildingNbElems);
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
