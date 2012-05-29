#include "DrawAlgorithm.h"

DrawAlgorithm::DrawAlgorithm()
{
}

DrawAlgorithm::~DrawAlgorithm()
{
}

void DrawAlgorithm::setup(CityMap& cityMap)
{
    Algorithm::setup( cityMap );
}

void DrawAlgorithm::updateViewMatrix(const cellar::Matrix4x4<float>& view)
{
    _viewSubMat3 = view.subMat3();
    _viewMat = view;
}

void DrawAlgorithm::updateProjectionMatrix(const cellar::Matrix4x4<float>& proj)
{
    _projMat = proj;
}
