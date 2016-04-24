#pragma once

#include <string>
#include "RealAnt.hpp"
#include <tuple>
#include <vector>

namespace antlogic
{
    class Cell
    {
        int smell;
        int smellintensity;
        int antmask;
        int hill;
        int foodamount;
        int iswall;
    public:
        Cell(int, int, int);
        AntSensor getSensor(int) const;
        int foodAmount() const;
        void foodTaken();
        void foodDropped();
        void setSmell(int);
        void addAnt(int);
        void cleanAnts();
        int getHill() const;
    };

    class Field
    {
        int n, m;
        Cell **t;
    public:
        Field(int, int);
        Field(const Field&);
        Field& operator=(const Field&);
        void fill(const std::vector <std::tuple <int, int, int> >&);
        Cell at(int, int) const;
        bool isCorrectPos(int, int) const;
        int getN() const;
        int getM() const;
        void cleanAnts();
        void addAnt(const RealAnt&);
        void foodTaken(int, int);
        void foodDropped(int, int);
        ~Field();
    };
}
