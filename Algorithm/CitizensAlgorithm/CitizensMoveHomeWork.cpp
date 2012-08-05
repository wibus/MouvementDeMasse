#include "CitizensMoveHomeWork.h"

using namespace std;
using namespace cellar;


CitizensMoveHomeWork::CitizensMoveHomeWork()
{
}

void CitizensMoveHomeWork::setup(City &city)
{
    CitizensAlgorithm::setup( city );

    _streetLength = 1.0f - _description->roadWidth;
    _juncLengthBeg = _description->roadWidth / 2.0f;
    _juncLengthEnd = 1.0f - _description->roadWidth / 2.0f;
}

void CitizensMoveHomeWork::update()
{
    // Global invariant data
    Time now = _city->dateAndTime();
    Time nextTime = now + _city->timeJump();
    int remainingSecs = _city->timeJump().toSeconds();


    CitizenIterator cit = _city->citizens().begin();
    CitizenIterator citEnd = _city->citizens().end();

    // Main loop (foreach citizens)
    for(; cit != citEnd; ++cit)
    {
        // Current citizen data
        Citizen& ctz = cit->second;
        Schedule::Event evt = ctz.schedule.currentEvent( now );
        Schedule::Event nextEvt = ctz.schedule.currentEvent( nextTime );
        float secsTotal = remainingSecs;
        float nextEvtIn = (nextEvt.time - evt.time).toSeconds();


        if(ctz.curState == CITIZEN_AT_HOME)
        {
            if(evt.state == CITIZEN_AT_HOME)
            {
            }
            else if(evt.state == CITIZEN_GOTO_WORK)
            {
                secsTotal -= gotoWork(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_AT_WORK)
            {
                secsTotal -= gotoWork(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_GOTO_HOME)
            {
            }
        }

        if(ctz.curState == CITIZEN_GOTO_WORK)
        {
            if(evt.state == CITIZEN_AT_HOME)
            {
                secsTotal -= gotoHome(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_GOTO_WORK)
            {
                secsTotal -= gotoWork(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_AT_WORK)
            {
                secsTotal -= gotoWork(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_GOTO_HOME)
            {
                secsTotal -= gotoHome(ctz, secsTotal);
            }
        }

        if(ctz.curState == CITIZEN_AT_WORK)
        {
            if(evt.state == CITIZEN_AT_HOME)
            {
                secsTotal -= gotoHome(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_GOTO_WORK)
            {
            }
            else if(evt.state == CITIZEN_AT_WORK)
            {
            }
            else if(evt.state == CITIZEN_GOTO_HOME)
            {
                secsTotal -= gotoHome(ctz, secsTotal);
            }
        }

        if(ctz.curState == CITIZEN_GOTO_HOME)
        {
            if(evt.state == CITIZEN_AT_HOME)
            {
                secsTotal -= gotoHome(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_GOTO_WORK)
            {
                secsTotal -= gotoWork(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_AT_WORK)
            {
                secsTotal -= gotoWork(ctz, secsTotal);
            }
            else if(evt.state == CITIZEN_GOTO_HOME)
            {
                secsTotal -= gotoHome(ctz, secsTotal);
            }
        }

        placeCitizenOnRoad( ctz );

    } // end of foreach( ctz )
}

float CitizensMoveHomeWork::gotoWork(Citizen& ctz, float timeToMove)
{
    if(ctz.curState != CITIZEN_GOTO_WORK)
    {
        cout << _city->dateAndTime().Time::toString() << "\tGoing to work (" << ctz.id() << ')' << endl;

        if(ctz.curState == CITIZEN_AT_HOME)
            ctz.homeToWorkPath.gotoBegin();
        else if(ctz.curState == CITIZEN_AT_WORK)
            ctz.homeToWorkPath.gotoEnd();

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
    return timeToMove - canWalk/ctz.walkingSpeed;
}

float CitizensMoveHomeWork::gotoHome(Citizen& ctz, float timeToMove)
{
    if(ctz.curState != CITIZEN_GOTO_HOME)
    {
        cout << _city->dateAndTime().Time::toString() << "\tGoing home (" << ctz.id() << ')' << endl;

        if(ctz.curState == CITIZEN_AT_HOME)
            ctz.homeToWorkPath.gotoBegin();
        else if(ctz.curState == CITIZEN_AT_WORK)
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
    return timeToMove - canWalk/ctz.walkingSpeed;
}

void CitizensMoveHomeWork::placeCitizenOnRoad(Citizen& ctz)
{
    Path& path = ctz.homeToWorkPath;
    float progression = path.nodeProgession;

    if(ctz.curState == CITIZEN_GOTO_HOME || ctz.curState == CITIZEN_GOTO_WORK)
    {
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

        if(_juncLengthBeg <= path.nodeProgession && path.nodeProgession <= _juncLengthEnd)
        {
            float begHeight = _ground->heightAt(begPos);
            float endHeight = _ground->heightAt(endPos);
            float deltaz = endHeight - begHeight;

            Vec2f dir2d = endPos - begPos;
            float dist = dir2d.length() - 2*_juncLengthBeg;
            float realStreetLength = Vec2f(dist, deltaz).length();
            float streetProg = (path.nodeProgession - _juncLengthBeg) / _juncLengthEnd;
            ctz.direction(dir2d.x(), dir2d.y(), deltaz/dist).normalize();

            Vec2f right( (dir2d.normalize() * _description->roadWidth / 4.0f).rotateQuarterCW() );
            ctz.position = vec3(right + begPos + dir2d*_juncLengthBeg, begHeight) + ctz.direction*streetProg*realStreetLength;
        }
        else
        {
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
            Vec2f lastDir2d,  lastRight, lastRealPos,
                  nextDir2d, nextRight, nextRealPos;

            if(progression < 0.5f)
            {
                juncHeight = _ground->heightAt(begPos);
                juncProg = (progression + _juncLengthBeg) / (2.0f*_juncLengthBeg);
                juncPos = begPos;
                lastDir2d = begPos - prevPos;
                nextDir2d = endPos - begPos;
            }
            else
            {
                juncHeight = _ground->heightAt(endPos);
                juncProg = ((progression-1.0f) + _juncLengthBeg) / (2.0f*_juncLengthBeg);
                juncPos = endPos;
                lastDir2d = endPos  - begPos;
                nextDir2d = nextPos - endPos;
            }

            float lastWeight = (1.0f - juncProg);
            float nextWeight = juncProg;
            Vec2f dirInterpol = (lastWeight*lastDir2d + nextWeight*nextDir2d).normalize();
            ctz.direction(dirInterpol.x(), dirInterpol.y(), 0.0f);

            lastDir2d.normalize();
            lastDir2d *= _description->roadWidth/2.0f;
            lastRight = (lastDir2d / 2.0f).rotateQuarterCW();
            lastRealPos = -lastDir2d + lastRight;

            nextDir2d.normalize();
            nextDir2d *= _description->roadWidth/2.0f;
            nextRight = (nextDir2d / 2.0f).rotateQuarterCW();
            nextRealPos = nextDir2d + nextRight;

            Vec2f posInterpol = juncPos + lastWeight*lastRealPos + nextWeight*nextRealPos;
            ctz.position(posInterpol.x(), posInterpol.y(), juncHeight);
        }
    }
}

void CitizensMoveHomeWork::placeCitizenOnBuilding(Citizen& ctz)
{
    Vec2i buildingPos;
    if(ctz.curState == CITIZEN_AT_HOME)
        buildingPos = ctz.homePos;
    else if(ctz.curState == CITIZEN_AT_WORK)
        buildingPos = ctz.workPos;

    Vec2f pos2d = Vec2f(0.5f, 0.5f) + buildingPos + Vec2f(randomRange(-0.4f, 0.4f), randomRange(-0.4f, 0.4f));
    ctz.position(pos2d.x(), pos2d.y(), _ground->heightAt(buildingPos) + _city->lands().get(buildingPos)->nbStories() * _description->storyHeight);
}
