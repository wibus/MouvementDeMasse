#include "BuildingsComponent.h"

#include <GL3/gl3w.h>
using namespace std;

#include <CellarWorkbench/Misc/CellarUtils.h>
using namespace cellar;

#include <MediaWorkbench/GL/GlToolkit.h>
#include <MediaWorkbench/Image/ImageBank.h>
using namespace media;


BuildingsComponent::BuildingsComponent(City &city, GlProgram &shader) :
    AbstractComponent(city, shader),
    _buildingBuffs(),
    _buildingWallsVao(0),
    _buildingNbElems(16),
    _roofBuffs(),
    _roofVao(0),
    _roofNbElems(),
    _roofTex(0),
    _roofSpec(0),
    _roofPos(),
    _apartmentTex(0),
    _apartmentSpec(0),
    _apartmentPos(),
    _apartmentNbStories(),
    _commerceTex(0),
    _commerceSpec(0),
    _commercePos(),
    _commerceNbStories()
{    
    glGenVertexArrays(1, &_buildingWallsVao);
    glGenBuffers(_BUILDING_NB_BUFFS, _buildingBuffs);

    glGenVertexArrays(1, &_roofVao);
    glGenBuffers(_ROOF_NB_BUFFS, _roofBuffs);


    _roofTex = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/roofTex.bmp")
    );

    _roofSpec = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/roofSpec.bmp")
    );

    _apartmentTex = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/apartTex.bmp")
    );

    _apartmentSpec = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/apartSpec.bmp")
    );

    _commerceTex = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/commerceTex.bmp")
    );

    _commerceSpec = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/commerceSpec.bmp")
    );
}

BuildingsComponent::~BuildingsComponent()
{
    glDeleteVertexArrays(1, &_buildingWallsVao);
    glDeleteBuffers(_BUILDING_NB_BUFFS, _buildingBuffs);
    glDeleteVertexArrays(1, &_roofVao);
    glDeleteBuffers(_ROOF_NB_BUFFS, _roofBuffs);

    GlToolkit::deleteTextureId(_roofTex);
    GlToolkit::deleteTextureId(_roofSpec);
    GlToolkit::deleteTextureId(_apartmentTex);
    GlToolkit::deleteTextureId(_apartmentSpec);
    GlToolkit::deleteTextureId(_commerceTex);
    GlToolkit::deleteTextureId(_commerceSpec);
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
    _apartmentPos.clear();
    _apartmentNbStories.clear();
    _commercePos.clear();
    _commerceNbStories.clear();


    for(int j=0; j<_city.size().y; ++j)
    {
        for(int i=0; i<_city.size().x; ++i)
        {
            if(_city.lands().get(i,j)->type() == Land::GRASS)
                continue;

            glm::vec3 pos = glm::vec3(i, j, _ground.landLowerCornerAt(i, j));
            float height = _city.lands().get(i,j)->nbStories() * _description.storyHeight;

            _roofPos.push_back(glm::vec3(pos.x, pos.y, pos.z + height));

            if(_city.lands().get(i,j)->type() == Land::RESIDENTIAL)
            {
                _apartmentPos.push_back( pos + glm::vec3(0.5f, 0.5f, 0.0f) );
                _apartmentNbStories.push_back( _city.lands().get(i,j)->nbStories() );
            }

            if(_city.lands().get(i,j)->type() == Land::COMMERCIAL)
            {
                _commercePos.push_back( pos + glm::vec3(0.5f, 0.5f, 0.0f) );
                _commerceNbStories.push_back( _city.lands().get(i,j)->nbStories() );
            }
        }
    }
}

