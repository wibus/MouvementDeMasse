#include "StreetsComponent.h"

using namespace std;

#include <CellarWorkbench/Misc/CellarUtils.h>
using namespace cellar;

#include <GL3/gl3w.h>
#include <MediaWorkbench/GL/GlToolkit.h>
#include <MediaWorkbench/Image/ImageBank.h>
using namespace media;


StreetsComponent::StreetsComponent(City &city, GlProgram &shader) :
    AbstractComponent(city, shader),
    _streetsBuffs(),
    _streetsVao(0),
    _streetsTex(0),
    _streetsNbElems(0)
{
    glGenVertexArrays(1, &_streetsVao);
    glGenBuffers(_STREETS_NB_BUFFS, _streetsBuffs);

    _streetsTex = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/street.bmp")
    );
}

StreetsComponent::~StreetsComponent()
{
    glDeleteVertexArrays(1, &_streetsVao);
    glDeleteBuffers(_STREETS_NB_BUFFS, _streetsBuffs);
    GlToolkit::deleteTextureId(_streetsTex);
}

void StreetsComponent::setup()
{
    // Collect streets to draw
    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<glm::vec2> texCoords;

    float roadHalfWidth = _description.roadWidth * 0.5f;
    float lengthRatio = (1-2*_description.roadWidth) / roadHalfWidth;


    for(int j=0; j<_city.size().y+1; ++j)
    {
        for(int i=0; i<_city.size().x+1; ++i)
        {
            if(_city.junctions().get(i , j)->getStreet(EAST) != 0x0)
            {
                float startHeight = _ground.heightAt(i, j);
                float endHeight   = _ground.heightAt(i+1, j);
                glm::vec3 dzdx = glm::normalize(glm::vec3(_ground.dzdx(i+0.5, j), 0, 1));

                positions.push_back(glm::vec3(i + roadHalfWidth, j-roadHalfWidth, startHeight));
                normals.push_back(dzdx);
                texCoords.push_back(glm::vec2(0, 0));

                positions.push_back(glm::vec3(i+1-roadHalfWidth, j-roadHalfWidth, endHeight));
                normals.push_back(dzdx);
                texCoords.push_back(glm::vec2(lengthRatio, 0));

                positions.push_back(glm::vec3(i+1-roadHalfWidth, j+roadHalfWidth, endHeight));
                normals.push_back(dzdx);
                texCoords.push_back(glm::vec2(lengthRatio, 1));

                positions.push_back(glm::vec3(i+1-roadHalfWidth, j+roadHalfWidth, endHeight));
                normals.push_back(dzdx);
                texCoords.push_back(glm::vec2(lengthRatio, 1));

                positions.push_back(glm::vec3(i + roadHalfWidth, j+roadHalfWidth, startHeight));
                normals.push_back(dzdx);
                texCoords.push_back(glm::vec2(0, 1));

                positions.push_back(glm::vec3(i + roadHalfWidth, j-roadHalfWidth, startHeight));
                normals.push_back(dzdx);
                texCoords.push_back(glm::vec2(0, 0));
            }
            if(_city.junctions().get(i , j)->getStreet(NORTH) != 0x0)
            {
                float startHeight = _ground.heightAt(i, j);
                float endHeight = _ground.heightAt(i, j+1);
                glm::vec3 dzdy = glm::normalize(glm::vec3(_ground.dzdy(i, j+0.5), 0, 1));

                positions.push_back(glm::vec3(i-roadHalfWidth, j + roadHalfWidth, startHeight));
                normals.push_back(dzdy);
                texCoords.push_back(glm::vec2(0, 1));

                positions.push_back(glm::vec3(i+roadHalfWidth, j + roadHalfWidth, startHeight));
                normals.push_back(dzdy);
                texCoords.push_back(glm::vec2(0, 0));

                positions.push_back(glm::vec3(i+roadHalfWidth, j+1-roadHalfWidth, endHeight));
                normals.push_back(dzdy);
                texCoords.push_back(glm::vec2(lengthRatio, 0));

                positions.push_back(glm::vec3(i+roadHalfWidth, j+1-roadHalfWidth, endHeight));
                normals.push_back(dzdy);
                texCoords.push_back(glm::vec2(lengthRatio, 0));

                positions.push_back(glm::vec3(i-roadHalfWidth, j+1-roadHalfWidth, endHeight));
                normals.push_back(dzdy);
                texCoords.push_back(glm::vec2(lengthRatio, 1));

                positions.push_back(glm::vec3(i-roadHalfWidth, j + roadHalfWidth, startHeight));
                normals.push_back(dzdy);
                texCoords.push_back(glm::vec2(0, 1));
            }
        }
    }

    _streetsNbElems = static_cast<int>(positions.size());


    // Setup Vao
    glBindVertexArray( _streetsVao );

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, _streetsBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _streetsBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _streetsBuffs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void StreetsComponent::draw()
{
    _shader.pushProgram();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _streetsTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _streetsTex);

    glBindVertexArray(_streetsVao);
    glDrawArrays(GL_TRIANGLES, 0, _streetsNbElems);

    _shader.popProgram();
}
