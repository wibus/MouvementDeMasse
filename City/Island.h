#ifndef ISLAND_H
#define ISLAND_H

#include <vector>
#include <map>

#include <GLM/glm.hpp>


class Island
{
public:
    Island();
    ~Island();

    void setIslandId(int id);
    int getIslandId() const;

    void addJunction(glm::ivec2 junction);
    const std::vector<glm::ivec2>& getJunctions() const;

    void addBridge (int islandBridgedTo, glm::ivec2 bridgePosition);
    std::multimap<int, glm::ivec2>::const_iterator bridgesForTravel (int islandToReachId) const;

private:
    int _islandId;
    std::vector<glm::ivec2> _junctions;
    std::multimap<int, glm::ivec2> _bridges;
};


// Implementation //
inline void Island::setIslandId(int id)
{
    _islandId = id;
}

inline int Island::getIslandId() const
{
    return _islandId;
}

inline void Island::addJunction(glm::ivec2 junction)
{
    _junctions.push_back(junction);
}

inline const std::vector<glm::ivec2>& Island::getJunctions() const
{
    return _junctions;
}

inline void Island::addBridge (int islandBridgedTo, glm::ivec2 bridgePosition)
{
    _bridges.insert(std::pair<int, glm::ivec2>(islandBridgedTo, bridgePosition));
}

inline std::multimap<int, glm::ivec2>::const_iterator Island::bridgesForTravel (int islandToReachId) const
{
    return _bridges.find(islandToReachId);
}

#endif // ISLANDS_H
