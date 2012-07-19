#include "Visual.h"

Visual::Visual() :
    sunRadius(0.99994f),
    sunLight(),
    sunColor(     1.00, 0.90, 0.20, 1.00),
    nightSkyColor(0.02, 0.01, 0.05, 1.00),
    daySkyColor(  0.40, 0.60, 0.80, 1.00),
    skylineColor( 0.12, 0.12, 0.12, 1.00),
    curSkyColor(  nightSkyColor         ),
    grassColor(   0.32, 0.90, 0.08, 1.00),
    mudColor(     0.20, 0.14, 0.01, 1.00),
    waterColor(   0.03, 0.03, 0.30, 0.42),
    waterShininess( 200.0f),
    unitPerMeter(1.0f/40.0f),
    meterPerUnit(1.0f/unitPerMeter),
    citizensHeight(0.9f * unitPerMeter),
    roadWidth(     8.00f * unitPerMeter),
    bridgeWidth(   2.0f*roadWidth),
    bridgeHeight(  bridgeWidth)
{
    sunLight.direction(1.0f, 1.0f, 0.0f, 0.0f).normalize();
    sunLight.ambient = curSkyColor;
    sunLight.diffuse( 0.76, 0.74, 0.72);
    sunLight.specular(0.65, 0.58, 0.46);
}
