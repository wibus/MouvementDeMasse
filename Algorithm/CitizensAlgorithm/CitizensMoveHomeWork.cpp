#include "CitizensMoveHomeWork.h"

#include <iostream>

#include <GLM/gtc/random.hpp>

#include <PropRoom2D/Prop/Shape/Segment2D.h>

using namespace std;
using namespace cellar;
using namespace prop2;


CitizensMoveHomeWork::CitizensMoveHomeWork() :
    _juncLength(0.0f),
    _juncHalfLength(0.0f)
{
}

void CitizensMoveHomeWork::setup(City &city)
{
    CitizensAlgorithm::setup( city );

    CitizenIterator cit = _city->citizens().begin();
    CitizenIterator citEnd = _city->citizens().end();

    // Main loop (foreach citizens)
    for(; cit != citEnd; ++cit)
    {
        Citizen& ctz = cit->second;

        Schedule::Event evt = ctz.schedule.currentEvent(_city->dateAndTime());
        if(evt.state == CITIZEN_GOTO_HOME || evt.state == CITIZEN_AT_HOME)
            ctz.curState = CITIZEN_AT_HOME;
        else if(evt.state == CITIZEN_GOTO_WORK || evt.state == CITIZEN_AT_WORK)
            ctz.curState = CITIZEN_AT_WORK;

        placeCitizenOnBuilding(ctz);
    }

    _juncLength = _description->roadWidth;
    _juncHalfLength = _juncLength / 2.0f;
}

void CitizensMoveHomeWork::update()
{
    // Global invariant data
    Time now = _city->dateAndTime();
    int remainingSecs = _city->timeJump().toSeconds();


    // Loop Iterators
    CitizenIterator cIt = _city->citizens().begin();
    CitizenIterator cItEnd = _city->citizens().end();

    // Main loop (foreach citizens)
    for(; cIt != cItEnd; ++cIt)
    {
        // Current citizen data
        Citizen& ctz = cIt->second;
        Schedule::Event currEvt = ctz.schedule.currentEvent( now );
        Schedule::Event nextEvt = ctz.schedule.nextEvent( now );
        float timeToSpend = remainingSecs;
        float nextEvtIn = (nextEvt.time - now).toSeconds();

        float timeSpent;

        while(timeToSpend > 0.0f)
        {
            if(currEvt.state == CITIZEN_AT_HOME)
            {
                if(ctz.curState == CITIZEN_AT_HOME)
                    break;

                if(timeToSpend <= nextEvtIn)
                {
                    timeSpent = gotoHome(ctz, timeToSpend);
                    timeToSpend -= timeSpent;
                    nextEvtIn   -= timeSpent;
                }
                else
                {
                    gotoHome(ctz, nextEvtIn);
                    gotoWork(ctz, timeToSpend - nextEvtIn);
                    timeToSpend = 0.0f;
                }
            }
            else if(currEvt.state == CITIZEN_GOTO_WORK)
            {
                if(ctz.curState == CITIZEN_AT_WORK)
                    break;

                timeSpent = gotoWork(ctz, timeToSpend);
                timeToSpend -= timeSpent;
                nextEvtIn   -= timeSpent;
            }
            else if(currEvt.state == CITIZEN_AT_WORK)
            {
                if(ctz.curState == CITIZEN_AT_WORK)
                    break;

                if(timeToSpend <= nextEvtIn)
                {
                    timeSpent = gotoWork(ctz, timeToSpend);
                    timeToSpend -= timeSpent;
                    nextEvtIn   -= timeSpent;
                }
                else
                {
                    gotoWork(ctz, nextEvtIn);
                    gotoHome(ctz, timeToSpend - nextEvtIn);
                    timeToSpend = 0.0f;
                }
            }
            else if(currEvt.state == CITIZEN_GOTO_HOME)
            {
                if(ctz.curState == CITIZEN_AT_HOME)
                    break;

                timeSpent = gotoHome(ctz, timeToSpend);
                timeToSpend -= timeSpent;
                nextEvtIn   -= timeSpent;
            }
        }

        if(ctz.curState == CITIZEN_GOTO_HOME || ctz.curState == CITIZEN_GOTO_WORK)
            placeCitizenOnRoad( ctz );

    } // end of foreach( ctz )
}