void BuildingsComponent::setupBuidlindSides()
{
    float halfWidth = 0.5f-_description.roadWidth*0.5f;

    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<glm::vec2> texCoords;


    for(int s=0; s <= Land::maxNbStories(); ++s)
    {
        // South
        positions.push_back(glm::vec3(-halfWidth, -halfWidth, s*_description.storyHeight));
        normals.push_back(  glm::vec3(0.0f,       -1.0f,      0.0f));
        texCoords.push_back(glm::vec2(0.0f,       s / 4.0f));

        positions.push_back(glm::vec3(+halfWidth, -halfWidth, s*_description.storyHeight));
        normals.push_back(  glm::vec3(0.0f,       -1.0f,      0.0f));
        texCoords.push_back(glm::vec2(1.0f,       s / 4.0f));

        // East
        positions.push_back(glm::vec3(+halfWidth, -halfWidth, s*_description.storyHeight));
        normals.push_back(  glm::vec3(+1.0f,      0.0f,       0.0f));
        texCoords.push_back(glm::vec2(0.0f,       s / 4.0f));

        positions.push_back(glm::vec3(+halfWidth, +halfWidth, s*_description.storyHeight));
        normals.push_back(  glm::vec3(+1.0f,      0.0f,       0.0f));
        texCoords.push_back(glm::vec2(1.0f,       s / 4.0f));

        // North
        positions.push_back(glm::vec3(+halfWidth, +halfWidth, s*_description.storyHeight));
        normals.push_back(  glm::vec3(0.0f,       +1.0f,      0.0f));
        texCoords.push_back(glm::vec2(0.0f,       s / 4.0f));

        positions.push_back(glm::vec3(-halfWidth, +halfWidth, s*_description.storyHeight));
        normals.push_back(  glm::vec3(0.0f,       +1.0f,      0.0f));
        texCoords.push_back(glm::vec2(1.0f,       s / 4.0f));

        // West
        positions.push_back(glm::vec3(-halfWidth, +halfWidth, s*_description.storyHeight));
        normals.push_back(  glm::vec3(-1.0f,      0.0f,       0.0f));
        texCoords.push_back(glm::vec2(0.0f,       s / 4.0f));

        positions.push_back(glm::vec3(-halfWidth, -halfWidth, s*_description.storyHeight));
        normals.push_back(  glm::vec3(-1.0f,      0.0f,       0.0f));
        texCoords.push_back(glm::vec2(1.0f,       s / 4.0f));


        // Primitive assembly for the a building of 's' number of stories
        _buildingIndices.push_back( vector<unsigned int>() );
        for(int w=0; w<4; ++w)
        {
            _buildingIndices.back().push_back(w*2);
            _buildingIndices.back().push_back(w*2+1);
            _buildingIndices.back().push_back(w*2+1 + s*8);
            _buildingIndices.back().push_back(w*2+1 + s*8);
            _buildingIndices.back().push_back(w*2   + s*8);
            _buildingIndices.back().push_back(w*2);
        }
    }


    // VAO setup
    glBindVertexArray(_buildingWallsVao);

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, _buildingBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _buildingBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _buildingBuffs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void BuildingsComponent::setupRoofTop()
{
    _roofNbElems = 4;

    float roadHalfWidth = _description.roadWidth * 0.5f;

    glm::vec3 positions[4] =
    {
        glm::vec3(roadHalfWidth,   roadHalfWidth,   0),
        glm::vec3(1-roadHalfWidth, roadHalfWidth,   0),
        glm::vec3(1-roadHalfWidth, 1-roadHalfWidth, 0),
        glm::vec3(roadHalfWidth,   1-roadHalfWidth, 0)
    };

    glm::vec2 texCoords[4] =
    {
        glm::vec2(0,0),
        glm::vec2(1,0),
        glm::vec2(1,1),
        glm::vec2(0,1)
    };


    // Ground VAO setup
    glBindVertexArray(_roofVao);

    int position_loc = _shader.getAttributeLocation("position_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, _roofBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _roofNbElems, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _roofBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*texCoords) * _roofNbElems, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void BuildingsComponent::draw()
{
    _shader.pushProgram();


    // Residential
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _apartmentSpec);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _apartmentTex);

    glBindVertexArray(_buildingWallsVao);

    _shader.setVec2f("RepeatFrom", glm::vec2(0.5f, 0.5f));
    for(size_t i=0; i<_apartmentPos.size(); ++i)
    {
        _shader.setVec3f("Translation", _apartmentPos[i]);
        glDrawElements(
            GL_TRIANGLES, 
            static_cast<GLsizei>(_buildingIndices[0].size()), 
            GL_UNSIGNED_INT,
            _buildingIndices[_apartmentNbStories[i]].data());
    }
    _shader.setVec3f("Translation", glm::vec3(0.0f, 0.0f, 0.0f));
    _shader.setVec2f("RepeatFrom", glm::vec2(1.0f, 1.0f));


    // Commercial
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _commerceSpec);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _commerceTex);

    glBindVertexArray(_buildingWallsVao);

    _shader.setVec2f("RepeatFrom", glm::vec2(0.5f, 0.5f));
    for(size_t i=0; i<_commercePos.size(); ++i)
    {
        _shader.setVec3f("Translation", _commercePos[i]);
        glDrawElements(GL_TRIANGLES, 
            static_cast<GLsizei>(_buildingIndices[0].size()), 
            GL_UNSIGNED_INT,
            _buildingIndices[_commerceNbStories[i]].data());
    }
    _shader.setVec3f("Translation", glm::vec3(0.0f, 0.0f, 0.0f));
    _shader.setVec2f("RepeatFrom", glm::vec2(1.0f, 1.0f));


    // Roof tops
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _roofSpec);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _roofTex);

    glBindVertexArray(_roofVao);
    glVertexAttrib3f(_shader.getAttributeLocation("normal_att"), 0,0,1);
    for(size_t i=0; i<_roofPos.size(); ++i)
    {
        _shader.setVec3f("Translation", _roofPos[i] );
        glDrawArrays(GL_TRIANGLE_FAN , 0, _roofNbElems);
    }
    _shader.setVec3f("Translation", glm::vec3(0.0f, 0.0f, 0.0f));

    _shader.popProgram();
}
