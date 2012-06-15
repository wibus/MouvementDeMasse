#include "Ground.h"

#include <iostream>
using namespace std;

#include <Misc/CellarUtils.h>
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

    Vec2i base(floor(x), floor(y));
    Vec2f bias(x - (float)base.x(), y - (float)base.y());

    if(bias == Vec2f(0, 0)) return _heights.get(base.x(), base.y());

    float dx, dy;
    const Vec2f n(0.707106781f, 0.707106781f); // sqrt(2)/2 = 0.707106781f
    Vec2f p(bias.x() - 1.0f, bias.y());

    if(p * n <= 0) // Which side of square (- \ +)
    {
        float baseHeight = heightAt(base);
        dx = heightAt(base.x()+1, base.y())   - baseHeight;
        dy = heightAt(base.x(),   base.y()+1) - baseHeight;
    }
    else
    {
        base += Vec2i(1, 1);
        bias -= Vec2f(1.0f, 1.0f);
        float baseHeight = heightAt(base);
        dx = baseHeight - heightAt(base.x()-1, base.y());
        dy = baseHeight - heightAt(base.x(), base.y()-1);
    }

    return heightAt(base) + bias.x()*dx + bias.y()*dy;
}
