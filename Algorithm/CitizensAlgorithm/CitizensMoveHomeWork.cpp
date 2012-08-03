#include "CitizensMoveHomeWork.h"

using namespace std;
using namespace cellar;


CitizensMoveHomeWork::CitizensMoveHomeWork()
{
}

void CitizensMoveHomeWork::setup(City &city)
{
    CitizensAlgorithm::setup( city );
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
        distance *= (1.0f - path.nodeProgession);

        if(distance < canWalk)
        {
            canWalk -= distance;
            path.gotoNextNode();
        }
        else
        {
            path.nodeProgession += canWalk / distance;
            return timeToMove;
        }
    }

    // Work was reached in this turn
    ctz.curState = CITIZEN_AT_WORK;
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
        distance *= (1.0f - path.nodeProgession);

        if(distance < canWalk)
        {
            canWalk -= distance;
            path.gotoRevNextNode();
        }
        else
        {
            path.nodeProgession += canWalk / distance;
            return timeToMove;
        }
    }

    // Home was reached in this turn
    ctz.curState = CITIZEN_AT_HOME;
    float elapsed = timeToMove - canWalk/ctz.walkingSpeed;
    Time after = Time().fromSeconds(elapsed) + _city->dateAndTime();
    cout << after.toString() << "\tAt home (" << ctz.id() << ')' << endl;
    return timeToMove - canWalk/ctz.walkingSpeed;
}
