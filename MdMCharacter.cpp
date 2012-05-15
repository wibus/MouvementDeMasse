#include <iostream>
using namespace std;

#include "MdMCharacter.h"
#include "CityMap.h"

#include "Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.h"

using namespace scaena;


MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter"),
    _cityMap( new CityMap(200, 200))
{
}

void MdMCharacter::enterStage()
{
    cout << "I'm here" << endl;

    chooseAlgorithms();
    _cityMap->setup();
}

void MdMCharacter::beginStep(const StageTime &time)
{
}

void MdMCharacter::endStep(const StageTime &time)
{
}

void MdMCharacter::exitStage()
{
    cout << "Goodbye" << endl;
}

CityMap& MdMCharacter::cityMap()
{
    return *_cityMap;
}

void MdMCharacter::setCityMap(CityMap* map)
{
    _cityMap.reset( map );
}

void MdMCharacter::chooseAlgorithms()
{
    // Height algorithm
    HeightByNoiseAlgo* heightAlgo = new HeightByNoiseAlgo();
    heightAlgo->setNbNoises( min(_cityMap->size().x(), _cityMap->size().y()) / 2 );
    heightAlgo->setWeightedNoisesRange(1, heightAlgo->nbNoises());
    _cityMap->setHeightsAlgorithm( heightAlgo );
}
