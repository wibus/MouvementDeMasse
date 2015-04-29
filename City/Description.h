#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <string>

#include <PropRoom3D/Prop/Lighting/Light3D.h>


class Description
{
public:
    Description();

    bool load(const std::string& fileName);
    bool save(const std::string& fileName);

    // Matrices
    glm::mat4 projMat;
    glm::mat4 viewMat;
    glm::mat3 normalMat;

    // Sun
    float sunRadius;
    prop3::DirectionnalLight3D sunLight;
    glm::vec4 viewedSunDirection;

    // Colors
    glm::vec4 sunColor;
    glm::vec4 nightSkyColor;
    glm::vec4 daySkyColor;
    glm::vec4 sunSetColor;
    glm::vec4 skylineColor;
    glm::vec4 grassColor;
    glm::vec4 mudColor;
    glm::vec4 waterColor;

    // Shininesses
    float waterShininess;

    // Sizes
    float unitPerMeter;
    float meterPerUnit;
    float citizensHeight;
    float roadWidth;
    float bridgeWidth;
    float bridgeHeight;
    float storyHeight;

    // Speeds
    float normalWalkingSpeed;
};

#endif // DESCRIPTION_H
