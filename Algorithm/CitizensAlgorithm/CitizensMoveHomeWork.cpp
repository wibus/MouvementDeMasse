#include "CitizensMoveHomeWork.h"

using namespace std;
using namespace cellar;


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

    while( !ctz.homeToWorkPath.isEndReached() )
    {
        float distance = Vec2f(path.nextNode->pos - path.curNode->pos).length();
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
        float distance = Vec2f(path.revNextNode->pos - path.revCurNode->pos).length();
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

    Vec2i begPos, endPos;
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

    Vec2f juncToJunc     = endPos - begPos;
    float totalLength    = juncToJunc.length();
    float roadLength     = totalLength - _juncLength;
    float walkedLength   = progression * totalLength;

    if(_juncHalfLength <= walkedLength && walkedLength <= _juncHalfLength + roadLength)
    {
        // Direction
        Vec2f dir2D        = juncToJunc.normalized();
        Vec2f roadVec      = dir2D * roadLength;

        // Height
        float begHeight = _ground->heightAt(begPos);
        float endHeight = _ground->heightAt(endPos);
        float deltaz = endHeight - begHeight;

        ctz.direction(roadVec.x(), roadVec.y(), deltaz).normalize();


        // Progression
        float road3DLength = sqrt(roadLength*roadLength + deltaz*deltaz);
        float streetProg   = (walkedLength - _juncHalfLength) / roadLength;
        Vec2f right( (dir2D * _juncLength / 4.0f).rotateQuarterCW() );

        ctz.position = vec3(right + begPos + dir2D*_juncHalfLength, begHeight) +
                       ctz.direction*streetProg*road3DLength;
    }
    else
    {
        //
        Vec2i prevPos, nextPos;
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
        Vec2f juncPos;
        Vec2f lastDir2d, lastRight, lastRealPos,
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
        Vec2f dirInterpol = (lastWeight*lastDir2d + nextWeight*nextDir2d).normalize();
        ctz.direction(dirInterpol.x(), dirInterpol.y(), 0.0f);

        // Position
        lastDir2d.normalize();
        lastDir2d *= _juncHalfLength;
        lastRight = (lastDir2d / 2.0f).rotateQuarterCW();
        lastRealPos = -lastDir2d + lastRight;

        nextDir2d.normalize();
        nextDir2d *= _juncHalfLength;
        nextRight = (nextDir2d / 2.0f).rotateQuarterCW();
        nextRealPos = nextDir2d + nextRight;

        Vec2f posInterpol = juncPos + lastWeight*lastRealPos + nextWeight*nextRealPos;
        ctz.position(posInterpol.x(), posInterpol.y(), juncHeight);
    }
}

void CitizensMoveHomeWork::placeCitizenOnBuilding(Citizen& ctz)
{
    Vec2i buildingPos;
    if(ctz.curState == CITIZEN_AT_HOME)
        buildingPos = ctz.homePos;
    else if(ctz.curState == CITIZEN_AT_WORK)
        buildingPos = ctz.workPos;
    else return;

    // XY Position
    const Vec2f center(0.5f, 0.5f);
    Vec2f randomRoofPos(randomRange(-0.4f, 0.4f), randomRange(-0.4f, 0.4f));
    Vec2f pos2d = center + buildingPos + randomRoofPos;

    // Height
    float baseHeight = _ground->landLowerCornerAt(buildingPos);
    float buildingHeight = _city->lands().get(buildingPos)->nbStories() * _description->storyHeight;

    ctz.position(pos2d.x(), pos2d.y(), baseHeight + buildingHeight);
}
