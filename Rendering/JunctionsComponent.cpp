#include "JunctionsComponent.h"

#include <GL3/gl3w.h>

#include <CellarWorkbench/GL/GlToolkit.h>
#include <CellarWorkbench/Image/ImageBank.h>


using namespace std;
using namespace cellar;


JunctionsComponent::JunctionsComponent(City &city, GlProgram &shader) :
    AbstractComponent(city, shader),
    _junctionsBuffs(),
    _junctionsVao(0),
    _junctionsTex(0),
    _junctionsNbElems(0)
{
    glGenVertexArrays(1, &_junctionsVao);
    glGenBuffers(_JUNCTIONS_NB_BUFFS, _junctionsBuffs);

    _junctionsTex = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/junction.bmp")
    );
}

JunctionsComponent::~JunctionsComponent()
{
    glDeleteVertexArrays(1, &_junctionsVao);
    glDeleteBuffers(_JUNCTIONS_NB_BUFFS, _junctionsBuffs);
    GlToolkit::deleteTextureId(_junctionsTex);
}

void JunctionsComponent::setup()
{
    // Collect junctions to draw
    vector<glm::vec3> positions;
    vector<glm::vec2> texCoords;

    float roadHalfWidth = _description.roadWidth * 0.5f;


    for(int j=0; j<_city.size().y+1; ++j)
    {
        for(int i=0; i<_city.size().x+1; ++i)
        {
            if(_city.junctions().get(i , j)->type() != Junction::GRASS)
            {
                float height = _ground.heightAt(i, j);
                positions.push_back(glm::vec3(i-roadHalfWidth, j-roadHalfWidth, height));
                texCoords.push_back(glm::vec2(0, 0));

                positions.push_back(glm::vec3(i+roadHalfWidth, j-roadHalfWidth, height));
                texCoords.push_back(glm::vec2(1, 0));

                positions.push_back(glm::vec3(i+roadHalfWidth, j+roadHalfWidth, height));
                texCoords.push_back(glm::vec2(1, 1));

                positions.push_back(glm::vec3(i+roadHalfWidth, j+roadHalfWidth, height));
                texCoords.push_back(glm::vec2(1, 1));

                positions.push_back(glm::vec3(i-roadHalfWidth, j+roadHalfWidth, height));
                texCoords.push_back(glm::vec2(0, 1));

                positions.push_back(glm::vec3(i-roadHalfWidth, j-roadHalfWidth, height));
                texCoords.push_back(glm::vec2(0, 0));
            }
        }
    }

    _junctionsNbElems = static_cast<int>(positions.size());


    // Setup Vao
    int position_loc = _shader.getAttributeLocation("position_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindVertexArray( _junctionsVao );

    glBindBuffer(GL_ARRAY_BUFFER, _junctionsBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _junctionsBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void JunctionsComponent::draw()
{
    _shader.pushProgram();
    glVertexAttrib3f(_shader.getAttributeLocation("normal_att"), 0,0,1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);

    glBindVertexArray(_junctionsVao);    
    glDrawArrays(GL_TRIANGLES, 0, _junctionsNbElems);

    _shader.popProgram();
}
