#ifndef GROUND_H
#define GROUND_H

#include <MathsAndPhysics/Vector.h>
#include <DataStructures/Grid.h>


class Ground
{
public:
    Ground(int width, int height);

    int width() const;
    int height() const;
    float minHeight() const;
    float maxHeight() const;

    void setMinHeight(float height);
    void setMaxHeight(float height);

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

inline void Ground::setMinHeight(float height)
{
    _minHeight = height;
}

inline void Ground::setMaxHeight(float height)
{
    _maxHeight = height;
}

inline float Ground::heightAt(int x, int y) const
{
    return _heights.get(x, y);
}

inline float Ground::heightAt(const cellar::Vec2i& pos) const
{
    return _heights.get(pos);
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
    return cellar::Vec3f(-dzdx(x, y), -dzdy(x, y), 1).normalize();
}

inline float Ground::dzdx(float x, float y) const
{
    return (heightAt(x + 0.5, y) - heightAt(x - 0.5, y));
}

inline float Ground::dzdy(float x, float y) const
{
    return (heightAt(x, y + 0.5) - heightAt(x, y - 0.5));
}

#endif // GROUND_H
