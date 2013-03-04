#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <DataStructure/Matrix.h>
#include <Graphics/Light/Light3D.h>


class Description
{
public:
    Description();

    bool load(const std::string& fileName);
    bool save(const std::string& fileName);

    // Matrices
    cellar::Mat4f projMat;
    cellar::Mat4f viewMat;
    cellar::Mat3f normalMat;

    // Sun
    float sunRadius;
    cellar::DirectionnalLight3D sunLight;
    cellar::Vec4f viewedSunDirection;

    // Colors
    cellar::Vec4f sunColor;
    cellar::Vec4f nightSkyColor;
    cellar::Vec4f daySkyColor;
    cellar::Vec4f sunSetColor;
    cellar::Vec4f skylineColor;
    cellar::Vec4f grassColor;
    cellar::Vec4f mudColor;
    cellar::Vec4f waterColor;

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
