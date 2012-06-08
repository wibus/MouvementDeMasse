#include "Ground.h"

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

    Vec2f  pos(clip(x, 0.0f, (float)(_width-1)), clip(y, 0.0f, (float)(_height-1)));
    Vec2i base(floor(pos.x()),              floor(pos.y()));
    Vec2f  bias(pos.x() - base.x(),          pos.y() - base.y());

    if(bias == Vec2f(0, 0)) return _heights.get(pos.x(), pos.y());

    float dx, dy;
    const Vec2f n(0.707106781f, 0.707106781f); // sqrt(2)/2 = 0.707106781f
    Vec2f p(1.0f - bias.x(), bias.y());

    if(p * n <= 0) // Which side of square (- \ +)
    {
        dx = _heights.get(base + Vec2i(1, 0)) - _heights.get(base);
        dy = _heights.get(base + Vec2i(0, 1)) - _heights.get(base);
    }
    else
    {
        base = Vec2i(cellar::min(base.x()+1, _width-1),
                     cellar::min(base.y()+1, _height-1));
        bias -= Vec2f(1.0f, 1.0f);
        dx = _heights.get(base) - _heights.get(base - Vec2i(1, 0));
        dy = _heights.get(base) - _heights.get(base - Vec2i(0, 1));
    }

    return _heights.get(base) + bias.x()*dx + bias.y()*dy;
}
