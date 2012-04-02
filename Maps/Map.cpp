#include "Map.h"

#include "Infrastructures/HStreet.h"
#include "Infrastructures/VStreet.h"
#include "Infrastructures/Intersection.h"
#include "Infrastructures/Land.h"


using namespace cellar;


Map::Map(unsigned int width,    unsigned int height,
         float minGroundHeight, float maxGroundHeight) :
    _width(width), _height(height),
    _minGroundHeight(minGroundHeight), _maxGroundHeight(maxGroundHeight),
    _middGroundHeight((_maxGroundHeight +_minGroundHeight) / 2.0f),
    _halfPeekToPeekGroundHeight((_maxGroundHeight - _minGroundHeight) / 2.0f),
    _intersections(width, height),
    _hStreets(width - 1, height - 1),
    _vStreets(width - 1, height - 1),
    _lands(width - 1, height - 1)
{
}

void Map::reset()
{
    _intersections.reset();
    _hStreets.reset();
    _vStreets.reset();
    _lands.reset();
}

Land* Map::setLandAt(unsigned int x, unsigned int y, Land* land)
{
    // Check bounds
    if(x < 0 || x >= _width - 1)
        return nullptr;
    if(y < 0 || y >= _height - 1)
        return nullptr;

    // Save current land at that place
    Land* oldLand = _lands.at(x, y);
    _lands.set(x, y, land);

    // Attach neighbor lands
    Land* tempLand;
    if(x != 0)
    {
        tempLand = _lands.at(x-1, y);
        land->attachLand(WEST, tempLand);
        tempLand->attachLand(EAST, land);
    }
    else
    {
        land->attachLand(WEST, nullptr);
    }

    if(x > _lands.width() - 1)
    {
        tempLand = _lands.at(x+1, y);
        land->attachLand(EAST, tempLand);
        tempLand->attachLand(WEST, land);
    }
    else
    {
        land->attachLand(EAST, nullptr);
    }

    if(y != 0)
    {
        tempLand = _lands.at(x, y-1);
        land->attachLand(SOUTH, tempLand);
        tempLand->attachLand(NORTH, land);
    }
    else
    {
        land->attachLand(SOUTH, nullptr);
    }

    if(y != _lands.height() - 1)
    {
        tempLand = _lands.at(x, y+1);
        land->attachLand(NORTH, tempLand);
        tempLand->attachLand(SOUTH, land);
    }
    else
    {
        land->attachLand(NORTH, nullptr);
    }

    // Attach neighbor streets
    HStreet* tempHStreet;
    tempHStreet = _hStreets.at(x, y);
    land->attachHStreet(DOWN, tempHStreet);
    tempHStreet->attachLand(UP, land);

    tempHStreet = _hStreets.at(x, y+1);
    land->attachHStreet(UP, tempHStreet);
    tempHStreet->attachLand(DOWN, land);

    VStreet* tempVStreet;
    tempVStreet = _vStreets.at(x, y);
    land->attachVStreet(LEFT, tempVStreet);
    tempVStreet->attachLand(RIGHT, land);

    tempVStreet = _vStreets.at(x+1, y);
    land->attachVStreet(RIGHT, tempVStreet);
    tempVStreet->attachLand(LEFT, land);


    return oldLand;
}

Intersection* Map::setIntersectionAt(
        unsigned int x,
        unsigned int y,
        Intersection* intersection)
{
    // Check bounds
    if(x < 0 || x >= _width)
        return nullptr;
    if(y < 0 || y >= _height)
        return nullptr;

    // Save current land at that place
    Intersection* oldInter = _intersections.at(x, y);
    _intersections.set(x, y, intersection);

    // Attach neighbor HStreets
    HStreet* hStreet;
    if(x != 0)
    {
        hStreet = _hStreets.at(x - 1, y);
        intersection->attachHStreet(LEFT, _hStreets);
        hStreet->attachIntersection(RIGHT, intersection);
    }
    else
    {
        intersection->attachHStreet(LEFT, nullptr);
    }

    if(x > _hStreets.width())
    {
        hStreet = _hStreets.at(x, y);
        intersection->attachHStreet(RIGHT, _hStreets);
        hStreet->attachIntersection(LEFT, intersection);
    }
    else
    {
        intersection->attachHStreet(RIGHT, nullptr);
    }


    // Attach neighbor VStreets
    VStreet* vStreet;
    if(y != 0)
    {
        vStreet = _vStreets.at(x, y - 1);
        intersection->attachVStreet(DOWN, _vStreets);
        vStreet->attachIntersection(UP, intersection);
    }
    else
    {
        intersection->attachHStreet(DOWN, nullptr);
    }

    if(y > _vStreets.heiht())
    {
        vStreet = _vStreets.at(x, y);
        intersection->attachVStreet(UP, _vStreets);
        vStreet->attachIntersection(DOWN, intersection);
    }
    else
    {
        intersection->attachVStreet(UP, nullptr);
    }

    return oldInter;
}

HStreet* Map::setHStreetAt(unsigned int x, unsigned int y, HStreet* street)
{

}

VStreet* Map::setVStreetAt(unsigned int x, unsigned int y, VStreet* street)
{

}

// Generation methods
void Map::genGroundHeights()
{
}

void Map::genRoads()
{
}

void Map::genLands()
{
}

// Height conversions
cellar::RGBColor Map::heightToRGBColor(float height) const
{
    RGBColor color;

    if(height <= _middGroundHeight)
    {
        color.setRed(0);
        color.setGreen((height - _minGroundHeight) * 255 /
                       _halfPeekToPeekGroundHeight);
        color.setBlue((_middGroundHeight - height) * 255 /
                      _halfPeekToPeekGroundHeight);
    }
    else
    {
        color.setRed((height - _middGroundHeight) * 255 /
                     _halfPeekToPeekGroundHeight);
        color.setGreen((_maxGroundHeight - height) * 255 /
                       _halfPeekToPeekGroundHeight);
        color.setBlue(0);
    }

    return color;
}

float Map::rgbColorToHeight(const cellar::RGBColor& color) const
{
    float height;

    if(color.blue() > 0)
    {
        height = _middGroundHeight -
                (color.blue() * _halfPeekToPeekGroundHeight / 255);
    }
    else
    {
        height = _middGroundHeight +
                (color.red() * _halfPeekToPeekGroundHeight / 255);
    }

    return height;
}
