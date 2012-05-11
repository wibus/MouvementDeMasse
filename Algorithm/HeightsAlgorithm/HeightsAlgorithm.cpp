#include "HeightsAlgorithm.h"

HeightsAlgorithm::HeightsAlgorithm():
    _minHeight(0.0f),
    _maxHeight(0.0f)
{
}

HeightsAlgorithm::~HeightsAlgorithm()
{
}

void HeightsAlgorithm::setMinHeight(float minHeight)
{
    _minHeight = minHeight;
}

void HeightsAlgorithm::setMaxHeight(float maxHeight)
{
    _maxHeight = maxHeight;
}

void HeightsAlgorithm::setup(CityMap& cityMap)
{
    Algorithm::setup( cityMap );
}