float CitizensMoveHomeWork::gotoWork(Citizen& ctz, float timeToMove)
{
    if(ctz.curState == CITIZEN_AT_WORK)
        return 0.0;

    if(ctz.curState != CITIZEN_GOTO_WORK)
    {
        cout << _city->dateAndTime().Time::toString() << "\tGoing to work (" << ctz.id() << ')' << endl;

        if(ctz.curState == CITIZEN_AT_HOME)
            ctz.homeToWorkPath.gotoBegin();

        ctz.curState = CITIZEN_GOTO_WORK;
    }

    Path& path = ctz.homeToWorkPath;
    float canWalk = timeToMove * ctz.walkingSpeed;

    while( !path.isEndReached() )
    {
        float distance = glm::length(glm::vec2(path.nextNode->pos - path.curNode->pos));
        float toWalk = distance * (1.0f - path.nodeProgession);

        if(toWalk < canWalk)
        {
            canWalk -= toWalk;
            path.gotoNextNode();
        }
        else
        {
            path.nodeProgession = 1.0f - (toWalk - canWalk)/distance;
            return timeToMove;
        }
    }

    // Work was reached in this turn
    ctz.curState = CITIZEN_AT_WORK;
    placeCitizenOnBuilding(ctz);

    float elapsed = timeToMove - canWalk/ctz.walkingSpeed;
    Time after = Time().fromSeconds(elapsed) + _city->dateAndTime();
    cout << after.toString() <<  "\tAt work (" << ctz.id() << ')' << endl;

    float timeSpent = timeToMove - canWalk/ctz.walkingSpeed;
    return timeSpent;
}

float CitizensMoveHomeWork::gotoHome(Citizen& ctz, float timeToMove)
{
    if(ctz.curState == CITIZEN_AT_HOME)
        return 0.0;

    if(ctz.curState != CITIZEN_GOTO_HOME)
    {
        cout << _city->dateAndTime().Time::toString()
             << "\tGoing home (" << ctz.id() << ')' << endl;

        if(ctz.curState == CITIZEN_AT_WORK)
            ctz.homeToWorkPath.gotoEnd();

        ctz.curState = CITIZEN_GOTO_HOME;
    }

    Path& path = ctz.homeToWorkPath;
    float canWalk = timeToMove * ctz.walkingSpeed;

    while( !ctz.homeToWorkPath.isRevEndReached() )
    {
        float distance = glm::length(glm::vec2(path.revNextNode->pos - path.revCurNode->pos));
        float toWalk = distance * (1.0f - path.nodeProgession);

        if(toWalk < canWalk)
        {
            canWalk -= toWalk;
            path.gotoRevNextNode();
        }
        else
        {
            path.nodeProgession = (distance - (toWalk - canWalk)) / distance;
            return timeToMove;
        }
    }

    // Home was reached in this turn
    ctz.curState = CITIZEN_AT_HOME;
    placeCitizenOnBuilding(ctz);

    float elapsed = timeToMove - canWalk/ctz.walkingSpeed;
    Time after = Time().fromSeconds(elapsed) + _city->dateAndTime();
    cout << after.toString() << "\tAt home (" << ctz.id() << ')' << endl;


    float timeSpent = timeToMove - canWalk/ctz.walkingSpeed;
    return timeSpent;
}

