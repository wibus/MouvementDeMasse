#ifndef MAP_H
#define MAP_H

#include <Graphics/Image.h>
#include <Graphics/Colors/RGBColor.h>

#include "MdMTypes.h"
#include "TemplateMap.h"


class Map
{
public:
    Map(unsigned int width,    unsigned int height,
        float minGroundHeight, float maxGroundHeight);

    void reset();

    // Geters
    unsigned int width() const;
    unsigned int height() const;
    float minGroundHeight() const;
    float maxGroundHeight() const;
    float getGroundHeightAt(unsigned int x, unsigned int y) const;
    Intersection* getIntersectionAt(unsigned int x, unsigned int y) const;
    HStreet* getHStreetAt(unsigned int x, unsigned int y) const;
    VStreet* getVStreetAt(unsigned int x, unsigned int y) const;
    Land* getLandAt(unsigned int x, unsigned int y) const;

    // Seters
    void setGroundHeightAt(unsigned int x, unsigned int y, float height);
    Intersection* setIntersectionAt(unsigned int x, unsigned int y, Intersection* intersection);
    HStreet* setHStreetAt(unsigned int x, unsigned int y, HStreet* street);
    VStreet* setVStreetAt(unsigned int x, unsigned int y, VStreet* street);
    Land* setLandAt(unsigned int x, unsigned int y, Land* land);

    // Generation methods
    void genGroundHeights();
    void genRoads();
    void genLands();

    // Height conversions
    cellar::RGBColor heightToRGBColor(float height) const;
    float rgbColorToHeight(const cellar::RGBColor& color) const;

private:
    Map();
    Map(const Map& map);
    Map& operator=(const Map& map);

    unsigned int _width;
    unsigned int _height;
    float _minGroundHeight;
    float _maxGroundHeight;
    float _middGroundHeight;
    float _halfPeekToPeekGroundHeight;

    float**                     _groundHeights;
    TemplateMap<Intersection>   _intersections;
    TemplateMap<HStreet>        _hStreets;
    TemplateMap<VStreet>        _vStreets;
    TemplateMap<Land>           _lands;
};



// IMPLEMENTATION //
// Geters
inline unsigned int Map::width() const
    {return _width;}
inline unsigned int Map::height() const
    {return _height;}
inline float Map::minGroundHeight() const
    {return _minGroundHeight;}
inline float Map::maxGroundHeight() const
    {return _maxGroundHeight;}
inline float Map::getGroundHeightAt(unsigned int x, unsigned int y) const
    {return _groundHeights[y][x];}
inline Intersection* Map::getIntersectionAt(unsigned int x, unsigned int y) const
    {return _intersections.at(x, y);}
inline HStreet* Map::getHStreetAt(unsigned int x, unsigned int y) const
    {return _hStreets.at(x, y);}
inline VStreet* Map::getVStreetAt(unsigned int x, unsigned int y) const
    {return _vStreets(x, y);}
inline Land* Map::getLandAt(unsigned int x, unsigned int y) const
    {return _lands.at(x, y);}

// Seters
inline void Map::setGroundHeightAt(unsigned int x, unsigned int y, float height)
    {_groundHeights[y][x] = height;}

#endif // MAP_H
