#ifndef GROUND_H
#define GROUND_H

#include <DataStructure/Vector.h>
#include <DataStructure/Grid.h>
#include <Misc/CellarUtils.h>


class Ground
{
public:
    Ground(int width, int height);

    int width() const;
    int height() const;
    float minHeight() const;
    float maxHeight() const;
    float waterHeight() const;

    void setMinHeight(float height);
    void setMaxHeight(float height);
    void setWaterHeight(float height);

    float heightAt(int x, int y) const;
    float heightAt(const cellar::Vec2i& pos) const;
    float heightAt(float x, float y) const;
    float heightAt(const cellar::Vec2f& pos) const;

    void setHeightAt(int x, int y, float height);
    void setHeightAt(const cellar::Vec2i& pos, float height);

    cellar::Vec3f normalAt(float x, float y) const;
    float dzdx(float x, float y) const;
    float dzdy(float x, float y) const;

private:
    int _width;
    int _height;
    float _minHeight;
    float _maxHeight;
    float _waterHeight;
    cellar::Grid<float> _heights;
};



// IMPLEMENTATION //
inline int Ground::width() const
{
    return _width;
}

inline int Ground::height() const
{
    return _height;
}

inline float Ground::minHeight() const
{
    return _minHeight;
}

inline float Ground::maxHeight() const
{
    return _maxHeight;
}

inline float Ground::waterHeight() const
{
    return _waterHeight;
}

inline void Ground::setMinHeight(float height)
{
    _minHeight = height;
}

inline void Ground::setMaxHeight(float height)
{
    _maxHeight = height;
}

inline void Ground::setWaterHeight(float height)
{
    _waterHeight = height;
}

inline float Ground::heightAt(int x, int y) const
{
    return _heights.get(cellar::clamp(x, 0, _width-1),
                        cellar::clamp(y, 0, _height-1));
}

inline float Ground::heightAt(const cellar::Vec2i& pos) const
{
    return _heights.get(cellar::clamp(pos.x(), 0, _width-1),
                        cellar::clamp(pos.y(), 0, _height-1));
}

inline float Ground::heightAt(const cellar::Vec2f& pos) const
{
    return heightAt(pos.x(), pos.y());
}

inline void Ground::setHeightAt(int x, int y, float height)
{
    _heights.set(x, y , height);
}

inline void Ground::setHeightAt(const cellar::Vec2i &pos, float height)
{
    _heights.set(pos, height);
}

inline cellar::Vec3f Ground::normalAt(float x, float y) const
{
    return cellar::Vec3f(-dzdx(x, y), -dzdy(x, y), 1.0f).normalize();
}

const float ds = 0.25;
const float dsd2 = 1.0f/(2.0f*ds);

inline float Ground::dzdx(float x, float y) const
{
    return (heightAt(x + ds, y) - heightAt(x - ds, y)) * dsd2;
}

inline float Ground::dzdy(float x, float y) const
{
    return (heightAt(x, y + ds) - heightAt(x, y - ds)) * dsd2;
}

#endif // GROUND_H
