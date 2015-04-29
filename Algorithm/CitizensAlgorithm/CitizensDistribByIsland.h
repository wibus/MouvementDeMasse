#ifndef CITIZENSDISTRIBBYISLAND_H
#define CITIZENSDISTRIBBYISLAND_H

#include "CitizensAlgorithm.h"


class CitizensDistribByIsland : public CitizensAlgorithm
{
public:
    CitizensDistribByIsland();

    virtual void setup(City &city);
    virtual void update();

protected:
    float dist(const glm::ivec2& p1, const glm::ivec2& p2);
    bool isAccessible(const glm::ivec2& pos);
    glm::ivec2 randomAccessPointTo(const glm::ivec2& pos);

    bool homeToWorkPathByAStar(Path& path, const glm::ivec2& src, const glm::ivec2& dst);
    void initializeAStarStructures();
    void cleanAStarStructures();


    struct DataStructures;
    DataStructures* _data;
};



// IMPLEMENTATION //
inline float CitizensDistribByIsland::dist(const glm::ivec2& p1, const glm::ivec2 &p2)
{
    return static_cast<float>(glm::distance(glm::vec2(p1), glm::vec2(p2)));
}

#endif // CITIZENSDISTRIBBYISLAND_H
