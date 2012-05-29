#ifndef DRAWALGORITHM_H
#define DRAWALGORITHM_H

#include <MathsAndPhysics/Matrix4x4.h>

#include "Algorithm/Algorithm.h"


class DrawAlgorithm : public Algorithm
{
protected:
    DrawAlgorithm();

public:
    virtual ~DrawAlgorithm();

    virtual void setup(CityMap& cityMap);
    virtual void draw() = 0;

    virtual void updateViewMatrix(const cellar::Matrix4x4<float>& view);
    virtual void updateProjectionMatrix(const cellar::Matrix4x4<float>& proj);

protected:
    cellar::Matrix3x3<float> _viewSubMat3;
    cellar::Matrix4x4<float> _viewMat;
    cellar::Matrix4x4<float> _projMat;
};

#endif // DRAWALGORITHM_H
