#ifndef LAND_H
#define LAND_H

class Land
{
public:
    Land();
    virtual ~Land();

    void setHeight(float height);
    float height() const;

private:
    float _height;
};



// Implementation //
inline void Land::setHeight(float height)
{
    _height = height;
}

inline float Land::height() const
{
    return _height;
}

#endif // LAND_H