void CitizensMoveHomeWork::placeCitizenOnRoad(Citizen& ctz)
{
    Path& path = ctz.homeToWorkPath;
    float progression = path.nodeProgession;

    glm::ivec2 begPos, endPos;
    if(ctz.curState == CITIZEN_GOTO_WORK)
    {
        begPos = path.curNode->pos;
        endPos = path.nextNode->pos;
    }
    else
    {
        begPos = path.revCurNode->pos;
        endPos = path.revNextNode->pos;
    }

    glm::vec2 juncToJunc     = endPos - begPos;
    float totalLength    = glm::length(juncToJunc);
    float roadLength     = totalLength - _juncLength;
    float walkedLength   = progression * totalLength;

    if(_juncHalfLength <= walkedLength && walkedLength <= _juncHalfLength + roadLength)
    {
        // Direction
        glm::vec2 dir2D        = glm::normalize(juncToJunc);
        glm::vec2 roadVec      = dir2D * roadLength;

        // Height
        float begHeight = _ground->heightAt(begPos);
        float endHeight = _ground->heightAt(endPos);
        float deltaz = endHeight - begHeight;

        ctz.direction = glm::normalize(glm::vec3(roadVec.x, roadVec.y, deltaz));


        // Progression
        float road3DLength = sqrt(roadLength*roadLength + deltaz*deltaz);
        float streetProg   = (walkedLength - _juncHalfLength) / roadLength;
        glm::vec2 right( Segment2D::perpCW(dir2D * _juncLength / 4.0f) );

        ctz.position  = glm::vec3(right + glm::vec2(begPos) + dir2D*_juncHalfLength, begHeight);
        ctz.position += ctz.direction*streetProg*road3DLength;
    }
    else
    {
        //
        glm::ivec2 prevPos, nextPos;
        if(ctz.curState == CITIZEN_GOTO_WORK)
        {
            if( !path.isBeginReached() ) prevPos = (path.curNode-1)->pos;
            else                         prevPos = path.curNode->pos;
            if( !path.isEndReached() ) nextPos = (path.nextNode+1)->pos;
            else                       nextPos = path.nextNode->pos;
        }
        else
        {
            if( !path.isRevBeginReached() ) prevPos = (path.revCurNode-1)->pos;
            else                            prevPos = path.revCurNode->pos;
            if( !path.isRevEndReached() ) nextPos = (path.revNextNode+1)->pos;
            else                          nextPos = path.revNextNode->pos;
        }

        float juncHeight;
        float juncProg;
        glm::vec2 juncPos;
        glm::vec2 lastDir2d, lastRight, lastRealPos,
              nextDir2d, nextRight, nextRealPos;

        if(progression < 0.5f)
        {
            juncHeight = _ground->heightAt(begPos);
            juncProg = (walkedLength + _juncHalfLength) / _juncLength;
            juncPos = begPos;
            lastDir2d = begPos - prevPos;
            nextDir2d = endPos - begPos;
        }
        else
        {
            juncHeight = _ground->heightAt(endPos);
            juncProg = ((walkedLength-totalLength) + _juncHalfLength) / _juncLength;
            juncPos = endPos;
            lastDir2d = endPos  - begPos;
            nextDir2d = nextPos - endPos;
        }

        // Interpolation coeffs
        juncProg = juncProg;
        float lastWeight = (1.0f - juncProg);
        float nextWeight = juncProg;

        // Direction
        glm::vec2 dirInterpol = glm::normalize(lastWeight*lastDir2d + nextWeight*nextDir2d);
        ctz.direction = glm::vec3(dirInterpol.x, dirInterpol.y, 0.0f);

        // Position
        lastDir2d = glm::normalize(lastDir2d);
        lastDir2d *= _juncHalfLength;
        lastRight = Segment2D::perpCW(lastDir2d / 2.0f);
        lastRealPos = -lastDir2d + lastRight;

        nextDir2d = glm::normalize(nextDir2d);
        nextDir2d *= _juncHalfLength;
        nextRight = Segment2D::perpCW(nextDir2d / 2.0f);
        nextRealPos = nextDir2d + nextRight;

        glm::vec2 posInterpol = juncPos + lastWeight*lastRealPos + nextWeight*nextRealPos;
        ctz.position = glm::vec3(posInterpol.x, posInterpol.y, juncHeight);
    }
}

void CitizensMoveHomeWork::placeCitizenOnBuilding(Citizen& ctz)
{
    glm::ivec2 buildingPos;
    if(ctz.curState == CITIZEN_AT_HOME)
        buildingPos = ctz.homePos;
    else if(ctz.curState == CITIZEN_AT_WORK)
        buildingPos = ctz.workPos;
    else return;

    // XY Position
    const glm::vec2 center(0.5f, 0.5f);
    glm::vec2 randomRoofPos(glm::linearRand(-0.4f, 0.4f), glm::linearRand(-0.4f, 0.4f));
    glm::vec2 pos2d = center + glm::vec2(buildingPos) + randomRoofPos;

    // Height
    float baseHeight = _ground->landLowerCornerAt(buildingPos);
    float buildingHeight = _city->lands().get(buildingPos)->nbStories() * _description->storyHeight;

    ctz.position = glm::vec3(pos2d.x, pos2d.y, baseHeight + buildingHeight);
}
