#ifndef DRAWCITYMODULE_H
#define DRAWCITYMODULE_H

#include <vector>

#include <CellarWorkbench/GL/GlProgram.h>

class City;
class Ground;
class Description;

class AbstractComponent;
class SkyComponent;
class GroundComponent;
class WaterComponent;
class CitizensComponent;
class JunctionsComponent;
class StreetsComponent;
class BuildingsComponent;
class BridgesComponent;


class DrawCityModule
{
public:
    DrawCityModule();
    ~DrawCityModule();

    void setup(City& city);
    void draw();
    void update();

    void updateProjectionMatrix(const glm::mat4& proj);
    void updateModelViewMatrix(const glm::mat4& view);

protected:
    void setupShaders();
    void updateShaders();
    void updateShadersProjectionMatrix();
    void updateShadersModelViewMatrix();

    City*   _city;
    Ground* _ground;
    Description* _description;

    // Shaders
    cellar::GlProgram _minimalistShader;
    cellar::GlProgram _skyShader;
    cellar::GlProgram _groundShader;
    cellar::GlProgram _waterShader;
    cellar::GlProgram _infrastructShader;

    std::vector<AbstractComponent*> _components;
    SkyComponent*       _skyComponent;
    GroundComponent*    _groundComponent;
    WaterComponent*     _waterComponent;
    CitizensComponent*  _citizensComponent;
    JunctionsComponent* _junctionsComponent;
    StreetsComponent*   _streetsComponent;
    BuildingsComponent* _buildingsComponent;
    BridgesComponent*   _bridgesComponent;
};



#endif // DRAWCITYMODULE_H
