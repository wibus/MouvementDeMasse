#ifndef VISUAL_H
#define VISUAL_H

#include <MathsAndPhysics/Matrix4x4.h>
#include <Graphics/Light/Light3D.h>


class Visual
{
public:
    Visual();

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

    // Roads
    float roadWidth;
};

#endif // VISUAL_H
