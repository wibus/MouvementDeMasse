#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <MathsAndPhysics/Matrix4x4.h>
#include <Graphics/Light/Light3D.h>


class Description
{
public:
    Description();

    // Matrices
    cellar::Matrix4x4<float> projMat;
    cellar::Matrix4x4<float> viewMat;
    cellar::Matrix3x3<float> normalMat;

    // Sun
    float sunRadius;
    cellar::DirectionnalLight3D sunLight;
    cellar::Vec4f viewedSunDirection;

    // Colors
    cellar::Vec4f sunColor;
    cellar::Vec4f nightSkyColor;
    cellar::Vec4f daySkyColor;
    cellar::Vec4f skylineColor;
    cellar::Vec4f curSkyColor;
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
