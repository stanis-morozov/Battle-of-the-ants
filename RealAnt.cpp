#include "RealAnt.hpp"
#include <iostream>


antlogic::RealAnt::RealAnt(int id, int n, int m) : hasfood(0), teamid(id), wait(0)
{
    mem.resize(antlogic::MAX_MEMORY);
    switch(teamid) {
        case 0:
            x = y = 0;
            break;
        case 1:
            x = 0, y = m - 1;
            break;
        case 2:
            x = n - 1, y = 0;
            break;
        case 3:
            x = n - 1, y = m - 1;
            break;
    }
}

char* antlogic::RealAnt::getMemory() const
{
    return const_cast<char *>(mem.data());
}

bool antlogic::RealAnt::hasFood() const
{
    return hasfood;
}

int antlogic::RealAnt::getTeamId() const
{
    return teamid;
}

int antlogic::RealAnt::getX() const
{
    return x;
}

int antlogic::RealAnt::getY() const
{
    return y;
}

int antlogic::RealAnt::getWait() const
{
    return wait;
}

bool antlogic::RealAnt::isBitten() const
{
    return wait > 0;
}

void antlogic::RealAnt::setFood()
{
    hasfood = 1;
}

void antlogic::RealAnt::putFood()
{
    hasfood = 0;
}

void antlogic::RealAnt::goUp()
{
    x--;
}

void antlogic::RealAnt::goLeft()
{
    y--;
}

void antlogic::RealAnt::goDown()
{
    x++;
}

void antlogic::RealAnt::goRight()
{
    y++;
}

void antlogic::RealAnt::bitten()
{
    wait = WAIT;
}

void antlogic::RealAnt::newStep()
{
    if (wait > 0) {
        wait--;
    }
}
