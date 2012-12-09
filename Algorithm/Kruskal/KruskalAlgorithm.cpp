#include <algorithm>

#include "KruskalAlgorithm.h"

// To make this faster, we could count how many different edges we have, then stop processing
// when we know all edges are linked

KruskalAlgorithm::KruskalAlgorithm():
    _processed(false)
{
}

void KruskalAlgorithm::addSegment(Segment seg)
{
    _processed = false;
    _fullGraph.insert(seg);
}

void KruskalAlgorithm::addSegment(int edgeA, int edgeB, double distance)
{
    _processed = false;
    _fullGraph.insert(Segment(edgeA, edgeB, distance));
}

void KruskalAlgorithm::process()
{
    if (!_processed)
    {
        _linkedIslands.clear();
        _cheapestGraph.clear();

        processList();

        _processed = true;
    }
}

const Graph& KruskalAlgorithm::getCheapestGraph()
{
    if (!_processed)
    {
        process();
    }
    return _cheapestGraph;
}

SetsList::iterator KruskalAlgorithm::findSet(int edge)
{
    for (auto itrList = _linkedIslands.begin(); itrList != _linkedIslands.end(); itrList++)
    {
        auto itrSet = itrList->find(edge);
        if (itrSet != itrList->end())
        {
            return itrList;
        }
    }
    return createNewSet(edge);
}

SetsList::iterator KruskalAlgorithm::createNewSet(int edge)
{
    std::set<int> newSet;
    newSet.insert(edge);
    return _linkedIslands.insert(_linkedIslands.end(), newSet);
}

void KruskalAlgorithm::processList()
{
    for (Graph::iterator itr = _fullGraph.begin(); itr != _fullGraph.end(); itr++)
    {
        handleMerges(itr);
    }
}

void KruskalAlgorithm::handleMerges(Graph::iterator itr)
{
    SetsList::iterator firstSet = findSet(itr->edgeA());
    SetsList::iterator secondSet = findSet(itr->edgeB());
    if (firstSet != secondSet)
    {
        firstSet->insert(secondSet->begin(), secondSet->end());
        _linkedIslands.erase(secondSet);
        _cheapestGraph.insert(*itr);
    }
}
