#pragma once
#include "IAntLogic.hpp"
#include <vector>

namespace antlogic
{
    const int WAIT = 8;
    class RealAnt: public Ant
    {
        std::vector <char> mem;
        int x, y;
        bool hasfood;
        int teamid;
        int wait;
    public:
        RealAnt(int, int, int);
        virtual char* getMemory() const;
        virtual bool hasFood() const;
        virtual int getTeamId() const;
        int getX() const;
        int getY() const;
        int getWait() const;
        bool isBitten() const;
        void setFood();
        void putFood();
        void goUp();
        void goLeft();
        void goDown();
        void goRight();
        void bitten();
        void newStep();
        virtual ~RealAnt() {}
    };
}
