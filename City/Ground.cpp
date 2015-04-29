#include "Ground.h"

#include <iostream>
using namespace std;


using namespace cellar;


Ground::Ground(int width, int height) :
    _width(width),
    _height(height),
    _minHeight(-1.0f),
    _maxHeight(1.0f),
    _heights(width, height, 0.0f)
{
}

float Ground::heightAt(float x, float y) const
{
    // Check side of square for determining dx and dy
    // *-------*
    // | \   + |
    // |   \   |
    // | -   \ |
    // *-------*

    glm::ivec2 base(floor(x), floor(y));
    glm::vec2 bias(x - (float)base.x, y - (float)base.y);

    if(bias == glm::vec2(0, 0)) return _heights.get(base.x, base.y);

    float dx, dy;
    const glm::vec2 n(0.707106781f, 0.707106781f); // sqrt(2)/2 = 0.707106781f
    glm::vec2 p(bias.x - 1.0f, bias.y);

    if(dot(p, n) <= 0) // Which side of square (- \ +)
    {
        float baseHeight = heightAt(base);
        dx = heightAt(base.x+1, base.y)   - baseHeight;
        dy = heightAt(base.x,   base.y+1) - baseHeight;
    }
    else
    {
        base += glm::ivec2(1, 1);
        bias -= glm::vec2(1.0f, 1.0f);
        float baseHeight = heightAt(base);
        dx = baseHeight - heightAt(base.x-1, base.y);
        dy = baseHeight - heightAt(base.x, base.y-1);
    }

    return heightAt(base) + bias.x*dx + bias.y*dy;
}

float Ground::landLowerCornerAt(int x, int y) const
{
    return glm::min(glm::min(glm::min(
        heightAt(x,   y),
        heightAt(x+1, y)),
        heightAt(x+1, y+1)),
        heightAt(x,   y+1)
    );
}

float Ground::landHigherCornerAt(int x, int y) const
{
    return glm::max(glm::max(glm::max(
        heightAt(x,   y),
        heightAt(x+1, y)),
        heightAt(x+1, y+1)),
        heightAt(x,   y+1)
    );
}
