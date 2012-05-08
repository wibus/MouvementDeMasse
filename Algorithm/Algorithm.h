#ifndef ALGORITHM_H
#define ALGORITHM_H

class CityMap;

class Algorithm
{
protected:
    Algorithm();

public:    
    virtual ~Algorithm();

    virtual void setup(CityMap& cityMap);
    virtual void process() = 0;

protected:
    CityMap* _cityMap;
};

#endif // ALGORITHM_H
