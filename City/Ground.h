#ifndef GROUND_H
#define GROUND_H

#include <CellarWorkbench/DataStructure/Grid2D.h>



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
    float heightAt(const glm::ivec2& pos) const;
    float heightAt(float x, float y) const;
    float heightAt(const glm::vec2& pos) const;
    float landLowerCornerAt(int x, int y) const;
    float landLowerCornerAt(const glm::ivec2& pos) const;
    float landHigherCornerAt(int x, int y) const;
    float landHigherCornerAt(const glm::ivec2& pos) const;

    void setHeightAt(int x, int y, float height);
    void setHeightAt(const glm::ivec2& pos, float height);

    glm::vec3 normalAt(float x, float y) const;
    float dzdx(float x, float y) const;
    float dzdy(float x, float y) const;

private:
    int _width;
    int _height;
    float _minHeight;
    float _maxHeight;
    float _waterHeight;
    cellar::Grid2D<float> _heights;
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
    return _heights.get(glm::clamp(x, 0, _width-1),
                        glm::clamp(y, 0, _height-1));
}

inline float Ground::heightAt(const glm::ivec2& pos) const
{
    return _heights.get(glm::clamp(pos.x, 0, _width-1),
                        glm::clamp(pos.y, 0, _height-1));
}

inline float Ground::heightAt(const glm::vec2& pos) const
{
    return heightAt(pos.x, pos.y);
}

inline float Ground::landLowerCornerAt(const glm::ivec2& pos) const
{
    return landLowerCornerAt(pos.x, pos.y);
}

inline float Ground::landHigherCornerAt(const glm::ivec2& pos) const
{
    return landHigherCornerAt(pos.x, pos.y);
}

inline void Ground::setHeightAt(int x, int y, float height)
{
    _heights.set(x, y , height);
}

inline void Ground::setHeightAt(const glm::ivec2 &pos, float height)
{
    _heights.set(pos, height);
}

inline glm::vec3 Ground::normalAt(float x, float y) const
{
    return glm::normalize(glm::vec3(-dzdx(x, y), -dzdy(x, y), 1.0f));
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
